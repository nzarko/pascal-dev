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
}

void Configuration::write() const
{
}
