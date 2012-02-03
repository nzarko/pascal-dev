#include "outputwidget.h"
#include "ui_outputwidget.h"
#include "qspeditor.h"

OutputWidget::OutputWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::OutputWidget)
{
    ui->setupUi(this);
    setMinimumHeight(0);
    setCurrentIndex(0);
}

OutputWidget::~OutputWidget()
{
    delete ui;
}

QTextBrowser * OutputWidget::compilerWidget()
{
    return ui->compilerBrowserOutput;
}

QTextBrowser * OutputWidget::applicationWidget()
{
    return ui->appBrowserOutput;
}

QSPErrorListWidget * OutputWidget::errorWidget()
{
    return ui->errorListWidget;
}

void OutputWidget::setEditor(QSPEditor *edt)
{
    m_editor = edt;
}
