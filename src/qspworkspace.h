#ifndef QSPWORKSPACE_H
#define QSPWORKSPACE_H

#include <QTabWidget>
#include <QStringList>
#include <QList>

class QSPEditor;
class QscTabBar;

class QSpWorkspace : public QTabWidget
{
    Q_OBJECT
public:
    explicit QSpWorkspace(QWidget *parent = 0);
    QStringList &documents() {return m_documents;}
    QString document(int i) { return m_documents.value(i); }
    QString  currentDocument();
    QList<QWidget *> editors() { return m_spEditors; }
    QWidget * editor(int i) { return m_spEditors.value(i); }
    QWidget *currentEditor();
    QList<bool> isDocumentModified() { return m_docModified; }
    void setDocumentModified(int i,bool m);
    void setDocumentsModified(bool m);


signals:
    void cursorPositionChanged(int l, int c);
    void documentNeedToBeSaved(int i);
    void currentFileChanged(const QString& fn);

public slots:
    int add();
    void closeTab(QSPEditor *w);
    void closeTab(int index);
    void onTabChanged(int idx);
    void documentModified(bool);

private:
    QSPEditor *m_editor;
    int tabsCount;
    int index;
    QList<bool> m_docModified;
    QStringList m_documents;
    QList<QWidget *> m_spEditors;
    QscTabBar *m_tabBar;
};

#endif // QSPWORKSPACE_H
