#ifndef MAYBESAVEDIALOG_H
#define MAYBESAVEDIALOG_H

#include <QDialog>

namespace Ui {
    class MaybeSaveDialog;
}

/** \class MaybeSaveDialog
  * Core sources and functionality taken from
  * QtCreator-2.4.1 source tree with a few modifications
  * to implemement our job.
  * The dialog is used to allow the user to select witch
  * modified files want to save when the application exits.
  */
class MaybeSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaybeSaveDialog(QWidget *parent = 0);
    explicit MaybeSaveDialog(const QStringList & , QWidget* parent = 0);
    ~MaybeSaveDialog();
    QStringList itemsToSave() const;

private slots:
    void updateSaveButton();
    void collectItemsToSave();
    void discardAll();

private:
    void init();
    Ui::MaybeSaveDialog *ui;
    QStringList m_itemsToSave;
};

#endif // MAYBESAVEDIALOG_H
