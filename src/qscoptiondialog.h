#ifndef QSCOPTIONDIALOG_H
#define QSCOPTIONDIALOG_H

#include <QDialog>

class PascalOptionsForm;

namespace Ui {
    class QscOptionDialog;
}

class QListWidgetItem;

class QscOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QscOptionDialog(QWidget *parent = 0);
    ~QscOptionDialog();

private slots:
    void on_m_categoriesLW_itemClicked(QListWidgetItem *item);
    void onPascalPathChanged(const QString &);

private:
    Ui::QscOptionDialog *ui;
    PascalOptionsForm *m_poForm;
};

#endif // QSCOPTIONDIALOG_H
