#ifndef QSPEDITOR_H
#define QSPEDITOR_H

#define QSCINTILLA_DLL

#include <Qsci/qsciscintilla.h>

class QsciPrinter;
class QsciLexerPascal;
class QsciAPIs;

class QSPEditor : public QsciScintilla
{
    Q_OBJECT
public:
    enum {  ARROW_MARKER_NUM = 8 };
    explicit QSPEditor(QWidget *parent = 0);
    bool loadApis(QsciLexerPascal *lexer);

signals:

public slots:
    void print();
    void print(QsciPrinter* prn);
    void onMarginClicked(int,int, Qt::KeyboardModifiers);
private:
    QsciAPIs* api;
    QsciLexerPascal *m_lexer;
};

#endif // QSPEDITOR_H
