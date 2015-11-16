#-------------------------------------------------
#
# Project created by QtCreator 2012-02-01T21:50:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = settings
TEMPLATE = lib

unix:QMAKE_CXXFLAGS += -std=c++11

DEFINES += SETTINGS_LIBRARY

SOURCES += settings.cpp \
    pascaloptionsform.cpp \
    configuration.cpp \
    editorfontoptionsform.cpp

HEADERS += settings.h\
        settings_global.h \
    pascaloptionsform.h \
    configuration.h \
    editorfontoptionsform.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEBBEFDD3
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = settings.dll
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

FORMS += \
    pascaloptionsform.ui \
    editorfontoptionsform.ui

RESOURCES +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../pdutils/release/ -lpdutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../pdutils/debug/ -lpdutils
else:unix: LIBS += -L$$OUT_PWD/../pdutils/ -lpdutils

INCLUDEPATH += $$PWD/../pdutils
DEPENDPATH += $$PWD/../pdutils
