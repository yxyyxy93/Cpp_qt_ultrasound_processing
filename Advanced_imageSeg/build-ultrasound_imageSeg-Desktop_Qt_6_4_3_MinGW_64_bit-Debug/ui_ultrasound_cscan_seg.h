/********************************************************************************
** Form generated from reading UI file 'ultrasound_cscan_seg.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ULTRASOUND_CSCAN_SEG_H
#define UI_ULTRASOUND_CSCAN_SEG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ultrasound_cscan_seg
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ultrasound_cscan_seg)
    {
        if (ultrasound_cscan_seg->objectName().isEmpty())
            ultrasound_cscan_seg->setObjectName("ultrasound_cscan_seg");
        ultrasound_cscan_seg->resize(800, 600);
        centralwidget = new QWidget(ultrasound_cscan_seg);
        centralwidget->setObjectName("centralwidget");
        ultrasound_cscan_seg->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ultrasound_cscan_seg);
        menubar->setObjectName("menubar");
        ultrasound_cscan_seg->setMenuBar(menubar);
        statusbar = new QStatusBar(ultrasound_cscan_seg);
        statusbar->setObjectName("statusbar");
        ultrasound_cscan_seg->setStatusBar(statusbar);

        retranslateUi(ultrasound_cscan_seg);

        QMetaObject::connectSlotsByName(ultrasound_cscan_seg);
    } // setupUi

    void retranslateUi(QMainWindow *ultrasound_cscan_seg)
    {
        ultrasound_cscan_seg->setWindowTitle(QCoreApplication::translate("ultrasound_cscan_seg", "ultrasound_cscan_seg", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ultrasound_cscan_seg: public Ui_ultrasound_cscan_seg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ULTRASOUND_CSCAN_SEG_H
