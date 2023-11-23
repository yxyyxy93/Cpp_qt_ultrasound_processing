#include "ultrasound_cscan_process_2das.h"
#include "..\..\Firststage_read\Ultrasonic_dataset_reader\utils.h"
#include "graphwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // create the instance of the class
    int fs = 250e6;
    double fx = 2e-3;
    double fy = 2e-3;
    QString default_fn = "default filename";
    ultrasound_cscan_process_2DAS process(nullptr,
                                          default_fn,
                                          fs,
                                          fx,
                                          fy);
    process.show();
    return a.exec();
}

// ******************* for testing classes
//int main(int argc, char *argv[])
//{
//    QApplication app(argc, argv);

//    GraphWindow window;

//    // Example data:
//    QVector<QVector<double>> data = {
//        QVector<double>({0, 1, 2, 3, 4, 5}),
//        QVector<double>({0, 1, 4, 9, 16, 25}),
//        QVector<double>({5, 4, 3, 2, 1, 0})
//    };

//    window.setData(data);
//    window.show();

//    return app.exec();
//}

//int main(int argc, char *argv[])
//{
//    QVector<double> data = {1.2, 2.3, 2.4, 3.1, 3.2, 3.9, 4.1, 4.2, 4.8, 4.9};
//    int numBins = 4;
//    QMap<int, int> histogram = calculateHistogram(data, numBins);

//    // print the histogram
//    QMapIterator<int, int> i(histogram);
//    while (i.hasNext()) {
//        i.next();
//        qDebug() << "Bin " << i.key() << ": " << i.value();
//    }

//    return 0;
//}
