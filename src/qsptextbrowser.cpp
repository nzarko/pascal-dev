#include <QMouseEvent>
#include <QTextCursor>
#include <QTextLine>
#include <QTextBlock>
#include <QTextLayout>
#include <QtDebug>
#include <QRegExp>
#include "qsptextbrowser.h"
#include "qspeditor.h"

QSPTextBrowser::QSPTextBrowser(QWidget *parent) :
    m_editor(0),QTextBrowser(parent)
{
}

void QSPTextBrowser::mousePressEvent(QMouseEvent *ev)
{
}

void QSPTextBrowser::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << "Double click happed" << endl;
}

void QSPTextBrowser::mouseReleaseEvent(QMouseEvent *ev)
{
    QTextCursor cursor = cursorForPosition(ev->pos());
    QTextBlock block = cursor.block();
    QString str = block.text();
    qDebug() << "In line " << currentTextLine(cursor).lineNumber()
                << " the text is : " << str;
    QRegExp re("(\\d{1,2},\\d{1,2})");
    if (str.contains(re)) {
        qDebug() << "Something bad happed" << endl;
        int pos = str.indexOf("(",0);
        int lpos = str.indexOf(")",0);
        qDebug() << pos << ", " << lpos << endl;
        QStringRef err_str = str.midRef(pos+1, lpos - pos -   1);
        qDebug() << err_str << endl;
        QStringList list = err_str.toString().split(",");
        ErrorLine el;
        el.lineNo = list.at(0).toInt();
        el.colNo = list.at(1).toInt();
        qDebug() << "Line Numper : " << el.lineNo << "Col Number : " << el.colNo
                    << endl;
        emit errorLineClicked(el.lineNo, el.colNo);

    }
    else {
        cursor.select(QTextCursor::BlockUnderCursor);
        qDebug() << "Should highlight the entire line"<< endl;
    }
}

QTextLine QSPTextBrowser::currentTextLine(const QTextCursor &cursor)
{
    const QTextBlock block = cursor.block();
    if (!block.isValid())
    return QTextLine();

    const QTextLayout *layout = block.layout();
    if (!layout)
    return QTextLine();

    const int relativePos = cursor.position() - block.position();
    return layout->lineForTextPosition(relativePos);
}
