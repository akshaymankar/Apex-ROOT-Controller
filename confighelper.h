#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <QString>
#include <QFile>

class ConfigHelper
{
public:
    explicit ConfigHelper(QString configFile);
    QString getValue(QString key);
private:
    QString fileName;
    QFile file;       //File Descriptor
};

#endif // CONFIGHELPER_H
