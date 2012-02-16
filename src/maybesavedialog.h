#ifndef MAYBESAVEDIALOG_H
#define MAYBESAVEDIALOG_H

#include <QDialog>

namespace Ui {
    class MaybeSaveDialog;
}

class MaybeSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaybeSaveDialog(QWidget *parent = 0);
    explicit MaybeSaveDialog(const QStringList & , QWidget* parent = 0);
    ~MaybeSaveDialog();

private:
    void init();
    Ui::MaybeSaveDialog *ui;
    QStringList m_files2SaveList;
};

#endif // MAYBESAVEDIALOG_H
