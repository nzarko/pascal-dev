#-------------------------------------------------
#
# Project created by QtCreator 2012-02-01T21:29:39
#
#-------------------------------------------------

QT       += core gui

TARGET = pascal-dev
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../settings/release/ -lsettings
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../settings/debug/ -lsettings
else:symbian: LIBS += -lsettings
else:unix: LIBS += -L$$OUT_PWD/../settings/ -lsettings

INCLUDEPATH += $$PWD/../settings
DEPENDPATH += $$PWD/../settings
