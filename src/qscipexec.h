#ifndef QSCIPEXEC_H
#define QSCIPEXEC_H

#ifdef Q_OS_WIN32
#include <qt_windows.h>
#endif

#include <string>

#include "qscipascalbuilder.h"
class QProcess;

class QsciPExec : public QsciPascalBuilder
{
    Q_OBJECT
public:
    explicit QsciPExec(QObject *parent = 0);
    void start();
#ifdef Q_OS_WIN
    QString createWin32RunScript(QString fileToExecute);
    size_t ExecuteProcess(std::wstring FullPathToExe, std::wstring Parameters, size_t SecondsToWait);
#endif
#ifdef Q_OS_LINUX
    // Return the full path of the created run script.
    QString createRunScript(QString fileToExecute);
#endif
private slots:
     void updateOutputWidget() ;
     void pexecFinished(int, QProcess::ExitStatus);
     void pexecError(QProcess::ProcessError);
private :
        QProcess fexec;

signals:

public slots:

};

#endif // QSCIPEXEC_H
