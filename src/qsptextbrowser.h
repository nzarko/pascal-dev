#ifndef QSPTEXTBROWSER_H
#define QSPTEXTBROWSER_H

class QMouseEvent;
class QTextLine;
class QTextCursor;
class QSPEditor;

#include <QTextBrowser>

class QSPTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    struct ErrorLine {
        int lineNo;
        int colNo;
    };

    explicit QSPTextBrowser(QWidget *parent = 0);
    QTextLine currentTextLine(const QTextCursor &cursor);
protected :
    QSPEditor *m_editor;
    void mousePressEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent ( QMouseEvent * event );
    void mouseReleaseEvent(QMouseEvent *ev);
signals:
    void errorLineClicked(const ErrorLine &);
    void errorLineClicked(int l,int c);
public slots:

};

#endif // QSPTEXTBROWSER_H
