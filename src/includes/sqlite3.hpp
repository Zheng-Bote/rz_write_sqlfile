#pragma once

#include "database.hpp"

class SQLite3 : public Database
{
public:
    std::tuple<bool, std::string> openDb(QString &pathToFile);

    int closeDb();

    int createTable() override;
    int deleteTable() override;

    int insertData() override;
    int selectData() override;
    int selectDataWithField() override;
    QList<QString> getTableFields(const QString tableName) override;
    int updateData() override;
    int deleteData() override;
};
