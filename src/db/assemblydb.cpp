#include "assemblydb.h"
#include "callmessagehs.h"


/*
 * Connected with databases
*/
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

            AssemblyDB::loadTablesName();
            AssemblyDB::LoadInterimTables();
            AssemblyDB::LoadFillTables();

            return true;
        }
    }

}

/*
 * Loading a list of tables | this->list_tables
*/
void AssemblyDB::loadTablesName()
{
    this->list_tables = db_lite.tables(QSql::Tables).toVector();
}

/*
 * A list containing all "intermediate tables" in the database is created | this->list_intermediate_tables
*/
void AssemblyDB::LoadInterimTables()
{
    //QVector<QString> this->list_tables
   QVector<QString> tables_two_fk={};
   QVector<QString> tables_interim={};

   QSqlQuery query(db_lite);
   QString query_is_fk=QString("PRAGMA foreign_key_list(\"%1\")");
   QString query_count_tables=QString("PRAGMA table_info(\"%1\")");

    //? Check if the columns are "foreign_key"
    int count_column=0;
    for(QString table : this->list_tables)
    {
        if(query.exec(query_is_fk.arg(table)))
        {
            while(query.next())
            {
                count_column++;
              
                // if(table=="assembly_component"){
                //     qDebug() << table << ":id= " << query.value(0).toString();
                //     qDebug() << table << ":seq= " << query.value(1).toString();
                //     qDebug() << table << ":table= " << query.value(2).toString();
                //     qDebug() << table << ":from= " << query.value(3).toString();
                //     qDebug() << table << ":to= " << query.value(4).toString();
                //     qDebug() << table << ":on_update= " << query.value(5).toString();
                //     qDebug() << table << ":on_delete= " << query.value(6).toString();
                //     qDebug() << table << ":match= " << query.value(7).toString();
                // }
               
            }
        }
        if(count_column==2){
            tables_two_fk.append(table);
        }
        qDebug() << table << " : fk=" << count_column;
        count_column=0;
    }

    //? Column count check
    count_column=0;
    for(QString table : tables_two_fk)
    {
        if(query.exec(query_count_tables.arg(table)))
        {
            while(query.next())
            {
                if(query.value(0)==0 || query.value(0)==1)
                {
                    if(query.value(2)=="integer")
                    {
                        count_column++;
                    }
                }
                /*
                    qDebug() << table << ":cid= " << query.value(0).toString();
                    qDebug() << table << ":name= " << query.value(1).toString();
                    qDebug() << table << ":type= " << query.value(2).toString();
                    qDebug() << table << ":notnull= " << query.value(3).toString();
                    qDebug() << table << ":dflt_value= " << query.value(4).toString();
                    qDebug() << table << ":pk= " << query.value(5).toString();
                */
            }
        }
        if(count_column==2){
            tables_interim.append(table);
        }
        qDebug() << table << " : count column=" << count_column;
        qDebug() << "huy=" << tables_interim;
        count_column=0;
    }


    this->list_intermediate_tables = tables_interim;
}

/*
 * List of tables to be filled in 
*/
void AssemblyDB::LoadFillTables()
{
    this->list_fill_tables = helper_func_HS.ExcludeListInList(this->list_tables, this->list_intermediate_tables);
}


/*
 * Get a list of tables: list_tables, list_intermediate_tables, list_fill_tables;
*/
QVector<QString> AssemblyDB::GetTableType(const QString type_list_table) const
{
    if(type_list_table=="all")
    {
        return list_tables;
    }
    else if(type_list_table=="intermediate")
    {
        return list_intermediate_tables;
    }
    else if(type_list_table=="fill")
    {
        return list_fill_tables;
    }
    else
    {
        return {"а иди ка ты нахуй"};
    }
}


/*
 * Setting the table name for the session | this->session_table
*/
void AssemblyDB::setTableName(const QString& name_table)
{
    this->session_table=name_table;
}


/*
 * I get a list containing all columns header
*/
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


/*
 * I get a list containing all elements in the table
*/
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


/*
 * I get a list containing all autoincrement columns in the table
*/
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

// void AssemblyDB::QueryCreateElement(const QVector<QString> query_columns, const QString& name_table, const QVector<QString>& columns_title)
// {
//     QSqlQuery query(db_lite);

//     //name_table

//     //QString | "title_1, title_2, title_3"
//     QStringList strlist_columns_title = QStringList::fromVector(columns_title);
//     QString str_columns_title = strlist_columns_title.join(", ");

//     //QString | "?, ?, ?"
//     QStringList strlist_question_marks;
//     for(int i=0; i<columns_title.length(); i++) 
//         strlist_question_marks.append("?");
//     QString question_marks = strlist_question_marks.join(", ");
    
//     //QString | Query="INSERT INTO table(title_1, title_2, title_3) VALUES (?, ?, ?)"
//     QString query_insert_into = QString("INSERT INTO 1% (2%) VALUES (3%)").arg(name_table).arg(str_columns_title).arg(question_marks);

//     query.prepare(query_insert_into);
//     for(QString)
//     query.bindValue();
// }
