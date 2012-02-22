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
    m_itemsToSave(flst)
{
    ui->setupUi(this);
     setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
     QPushButton *discardButton = ui->buttonBox->addButton(tr("Do not Save"), QDialogButtonBox::DestructiveRole);
     ui->buttonBox->button(QDialogButtonBox::Save)->setDefault(true);
     ui->buttonBox->button(QDialogButtonBox::Save)->setFocus(Qt::TabFocusReason);
     ui->buttonBox->button(QDialogButtonBox::Save)->setMinimumWidth(130); // bad magic number to avoid resizing of button
     ui->buttonBox->button(QDialogButtonBox::Save)->setIcon(QIcon(":/icons/icons/filesaveall.png"));
    //init();
    for ( int i = 0; i< m_itemsToSave.size(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        QFileInfo fi(m_itemsToSave.at(i));
        QString fn = fi.fileName();
        QString fp = fi.canonicalPath();
        item->setText(0,fn);
        item->setText(1,fp);
        item->setData(0,Qt::UserRole,m_itemsToSave.at(i));
        ui->m_treeWidget->addTopLevelItem(item);
    }

    ui->m_treeWidget->resizeColumnToContents(0);
    ui->m_treeWidget->selectAll();
    updateSaveButton();

    connect(ui->buttonBox->button(QDialogButtonBox::Save), SIGNAL(clicked()),
               this, SLOT(collectItemsToSave()));
    connect(discardButton, SIGNAL(clicked()), this, SLOT(discardAll()));
    connect(ui->m_treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(updateSaveButton()));
}

MaybeSaveDialog::~MaybeSaveDialog()
{
    delete ui;
}

QStringList MaybeSaveDialog::itemsToSave() const
{
    return m_itemsToSave;
}

void MaybeSaveDialog::updateSaveButton()
{
    int count = ui->m_treeWidget->selectedItems().count();
    QPushButton *button = ui->buttonBox->button(QDialogButtonBox::Save);
    if (count == ui->m_treeWidget->topLevelItemCount()) {
        button->setEnabled(true);
        button->setText(tr("Save All"));
    } else if (count == 0) {
        button->setEnabled(false);
        button->setText(tr("Save"));
    } else {
        button->setEnabled(true);
        button->setText(tr("Save Selected"));
    }
}

void MaybeSaveDialog::collectItemsToSave()
{
    m_itemsToSave.clear();
    foreach (QTreeWidgetItem *item, ui->m_treeWidget->selectedItems()) {
        m_itemsToSave.append(item->data(0, Qt::UserRole).value<QString>());
    }
    accept();
}

void MaybeSaveDialog::discardAll()
{
    ui->m_treeWidget->clearSelection();
    collectItemsToSave();
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
