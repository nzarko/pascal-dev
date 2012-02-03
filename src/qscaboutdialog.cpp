#include "qscaboutdialog.h"
#include "ui_qscaboutdialog.h"

QscAboutDialog::QscAboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QscAboutDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags wflags = 0;
    //wflags = wflags & ~Qt::WindowMinMaxButtonsHint;
//    wflags |= Qt::Tool | Qt::WindowCloseButtonHint;
//    this->setWindowFlags(wflags);
}

QscAboutDialog::~QscAboutDialog()
{
    delete ui;
}

void QscAboutDialog::mouseReleaseEvent(QMouseEvent *)
{
    accept();
}

void QscAboutDialog::keyReleaseEvent(QKeyEvent *)
{
    accept();
}
