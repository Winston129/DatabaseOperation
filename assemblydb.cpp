#include "assemblydb.h"
#include "callmessagehs.h"


bool AssemblyDB::ConnectAssemblyDB(const QString &path_db)
{
    db_lite = QSqlDatabase::addDatabase("QSQLITE");
    db_lite.setDatabaseName(path_db);

    QString msg_str;

    QFileInfo file_info(path_db);
    if(!file_info.exists() || !file_info.isFile()){
        msg_str = "File '" + path_db + "' not found";
        qDebug() << msg_str << ": " << db_lite.lastError().text();
        call_message_HS.CallMessage(msg_str);
        return false;
    }
    else{
        if(!db_lite.open()){
            msg_str = "Error opening '" + path_db + "' database";
            qDebug() << msg_str << ": " << db_lite.lastError().text();
            call_message_HS.CallMessage(msg_str);
            return false;

        }
        else{
            msg_str = "The database \"" + path_db + "\" has been successfully opened";
            qDebug() << msg_str;
            call_message_HS.CallMessage(msg_str);
            return true;
        }
    }

}


QStringList AssemblyDB::GetTablesName() const
{
    return db_lite.tables(QSql::Tables);
}


QVector<QString> AssemblyDB::GetColumnsTitle(const QString& name_table)
{
    QVector<QString> list_columns_title;

    if(!db_lite.isOpen()){
        qDebug() << "Error: database is NOT open";
        return {};
    }

    if(!db_lite.tables().contains(name_table))
    {
        qDebug() << "Table: " << name_table << " not found";
        return {};
    }

    QSqlQuery query(db_lite);
    QString query_sql=QString("PRAGMA table_info(\"%1\")").arg(name_table);
    if(query.exec(query_sql))
    {
        while(query.next())
        {
            list_columns_title.append(query.value("name").toString());
        }
    }
    else
    {
        qDebug() << "Error open db: " << query.lastError().text();
    }


    return list_columns_title;
}


QVector<QVector<QString>> AssemblyDB::GetElementTable(QString& name_table, const QVector<QString>& list_column_title)
{
    QVector<QVector<QString>> matrix_rows_table;
    QVector<QString> list_rows_table;
    int size_title = list_column_title.length();

    QSqlQuery query(db_lite);
    QString str_query = QString("SELECT * FROM %1").arg(name_table);
    if(query.exec(str_query))
    {
        int count_title=0;
        while(query.next())
        {
            for(QString name_title : list_column_title)
            {
                count_title++;
                if(count_title>=size_title)
                {
                    qDebug() << list_rows_table;
                    matrix_rows_table.append(list_rows_table);
                    count_title=0;
                    list_rows_table={};
                }
                else
                {
                    list_rows_table.append(query.value(name_title).toString());
                }
            }
        }
    }
    else
    {
        qDebug() << "Error open db: " << query.lastError().text();
    }

    return matrix_rows_table;
}


QVector<QString> AssemblyDB::GetAutoincrementElements(const QString& name_table)
{
    QSqlQuery query(db_lite);

    QString str_query = QString("PRAGMA table_info(\"%1\");").arg(name_table);
    int is_pk;
    QVector<QString> list_autoincrement;
    if(query.exec(str_query))
    {
        while(query.next())
        {
            QString title_column_iteration = query.value(1).toString();  //name column
            is_pk = query.value(5).toInt();  // check primary key; if(is_pk>0) is_pk=PK;
            if(is_pk>0)
            {
                list_autoincrement.append(title_column_iteration);
            }
        }

        return list_autoincrement;
    }
    else
    {
        qDebug() << ":(";
        return {};
    }

}




















































