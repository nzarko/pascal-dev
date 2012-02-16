#-------------------------------------------------
#
# Project created by QtCreator 2012-02-01T21:29:39
#
#-------------------------------------------------

QT       += core gui

TARGET = pascal-dev
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stylehelper.cpp \
    qsptextbrowser.cpp \
    qspworkspace.cpp \
    styleanimator.cpp \
    qsperrorlistwidget.cpp \
    qspeditor.cpp \
    qscoptiondialog.cpp \
    qscipexec.cpp \
    qscipascalbuilder.cpp \
    outputwidget.cpp \
    qscaboutdialog.cpp \
    manhattanstyle.cpp \
    maybesavedialog.cpp

HEADERS  += mainwindow.h \
    stylehelper.h \
    styleanimator.h \
    qsptextbrowser.h \
    qspworkspace.h \
    qsperrorlistwidget.h \
    qspeditor.h \
    qscoptiondialog.h \
    qscipexec.h \
    qscipascalbuilder.h \
    outputwidget.h \
    qscaboutdialog.h \
    manhattanstyle.h \
    maybesavedialog.h

FORMS    += mainwindow.ui \
    qscoptiondialog.ui \
    qscaboutdialog.ui \
    outputwidget.ui \
    maybesavedialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../settings/release/ -lsettings -lqscintilla2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../settings/debug/ -lsettings -lqscintilla2_debug
else:symbian: LIBS += -lsettings
else:unix: LIBS += -L$$OUT_PWD/../settings/ -lsettings -lqscintilla2

INCLUDEPATH += $$PWD/../settings
DEPENDPATH += $$PWD/../settings

RESOURCES += \
    resource.qrc

OTHER_FILES += \
    pascal.apis
