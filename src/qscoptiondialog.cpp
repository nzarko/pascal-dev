#include "qscoptiondialog.h"
#include "ui_qscoptiondialog.h"
#include "pascaloptionsform.h"

#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QFont>
#include <QFontMetrics>
#include <QFontDatabase>

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
    ui->m_fontPreviewLabel->setFont(ui->m_fontComboBox->currentFont());
    QFont font = ui->m_fontComboBox->currentFont();
    font.setStyle(QFont::StyleNormal);
    QFontDatabase f_database;
    QList<int> fontSizes = f_database.pointSizes(font.family(), font.styleName());
    foreach(int s, fontSizes) {
        ui->m_fontSizeCB->addItem(QString::number(s));
    }
    ui->m_fontSizeCB->setCurrentIndex(ui->m_fontSizeCB->findText(QString::number(9)));

    connect(m_poForm, SIGNAL(pascalPathChanged(QString)), this,
            SLOT(onPascalPathChanged(QString)));
    connect(ui->m_fontComboBox, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(OnCBFontChanged(QFont)));
    connect(ui->m_fontSizeCB, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(onCBFontSizeChanged(QString)));
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

void QscOptionDialog::OnCBFontChanged(QFont font)
{
    font.setPointSize(ui->m_fontSizeCB->currentText().toInt());

    ui->m_fontPreviewLabel->setFont(font);
}

void QscOptionDialog::onCBFontSizeChanged(QString pSize)
{
    int s = pSize.toInt();
    QFont font = ui->m_fontPreviewLabel->font();
    font.setPointSize(s);
    ui->m_fontPreviewLabel->setFont(font);
}
