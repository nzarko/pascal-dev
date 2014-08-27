#-------------------------------------------------
#
# Project created by QtCreator 2012-02-21T12:32:01
#
#-------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pdebugger
TEMPLATE = lib

DEFINES += PDEBUGGER_LIBRARY

SOURCES += pdebugger.cpp

HEADERS += pdebugger.h\
        pdebugger_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE2316F12
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = pdebugger.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
