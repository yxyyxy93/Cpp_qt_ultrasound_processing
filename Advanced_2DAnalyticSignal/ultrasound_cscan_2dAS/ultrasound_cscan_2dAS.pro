QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../Firststage_read/Ultrasonic_dataset_reader/trimdialog.cpp \
    as_2d_process_class.cpp \
    graphwindow.cpp \
    qcustomplot.cpp \
    ../../Firststage_read/Ultrasonic_dataset_reader/ultrasound_cscan_process.cpp \
    ../../Firststage_read/Ultrasonic_dataset_reader/utils.cpp \
    main.cpp \
    ultrasound_cscan_process_2das.cpp

HEADERS += \
    ../../Firststage_read/Ultrasonic_dataset_reader/trimdialog.h \
    ../../Firststage_read/Ultrasonic_dataset_reader/fftw3.h \
    as_2d_process_class.h \
    fftw3.h \
    graphwindow.h \
    npy.hpp \
    qcustomplot.h \
    ../../Firststage_read/Ultrasonic_dataset_reader/ultrasound_cscan_process.h \
    ../../Firststage_read/Ultrasonic_dataset_reader/utils.h \
    ultrasound_cscan_process_2das.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += "$$PWD/libfftw3-3.dll"
