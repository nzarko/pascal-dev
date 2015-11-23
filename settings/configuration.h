#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "settings_global.h"

#include <QString>
#include <QtGui/QFont>

class SETTINGSSHARED_EXPORT Configuration
{
public:
    //Read configuration
    void read();
    //Write configuration
    void write() const;

    //font configuration
    void setFontSettings(QFont font, int fsize, int fzoom);
    void writeFontSettings();
    void readFontSettings();

    QFont editorFont();
    int editorFontSize() { return m_editorFontSize; }
    int editorFontZoom() { return m_editorFontZoom; }

    void setPascalCompilerExec (const QString &s) { m_pascalCompilerExec = s ; }
    QString &pascalCompilerExec() { return m_pascalCompilerExec; }

private:
    Configuration();
    Configuration(const Configuration&);

    //private members variables
    QString m_fpcExecutable;
    //Editor Font Settings
    QFont m_editorFont;
    int m_editorFontSize;
    int m_editorFontZoom;

    //Pascal compiler executable
    QString m_pascalCompilerExec;

    // allow this function to create one instance
    friend SETTINGSSHARED_EXPORT Configuration& Config();

};

// use this function to access the settings
SETTINGSSHARED_EXPORT Configuration& Config();

#endif // CONFIGURATION_H
