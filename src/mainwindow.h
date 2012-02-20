#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qscipexec.h"

namespace Ui {
    class MainWindow;
}
class QSPEditor;
class QLabel;
class QSpWorkspace;
class QSplitter;
class OutputWidget;
class QscOptionDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    //QSPEditor *m_editor;
    QLabel *lineLbl, *colLbl;
    QLabel *messageLbl;
    QSpWorkspace *m_workSpace;
    QSplitter *m_splitter;
    OutputWidget *m_outWidget;
    QString curFile;
    QsciPascalBuilder pbuilder;
    enum OutputChooser { AppOutput=0, CompilerOutput, ErrorOutput };
    QsciPExec pexec;
    QscOptionDialog *m_prefDialog;

    void setupAction();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);

    QString strippedName(const QString &fullFileName);
    void updateRecentFileActions();
    void updateRecentFiles(const QString &);
    QSPEditor *currentEditor();
    QStringList recentFiles;

    enum { MaxRecentFiles = 20 };
    enum SaveState { SaveAll=0, Discard, Cancel };
    QAction *recentFileActions[MaxRecentFiles];

private slots:
    void displayCursorPos(int line, int pos);
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
    void openRecentFile();
    void setCurrentFile(const QString &fileName);
    void fileprint();
    void filePrintPreview();
    void printPreview(QPrinter *printer);
    void build();
    void compile();
    void buildRun();
    void showErrorsWidget(int nShow);
    void onErrorClicked(const QString &, int);
    void showPreferencesDialog();
};

#endif // MAINWINDOW_H
