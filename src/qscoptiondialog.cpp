#include "qscoptiondialog.h"
#include "ui_qscoptiondialog.h"
#include "pascaloptionsform.h"
#include "ui_pascaloptionsform.h"
#include "editorfontoptionsform.h"
#include "ui_editorfontoptionsform.h"
#include "configuration.h"
#include "mainwindow.h"
#include "qspworkspace.h"
#include "qspeditor.h"

#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QFont>
#include <QFontMetrics>
#include <QFontDatabase>
#include <QPushButton>
#include <QtDebug>
#include <Qsci/qscilexer.h>

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
    //m_poForm->uiHinst()->m_fpcPathLE->setText(Config().pascalCompilerExec());

    m_efoForm = new EditorFontOptionsForm(this);
    ui->m_editorTabWidget->addTab(m_efoForm,tr("Fonts & Colors"));
    ui->m_categoriesLW->setCurrentRow(0);
    ui->stackedWidget->setCurrentIndex(0);

    ui->m_titleLabel->setText(ui->m_categoriesLW->currentItem()->text());



    connect(m_poForm, SIGNAL(pascalPathChanged(QString)), this,
            SLOT(onPascalPathChanged(QString)));
    /*connect(ui->m_fontComboBox, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(OnCBFontChanged(QFont)));
    connect(ui->m_fontSizeCB, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(onCBFontSizeChanged(QString)));
            */
    QPushButton *btn = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(btn,SIGNAL(clicked()),this, SLOT(onApplyChanges()));
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
    /*font.setPointSize(ui->m_fontSizeCB->currentText().toInt());

    ui->m_fontPreviewLabel->setFont(font);*/
}

void QscOptionDialog::onCBFontSizeChanged(QString pSize)
{
    /*int s = pSize.toInt();
    QFont font = ui->m_fontPreviewLabel->font();
    font.setPointSize(s);
    ui->m_fontPreviewLabel->setFont(font);*/
}

void QscOptionDialog::accept()
{
    onApplyChanges();
    QDialog::accept();
}

void QscOptionDialog::reject()
{
    QDialog::reject();
}

void QscOptionDialog::onApplyChanges()
{
    Ui::EditorFontOptionsForm *hinstance = m_efoForm->uiHinst();
    Config().setFontSettings(hinstance->m_fontComboBox->currentFont(),
                             hinstance->m_fontSizeCB->currentText().toInt(),
                             hinstance->m_fontZoomSB->value());
    qDebug() << "onApplyChanges() : Font Settings --> " <<
                hinstance->m_fontComboBox->currentFont() <<
            hinstance->m_fontSizeCB->currentText().toInt() <<
            hinstance->m_fontZoomSB->value() << endl;

    Ui::PascalOptionsForm *pHinst  = m_poForm->uiHinst();
    Config().setPascalCompilerExec(pHinst->m_fpcPathLE->text());

    Config().write();
    QSpWorkspace* workSpace = MainWindow::instance()->workSpace();
    for(int i = 0; i < workSpace->editors().size(); i++)
    {
        QSPEditor* ed = qobject_cast<QSPEditor*>(workSpace->editors().at(i));
        QFont font(hinstance->m_fontComboBox->currentFont());
        font.setPointSize(hinstance->m_fontSizeCB->currentText().toInt());
        ed->lexer()->setFont(font);
    }
}
