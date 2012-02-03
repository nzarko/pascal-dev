#include "qscoptiondialog.h"
#include "ui_qscoptiondialog.h"
#include "pascaloptionsform.h"

#include <QListWidgetItem>
#include <QVBoxLayout>

QscOptionDialog::QscOptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QscOptionDialog)
{
    ui->setupUi(this);

    //QVBoxLayout *globalLayout = new QVBoxLayout(this);
    QVBoxLayout *pascalVB = new QVBoxLayout(this);
    m_poForm = new PascalOptionsForm(this);
    pascalVB->addWidget(m_poForm);
    ui->m_pascalPage->setLayout(pascalVB);

    ui->m_categoriesLW->setCurrentRow(0);
    ui->stackedWidget->setCurrentIndex(0);
    ui->m_titleLabel->setText(ui->m_categoriesLW->currentItem()->text());

    connect(m_poForm, SIGNAL(pascalPathChanged(QString)), this,
            SLOT(onPascalPathChanged(QString)));
}

QscOptionDialog::~QscOptionDialog()
{
    delete ui;
}

void QscOptionDialog::on_m_categoriesLW_itemClicked(QListWidgetItem *item)
{
    ui->stackedWidget->setCurrentIndex(ui->m_categoriesLW->currentIndex().row());
    ui->m_titleLabel->setText(item->text());
}

void QscOptionDialog::onPascalPathChanged(const QString &)
{
    ///TODO: Implement me!
}
