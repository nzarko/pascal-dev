#include <QtDebug>
#include "configuration.h"
#include "editorfontoptionsform.h"
#include "ui_editorfontoptionsform.h"

EditorFontOptionsForm::EditorFontOptionsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorFontOptionsForm)
{
    ui->setupUi(this);

    ui->m_fontComboBox->setCurrentFont(Config().editorFont());

    ui->m_fontZoomSB->setValue(Config().editorFontZoom());
    QFont font = ui->m_fontComboBox->currentFont();
    font.setStyle(QFont::StyleNormal);

    QFontDatabase f_database;
    QList<int> fontSizes = f_database.standardSizes();
    foreach(int s, fontSizes) {
        ui->m_fontSizeCB->addItem(QString::number(s));
    }
    //ui->m_fontSizeCB->setCurrentIndex(ui->m_fontSizeCB->findText(QString::number(9)));

    int ci = ui->m_fontSizeCB->findText(QString::number(Config().editorFontSize()));
    if ( ci != -1)
    {
        ui->m_fontSizeCB->setCurrentIndex(ci);
        qDebug() << "EditorFontOptionsForm Constructor : Font Size --> " <<
                    ui->m_fontSizeCB->itemText(ci);
    }

    font.setPointSize(ui->m_fontSizeCB->currentText().toInt());
    ui->m_fontPreviewLabel->setFont(font);

    connect(ui->m_fontComboBox, SIGNAL(currentFontChanged(QFont)),
                this, SLOT(OnCBFontChanged(QFont)));
    connect(ui->m_fontSizeCB, SIGNAL(currentIndexChanged(QString)),
                this, SLOT(onCBFontSizeChanged(QString)));
}

EditorFontOptionsForm::~EditorFontOptionsForm()
{
    delete ui;
}

Ui::EditorFontOptionsForm *EditorFontOptionsForm::uiHinst()
{
    return ui;
}



void EditorFontOptionsForm::OnCBFontChanged(QFont font)
{

    font.setPointSize(ui->m_fontSizeCB->currentText().toInt());

    ui->m_fontPreviewLabel->setFont(font);
}

void EditorFontOptionsForm::onCBFontSizeChanged(QString pSize)
{
    int s = pSize.toInt();
    QFont font = ui->m_fontPreviewLabel->font();
    font.setPointSize(s);
    ui->m_fontPreviewLabel->setFont(font);
}
