#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QtDebug>

#include "pascaloptionsform.h"
#include "ui_pascaloptionsform.h"
#include "configuration.h"
#include "pdutils.h"

namespace {
QString pascal_executable = "fpc"
        #ifdef Q_OS_LINUX
                            "";
        #else
        ".exe";
#endif
}

PascalOptionsForm::PascalOptionsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PascalOptionsForm)
{
    ui->setupUi(this);

    connect(ui->m_browseBtn, SIGNAL(clicked()), this, SLOT(setPascalPath()));

    //Try to find pascal compiler executable
    PDUtils utils;

    if ( utils.env_prog_exists(pascal_executable))
    {
        m_pascalCompilerExec = pascal_executable;
    }
    else
    {
        m_pascalCompilerExec =  "";
    }

    qDebug() << "Pascal compiler full path : " << m_pascalCompilerExec << endl;

    if (Config().pascalCompilerExec().isEmpty())
        ui->m_fpcPathLE->setText(m_pascalCompilerExec);
    else
        ui->m_fpcPathLE->setText(Config().pascalCompilerExec());
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
