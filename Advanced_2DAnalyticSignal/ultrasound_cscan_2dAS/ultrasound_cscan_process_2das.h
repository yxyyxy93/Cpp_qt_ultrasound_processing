#ifndef ULTRASOUND_CSCAN_PROCESS_2DAS_H
#define ULTRASOUND_CSCAN_PROCESS_2DAS_H

#include <QtWidgets>
#include "..\..\Firststage_read\Ultrasonic_dataset_reader\ultrasound_cscan_process.h"
#include <memory>

// do not need to set default arguments for a constructor in a derived class
// if the base class already has default arguments set in its constructor.
class ultrasound_cscan_process_2DAS: public ultrasound_Cscan_process
{

public:
    ultrasound_cscan_process_2DAS(QWidget *parent,
                                  QString fn,
                                  int fs,
                                  double fx,
                                  double fy);
    ~ultrasound_cscan_process_2DAS();

private:
    QVBoxLayout *layout2;

    QProgressBar *m_progressBar_page2;

    // Qt plots
    QCustomPlot *customPlot1_page2;
    QCustomPlot *customPlot2_page2;
    QCustomPlot *customPlot3_page2;
    QScrollBar *scrollBarZ_page2;

    QLabel* imageLabel;

    // 2D AS properties
    QVector<QVector<double>> img_Cscan;
    double sc;
    double sf;
    int n;

private slots:
    // 2D　analytic-signal
    void handleButton_2DAS();
    void updatePlot_page2();
    void handleButton_2DAS_oneslice();

    // in "handleButton_surface"
    void handleButton_myButton_showkernel();

    // statistic for 2d as
    void handleButton_myButton_stat();
};

#endif // ULTRASOUND_CSCAN_PROCESS_2DAS_H
