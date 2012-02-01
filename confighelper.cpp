#include "confighelper.h"

ConfigHelper::ConfigHelper(QString configFile)
{
    /*fileName = configFile;
    file.open(configFile);
    if(file.error())
        qDebug("Error Reading Configuration File.");
        */
}
QString ConfigHelper::getValue(QString key)
{
    //TODO This is hardcoded UserName, Password Setting Make it by reading Config File
    if(key.compare("db_username",Qt::CaseInsensitive))
    {
        return "";
    }
    else if(key.compare("db_password",Qt::CaseInsensitive))
    {
        return "";
    }
    else if(key.compare("db_port",Qt::CaseInsensitive))
    {
        return "";
    }
    else if(key.compare("db_host",Qt::CaseInsensitive))
    {
        return "localhost";
    }
}
