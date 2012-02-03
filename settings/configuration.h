#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QString>

class Configuration
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
    friend Configuration& Config();

};

// use this function to access the settings
Configuration& Config();

#endif // CONFIGURATION_H
