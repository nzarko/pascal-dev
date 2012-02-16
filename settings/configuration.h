#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "settings_global.h"

#include <QString>

class SETTINGSSHARED_EXPORT Configuration
{
public:
    //Read configuration
    void read();
    //Write configuration
    void write() const;

private:
    Configuration();
    Configuration(const Configuration&);

    //private members variables
    QString m_fpcExecutable;
    // allow this function to create one instance
    friend  Configuration& Config();

};

// use this function to access the settings
SETTINGSSHARED_EXPORT Configuration& Config();

#endif // CONFIGURATION_H
