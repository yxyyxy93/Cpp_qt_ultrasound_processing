QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customgraphicsscene.cpp \
    jetcolormap.cpp \
    main.cpp \
    ultrasound_cscan_seg.cpp \
    ../../Firststage_read/Ultrasonic_dataset_reader/trimdialog.cpp \
    ../../Firststage_read/Ultrasonic_dataset_reader/ultrasound_cscan_process.cpp \
    ../../Firststage_read/Ultrasonic_dataset_reader/utils.cpp \
    ../../Firststage_read/Ultrasonic_dataset_reader/qcustomplot.cpp \

HEADERS += \
    customgraphicsscene.h \
    jetcolormap.h \
    ultrasound_cscan_seg.h \
    ../../Firststage_read/Ultrasonic_dataset_reader/trimdialog.h \
    ../../Firststage_read/Ultrasonic_dataset_reader/fftw3.h \
    ../../Firststage_read/Ultrasonic_dataset_reader/ultrasound_cscan_process.h \
    ../../Firststage_read/Ultrasonic_dataset_reader/utils.h \
    ../../Firststage_read/Ultrasonic_dataset_reader/qcustomplot.h \

FORMS += \
    ultrasound_cscan_seg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += "$$PWD/libfftw3-3.dll"


