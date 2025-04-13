#ifndef ASSEMBLYDB_H
#define ASSEMBLYDB_H

#include "callmessagehs.h"
#include <QString>
#include <QtSql>
#include <QStringList>
#include <QDebug>
#include <QFileInfo>
#include <QVector>


class AssemblyDB
{
private:
    QSqlDatabase db_lite;
    CallMessageHS call_message_HS;

public:
    bool ConnectAssemblyDB(const QString &path_db);
    QStringList GetTablesName() const;
    QVector<QString> GetColumnsTitle(const QString& name_table);
};

#endif // ASSEMBLYDB_H
