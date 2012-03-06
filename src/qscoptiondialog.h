#ifndef QSCOPTIONDIALOG_H
#define QSCOPTIONDIALOG_H

#include <QDialog>

class PascalOptionsForm;
class QFont;

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
    void OnCBFontChanged(QFont font);
    void onCBFontSizeChanged(QString pSize);
signals:
    void applyChanges();
private:
    Ui::QscOptionDialog *ui;
    PascalOptionsForm *m_poForm;
};

#endif // QSCOPTIONDIALOG_H
