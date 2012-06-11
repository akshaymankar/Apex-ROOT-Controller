#include "DB_Operation.h"
#include <iostream>

using namespace std;

QSqlDatabase getMySqlDB()
{
    QSettings dbSettings("db.ini",QSettings::IniFormat);

    if(dbSettings.status()==QSettings::FormatError)
        cout<<"Format Error Occured..!!\n";

    //Read Settings
    QString host(dbSettings.value("host","localhost").toString());
    QString user(dbSettings.value("user","root").toString());
    QString password(dbSettings.value("password","").toString());
    QString database(dbSettings.value("database_name","dbApex").toString());
    int port(dbSettings.value("port",3306).toInt());

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

