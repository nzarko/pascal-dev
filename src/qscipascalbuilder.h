#ifndef QSCIPASCALBUILDER_H
#define QSCIPASCALBUILDER_H

#include <QObject>
#include <QProcess>
#include <QWidget>

class QSPEditor;
class QSPTextBrowser;
class QSPErrorListWidget;
class QTreeWidgetItem;

class QsciPascalBuilder : public QObject
{
    Q_OBJECT
public:
    explicit QsciPascalBuilder(QObject *parent = 0);
    void setOutputWidget(QWidget *widget);
    void setErrorListWidget(QWidget *);
    void setSourceFile(const QString &sf);
    void setEditor(QSPEditor *editor) {m_editor = editor;}
    void start();

signals:
    void builderFinished(int);

public slots:
private slots:
     void updateOutputWidget() ;
     void fpcFinished(int, QProcess::ExitStatus);
     void fpcError(QProcess::ProcessError);

private:
    QProcess fpc;
    QSPEditor *m_editor;
    int _errNo; //Number of errors

    void format(const QString &);
protected:
    QString sourcefile;
    QString targetfile;
    QSPTextBrowser *browser;
    QSPErrorListWidget *errorListWidget;
};

#endif // QSCIPASCALBUILDER_H
