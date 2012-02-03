#include <QtDebug>
#include "qspworkspace.h"
#include "qspeditor.h"


QSpWorkspace::QSpWorkspace(QWidget *parent) :
    QTabWidget(parent),index(0), tabsCount(0)
{
//    m_tabBar = new QscTabBar(this);
//    setTabBar(m_tabBar);
    setTabsClosable(true);
    setTabShape(QTabWidget::Rounded);
    setDocumentMode(true);

    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
}

int QSpWorkspace::add()
{
    static int titleNo = 0;
    m_editor = new QSPEditor;
    QString title = tr("Untitled") + QString::number(titleNo + 1) + ".pas";
    m_docModified.append(false);
    m_documents.append(title);
    m_spEditors.append(m_editor);
    qDebug() << m_documents << endl;
    titleNo++;

    int id = addTab(m_editor,title);
    setWindowTitle(title+tr("[*]"));
    tabsCount ++;
    setCurrentIndex(id);
    index = currentIndex();
    connect(m_editor,SIGNAL(modificationChanged(bool)),this,
            SLOT(documentModified(bool)));
    return id;

}

void QSpWorkspace::closeTab(int idx)
{
    if ( m_docModified.value(idx))
        emit documentNeedToBeSaved(idx);
    removeTab(idx);
    m_documents.removeAt(idx);
    m_spEditors.removeAt(idx);

    tabsCount = count();
    if(tabsCount == 0)
        add();
    if (idx == tabsCount)
        setCurrentIndex(tabsCount -1);
    else
        setCurrentIndex(idx);
    index = currentIndex();
    m_docModified.removeAt(idx);
    emit currentFileChanged(m_documents.value(index));

    qDebug() << m_documents << endl;
}

void QSpWorkspace::closeTab(QSPEditor *)
{

}

void QSpWorkspace::onTabChanged(int idx)
{
    QSPEditor *editor= qobject_cast<QSPEditor *>(currentWidget());
    if (editor)
    {
        int l,index;
        editor->getCursorPosition(&l, &index);
        emit cursorPositionChanged(l,index);
    }
    index = currentIndex();
    emit currentFileChanged(m_documents.value(idx));
    qDebug() << "QSpWorkspace::index = " << index
             << "QSpWorkspace::currentIndex() = " << currentIndex() << endl;

}

void QSpWorkspace::documentModified(bool m)
{
    setWindowModified(m);
    m_docModified.replace(currentIndex(),m);
    qDebug() << "Modified : " << m << endl;
}

QString QSpWorkspace::currentDocument()
{
    return m_documents.value(this->currentIndex());
}

QWidget * QSpWorkspace::currentEditor()
{
    return m_spEditors.value(this->currentIndex());
}

void QSpWorkspace::setDocumentModified(int i, bool m)
{
    m_docModified.replace(i,m);
}

void QSpWorkspace::setDocumentsModified(bool m)
{
    QMutableListIterator<bool> iter(m_docModified);
    while(iter.hasNext()) {
        iter.next();
        iter.setValue(m);
        qDebug() << iter.value() << endl;
    }
}
