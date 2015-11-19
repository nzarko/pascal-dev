#include "qscipascalbuilder.h"
#include "qspeditor.h"
#include "qsptextbrowser.h"
#include "qsperrorlistwidget.h"
#include "configuration.h"

QsciPascalBuilder::QsciPascalBuilder(QObject *parent) :
    QObject(parent)
{
    sourcefile = "";

    connect(&fpc,SIGNAL(readyReadStandardError()),this, SLOT(updateOutputWidget()));
    connect(&fpc,SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutputWidget()));
    connect(&fpc,SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(fpcFinished(int,QProcess::ExitStatus)));
    connect (&fpc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(fpcError(QProcess::ProcessError)));
}

void QsciPascalBuilder::setOutputWidget(QWidget *widget)
{
        browser = qobject_cast<QSPTextBrowser*>(widget);
}

void QsciPascalBuilder::setSourceFile(const QString &sf)
{
        sourcefile = sf;
}

void QsciPascalBuilder::start()
{
        QString pcommand = Config().pascalCompilerExec();
        QStringList args;
        args /*<<"-veiwnhl"*/ << "-g" << sourcefile;
        fpc.setProcessChannelMode(QProcess::MergedChannels);

        browser->clear();
        QTextCharFormat charFormat = browser->currentCharFormat();
        QTextCharFormat newCharFormat;
        newCharFormat.setFontWeight(QFont::Bold);
        browser->setCurrentCharFormat(newCharFormat);
        browser->append(tr("Start Compiling %1").arg(sourcefile));
        browser->setCurrentCharFormat(charFormat);

        _errNo = 0;

        errorListWidget->clear();
        fpc.start(pcommand, args);

//        if (!fpc.waitForFinished()) {
//                format( fpc.errorString());
//        }else {
//                format( fpc.readAll());
//        }
}

void QsciPascalBuilder::updateOutputWidget()
{
        QProcess::ProcessChannel processChannel = fpc.readChannel();
        QByteArray newData;

        if ( processChannel == QProcess::StandardError)
        {
                newData = fpc.readAllStandardError();
                format(QString::fromLocal8Bit(newData));
        }
        else if ( processChannel == QProcess::StandardOutput)
        {
            while(fpc.canReadLine()) {
                //newData = fpc.readAllStandardOutput();
                newData = fpc.readLine();
                format(QString::fromLocal8Bit(newData));
            }
        }
}

void QsciPascalBuilder::format(const QString &str)
{
        QTextCharFormat currentCharFormat = browser->currentCharFormat();

        QTextCharFormat newCharFormat;
        if ( str.contains(QString("Error"),Qt::CaseInsensitive) ||
                str.contains(QString("Fatal"),Qt::CaseInsensitive) ||
                str.contains(QString("Warning"),Qt::CaseInsensitive)) {
            _errNo ++;
            newCharFormat.setForeground(Qt::red);
            errorListWidget->parseErrorLine(str);
        } else if (str.contains(QString("Hint"),Qt::CaseInsensitive) ||
                   str.contains(QString("Note"),Qt::CaseInsensitive) ) {            
            newCharFormat.setFontWeight(QFont::Bold);
            newCharFormat.setForeground(Qt::black);
            //errorListWidget->parseErrorLine(str);
        } else if ( str.contains(QString("Compiling"), Qt::CaseInsensitive) ||
                   str.contains(QString("Linking"), Qt::CaseInsensitive)) {
            newCharFormat.setForeground(Qt::blue);
        }

        browser->setCurrentCharFormat(newCharFormat);
        QString outStr = str;
        outStr.remove(QChar('\n'),Qt::CaseInsensitive);
        browser->append(outStr);
        browser->setCurrentCharFormat(currentCharFormat);

}

void QsciPascalBuilder::fpcFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
        QTextCharFormat curFormat = browser->currentCharFormat();

        QTextCharFormat newFormat;
        newFormat.setFontWeight(QFont::Bold);
        newFormat.setForeground(Qt::red);
        browser->setCurrentCharFormat(newFormat);
        if (exitStatus == QProcess::CrashExit) {
                browser->append(tr("fpc program crashed!"));
       } else if (exitCode == 0) {
                browser->append(tr("Done."));
        }
       else if ( exitCode != 0)
               browser->append("Compilation Failed");

        browser->setCurrentCharFormat(curFormat);
        emit builderFinished(_errNo);
}

void QsciPascalBuilder::fpcError(QProcess::ProcessError error)
{
        if ( error == QProcess::FailedToStart )
            browser->append(tr("fpc command not found"));
}

void QsciPascalBuilder::setErrorListWidget(QWidget *widget)
{
    errorListWidget = qobject_cast<QSPErrorListWidget *>(widget);
    if ( errorListWidget )
        errorListWidget->setSourceFile(sourcefile);
}
