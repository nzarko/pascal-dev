#include <QApplication>
#include <QtDebug>
#include <QDateTime>
#include <iostream>

#include "mainwindow.h"
#include "manhattanstyle.h"

namespace  {
QString debugMsg = "%{file}:%{line}:%{function} - %{message}";
}

void verboseMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static const char* typeStr[] = {"[   Debug]", "[ Warning]", "[Critical]", "[   Fatal]" };

    if(type <= QtFatalMsg)
    {
        QByteArray localMsg = msg.toLocal8Bit();
        QString contextString(QStringLiteral("(%1, %2, %3)")
                              .arg(context.file)
                              .arg(context.function)
                              .arg(context.line));

        QString timeStr(QDateTime::currentDateTime().toString("dd-MM-yy HH:mm:ss:zzz"));

        std::cerr << timeStr.toLocal8Bit().constData() << " - "
                  << typeStr[type] << " "
                  << contextString.toLocal8Bit().constData() << " "
                  << localMsg.constData() << std::endl;

        if(type == QtFatalMsg)
        {
            abort();
        }
    }
}

int main(int argc, char *argv[])
{
    //qSetMessagePattern(debugMsg);
    qInstallMessageHandler(verboseMessageHandler);
    QApplication a(argc, argv);

    //The bellow info are being used with the default QSettings constructor.
    QCoreApplication::setOrganizationName("Algorithmos");
    QCoreApplication::setOrganizationDomain("online-edu.com");
    QCoreApplication::setApplicationName("Pascal-Dev");

    QString basename = a.style()->objectName();
    a.setStyle(new ManhattanStyle(basename));
    MainWindow *w = MainWindow::instance();
    w->show();

    qSetMessagePattern(debugMsg);

    return a.exec();
}
