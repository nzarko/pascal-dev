#ifndef QSPERRORLISTWIDGET_H
#define QSPERRORLISTWIDGET_H

#include <QTreeWidget>

class QSPErrorListWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QSPErrorListWidget(QWidget *parent = 0);
    void setSourceFile(const QString &);
    void parseErrorLine(const QString &);

signals:
    void errorClicked(const QString &,int);
public slots:
    void onItemPressed(QTreeWidgetItem *,int);
protected:
private:
    QString m_sourceFile;
    QStringList m_errorList;

};

#endif // QSPERRORLISTWIDGET_H
