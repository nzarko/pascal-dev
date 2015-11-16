QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pdutils
TEMPLATE = lib

unix:QMAKE_CXXFLAGS += -std=c++11

DEFINES += PDUTILS_LIBRARY

SOURCES += pdutils.cpp

HEADERS += pdutils.h \
    pdutils_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
