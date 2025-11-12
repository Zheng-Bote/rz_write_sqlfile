#include "sqlite3.hpp"

std::tuple<bool, std::string> SQLite3::openDb(QString &pathToFile)
{
    return (openSQLiteDatabase(pathToFile));
}

int SQLite3::closeDb()
{
    return (closeDatabase());
}

int SQLite3::createTable()
{
    QString query
        = "CREATE TABLE IF NOT EXISTS MyTable(Id INTEGER PRIMARY KEY, Name TEXT, Age INTEGER);";
    return executeQuery(query);
}

int SQLite3::deleteTable()
{
    return 0;
}

int SQLite3::insertData()
{
    QString query = "INSERT INTO MyTable(Name, Age) VALUES('Bote', 60);";
    return executeQuery(query);
}

int SQLite3::selectData()
{
    QString query = "SELECT * FROM MyTable;";
    return executeQuery(query);
}

int SQLite3::selectDataWithField()
{
    QString query = "SELECT * FROM MyTable;";
    return executeQueryWithFields(query);
}

QList<QString> SQLite3::getTableFields(const QString tableName)
{
    QSqlQuery query(*db);
    QString queryString = "SELECT attribute FROM " + tableName + " WHERE active = 1";
    //query.prepare("SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = N'(:tablename)'");
    query.prepare(queryString);

    QList<QString> fieldNames;

    if (!query.exec()) {
        qDebug() << "Error executeQuery:" << query.lastError().text();
        return fieldNames;
    }

    QSqlRecord record = query.record();
    while (query.next()) {
        for (int i = 0; i < record.count(); i++) {
            QSqlField field = record.field(i);
            QString name = field.metaType().name();
            if (name.compare("int") == 0) {
                //qDebug() << "Field name:" << field.name()
                //         << "Value:" << query.value(field.name()).toInt();
            } else {
                fieldNames.append(query.value(field.name()).toString());
            }
        }
    }

    return fieldNames;
}

int SQLite3::updateData()
{
    return 0;
}

int SQLite3::deleteData()
{
    return 0;
}
