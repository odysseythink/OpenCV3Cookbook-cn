TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

win32 {
    INCLUDEPATH += D:\Workspace\C_Workspace\C++_Research\opencv\opencv-3.4.4-dev\include
    LIBS += D:\Workspace\C_Workspace\C++_Research\opencv\opencv-3.4.4-dev\x86\mingw\bin\libopencv_*.dll
}

#INCLUDEPATH += D:\Workspace\C_Workspace\C++_Research\opencv\opencv-4.0.0-dev\include
#LIBS += D:\Workspace\C_Workspace\C++_Research\opencv\opencv-4.0.0-dev\x86\mingw\lib\libopencv_*.a

