QT       += core gui widgets

CONFIG += c++17


SOURCES += \
    getspfeatures.cpp \
    main.cpp \
    mainwidget.cpp

HEADERS += \
    getspfeatures.h \
    mainwidget.h

win32{
## Windows standard install location
    INCLUDEPATH += \
        "C:\Program Files (x86)\Intel\oneAPI\2024.0\include\ipp"
    LIBS += \
        -L"C:\Program Files (x86)\Intel\oneAPI\2024.0\bin" \
        -lippcore -lipps -lippvm
} else{
## Linux/Mac standard install location
    INCLUDEPATH += \
        /opt/intel/oneapi/ipp/latest/include/ipp
    LIBS += \
        -L/opt/intel/oneapi/ipp/latest/lib \
        -lippcore -lipps -lippvm
}
