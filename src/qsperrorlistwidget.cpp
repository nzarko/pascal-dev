#include <QRegExp>
#include <QtDebug>
#include <QFileInfo>
#include "qsperrorlistwidget.h"

QSPErrorListWidget::QSPErrorListWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setAlternatingRowColors(true);
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setWordWrap(true);
    setColumnWidth(0,490);
    QStringList headersList;
    headersList << tr("Message") << tr("Filename") << tr("Line")
                   << tr("Column");
    setColumnCount(4);
    setHeaderLabels(headersList);
    connect(this, SIGNAL(itemPressed(QTreeWidgetItem*,int)),
            SLOT(onItemPressed(QTreeWidgetItem*, int)));
}

void QSPErrorListWidget::onItemPressed(QTreeWidgetItem *item, int /*col */)
{
    bool isOk;
    int line = item->text(2).toInt(&isOk,10);
    if ( isOk)
        emit errorClicked(m_sourceFile, line);
    else
        emit errorClicked(m_sourceFile,0);
}

void QSPErrorListWidget::setSourceFile(const QString &fn)
{
    m_sourceFile = fn;
}

/**
  \fn parseErrorLine(const QString &str)
  \par str: The error string.
  Parse the \i str and extract the line and column number .
  */
void QSPErrorListWidget::parseErrorLine(const QString &str)
{
    QStringList lst;
    QString errorMsg;
    QRegExp re("(\\d{1,2},\\d{1,2})");
    if (str.contains(re)) {
        qDebug() << "Something bad happed" << endl;
        int pos = str.indexOf("(",0);
        int lpos = str.indexOf(")",0);
        qDebug() << pos << ", " << lpos << endl;
        QStringRef err_str = str.midRef(pos+1, lpos - pos -   1);
        qDebug() << err_str << endl;
        QStringList list = err_str.toString().split(",");
        int lineNo, colNo;
        lineNo = list.at(0).toInt();
        colNo = list.at(1).toInt();
        errorMsg = str.right(str.size() - lpos - 1);
//        qDebug() << "Message : " << errorMsg << "Line Number : " << lineNo
//                 << "Filename : " << QFileInfo(m_sourceFile).fileName()
//                 << "Col Number : " << colNo << endl;

        lst << errorMsg << QFileInfo(m_sourceFile).fileName()
            << QString::number(lineNo) << QString::number(colNo);
    } else {
        lst << str << QFileInfo(m_sourceFile).fileName()
               << tr("") << tr("");
    }
    QTreeWidgetItem * item= new QTreeWidgetItem(lst);
    for (int i = 0; i < item->columnCount(); i++)
        item->setTextAlignment(i, Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip(0,str);
    this->addTopLevelItem(item);

}
