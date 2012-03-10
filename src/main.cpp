#include <QtGui/QApplication>
#include "mainwindow.h"
#include "manhattanstyle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString basename = a.style()->objectName();
    a.setStyle(new ManhattanStyle(basename));
    MainWindow *w = MainWindow::instance();
    w->show();

    return a.exec();
}
