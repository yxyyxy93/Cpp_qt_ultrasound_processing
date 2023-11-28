cmake_minimum_required(VERSION 3.10.0)
project(Ultrasonic_dataset_reader VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

add_executable(Ultrasonic_dataset_reader 
    main.cpp

    qcustomplot.cpp  
    qcustomplot.h 

    trimdialog.cpp 
    trimdialog.h 

    ultrasound_cscan_process.cpp 
    ultrasound_cscan_process.h 

    utils.cpp
    utils.h    


    fftw3.h 
    npy.hpp 
)

target_link_libraries(Ultrasonic_dataset_reader PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport fftw3)
