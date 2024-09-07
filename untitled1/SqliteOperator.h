#ifndef SQLITEOPERATOR_H
#define SQLITEOPERATOR_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class SqliteOperator
{
public:
    SqliteOperator();

    // 打开数据库
    bool openDb(void);
    // 关闭数据库
    void closeDb(void);
    //通用的执行函数
    void normalExec(QString sql);
    //通用的查询函数，查询到的内容在sqlQuery里面
    void normalQuery(QSqlQuery & sqlQuery, QString sql);
private:
    QSqlDatabase database;// 用于建立和数据库的连接
};

#endif //  SQLITEOPERATOR_H
