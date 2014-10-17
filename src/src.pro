#-------------------------------------------------
#
# Project created by QtCreator 2012-02-01T21:29:39
#
#-------------------------------------------------

QT       += core gui printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    maybesavedialog.cpp \
    fancylineedit.cpp \
    historycompleter.cpp \
    qtcassert.cpp

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
    maybesavedialog.h \
    fancylineedit.h \
    historycompleter.h \
    qtcassert.h

FORMS    += mainwindow.ui \
    qscoptiondialog.ui \
    qscaboutdialog.ui \
    outputwidget.ui \
    maybesavedialog.ui

#win32: LIBS+= User32.lib
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../settings/release/  User32.lib -lsettings -lqscintilla2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../settings/debug/ User32.lib -lsettings -lqscintilla2d
else:symbian: LIBS += -lsettings
else:unix: {
contains(QT_VERSION,  "^5.*") {
LIBS += -L$$OUT_PWD/../settings/ -lsettings -lqt5scintilla2
} else {
LIBS += -L$$OUT_PWD/../settings/ -lsettings -lqscintilla2
}
}

INCLUDEPATH += $$PWD/../settings $$OUT_PWD/../settings
DEPENDPATH += $$PWD/../settings

RESOURCES += \
    resource.qrc

OTHER_FILES += \
    pascal.apis

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../pdebugger/release/ -lpdebugger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../pdebugger/debug/ -lpdebugger
else:symbian: LIBS += -lpdebugger
else:unix: LIBS += -L$$OUT_PWD/../pdebugger/ -lpdebugger

INCLUDEPATH += $$PWD/../pdebugger
DEPENDPATH += $$PWD/../pdebugger
