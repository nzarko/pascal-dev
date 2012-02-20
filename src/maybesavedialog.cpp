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
    for ( int i = 0; i< m_files2SaveList.size(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        QFileInfo fi(m_files2SaveList.at(i));
        QString fn = fi.fileName();
        QString fp = fi.canonicalPath();
        item->setText(0,fn);
        item->setText(1,fp);
        item->setData(0,Qt::UserRole,m_files2SaveList.at(i));
        ui->m_treeWidget->addTopLevelItem(item);
    }
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
        connect(btn,SIGNAL(clicked()),this,SLOT(reject()));
    }
    btn = ui->buttonBox->button(QDialogButtonBox::SaveAll);
    if (btn)
        btn->setIcon(QIcon(":/icons/icons/filesaveall.png"));
}
