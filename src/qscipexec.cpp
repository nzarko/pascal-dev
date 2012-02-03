#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QtDebug>
#include <QMessageBox>
#include <QTextStream>
#include <QApplication>
#include "qscipexec.h"
#include "qsptextbrowser.h"

QsciPExec::QsciPExec(QObject *parent) :
    QsciPascalBuilder(parent)
{
    connect(&fexec,SIGNAL(readyReadStandardError()),this, SLOT(updateOutputWidget()));
    connect(&fexec,SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutputWidget()));
    connect(&fexec,SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(pexecFinished(int,QProcess::ExitStatus)));
    connect (&fexec, SIGNAL(error(QProcess::ProcessError)), this, SLOT(pexecError(QProcess::ProcessError)));
}

#ifdef Q_OS_WIN32
QString QsciPExec::createWin32RunScript(QString fileToExecute) {
    QFile f("runScriptWin32.bat");
    if (!f.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0L, tr("Error reading"),
                              tr("Can't open %1 file for read").arg(f.fileName()),
                              QMessageBox::Ok);
        return QString();
    }

    QTextStream ts(&f);
    QString all = ts.readAll();
    fileToExecute.replace("/", "\\");
    fileToExecute.prepend("\"");
    fileToExecute.append("\"");
    all.replace("@progname@", fileToExecute);
    QFile runFile("runScriptWin32_cur.bat");
    if (!runFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(0L, tr("Error Writing"),
                              tr("Can't open %1 file for write").arg(f.fileName()),
                              QMessageBox::Ok);
                return QString();
    }
    ts.setDevice(&runFile);
    ts << all << endl;
    QString result = QFileInfo(runFile.fileName()).absoluteFilePath();
    runFile.close();
    return result;
}

size_t QsciPExec::ExecuteProcess(std::wstring FullPathToExe, std::wstring Parameters, size_t SecondsToWait)
{
    size_t iMyCounter = 0, iReturnVal = 0, iPos = 0;
    DWORD dwExitCode = 0;
    std::wstring sTempStr = L"";

    /* - NOTE - You should check here to see if the exe even exists */

    /* Add a space to the beginning of the Parameters */
    if (Parameters.size() != 0)
    {
        if (Parameters[0] != L' ')
        {
            Parameters.insert(0,L" ");
        }
    }

    /* The first parameter needs to be the exe itself */
    sTempStr = FullPathToExe;
    iPos = sTempStr.find_last_of(L"\\");
    sTempStr.erase(0, iPos +1);
    Parameters = sTempStr.append(Parameters);

    /* CreateProcessW can modify Parameters thus we allocate needed memory */
    wchar_t * pwszParam = new wchar_t[Parameters.size() + 1];
    if (pwszParam == 0)
    {
        return 1;
    }
    const wchar_t* pchrTemp = Parameters.c_str();
    wcsncpy(pwszParam, pchrTemp ,Parameters.size() + 1);

    /* CreateProcess API initialization */
    STARTUPINFOW siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));
    siStartupInfo.cb = sizeof(siStartupInfo);

    if (!CreateProcessW(const_cast<LPCWSTR>(FullPathToExe.c_str()),
                        pwszParam, 0, 0, false,
                        CREATE_DEFAULT_ERROR_MODE, 0, 0,
                        &siStartupInfo, &piProcessInfo) != false)
    {
        /* CreateProcess failed */
        iReturnVal = (size_t)GetLastError();
    }
    else
    {
        /* Watch the process. */
        dwExitCode = WaitForSingleObject(piProcessInfo.hProcess, INFINITE);
    }

    /* Free memory */
    delete[]pwszParam;
    pwszParam = 0;

    /* Release handles */
    CloseHandle(piProcessInfo.hProcess);
    CloseHandle(piProcessInfo.hThread);

    return iReturnVal;
}
#endif

void QsciPExec::start()
{
    QStringList args;
    QString command;
    QFileInfo fileInfo(sourcefile);
#ifdef Q_OS_UNIX
    targetfile =fileInfo.absolutePath() + "/" + fileInfo.baseName();
    command = "xterm";
    args  <<  "-e" << "sh";
#endif

#ifdef Q_OS_WIN
    targetfile = fileInfo.absolutePath() + "/" + fileInfo.baseName() + ".exe";
    targetfile.replace("/", "\\");
    command = "C:\\WINDOWS\\SYSTEM32\\cmd.exe";
#endif

    fileInfo.setFile(targetfile);

    //args << createRunScript(fileInfo.absoluteFilePath());

    QTextCharFormat charFormat = browser->currentCharFormat();
    QTextCharFormat newCharFormat;
    newCharFormat.setFontWeight(QFont::Bold);
    browser->setCurrentCharFormat(newCharFormat);
    browser->append(tr("Executing %1").arg(targetfile));
    browser->setCurrentCharFormat(charFormat);

    fexec.setWorkingDirectory(fileInfo.absolutePath());
    if (!QFile::exists(targetfile))
        QsciPascalBuilder::start();
#ifdef Q_OS_WIN
    QString str = createWin32RunScript(fileInfo.absoluteFilePath());
    str.replace("/", "\\");
    str.prepend("\"");
    str.append("\"");
    str.prepend("/k ");
    int res =
            ExecuteProcess(command.toStdWString().c_str(),
                           str.toStdWString().c_str(), INFINITE);
    if (res != 0)
    {
        TCHAR szBuf[80];
        LPVOID lpMsgBuf;
        DWORD dw = DWORD(res);

        FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    dw,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR) &lpMsgBuf,
                    0, NULL );

        wsprintf(szBuf,
                 L"%s failed with error %d: %s",
                 targetfile.toStdWString().c_str(), dw, lpMsgBuf);

        MessageBox(NULL, szBuf, L"Error", MB_OK);

        LocalFree(lpMsgBuf);
        ExitProcess(dw);

    }
    browser->append(
                tr("%1 returned exit code %2").arg(targetfile).arg(QString::number(res)));
    qApp->activeWindow();
#else
    args << createRunScript(fileInfo.absoluteFilePath());

    fexec.start(command,args);
    QString msg = tr("echo \" Executing %1\"").arg(targetfile);
    fexec.write(msg.toLocal8Bit());
    fexec.closeWriteChannel();
#endif
}

void QsciPExec::pexecFinished(int exitCode , QProcess::ExitStatus)
{
    browser->append(
                tr("%1 returned exit code %2").arg(targetfile).arg(QString::number(exitCode))
                );
}

void QsciPExec::updateOutputWidget()
{

}

void QsciPExec::pexecError(QProcess::ProcessError error)
{

}

#ifdef Q_OS_LINUX
QString QsciPExec::createRunScript(QString fileToExecute)
{
    QFile f("scripts/runScript.sh");
    if (!f.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0L, tr("Error reading"),
                              tr("Can't open %1 file for read").arg(f.fileName()),
                              QMessageBox::Ok);
        return QString();
    }

    QTextStream ts(&f);
    QString all = ts.readAll();
    fileToExecute.prepend("\"");
    fileToExecute.append("\"");
    all.replace("@progname@", fileToExecute);
    QFile runFile("runScriptLinux.sh");
    if (!runFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(0L, tr("Error Writing"),
                              tr("Can't open %1 file for write").arg(f.fileName()),
                              QMessageBox::Ok);
                return QString();
    }
    ts.setDevice(&runFile);
    ts << all<< endl;
    QString result = QFileInfo(runFile.fileName()).absoluteFilePath();
    runFile.close();
    return result;
}
#endif

