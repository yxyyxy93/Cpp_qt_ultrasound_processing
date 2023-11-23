#include "statistic_2das.h"

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "qcustomplot.h" // include the QCustomPlot library

class statistic_2das : public QWidget
{
    Q_OBJECT

public:
    statistic_2das(QWidget *parent = 0)
        : QWidget(parent)
    {
        QPushButton *button = new QPushButton("Button", this);

        // create a plot widget
        QCustomPlot *customPlot = new QCustomPlot();

        // generate some data
        QVector<double> x(101), y(101); // initialize with entries 0..100
        for (int i=0; i<101; ++i)
        {
            x[i] = i/50.0 - 1; // x goes from -1 to 1
            y[i] = x[i]*x[i]; // let's plot a quadratic function
        }

        // create graph and assign data to it:
        customPlot->addGraph();
        customPlot->graph(0)->setData(x, y);

        // set axes ranges, so we see all data:
        customPlot->xAxis->setRange(-1, 1);
        customPlot->yAxis->setRange(0, 1);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(button);
        layout->addWidget(customPlot);
        setLayout(layout);
    }
};


Statistic_2das::Statistic_2das()
{

}
