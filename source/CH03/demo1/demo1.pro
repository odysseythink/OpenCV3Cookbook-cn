TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


win32 {
    INCLUDEPATH += D:\Workplace\opencv_learn\opencv-3.4.4-dev\include
    LIBS += D:\Workplace\opencv_learn\opencv-3.4.4-dev\x86\mingw\bin\libopencv_*.dll
}


