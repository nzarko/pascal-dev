#include <QSettings>
#include <QtDebug>

#include "configuration.h"

Configuration::Configuration()
{
    QSettings settings;
    qDebug() <<  settings.applicationName() << "\n" <<
              settings.organizationName() << endl;
}

Configuration& Config() {
    static Configuration conf;
    return conf;
}

void Configuration::read()
{
    QSettings settings;
    //Read of Editor Font Settings
    QFont defaultFont("Courier", 10, QFont::Normal);
    m_editorFont = settings.value("editor/Font", defaultFont).value<QFont>();
    m_editorFontSize = settings.value("editor/fontSize",10).toInt();
    m_editorFontZoom = settings.value("editor/fontZoom", 100).toInt();
    //Show some usefull output
    qDebug() << "Configuration::read() : Font Settings --> " <<
                m_editorFont << m_editorFontSize << m_editorFontZoom << endl;
    //End of Editor Font Settings
}

void Configuration::write() const
{
    QSettings settings;

    //Editor Font Settings
    settings.setValue("editor/Font",m_editorFont);
    settings.setValue("editor/fontSize", m_editorFontSize);
    settings.setValue("editor/fontZoom",m_editorFontZoom);
    //End of Editor Font Settings.
}

void Configuration::setFontSettings(QFont font, int fsize, int fzoom)
{
    m_editorFont = font;
    m_editorFontSize = fsize;
    m_editorFontZoom = fzoom;
}

QFont Configuration::editorFont()
{
     return m_editorFont;
}
