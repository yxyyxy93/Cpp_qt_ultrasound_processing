#include "ultrasound_cscan_process_2das.h"
#include "..\..\Firststage_read\Ultrasonic_dataset_reader\ultrasound_cscan_process.h"
#include "..\..\Firststage_read\Ultrasonic_dataset_reader\utils.h"
#include "as_2d_process_class.h"
#include "graphwindow.h"

#include <memory>
#include <cmath> // Include the cmath library to use pow() and atan2()
#include <QMetaType>

// constructor
ultrasound_cscan_process_2DAS::ultrasound_cscan_process_2DAS(QWidget *parent,
                                                             QString fn,
                                                             int fs,
                                                             double fx,
                                                             double fy)
    : ultrasound_Cscan_process(parent, fn, fs, fx, fy),
      sc(3.5),
      sf(1.5),
      n(10){
    // Add some widgets to the first page
    QPushButton *button1 = new QPushButton("Go to 2D Analytic-signal analysis (better do it after surface alignment!)");
    ultrasound_Cscan_process::layout->addWidget(button1);

    // Create a new widget for the second page
    QWidget *page2 = new QWidget();
    this->layout2 = new QVBoxLayout();
    page2->setLayout(this->layout2);
    QPushButton *button2 = new QPushButton("Go to simple C-scan check");
    this->layout2->addWidget(button2);

    // Create a stacked widget and add the pages to it
    QStackedWidget *stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(this);
    stackedWidget->addWidget(page2);
    // Set the current index to display the first page
    stackedWidget->setCurrentIndex(0);
    // Connect the buttons to switch between pages
    connect(button1, &QPushButton::clicked, this,
            [stackedWidget](){ stackedWidget->setCurrentIndex(1); });
    connect(button2, &QPushButton::clicked, this,
            [stackedWidget](){ stackedWidget->setCurrentIndex(0); });
    // Show the stacked widget
    stackedWidget->show();

    // ************** create the push buttons and correponding labels on the 2nd page
    QPushButton *myButton_2DAS = new QPushButton(tr("Plot C-scan images"), this);
    connect(myButton_2DAS, &QPushButton::clicked, this,
            &ultrasound_cscan_process_2DAS::handleButton_2DAS);
    layout2->addWidget(myButton_2DAS);
    QPushButton* myButton_showkernel = new QPushButton(tr("Plot 2D hilbert kernels"), this);
    this->layout2->addWidget(myButton_showkernel);
    connect(myButton_showkernel, &QPushButton::clicked, this,
            &ultrasound_cscan_process_2DAS::handleButton_myButton_showkernel);
    // ************** progress bar
    this->m_progressBar_page2 = new QProgressBar;
    this->m_progressBar_page2->setRange(0, 100);
    this->layout2->addWidget(this->m_progressBar_page2);
    // Create QLineEdit widgets
    QLineEdit *scEdit = new QLineEdit(this);
    QDoubleValidator* validator = new QDoubleValidator(scEdit);
    scEdit->setValidator(validator);
    // Create a label for the QLineEdit widget
    QLabel *scLabel = new QLabel(tr("coarse scale space parameter:"));
    QLineEdit *sfEdit = new QLineEdit(this);
    // Create a label for the QLineEdit widget
    QLabel *sfLabel = new QLabel(tr("fine scale space parameter:"));
    QLineEdit *nEdit = new QLineEdit(this);
    // Create a label for the QLineEdit widget
    QLabel *nLabel = new QLabel(tr("mask size:"));
    QHBoxLayout *layout_para = new QHBoxLayout;
    layout_para->addWidget(scLabel);
    layout_para->addWidget(scEdit);
    layout_para->addWidget(sfLabel);
    layout_para->addWidget(sfEdit);
    layout_para->addWidget(nLabel);
    layout_para->addWidget(nEdit);
    // Create a button to get the input
    QPushButton *button = new QPushButton(tr("Update 2D analytic-signal parameters"), this);
    // Connect the button clicked signal to a slot to get the input
    connect(button, &QPushButton::clicked, [=]() {
        // Get the input from the QLineEdit widget and convert it to a number
        this->sc = scEdit->text().toDouble();
        this->sf = sfEdit->text().toDouble();
        this->n = nEdit->text().toInt();
        // Use the number input
        qDebug() << "The number input is: " << this->sc << " " << this->sf << " " << this->n;
    });
    // Create a layout for the button
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(button);
    this->layout2->addLayout(layout_para);
    this->layout2->addLayout(buttonLayout);

    // create a button to calculate the statistic and display
    QPushButton *stat_button = new QPushButton(tr("statistic"), this);
    this->layout2->addWidget(stat_button);
    // Connect the button clicked signal to a slot to get the input
    connect(stat_button, &QPushButton::clicked, this,
            &ultrasound_cscan_process_2DAS::handleButton_myButton_stat);
}

// deconstructor
ultrasound_cscan_process_2DAS::~ultrasound_cscan_process_2DAS(){
    delete layout2;
    delete m_progressBar_page2;
    // Qt plots
    delete customPlot1_page2;
    delete customPlot2_page2;
    delete customPlot3_page2;
    delete scrollBarZ_page2;
    delete imageLabel;
}

// ************* 2D analytic-signal and ｖｉｓｕａｌｉｚａｔｉｏｎ
void ultrasound_cscan_process_2DAS::handleButton_2DAS() {
    // ****************** create the orthoslice visual
    // Create the QCustomPlot widget
    this->customPlot1_page2 = new QCustomPlot();
    this->customPlot2_page2 = new QCustomPlot();
    this->customPlot3_page2 = new QCustomPlot();
    // Qcustomplot
    this->scrollBarZ_page2 = new QScrollBar(Qt::Horizontal);
    this->scrollBarZ_page2->setGeometry(50, 470, 600, 20);
    // add an value label
    QLabel *sBZ_label = new QLabel();
    sBZ_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // Connect the scrollbar's valueChanged() signal to a slot that updates the label text
    connect(scrollBarZ_page2, &QScrollBar::valueChanged, [=](int value) {
        QString labelText = QString("Min: %1 Max: %2 Current: %3").
                            arg(scrollBarZ_page2->minimum()).arg(scrollBarZ_page2->maximum()).arg(value);
        sBZ_label->setText(labelText);
    });
    // Create a horizontal layout for the main window
    QWidget *rightPanel = new QWidget();
    QHBoxLayout *hLayout = new QHBoxLayout();
    rightPanel->setLayout(hLayout);
    // Create a vertical layout for 1
    QVBoxLayout *plot1l = new QVBoxLayout();
    QWidget *plot1w = new QWidget();
    plot1w->setLayout(plot1l);
    // Add some widgets to the right panel
    plot1l->addWidget(this->customPlot1_page2);
    plot1l->addWidget(this->scrollBarZ_page2);
    plot1l->addWidget(sBZ_label);
    // Create a vertical layout for 2
    QVBoxLayout *plot2l = new QVBoxLayout();
    QWidget *plot2w = new QWidget();
    plot2w->setLayout(plot2l);
    // Add some widgets to the right panel
    plot2l->addWidget(this->customPlot2_page2);
    // Create a vertical layout for 3
    QVBoxLayout *plot3l = new QVBoxLayout();
    QWidget *plot3w = new QWidget();
    plot3w->setLayout(plot3l);
    // Add some widgets to the right panel
    plot3l->addWidget(this->customPlot3_page2);
    hLayout->addWidget(plot1w);
    hLayout->addWidget(plot2w);
    hLayout->addWidget(plot3w);
    this->layout2->addWidget(rightPanel);
    // Set the range of the QScrollBars based on the size of the data
    this->scrollBarZ_page2->setRange(0, ultrasound_Cscan_process::C_scan_double[0][0].size() - 1);
    // Set the initial values of the QScrollBars
    this->scrollBarZ_page2->setValue(ultrasound_Cscan_process::C_scan_double[0][0].size() / 2);
    // Connect the valueChanged() signals of the QScrollBars to update the plot data
    QObject::connect(this->scrollBarZ_page2, &QScrollBar::valueChanged, this,
                     &ultrasound_cscan_process_2DAS::updatePlot_page2);
    Q_ASSERT(this->scrollBarZ_page2 != nullptr);
   }

void ultrasound_cscan_process_2DAS::updatePlot_page2() {
    int sliceZ = this->scrollBarZ_page2->value();
    qDebug() << "slice Z index: " << sliceZ;
    // Create QCPColorMap objects
    QCPColorMap *map1 = new QCPColorMap(this->customPlot1_page2->xAxis, this->customPlot1_page2->yAxis);
    // set the data for each QCPColorMap
    Q_ASSERT(!ultrasound_Cscan_process::C_scan_AS.isEmpty());
    int x_size = ultrasound_Cscan_process::C_scan_AS.size();
    int y_size = ultrasound_Cscan_process::C_scan_AS[0].size();
    map1->data()->setSize(x_size, y_size);
    map1->data()->setRange(QCPRange(0, x_size), QCPRange(0, y_size));
    // *************** save the Cscan image *************
    this->img_Cscan.clear();
    QVector<double> Scanline;
    for (int i = 0; i < x_size; ++i) {
        Scanline.clear();
        for (int j = 0; j < y_size; ++j) {
            Scanline.push_back(std::abs(ultrasound_Cscan_process::C_scan_AS[i][j][sliceZ]));
            map1->data()->setCell(i, j, std::abs(ultrasound_Cscan_process::C_scan_AS[i][j][sliceZ]));
        }
        this->img_Cscan.push_back(Scanline);
    }
    map1->setGradient(QCPColorGradient::gpJet);
    map1->setInterpolate(true);
    // Rescale the color map data range to fit the new data
    map1->rescaleDataRange(true);
    map1->rescaleAxes();
    this->customPlot1_page2->xAxis->setRange(0, x_size);
    this->customPlot1_page2->yAxis->setRange(0, y_size);
    // Call replot() to update the plot with the new data
    this->customPlot1_page2->replot();
    delete map1;
    // 2d analytic-signals
    // calculate 2D analytic-signal
    QVector<QVector<double>> img_ori(x_size, QVector<double>(y_size, 0));
    QVector<QVector<double>> img_pha(x_size, QVector<double>(y_size, 0));
    QVector<QVector<double>> img_amp(x_size, QVector<double>(y_size, 0));
    QVector<QVector<double>> img_ape(x_size, QVector<double>(y_size, 0));
    // *************** freq. domain convolution
    AS_2D_process_class AS_2D_process(this->n);
    QVector<QVector<double>> kernel_1st_f;
    QVector<QVector<double>> kernel_2nd_f;
    QVector<QVector<double>> kernel_1st_c;
    QVector<QVector<double>> kernel_2nd_c;
    //
    AS_2D_process.fx_2DHilbertKernel(this->sf, kernel_1st_f, kernel_2nd_f);
    AS_2D_process.fx_2DHilbertKernel(this->sc, kernel_1st_c, kernel_2nd_c);
    //
    QVector<QVector<double>> fp_kernel = kernel_1st_f * (this->sf) - kernel_1st_c * (this->sc);
    QVector<QVector<double>> q1_x_kernel = (kernel_1st_f - kernel_1st_c) * AS_2D_process.x_mesh;
    QVector<QVector<double>> q1_y_kernel = (kernel_1st_f - kernel_1st_c) * AS_2D_process.y_mesh;
    QVector<QVector<double>> q2_xx_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.x_mesh * AS_2D_process.x_mesh;
    QVector<QVector<double>> q2_xy_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.x_mesh * AS_2D_process.y_mesh;
    QVector<QVector<double>> q2_yy_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.y_mesh * AS_2D_process.y_mesh;
    //
    QVector<QVector<double>> fp = AS_2D_process.convolve2DFreq(this->img_Cscan, fp_kernel);
    QVector<QVector<double>> fx = AS_2D_process.convolve2DFreq(this->img_Cscan, q1_x_kernel);
    QVector<QVector<double>> fy = AS_2D_process.convolve2DFreq(this->img_Cscan, q1_y_kernel);
    QVector<QVector<double>> fxx = AS_2D_process.convolve2DFreq(this->img_Cscan, q2_xx_kernel);
    QVector<QVector<double>> fxy = AS_2D_process.convolve2DFreq(this->img_Cscan, q2_xy_kernel);
    QVector<QVector<double>> fyy = AS_2D_process.convolve2DFreq(this->img_Cscan, q2_yy_kernel);
    //
    QVector<QVector<double>> f_pm = (fxx-fyy) * 0.5;
    QVector<QVector<double>> f_s = fp * 0.5;
    for (int i = 0; i<x_size; ++i){
        for (int j = 0; j<y_size; ++j){
            double e = sqrt(pow(f_pm[i][j], 2) + pow(fxy[i][j], 2)) / abs(f_s[i][j]);
            double l = pow(fx[i][j], 2) + pow(fy[i][j], 2);
            double q = l*2/(1+e);
            img_pha[i][j] = atan2(sqrt(q),fp[i][j]);
            img_ori[i][j] = 0.5 * atan2(fxy[i][j], f_pm[i][j]);
            //    img_ori[i][j] = img_pha[i][j]==0?
            //         0.5 * atan2(fxy[i][j], f_pm[i][j])+M_PI/2:atan2(fy[i][j], fx[i][j]);
            img_amp[i][j] = 0.5 * sqrt(pow(fp[i][j], 2) + q);
            img_ape[i][j] = atan2(sqrt(pow(f_s[i][j],2)-pow(fxy[i][j],2)-pow(f_pm[i][j],2)),
                                  sqrt(pow(fxy[i][j],2)+pow(f_pm[i][j],2)));
            //            if (std::real(sqrt(pow(f_s[i][j],2)-pow(fxy[i][j],2)-pow(f_pm[i][j],2)))<=0)
            //                qDebug() << "negative nominator" << pow(f_s[i][j],2)-pow(fxy[i][j],2)-pow(f_pm[i][j],2);
        }
    }
    // Create QCPColorMap objects
    // ************************
    QCPColorMap *map2 = new QCPColorMap(this->customPlot2_page2->xAxis, this->customPlot2_page2->yAxis);
    QCPColorMap *map3 = new QCPColorMap(this->customPlot3_page2->xAxis, this->customPlot3_page2->yAxis);
    // set the data for each QCPColorMap
    map2->data()->setSize(x_size, y_size);
    map2->data()->setRange(QCPRange(0, x_size-1), QCPRange(0, y_size-1));
    map3->data()->setSize(x_size, y_size);
    map3->data()->setRange(QCPRange(0, x_size-1), QCPRange(0, y_size-1));
    for (int i = 0; i < x_size; ++i) {
        for (int j = 0; j < y_size; ++j) {
            if ( std::isnan(img_ape[i][j])  || std::abs(img_ape[i][j])<M_PI/5 || std::abs(img_ape[i][j])>M_PI*4/5)
                map2->data()->setCell(i, j, img_ori[i][j]);
            else
                map2->data()->setAlpha(i, j, 0);
            map3->data()->setCell(i, j, img_ape[i][j]);
        }
    }
    // Rescale the color map data range to fit the new data
    map2->rescaleAxes();
    map3->rescaleAxes();
    //
    this->customPlot2_page2->xAxis->setRange(0, x_size);
    this->customPlot2_page2->yAxis->setRange(0, y_size);
    this->customPlot3_page2->xAxis->setRange(0, x_size);
    this->customPlot3_page2->yAxis->setRange(0, y_size);
    // add color scales:
    QCPColorScale *colorScale = new QCPColorScale(this->customPlot2_page2);
    if (this->customPlot2_page2->plotLayout()->elementCount() != 0)
        this->customPlot2_page2->plotLayout()->removeAt(1); //to update color scale
    this->customPlot2_page2->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    map2->setColorScale(colorScale); // associate the color map with the color scale
    colorScale->axis()->setLabel("Orient angle [rad.]");
    // set the color gradient of the color map to one of the presets:
    map2->setGradient(QCPColorGradient::gpHues);
    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    //    map2->rescaleDataRange();
    map2->setDataRange(QCPRange(-M_PI/2, M_PI/2));
    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(this->customPlot2_page2);
    this->customPlot2_page2->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    // add color scales:
    QCPColorScale *colorScale3 = new QCPColorScale(this->customPlot3_page2);
    if (this->customPlot3_page2->plotLayout()->elementCount() != 0)
        this->customPlot3_page2->plotLayout()->removeAt(1); //to update color scale
    this->customPlot3_page2->plotLayout()->addElement(0, 1, colorScale3); // add it to the right of the main axis rect
    colorScale3->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    map3->setColorScale(colorScale3); // associate the color map with the color scale
    colorScale3->axis()->setLabel("Apex angle [rad.]");
    // set the color gradient of the color map to one of the presets:
    map3->setGradient(QCPColorGradient::gpGrayscale);
    //    map3->rescaleDataRange();
    map3->setDataRange(QCPRange(-M_PI, M_PI));
    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup3 = new QCPMarginGroup(this->customPlot3_page2);
    this->customPlot3_page2->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup3);
    delete marginGroup;
    delete marginGroup3;
    // Call replot() to update the plot with the new data
    this->customPlot2_page2->replot();
    this->customPlot3_page2->replot();
    //
    delete map2;
    delete map3;
    qDebug() << min_max_2d(img_ape);
    qDebug() << min_max_2d(img_ori);
}

void ultrasound_cscan_process_2DAS::handleButton_2DAS_oneslice() {
    int x_size = this->img_Cscan.size();
    int y_size = this->img_Cscan[0].size();
    // calculate 2D analytic-signal
    QVector<QVector<double>> img_ori(x_size, QVector<double>(y_size, 0));
    QVector<QVector<double>> img_pha(x_size, QVector<double>(y_size, 0));
    QVector<QVector<double>> img_amp(x_size, QVector<double>(y_size, 0));
    QVector<QVector<double>> img_ape(x_size, QVector<double>(y_size, 0));
    // *************** freq. domain convolution
    AS_2D_process_class AS_2D_process(this->n);
    QVector<QVector<double>> kernel_1st_f;
    QVector<QVector<double>> kernel_2nd_f;
    QVector<QVector<double>> kernel_1st_c;
    QVector<QVector<double>> kernel_2nd_c;
    //
    AS_2D_process.fx_2DHilbertKernel(this->sf, kernel_1st_f, kernel_2nd_f);
    AS_2D_process.fx_2DHilbertKernel(this->sc, kernel_1st_c, kernel_2nd_c);
    //
    QVector<QVector<double>> fp_kernel = kernel_1st_f * (this->sf) - kernel_1st_c * (this->sc);
    QVector<QVector<double>> q1_x_kernel = (kernel_1st_f - kernel_1st_c) * AS_2D_process.x_mesh;
    QVector<QVector<double>> q1_y_kernel = (kernel_1st_f - kernel_1st_c) * AS_2D_process.y_mesh;
    QVector<QVector<double>> q2_xx_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.x_mesh * AS_2D_process.x_mesh;
    QVector<QVector<double>> q2_xy_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.x_mesh * AS_2D_process.y_mesh;
    QVector<QVector<double>> q2_yy_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.y_mesh * AS_2D_process.y_mesh;
    //
    QVector<QVector<double>> fp = AS_2D_process.convolve2DFreq(this->img_Cscan, fp_kernel);
    QVector<QVector<double>> fx = AS_2D_process.convolve2DFreq(this->img_Cscan, q1_x_kernel);
    QVector<QVector<double>> fy = AS_2D_process.convolve2DFreq(this->img_Cscan, q1_y_kernel);
    QVector<QVector<double>> fxx = AS_2D_process.convolve2DFreq(this->img_Cscan, q2_xx_kernel);
    QVector<QVector<double>> fxy = AS_2D_process.convolve2DFreq(this->img_Cscan, q2_xy_kernel);
    QVector<QVector<double>> fyy = AS_2D_process.convolve2DFreq(this->img_Cscan, q2_yy_kernel);
    //
    QVector<QVector<double>> f_pm = (fxx-fyy) * 0.5;
    QVector<QVector<double>> f_s = fp * 0.5;
    //
    for (int i = 0; i<x_size; ++i){
        for (int j = 0; j<y_size; ++j){
            double e = sqrt(pow(f_pm[i][j], 2) + pow(fxy[i][j], 2)) / abs(f_s[i][j]);
            double l = pow(fx[i][j], 2) + pow(fy[i][j], 2);
            double q = l*2/(1+e);
            img_pha[i][j] = atan2(sqrt(q),fp[i][j]);
            img_ori[i][j] = img_pha[i][j]==0?
                        0.5 * atan2(fxy[i][j], f_pm[i][j])+M_PI/2:atan2(fy[i][j], fx[i][j]);
            img_amp[i][j] = 0.5 * sqrt(pow(fp[i][j], 2) + q);
            img_ape[i][j] = atan2(sqrt(pow(f_s[i][j],2)-pow(fxy[i][j],2)-pow(f_pm[i][j],2)),
                                  sqrt(pow(fxy[i][j],2)+pow(f_pm[i][j],2)));
        }
        this->m_progressBar_page2->setValue(100 * i / x_size);
        // Update the progress bar
        QCoreApplication::processEvents(); // Allow GUI updates
    }
    // ***********************************
    // ***************** spatial domain convolution
    //    double Orientation, phase, amplitude, apexangle;
    //    for (int j = 0;j < x_size;j++)
    //    {
    //        for (int i = 0;i < y_size;i++)
    //        {
    //            if (i<this->n || j<this->n || i>y_size-this->n-1 || j>x_size-this->n-1) {
    //                img_ori[j][i] = 0;
    //                img_pha[j][i] = 0;
    //                img_amp[j][i] = 0;
    //                img_ape[j][i] = 0;
    //                continue;
    //            }
    //            AnalyticSignal(double(j), double(i),
    //                           Orientation, phase, amplitude, apexangle,
    //                           this->n, this->sc, this->sf, this->dx,
    //                           this->img_Cscan);
    //            img_ori[j][i] = Orientation;
    //            img_pha[j][i] = phase;
    //            img_amp[j][i] = amplitude;
    //            img_ape[j][i] = apexangle;
    //        }
    //        this->m_progressBar_page2->setValue(100 * j / x_size);
    //        // Update the progress bar
    //        QCoreApplication::processEvents(); // Allow GUI updates
    //    }
    this->m_progressBar_page2->setValue(100);
    // Create QCPColorMap objects
    // ************************
    QCPColorMap *map2 = new QCPColorMap(this->customPlot2_page2->xAxis,
                                        this->customPlot2_page2->yAxis);
    QCPColorMap *map3 = new QCPColorMap(this->customPlot3_page2->xAxis,
                                        this->customPlot3_page2->yAxis);
    // set the data for each QCPColorMap
    map2->data()->setSize(x_size, y_size);
    map2->data()->setRange(QCPRange(0, x_size), QCPRange(0, y_size));
    map3->data()->setSize(x_size, y_size);
    map3->data()->setRange(QCPRange(0, x_size), QCPRange(0, y_size));
    //
    for (int i = 0; i < x_size; ++i) {
        for (int j = 0; j < y_size; ++j) {
            map2->data()->setCell(i, j, (img_ape[j][i]<2)?img_ori[j][i]:qQNaN());
            map3->data()->setCell(i, j, img_ape[j][i]);
        }
    }
    //
    map2->setGradient(QCPColorGradient::gpHues);
    map3->setGradient(QCPColorGradient::gpGrayscale);
    // Add a color scale to the custom plot widget
    QCPColorScale *colorScale2 = new QCPColorScale(this->customPlot2_page2);
    QCPColorScale *colorScale3 = new QCPColorScale(this->customPlot3_page2);
    // Set the color map for the color scale
    colorScale2->setDataRange(map2->dataRange());
    colorScale2->setGradient(map2->gradient());
    colorScale3->setDataRange(map3->dataRange());
    colorScale3->setGradient(map3->gradient());
    //
    map2->setColorScale(colorScale2);
    map3->setColorScale(colorScale3);
    // add a color scale:
    this->customPlot2_page2->plotLayout()->addElement(0, 1, colorScale2); // add it to the right of the main axis rect
    colorScale2->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)// associate the color map with the color scale
    colorScale2->axis()->setLabel("Orientation (rad.)");
    this->customPlot3_page2->plotLayout()->addElement(0, 1, colorScale3); // add it to the right of the main axis rect
    colorScale3->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)// associate the color map with the color scale
    colorScale3->axis()->setLabel("Apex angle. (rad.)");
    // Rescale the color map data range to fit the new data
    map2->rescaleDataRange(true);
    map2->rescaleAxes();
    map3->rescaleDataRange(true);
    map3->rescaleAxes();
    //
    this->customPlot2_page2->xAxis->setRange(0, x_size);
    this->customPlot2_page2->yAxis->setRange(0, y_size);
    this->customPlot3_page2->xAxis->setRange(0, x_size);
    this->customPlot3_page2->yAxis->setRange(0, y_size);
    // Call replot() to update the plot with the new data
    this->customPlot2_page2->replot();
    this->customPlot3_page2->replot();
    delete map2;
    delete map3;
}

// ************************** general slots
void ultrasound_cscan_process_2DAS::handleButton_myButton_showkernel(){
    AS_2D_process_class AS_2D_process(this->n);
    //
    QVector<QVector<double>> kernel_1st_f;
    QVector<QVector<double>> kernel_2nd_f;
    QVector<QVector<double>> kernel_1st_c;
    QVector<QVector<double>> kernel_2nd_c;
    //
    AS_2D_process.fx_2DHilbertKernel(this->sf, kernel_1st_f, kernel_2nd_f);
    AS_2D_process.fx_2DHilbertKernel(this->sc, kernel_1st_c, kernel_2nd_c);
    //
    QVector<QVector<double>> fp_kernel = kernel_1st_f * (this->sf) - kernel_1st_c * (this->sc);
    QVector<QVector<double>> q1_x_kernel = (kernel_1st_f - kernel_1st_c) * AS_2D_process.x_mesh;
    QVector<QVector<double>> q1_y_kernel = (kernel_1st_f - kernel_1st_c) * AS_2D_process.y_mesh;
    QVector<QVector<double>> q2_xx_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.x_mesh * AS_2D_process.x_mesh;
    QVector<QVector<double>> q2_xy_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.x_mesh * AS_2D_process.y_mesh;
    QVector<QVector<double>> q2_yy_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.y_mesh * AS_2D_process.y_mesh;
    // Create a horizontal layout for the main window
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setSpacing(0);
    QWidget *hpanel = new QWidget;
    //
    QCustomPlot *customPlot_fp = new QCustomPlot();
    QCustomPlot *customPlot_q1x = new QCustomPlot();
    QCustomPlot *customPlot_q1y = new QCustomPlot();
    QCustomPlot *customPlot_q2xx = new QCustomPlot();
    QCustomPlot *customPlot_q2xy = new QCustomPlot();
    QCustomPlot *customPlot_q2yy = new QCustomPlot();
    //
    hLayout->addWidget(customPlot_fp);
    hLayout->addWidget(customPlot_q1x);
    hLayout->addWidget(customPlot_q1y);
    hLayout->addWidget(customPlot_q2xx);
    hLayout->addWidget(customPlot_q2xy);
    hLayout->addWidget(customPlot_q2yy);
    hpanel->setLayout(hLayout);
    this->layout2->addWidget(hpanel);
    // Create QCPColorMap objects
    QCPColorMap *map_fp = new QCPColorMap(customPlot_fp->xAxis,
                                          customPlot_fp->yAxis);
    QCPColorMap *map_q1x = new QCPColorMap(customPlot_q1x->xAxis,
                                           customPlot_q1x->yAxis);
    QCPColorMap *map_q1y = new QCPColorMap(customPlot_q1y->xAxis,
                                           customPlot_q1y->yAxis);
    QCPColorMap *map_q2xx = new QCPColorMap(customPlot_q2xx->xAxis,
                                            customPlot_q2xx->yAxis);
    QCPColorMap *map_q2xy = new QCPColorMap(customPlot_q2xy->xAxis,
                                            customPlot_q2xy->yAxis);
    QCPColorMap *map_q2yy = new QCPColorMap(customPlot_q2yy->xAxis,
                                            customPlot_q2yy->yAxis);
    // set the data for each QCPColorMap
    map_fp->data()->setSize(2*this->n+1, 2*this->n+1);
    //    map_fp->data()->setRange(QCPRange(-this->n, this->n), QCPRange(-this->n, this->n));
    for (int i = 0; i < 2*this->n+1; ++i) {
        for (int j = 0; j < 2*this->n+1; ++j) {
            map_fp->data()->setCell(i, j, fp_kernel[i][j]);
        }
    }
    map_q1x->data()->setSize(2*this->n+1, 2*this->n+1);
    //    map_q1x->data()->setRange(QCPRange(-this->n, this->n), QCPRange(-this->n, this->n));
    for (int i = 0; i < 2*this->n+1; ++i) {
        for (int j = 0; j < 2*this->n+1; ++j) {
            map_q1x->data()->setCell(i, j, q1_x_kernel[i][j]);
        }
    }
    map_q1y->data()->setSize(2*this->n+1, 2*this->n+1);
    //    map_q1y->data()->setRange(QCPRange(-this->n, this->n), QCPRange(-this->n, this->n));
    for (int i = 0; i < 2*this->n+1; ++i) {
        for (int j = 0; j < 2*this->n+1; ++j) {
            map_q1y->data()->setCell(i, j, q1_y_kernel[i][j]);
        }
    }
    map_q2xx->data()->setSize(2*this->n+1, 2*this->n+1);
    //    map_q2xx->data()->setRange(QCPRange(-this->n, this->n), QCPRange(-this->n, this->n));
    for (int i = 0; i < 2*this->n+1; ++i) {
        for (int j = 0; j < 2*this->n+1; ++j) {
            map_q2xx->data()->setCell(i, j, q2_xx_kernel[i][j]);
        }
    }
    map_q2xy->data()->setSize(2*this->n+1, 2*this->n+1);
    //    map_q2xy->data()->setRange(QCPRange(-this->n, this->n), QCPRange(-this->n, this->n));
    for (int i = 0; i < 2*this->n+1; ++i) {
        for (int j = 0; j < 2*this->n+1; ++j) {
            map_q2xy->data()->setCell(i, j, q2_xy_kernel[i][j]);
        }
    }
    map_q2yy->data()->setSize(2*this->n+1, 2*this->n+1);
    //    map_q2yy->data()->setRange(QCPRange(-this->n, this->n), QCPRange(-this->n, this->n));
    for (int i = 0; i < 2*this->n+1; ++i) {
        for (int j = 0; j < 2*this->n+1; ++j) {
            map_q2yy->data()->setCell(i, j, q2_yy_kernel[i][j]);
        }
    }
    //
    map_fp->setGradient(QCPColorGradient::gpGrayscale);
    map_q1x->setGradient(QCPColorGradient::gpGrayscale);
    map_q1y->setGradient(QCPColorGradient::gpGrayscale);
    map_q2xx->setGradient(QCPColorGradient::gpGrayscale);
    map_q2xy->setGradient(QCPColorGradient::gpGrayscale);
    map_q2yy->setGradient(QCPColorGradient::gpGrayscale);
    // Add a color scale to the custom plot widget
    QCPColorScale *colorScale_fp = new QCPColorScale(customPlot_fp);
    colorScale_fp->setDataRange(map_fp->dataRange());
    colorScale_fp->setGradient(map_fp->gradient());
    map_fp->setColorScale(colorScale_fp);
    customPlot_fp->plotLayout()->addElement(0, 1, colorScale_fp); // add it to the right of the main axis rect
    colorScale_fp->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)// associate the color map with the color scale
    colorScale_fp->axis()->setLabel("Amp. (arb.)");
    //
    QCPColorScale *colorScale_q1x = new QCPColorScale(customPlot_q1x);
    colorScale_q1x->setDataRange(map_q1x->dataRange());
    colorScale_q1x->setGradient(map_q1x->gradient());
    map_q1x->setColorScale(colorScale_q1x);
    customPlot_q1x->plotLayout()->addElement(0, 1, colorScale_q1x); // add it to the right of the main axis rect
    colorScale_q1x->axis()->setLabel("Amp. (arb.)");
    //
    QCPColorScale *colorScale_q1y = new QCPColorScale(customPlot_q1y);
    colorScale_q1y->setDataRange(map_q1y->dataRange());
    colorScale_q1y->setGradient(map_q1y->gradient());
    map_q1y->setColorScale(colorScale_q1y);
    customPlot_q1y->plotLayout()->addElement(0, 1, colorScale_q1y); // add it to the right of the main axis rect
    colorScale_q1y->axis()->setLabel("Amp. (arb.)");
    //
    QCPColorScale *colorScale_q2xx = new QCPColorScale(customPlot_q2xx);
    colorScale_q2xx->setDataRange(map_q2xx->dataRange());
    colorScale_q2xx->setGradient(map_q2xx->gradient());
    map_q2xx->setColorScale(colorScale_q2xx);
    customPlot_q2xx->plotLayout()->addElement(0, 1, colorScale_q2xx); // add it to the right of the main axis rect
    colorScale_q2xx->axis()->setLabel("Amp. (arb.)");
    //
    QCPColorScale *colorScale_q2xy = new QCPColorScale(customPlot_q2xy);
    colorScale_q2xy->setDataRange(map_q2xy->dataRange());
    colorScale_q2xy->setGradient(map_q2xy->gradient());
    map_q2xy->setColorScale(colorScale_q2xy);
    customPlot_q2xy->plotLayout()->addElement(0, 1, colorScale_q2xy); // add it to the right of the main axis rect
    colorScale_q2xy->axis()->setLabel("Amp. (arb.)");
    //
    QCPColorScale *colorScale_q2yy = new QCPColorScale(customPlot_q2yy);
    colorScale_q2yy->setDataRange(map_q2yy->dataRange());
    colorScale_q2yy->setGradient(map_q2yy->gradient());
    map_q2yy->setColorScale(colorScale_q2yy);
    customPlot_q2yy->plotLayout()->addElement(0, 1, colorScale_q2yy); // add it to the right of the main axis rect
    colorScale_q2yy->axis()->setLabel("Amp. (arb.)");
    // Rescale the color map data range to fit the new data
    map_fp->rescaleDataRange(true);
    map_q1x->rescaleDataRange(true);
    map_q1y->rescaleDataRange(true);
    map_q2xx->rescaleDataRange(true);
    map_q2xy->rescaleDataRange(true);
    map_q2yy->rescaleDataRange(true);
    // Call replot() to update the plot with the new data
    customPlot_fp->replot();
    customPlot_q1x->replot();
    customPlot_q1y->replot();
    customPlot_q2xx->replot();
    customPlot_q2xy->replot();
    customPlot_q2yy->replot();
}

// *************************** statistic
void ultrasound_cscan_process_2DAS::handleButton_myButton_stat(){
    AS_2D_process_class AS_2D_process(this->n);
    // *************** freq. domain convolution
    QVector<QVector<double>> kernel_1st_f;
    QVector<QVector<double>> kernel_2nd_f;
    QVector<QVector<double>> kernel_1st_c;
    QVector<QVector<double>> kernel_2nd_c;
    //
    AS_2D_process.fx_2DHilbertKernel(this->sf, kernel_1st_f, kernel_2nd_f);
    AS_2D_process.fx_2DHilbertKernel(this->sc, kernel_1st_c, kernel_2nd_c);
    //
    QVector<QVector<double>> fp_kernel = kernel_1st_f * (this->sf) - kernel_1st_c * (this->sc);
    QVector<QVector<double>> q1_x_kernel = (kernel_1st_f - kernel_1st_c) * AS_2D_process.x_mesh;
    QVector<QVector<double>> q1_y_kernel = (kernel_1st_f - kernel_1st_c) * AS_2D_process.y_mesh;
    QVector<QVector<double>> q2_xx_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.x_mesh * AS_2D_process.x_mesh;
    QVector<QVector<double>> q2_xy_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.x_mesh * AS_2D_process.y_mesh;
    QVector<QVector<double>> q2_yy_kernel = (kernel_2nd_f - kernel_2nd_c) * AS_2D_process.y_mesh * AS_2D_process.y_mesh;
    //
    QVector<QVector<double>> fp;
    QVector<QVector<double>> fx;
    QVector<QVector<double>> fy;
    QVector<QVector<double>> fxx;
    QVector<QVector<double>> fxy;
    QVector<QVector<double>> fyy;
    //
    QVector<QVector<double>> f_pm;
    QVector<QVector<double>> f_s;
    // set the data for each QCPColorMap
    Q_ASSERT(!ultrasound_Cscan_process::C_scan_AS.isEmpty());
    int x_size = ultrasound_Cscan_process::C_scan_AS.size();
    int y_size = ultrasound_Cscan_process::C_scan_AS[0].size();
    int z_size = ultrasound_Cscan_process::C_scan_AS[0][0].size();
    // calcualte
    QVector<QVector<double>> angular_1dstack;
    QVector<double> angular_1d;
    double bins(100);
    double max(M_PI/2);
    double min(-M_PI/2);

    double e;
    double l;
    double q;
    double pha;
    double ori;

    for (int z = 0; z<z_size; ++z){
        // get the Cscan image
        this->img_Cscan.clear();
        QVector<double> Scanline;
        for (int i = 0; i < x_size; ++i) {
            Scanline.clear();
            for (int j = 0; j < y_size; ++j) {
                Scanline.push_back(std::abs(ultrasound_Cscan_process::C_scan_AS[i][j][z]));
            }
            this->img_Cscan.push_back(Scanline);
        }
        // calcualte 2d as
        fp = AS_2D_process.convolve2DFreq(this->img_Cscan, fp_kernel);
        fx = AS_2D_process.convolve2DFreq(this->img_Cscan, q1_x_kernel);
        fy = AS_2D_process.convolve2DFreq(this->img_Cscan, q1_y_kernel);
        fxx = AS_2D_process.convolve2DFreq(this->img_Cscan, q2_xx_kernel);
        fxy = AS_2D_process.convolve2DFreq(this->img_Cscan, q2_xy_kernel);
        fyy = AS_2D_process.convolve2DFreq(this->img_Cscan, q2_yy_kernel);
        //
        f_pm = (fxx-fyy) * 0.5;
        f_s = fp * 0.5;
        //
        angular_1d.clear();
        for (int i = 0; i<x_size; ++i){
            for (int j = 0; j<y_size; ++j){
                e = sqrt(pow(f_pm[i][j], 2) + pow(fxy[i][j], 2)) / abs(f_s[i][j]);
                l = pow(fx[i][j], 2) + pow(fy[i][j], 2);
                q = l*2/(1+e);
                pha = atan2(sqrt(q), fp[i][j]);
                ori = pha==0?
                      0.5 * atan2(fxy[i][j], f_pm[i][j])+M_PI/2:atan2(fy[i][j], fx[i][j]);
                angular_1d.push_back(ori);
            }
        }
//        angular_1dstack.push_back(calculateHistogram(angular_1d, bins, max, min));
        this->m_progressBar_page2->setValue(100 * z / z_size);
        // Update the progress bar
        QCoreApplication::processEvents(); // Allow GUI updates
    }
    GraphWindow *window = new GraphWindow;
    window->setData(angular_1dstack);
    window->show();
}
