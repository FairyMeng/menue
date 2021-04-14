#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>


class DatabaseConnector
{
public:
    DatabaseConnector();
    static bool ConnectDatabase();
    static bool creatingDatabase(QString db_name);
    static QVector<QString> getDatabaseName();
    static bool TableCreated(QString db_name);

};

#endif // DATABASECONNECTOR_H
