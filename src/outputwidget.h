#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QTabWidget>
#include <QTextBrowser>
#include "qsperrorlistwidget.h"

class QSPEditor;

namespace Ui {
    class OutputWidget;
}

class OutputWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit OutputWidget(QWidget *parent = 0);
    ~OutputWidget();
    QTextBrowser *compilerWidget();
    QTextBrowser *applicationWidget();
    QSPErrorListWidget *errorWidget();
    void setEditor(QSPEditor *edt);

private:
    Ui::OutputWidget *ui;
    QSPEditor *m_editor;
};

#endif // OUTPUTWIDGET_H
