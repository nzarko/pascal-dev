#include <QLabel>
#include <QStatusBar>
#include <QMenu>
#include <QApplication>
#include <QMessageBox>
#include <QSplitter>
#include <QSettings>
#include <QByteArray>
#include <QList>
#include <QtDebug>
#include <QCloseEvent>
#include <QFileInfo>
#include <QFileDialog>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <Qsci/qsciprinter.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qscaboutdialog.h"
#include "qscoptiondialog.h"
#include "qspeditor.h"
#include "qspworkspace.h"
#include "outputwidget.h"
#include "configuration.h"
#include "maybesavedialog.h"
#include "pdebugger.h"

MainWindow* MainWindow::m_pInstance = NULL;

MainWindow* MainWindow::instance()
{
    if ( !m_pInstance )
        m_pInstance = new MainWindow;

    return m_pInstance;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_prefDialog(0),
    m_workingDir(QDir::homePath()),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    m_outWidget = new OutputWidget;
    resize(800,600);
    //m_editor = new QSPEditor(this);

    //ui->tabWidget->addTab(m_editor,tr("Untitled.pas"));
    setWindowIcon(QIcon(":/icons/icons/pascal64.png"));
    m_splitter = new QSplitter(this);
    m_splitter->setOrientation(Qt::Vertical);
    setupAction();
    m_workSpace = new QSpWorkspace;
    //m_workSpace->add();
    newFile();
    QSPEditor *editor = qobject_cast<QSPEditor*>(m_workSpace->currentWidget());
    if(editor) {
        connect(editor, SIGNAL(cursorPositionChanged(int,int)),
                this,SLOT(displayCursorPos(int,int)));
        connect(editor,SIGNAL(modificationChanged(bool)),this,
                SLOT(documentWasModified()));
    }
    setCurrentFile("");

    m_splitter->addWidget(m_workSpace);
    m_splitter->addWidget(m_outWidget);

    setCentralWidget(m_splitter);


    lineLbl = new QLabel(tr("L : 1"));
    colLbl = new QLabel(tr("C : 1)"));
    messageLbl = new QLabel();
    messageLbl->setText(tr("Ready "));
    statusBar()->addWidget(messageLbl);
    statusBar()->addWidget(lineLbl);
    statusBar()->addWidget(colLbl);
    displayCursorPos(1,1);


    readSettings();

    Config().read();

    connect(m_workSpace, SIGNAL(cursorPositionChanged(int,int)),this,
             SLOT(displayCursorPos(int,int)));
    connect(m_workSpace,SIGNAL(currentFileChanged(QString)),
            this, SLOT(setCurrentFile(QString)));
    connect(m_workSpace,SIGNAL(currentFileChanged(QString)),
            this, SLOT(documentWasModified()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupAction()
{
    connect(ui->actionExit,SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_New, SIGNAL(triggered()), this,
            SLOT(newFile()));
    connect(ui->action_Open, SIGNAL(triggered()), this,
            SLOT(open()));
    connect(ui->actionSave, SIGNAL(triggered()), this,
            SLOT(save()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this,
            SLOT(saveAs()));
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(fileprint()));
    ui->actionPrint->setStatusTip(tr("Print's the current document"));

    connect(ui->actionPrint_Preview, SIGNAL(triggered()), this, SLOT(filePrintPreview()));
    ui->actionPrint_Preview->setStatusTip(tr("Preview the contents of current document"));
    ui->actionPrint_Preview->setEnabled(false);

    /* ************************************ *
      Recent File Menu
      * *********************************** */
    ui->menuRecent_Files->removeAction(ui->actionRecent_file1);
    for(int i = 0 ; i < MaxRecentFiles; i++)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this,
                SLOT(openRecentFile()));
        ui->menuRecent_Files->addAction(recentFileActions[i]);
    }
    updateRecentFileActions();

    /* ******************************* *
      Edit Menu
      * ****************************** */
    ui->actionCopy->setStatusTip(tr("Copy the current selection's contents to the "
                                    "clipboard"));
    ui->actionCut->setStatusTip(tr("Cut the current selection's contents to the "
                                   "clipboard"));
    ui->actionPaste->setStatusTip(tr("Paste the clipboard's contents into the current "
                                     "selection"));
    ui->actionDelete->setStatusTip(tr("Removes the selected text."));
    ui->actionSelect_All->setStatusTip(tr("Select the entire document."));
    /* ******************************* *
      Build Menu
      * ****************************** */
    connect(ui->actionBuild, SIGNAL(triggered()), this,
            SLOT(compile()));
    ui->actionBuild->setToolTip(tr("Compile current file."));
    connect(ui->actionRun, SIGNAL(triggered()), this,
            SLOT(buildRun()));
    ui->actionRun->setToolTip(tr("Run current file"));
    //No Gui Build action
    connect(&pbuilder, SIGNAL(builderFinished(int)), this,
            SLOT(showErrorsWidget(int)));
    connect(m_outWidget->errorWidget(), SIGNAL(errorClicked(QString,int)),this,
            SLOT(onErrorClicked(QString,int)));
    /* ******************************* *
      Tools Menu
      * ****************************** */
    ui->actionSettings->setToolTip(tr("Open The Application Option Dialog."));
    connect(ui->actionSettings, SIGNAL(triggered()), this,
            SLOT(showPreferencesDialog()));
    /* ******************************* *
      Help Menu
      * ****************************** */
      connect(ui->actionAbout, SIGNAL(triggered()), this,
               SLOT(about()));
}

void MainWindow::displayCursorPos(int line, int pos)
{
    lineLbl->setText(tr("L : ") +QString::number(line+1));
    colLbl->setText(tr("C : ") +QString::number(pos+1));
}

void MainWindow::newFile()
{
    m_workSpace->add();
    QSPEditor *editor = qobject_cast<QSPEditor*>(m_workSpace->currentEditor());
    if(editor) {
        connect(editor, SIGNAL(cursorPositionChanged(int,int)),
                this,SLOT(displayCursorPos(int,int)));
        connect(editor, SIGNAL(textChanged()),this,
                SLOT(documentWasModified()));
        //connect the new editor with edit actions
        connect(ui->actionCopy,SIGNAL(triggered()),editor,
                SLOT(copy()));
        connect((ui->actionCut),SIGNAL(triggered()),editor,
                SLOT(cut()));
        connect(ui->actionPaste,SIGNAL(triggered()),editor,
                SLOT(paste()));
        connect(ui->actionDelete, SIGNAL(triggered()),editor,
                 SLOT(removeSelectedText()));
        connect(ui->actionSelect_All, SIGNAL(triggered()),editor,
                SLOT(selectAll()));
        ui->actionCopy->setEnabled(false);
        ui->actionCut->setEnabled(false);
        connect(editor,SIGNAL(copyAvailable(bool)),
                ui->actionCopy, SLOT(setEnabled(bool)));
        connect(editor,SIGNAL(copyAvailable(bool)),
                ui->actionCut, SLOT(setEnabled(bool)));
    }
    setCurrentFile("");
    updateRecentFiles(m_workSpace->tabText(m_workSpace->currentIndex()));
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"),m_workingDir,
        tr("All Pascal Files (*.pas *.pp)"));
    if (!fileName.isEmpty())
        loadFile(fileName);
}

bool MainWindow::save()
{
    if (!QFileInfo(curFile).exists()) {
            return saveAs();
    } else {
            return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), m_workingDir,tr("Pascal Source Files (*.pas *.pp)"));
    if (fileName.isEmpty())
            return false;

    if (! (fileName.endsWith(".pas", Qt::CaseInsensitive) || fileName.endsWith(".pp", Qt::CaseInsensitive) ))
            fileName += ".pas";
    m_workingDir = QFileInfo(fileName).absolutePath();
    return saveFile(fileName);
}

bool MainWindow::saveAll()
{
    if (fileToBeSaved.isEmpty())
    {
        qDebug() << "MainWindow::saveAll() --> Nothing to save" << endl;
        return false;
    }
    else
    {
        foreach (const QString &f, fileToBeSaved) {
            QFile file(f);
            if ( !file.exists() )
            {
                saveAs();
                updateRecentFiles(curFile);
            }
            else
            {
                if (!file.open(QFile::WriteOnly | QFile::Text)) {
                    QMessageBox::warning(this, tr("Pascal-Dev"),
                                         tr("Cannot write file %1:\n%2.")
                                         .arg(f)
                                         .arg(file.errorString()));
                    return false;
                }

                QTextStream out(&file);
#ifndef QT_NO_CURSOR
                QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
                int index = m_workSpace->documents().indexOf(f);
                if ( index != -1 )
                {
                    QSPEditor *editor = qobject_cast<QSPEditor*>(m_workSpace->editor(index));
                    if (editor)
                        out << editor->text();
#ifndef QT_NO_CURSOR
                    QApplication::restoreOverrideCursor();
#endif
                    qDebug() << "File :" << f << " Saved" << endl;
                }
                file.close();
            }
        }

        return true;
    }
}

void MainWindow::about()
{
    QscAboutDialog *aboutDlg = new QscAboutDialog(this);
    aboutDlg->exec();

    delete aboutDlg;
}

void MainWindow::documentWasModified()
{
    if (currentEditor())
        setWindowModified(currentEditor()->isModified());
    else
        setWindowModified(false);
}

void MainWindow::readSettings()
{
    QSettings settings("Algorithmos", "Pascal-Dev");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    m_splitter->restoreState(settings.value("splitterSizes").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("Algorithmos", "Pascal-Dev");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("splitterSizes", m_splitter->saveState());
    settings.setValue("windowState", saveState());
    Config().read();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        saveAll();
        writeSettings();
        Config().write();
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::maybeSave()
{
    fileToBeSaved.clear();
    for(int i = 0; i < m_workSpace->isDocumentModified().size(); i++)
    {
        if ( m_workSpace->isDocumentModified().at(i))
            fileToBeSaved.append(m_workSpace->documents().at(i));
    }
    if (!fileToBeSaved.isEmpty())
    {
        MaybeSaveDialog msDialog(fileToBeSaved);
        if (msDialog.exec() == QDialog::Accepted)
        {
            fileToBeSaved.clear();
            fileToBeSaved = msDialog.itemsToSave();
            return true;
        }
        else return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Pascal-Dev"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    int isAlreadyOpenId = m_workSpace->documents().indexOf(fileName);
    if ( isAlreadyOpenId != -1)
        m_workSpace->setCurrentIndex(isAlreadyOpenId);
    else {
        int id = m_workSpace->add();
        m_workSpace->setTabText(id, strippedName(fileName));
        currentEditor()->setText(in.readAll());
        m_workSpace->documents().replace(id,fileName);
        connect(currentEditor(),SIGNAL(cursorPositionChanged(int,int)),
                this,SLOT(displayCursorPos(int,int)));
        connect(currentEditor(), SIGNAL(textChanged()), this, SLOT(documentWasModified()));
    }
    //editor->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    updateRecentFiles(fileName);
    this->messageLbl->setText(tr("File loaded"));
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Pascal-Dev"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    QSPEditor *editor = currentEditor();
    if (editor)
        out << editor->text();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    editor->setModified(false);
    setCurrentFile(fileName);
    m_workSpace->documents().replace(m_workSpace->currentIndex(),fileName);
    m_workSpace->setTabText(m_workSpace->currentIndex(),
                            QFileInfo(fileName).fileName());
    editor->setModified(false);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    if ( currentEditor() )
        setWindowModified(/*currentEditor()->isModified()*/false);

    QString shownName = QFileInfo(fileName).fileName();
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Pascal-Dev")));
    setWindowFilePath(curFile);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QSPEditor * MainWindow::currentEditor()
{
    QSPEditor *editor = qobject_cast<QSPEditor*>(m_workSpace->currentEditor());
    if(editor)
        return editor;
    else
    {
        /*QMessageBox::critical(this,tr("Error !"),
                              tr("Could not get the appropriate editor pointer!"),
                              QMessageBox::Ok); */
        return 0;
    }
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(action) {
        loadFile(action->data().toString());
        if(currentEditor())
            currentEditor()->setModified(false);
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings("Algorithmos", "Pascal-Dev");
    recentFiles = settings.value("recentFileList").toStringList();

    //  int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);
    QMutableStringListIterator i(recentFiles);
    while(i.hasNext()) {
        if(!QFile::exists(i.next()))
            i.remove();
    }

    for(int j = 0; j < MaxRecentFiles;  ++j) {
        if ( j < recentFiles.count()) {
            QString text = tr("%1").arg(recentFiles[j]);
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        } else {
            recentFileActions[j]->setVisible(false);
        }
    }
}

void MainWindow::updateRecentFiles(const QString &fileName)
{
    QSettings settings("Algorithmos", "Pascal-Dev");
    recentFiles = settings.value("recentFileList").toStringList();
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    while (recentFiles.size() > MaxRecentFiles)
        recentFiles.removeLast();

    settings.setValue("recentFileList", recentFiles);
    updateRecentFileActions();
}

void MainWindow::fileprint()
{
    QSPEditor *editor = currentEditor();
    if(editor) {
        editor->print();
    }
}

void MainWindow::filePrintPreview()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    preview.setWindowTitle(tr("Print preview for : %1").arg(curFile));
    connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printPreview(QPrinter *)));
    preview.exec();
#endif
}

void MainWindow::printPreview(QPrinter *printer)
{
    QsciPrinter *sciPrinter = (QsciPrinter *)printer;
    if(sciPrinter) {
#ifdef QT_NO_PRINTER
    Q_UNUSED(sciPrinter);
#else
    currentEditor()->print(sciPrinter);
#endif
    }
}

void MainWindow::build()
{
}

void MainWindow::compile()
{
    if ( !QFileInfo(curFile).exists())
    {
        int ret = QMessageBox::question(this,tr("Save File"),
                                        tr("The file must be saved before compile.\n "
                                           "Do you want to save file? "), QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes )
            if ( !save()) return;
        else
            return;
    }
    m_outWidget->setCurrentIndex(MainWindow::CompilerOutput);
    pbuilder.setSourceFile(curFile);
    pbuilder.setOutputWidget(this->m_outWidget->compilerWidget());
    pbuilder.setErrorListWidget(m_outWidget->errorWidget());
    pbuilder.setEditor(currentEditor());
    pbuilder.start();
}

void MainWindow::buildRun()
{
    m_outWidget->setCurrentIndex(MainWindow::AppOutput);
    pexec.setOutputWidget(this->m_outWidget->applicationWidget());
    pexec.setSourceFile(curFile);
    pexec.start();
}

void MainWindow::showErrorsWidget(int nShow)
{
    if ( nShow != 0)
        m_outWidget->setCurrentIndex(MainWindow::ErrorOutput);

}

void MainWindow::onErrorClicked(const QString &fileName, int line)
{
    loadFile(fileName);
    currentEditor()->setFocus();
    if (line > 0 )
        currentEditor()->setCursorPosition(line -1 ,0);
}

void MainWindow::showPreferencesDialog()
{
    if (m_prefDialog == 0) {
        m_prefDialog = new QscOptionDialog(this);
    }
    if(m_prefDialog->exec() == QDialog::Accepted)
        qDebug()<< "Options Dialog return OK";
}
