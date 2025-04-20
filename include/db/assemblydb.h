#ifndef ASSEMBLYDB_H
#define ASSEMBLYDB_H

#include <QString>
#include <QtSql>
#include <QStringList>
#include <QDebug>
#include <QFileInfo>
#include <QVector>
#include "callmessagehs.h"
#include "HelperFuncHS.h"


class AssemblyDB
{
private:
    QSqlDatabase db_lite;
    //CALL MASSEGER
    CallMessageHS call_message_HS;
    //HELPER FUNTION
    HelperFuncHS helper_func_HS;
    //LIST TABLES
    QVector<QString> list_tables;   //! ok
    QVector<QString> list_intermediate_tables;  //! ok
    QVector<QString> list_fill_tables;  //! ok
    QString session_table;  //! ok

    void loadTablesName();
    void LoadInterimTables();
    void LoadFillTables();

public:
    bool ConnectAssemblyDB(const QString &path_db);
    void setTableName(const QString& name_table);
    QVector<QString> GetTableType(const QString type_list_table="all") const;
    QVector<QString> GetColumnsTitle(const QString& name_table);
    QVector<QVector<QString>> GetElementTable(QString& name_table, const QVector<QString>& list_column_title);
    QVector<QString> GetAutoincrementElements(const QString& name_table);
};

#endif // ASSEMBLYDB_H
