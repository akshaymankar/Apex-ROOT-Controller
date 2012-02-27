#include "DB_Operation.h"
#include <iostream>

using namespace std;

QSqlDatabase getMySqlDB()
{
    QSettings settings("apex.ini",QSettings::IniFormat);

    if(settings.status()==QSettings::FormatError)
        cout<<"Format Error Occured..!!\n";

    //Read Settings
    QString host(settings.value("host","localhost").toString());
    QString user(settings.value("user","root").toString());
    QString password(settings.value("password","").toString());
    QString database(settings.value("database_name","apex").toString());
    int port(settings.value("port",3306).toInt());

    /*
    cout<<"DB_HOST: "<<host.toAscii().data()<<endl
       <<"DB_User: "<<user.toAscii().data()<<endl
      <<"DB_Name: "<<database.toAscii().data()<<endl
     <<"DB_Password: "<<password.toAscii().data()<<endl;
     */

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setUserName(user);
    db.setPassword(password);
    db.setDatabaseName(database);
    db.setPort(port);
    if(!db.open()) qDebug("Failed to Connect to MySQL..!!");
    return db;
}

