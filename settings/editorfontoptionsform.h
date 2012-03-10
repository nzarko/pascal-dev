#ifndef EDITORFONTOPTIONSFORM_H
#define EDITORFONTOPTIONSFORM_H
#include "settings_global.h"
#include <QWidget>

namespace Ui {
class EditorFontOptionsForm;
}

class SETTINGSSHARED_EXPORT EditorFontOptionsForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit EditorFontOptionsForm(QWidget *parent = 0);
    ~EditorFontOptionsForm();
    Ui::EditorFontOptionsForm* uiHinst();

    
private:
    Ui::EditorFontOptionsForm *ui;
private slots :
    void OnCBFontChanged(QFont font);
    void onCBFontSizeChanged(QString pSize);
};

#endif // EDITORFONTOPTIONSFORM_H
