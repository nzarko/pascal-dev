
#include "qspeditor.h"

#include <QFont>
#include <QFontMetrics>
#include <QFile>
#include <QTextStream>
#include <QtGui/QMessageBox>
#include <Qsci/qscilexerpascal.h>
#include <Qsci/qsciprinter.h>
#include <Qsci/qsciapis.h>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QtDebug>

QSPEditor::QSPEditor(QWidget *parent) :
    QsciScintilla(parent)
{
    setUtf8(true);
    QFont m_font;
    m_font.setFamily("Monospace");
    m_font.setFixedPitch(true);
    m_font.setPointSize(11);

    QFontMetrics m_fm (m_font);

    this->setFont(m_font);
    this->setMarginsFont(m_font);

    this->setMarginWidth(0, m_fm.width("00000")+ 5);
    this->setMarginLineNumbers(0,true);

    //Clickable margin 1 for showing markers
    this->setMarginSensitivity(1, true);
    connect(this,SIGNAL(marginClicked(int,int,Qt::KeyboardModifiers)),
            this, SLOT(onMarginClicked(int,int,Qt::KeyboardModifiers)));
    this->markerDefine(QsciScintilla::RightArrow, ARROW_MARKER_NUM);
    this->setMarkerBackgroundColor(QColor("#ee1111"),ARROW_MARKER_NUM);

    this->setEdgeMode(QsciScintilla::EdgeLine);
    this->setEdgeColumn(120);
    this->setEdgeColor(QColor("#FF0000"));


    this->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    this->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    this->setCaretLineVisible(true);
    this->setCaretLineBackgroundColor(QColor("#F1EFD8"));

    this->setMarginsBackgroundColor(QColor("#333333"));
    this->setMarginsForegroundColor(QColor("#CCCCCC"));

    this->setFoldMarginColors(QColor("#99CC66"),QColor("#333300"));

    this->setIndentationWidth(4);

    m_lexer = new QsciLexerPascal();
    m_lexer->setDefaultFont(m_font);
    m_lexer->setAutoIndentStyle(QsciScintilla::AiOpening | QsciScintilla::AiClosing);
    const char *keywords = m_lexer->keywords(0);
    qDebug() << "Pascal keywords : " << keywords << endl;
    this->setLexer(m_lexer);
    //this->SendScintilla(QsciScintilla::SCI_STYLESETFONT,1,"Courier");
    loadApis(m_lexer);

    this->setAutoIndent(true);
    this->setBackspaceUnindents(true);
    this->setAutoCompletionThreshold(1);
    this->setAutoCompletionSource(QsciScintilla::AcsAPIs);
}

void QSPEditor::print()
{
    QsciPrinter prn;
    QPrintDialog dlg(&prn, this);
    if (dlg.exec() == QDialog::Accepted) {
        prn.setWrapMode(QsciScintilla::WrapWord);

        int line1(-1), line2(-1), col1(-1), col2(-1);
        //        JuffScintilla* edit = getActiveEdit();
        //        if ( edit ) {
        //            QsciLexer* lexer = edit->lexer();
        //            if ( !PrintSettings::keepBgColor() ) {
        //                lexer->setDefaultPaper(Qt::white);
        //                lexer->setPaper(Qt::white);
        //                lexer->setDefaultColor(Qt::black);
        //            }
        //            if ( !PrintSettings::keepColors() ) {
        //                lexer->setColor(Qt::black);
        //            }
        getSelection(&line1, &col1, &line2, &col2);
        if (line1 >=0 && line2 >= 0 && col1 >= 0 && col2 >= 0) {
            //	We have selection. Print it.

            --line2;
            prn.printRange(this, line1, line2);
        }
        else {
            //	We don't have selection. Print the whole text.
            prn.printRange(this, 0);
        }
        //            QFont font = TextDocSettings::font();
        //            LexerStorage::instance()->updateLexers(font);
    }
}

void QSPEditor::print(QsciPrinter *prn)
{
    QPrintDialog dlg(prn, this);
    if (dlg.exec() == QDialog::Accepted) {
        prn->setWrapMode(QsciScintilla::WrapWord);

        int line1(-1), line2(-1), col1(-1), col2(-1);
        //        JuffScintilla* edit = getActiveEdit();
        //        if ( edit ) {
        //            QsciLexer* lexer = edit->lexer();
        //            if ( !PrintSettings::keepBgColor() ) {
        //                lexer->setDefaultPaper(Qt::white);
        //                lexer->setPaper(Qt::white);
        //                lexer->setDefaultColor(Qt::black);
        //            }
        //            if ( !PrintSettings::keepColors() ) {
        //                lexer->setColor(Qt::black);
        //            }
        getSelection(&line1, &col1, &line2, &col2);
        if (line1 >=0 && line2 >= 0 && col1 >= 0 && col2 >= 0) {
            //	We have selection. Print it.

            --line2;
            prn->printRange(this, line1, line2);
        }
        else {
            //	We don't have selection. Print the whole text.
            prn->printRange(this, 0);
        }
        //            QFont font = TextDocSettings::font();
        //            LexerStorage::instance()->updateLexers(font);
    }
}

void QSPEditor::onMarginClicked(int nmargin, int nline,
                                Qt::KeyboardModifiers modifiers)
{
    // Toggle marker for the line the margin was clicked on
    if (markersAtLine(nline) != 0 )
        markerDelete(nline, ARROW_MARKER_NUM);
    else
        markerAdd(nline, ARROW_MARKER_NUM);
}

bool QSPEditor::loadApis(QsciLexerPascal* lexer)
{
    QFile f("pascal.apis");
    if(!f.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0L, tr("Error reading"),
                              tr("Can't open %1 file for read").arg(f.fileName()),
                              QMessageBox::Ok);
        return false;
    }
    QTextStream ts(&f);
    api= new QsciAPIs(m_lexer);
    while(!ts.atEnd())
    {
        QString line = ts.readLine();
        line = line.toLower();
        api->add(line);
    }
    api->prepare();
    f.close();
    return true;

}
