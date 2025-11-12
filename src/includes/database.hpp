/**
 * @file database.hpp
 * @author ZHENG Robert (hase-zheng.net)
 * @brief
 * @version 0.2.0
 * @date 2025-11-12
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 */

#pragma once

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QSqlField>
#include <QSqlRecord>

#include <QDebug>

class Database
{
protected:
    QSqlDatabase *db;

    virtual int executeQuery(const QString &queryString);
    virtual int executeQueryWithFields(const QString &queryString);

public:
    virtual std::tuple<bool, std::string> openSQLiteDatabase(const QString &pathToDbFile);
    virtual int closeDatabase();

    virtual int createTable() = 0;
    virtual int deleteTable() = 0;
    virtual int insertData() = 0;
    virtual int selectData() = 0;
    virtual int selectDataWithField() = 0;
    virtual QList<QString> getTableFields(const QString tableName);
    virtual int updateData() = 0;
    virtual int deleteData() = 0;
};
