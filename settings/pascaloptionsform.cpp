#include <QPushButton>
#include <QFileDialog>
#include <QDir>

#include "pascaloptionsform.h"
#include "ui_pascaloptionsform.h"

PascalOptionsForm::PascalOptionsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PascalOptionsForm)
{
    ui->setupUi(this);

    connect(ui->m_browseBtn, SIGNAL(clicked()), this, SLOT(setPascalPath()));
}

PascalOptionsForm::~PascalOptionsForm()
{
    delete ui;
}

void PascalOptionsForm::setPascalPath()
{
    QString fpcExt;
#ifdef Q_OS_LINUX
    fpcExt = "fpc*";
#elif defined Q_OS_WIN32
    fpcExt = "fpc.exe";
#endif
    QString pascalPath = QFileDialog::getOpenFileName(this,
        tr("Select fpc Executable"),QDir::homePath(),
        tr("fpc executable (%1)").arg(fpcExt));
    if (!pascalPath.isEmpty())
    {
        ui->m_fpcPathLE->setText(pascalPath);
        emit pascalPathChanged(pascalPath);
    }
}
