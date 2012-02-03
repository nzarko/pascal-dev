#ifndef QSCABOUTDIALOG_H
#define QSCABOUTDIALOG_H

#include <QDialog>
class QMouseEvent;
class QKeyEvent;
namespace Ui {
    class QscAboutDialog;
}

class QscAboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QscAboutDialog(QWidget *parent = 0);
    ~QscAboutDialog();

private:
    Ui::QscAboutDialog *ui;

protected:
    void mouseReleaseEvent(QMouseEvent *);
    void keyReleaseEvent(QKeyEvent *);
};

#endif // QSCABOUTDIALOG_H
