#include <QtGui/QPushButton>
#include <QFileInfo>
#include <QDir>


#include "maybesavedialog.h"
#include "ui_maybesavedialog.h"

MaybeSaveDialog::MaybeSaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaybeSaveDialog)
{
    ui->setupUi(this);
    init();
}

MaybeSaveDialog::MaybeSaveDialog(const QStringList &flst, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaybeSaveDialog),
    m_files2SaveList(flst)
{
    ui->setupUi(this);
    init();

}

MaybeSaveDialog::~MaybeSaveDialog()
{
    delete ui;
}

void MaybeSaveDialog::init()
{
    QPushButton *btn = ui->buttonBox->button(QDialogButtonBox::Discard);
    if (btn)
    {
        btn->setText(tr("Don't Save"));
    }
    btn = ui->buttonBox->button(QDialogButtonBox::SaveAll);
    if (btn)
        btn->setIcon(QIcon(":/icons/icons/filesaveall.png"));
}
