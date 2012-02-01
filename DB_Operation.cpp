/*
#ifndef DB_OPERATION
#define DB_OPERATION
#include <QtSql/QSqlDatabase>
void QSqlDatabase getMySqlDB(const QString &host, const QString &user, const QString &password, const QString &database,const QString &port=3306)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setUserName(user);
    db.setPassword(password);
    db.setDatabaseName(database);
    db.setPort(port);
    if(!db.open()) qDebug("Failed to Connect to MySQL");
    return db;
}
#endif
*/
