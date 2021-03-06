#ifndef PASCALOPTIONSFORM_H
#define PASCALOPTIONSFORM_H

#include "settings_global.h"

#include <QWidget>

namespace Ui {
    class PascalOptionsForm;
}

class SETTINGSSHARED_EXPORT PascalOptionsForm : public QWidget
{
    Q_OBJECT

public:
    explicit PascalOptionsForm(QWidget *parent = 0);
    ~PascalOptionsForm();

    Ui::PascalOptionsForm *uiHinst() { return ui; }

private:
    Ui::PascalOptionsForm *ui;
    QString m_pascalCompilerExec;
private slots:
    void setPascalPath();
signals:
    void pascalPathChanged(const QString&);
};

#endif // PASCALOPTIONSFORM_H
