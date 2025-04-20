#include "MainPages.h"

MainPages::MainPages(QStackedWidget* stack_widget)
{
    //PAGES
    QWidget* page_1=new QWidget();
    QWidget* page_2=new QWidget();
    QWidget* page_3=new QWidget();
    QWidget* page_create=new QWidget();

    stack_widget->addWidget(page_1);
    stack_widget->addWidget(page_2);
    stack_widget->addWidget(page_3);
    stack_widget->addWidget(page_create);

    Page1(stack_widget);
}
MainPages::~MainPages(){};


void MainPages::ClearLayoutAndChildren(QWidget* page)
{
    // Удаляем layout, если есть
    if (page->layout()) {
        QLayoutItem* item;
        while ((item = page->layout()->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget(); // удаляем виджет
            }
            delete item; // удаляем сам layout item
        }
        delete page->layout();
    }

    // Перестраховка: удаляем детей напрямую, если какие-то остались
    QList<QWidget*> children = page->findChildren<QWidget*>();
    for (QWidget* child : children) {
        delete child;
    }
}


//PAGE1: DataBase Connect
void MainPages::Page1(QStackedWidget* stack_widget)
{
    QWidget* page_1 = stack_widget->widget(0);

    page_1->setWindowTitle("AssemblyComputer");
    page_1->setMinimumHeight(window_MinimumHeight);
    page_1->setMinimumWidth(window_MinimumWidth);
    QGridLayout* layout_page_1=new QGridLayout(page_1);

    //Lable
    QLabel* description=new QLabel();
    description->setText("Enter DataBase");
    //Input field
    QLineEdit* input_field=new QLineEdit();
    input_field->setPlaceholderText("sub/path/db.sqlite");
    //Button: DataBase Connect
    QPushButton* btn_connect=new QPushButton("Connect", page_1);
    QObject::connect(btn_connect, &QPushButton::clicked, [=](){
        ConnectDB(input_field->text());
    });
    //Button: Go to the page 2
    QPushButton* btn_go_page2=new QPushButton("Go to DataBase", page_1);
    QObject::connect(btn_go_page2, &QPushButton::clicked, [=](){
        GoPage2(stack_widget);
        list_name_db=connect_db.GetTableType("all");
        Page2(stack_widget);
    });


    description->setFixedSize(200, 20);
    input_field->setFixedSize(200, 20);
    btn_connect->setFixedSize(200, 20);
    btn_go_page2->setFixedSize(200, 20);

    layout_page_1->addWidget(description, 0, 0);
    layout_page_1->addWidget(input_field, 1, 0);
    layout_page_1->addWidget(btn_connect, 2, 0);
    layout_page_1->addWidget(btn_go_page2, 3, 0);
}


/*
* PAGE1
? Connecting to the database and Creating a list of "intermediate tables"
*/
void MainPages::ConnectDB(const QString& path_db)
{
    QString msg;

    if(path_db.isEmpty()){
        msg="Enter the path to the database";
        call_message_HS.CallMessage(msg);
    }
    else{
        open_connect=connect_db.ConnectAssemblyDB(path_db);
    }

}


/*
* PAGE1
? Go to the page2
*/
void MainPages::GoPage2(QStackedWidget* stack_widget)
{
    CallMessageHS call_message_HS;
    QString msg;

    if(open_connect){
        stack_widget->setCurrentIndex(1);
    }
    else{
        msg="Not connected to the DataBase";
        call_message_HS.CallMessage(msg);
    }
}


//PAGE2: Get Button all tables
void MainPages::Page2(QStackedWidget* stack_widget)
{
    QWidget* page_2 = stack_widget->widget(1);

    if(page_2->layout()){
        delete page_2->layout();
    }

    page_2->setWindowTitle("AssemblyComputer");
    page_2->setMinimumHeight(window_MinimumHeight);
    page_2->setMinimumWidth(window_MinimumWidth);
    QGridLayout* layout_page=new QGridLayout(page_2);

    int size_name_db=list_name_db.size();
    QVector<QPushButton*> list_button;

    for(int i=0; i < size_name_db; i++){
        qDebug() << i+1 << ": " << list_name_db[i];
        QPushButton* btn = new QPushButton(QString("%1").arg(list_name_db[i]));
        list_button.append(btn);
    }
    for(QPushButton* button_db : list_button){
        layout_page->addWidget(button_db);
    }


    for(QPushButton* button_db : list_button){
        QObject::connect(button_db, &QPushButton::clicked, [this, stack_widget, button_db](){
            GoPage3(stack_widget, button_db);
        });
    }
}


//PAGE2:: Go to the page3 and clear page3
void MainPages::GoPage3(QStackedWidget* stack_widget, QPushButton* button_db)
{
    QWidget* page = stack_widget->widget(2);

    ClearLayoutAndChildren(page);

    stack_widget->setCurrentIndex(2);

    Page3(stack_widget, button_db);
}

//PAGE3: View Table
void MainPages::Page3(QStackedWidget* stack_widget, QPushButton* button_db){
    QWidget* page_3 = stack_widget->widget(2);

    page_3->setWindowTitle("AssemblyComputer");
    page_3->setMinimumHeight(window_MinimumHeight);
    page_3->setMinimumWidth(window_MinimumWidth);
    QGridLayout* layout_page=new QGridLayout(page_3);

    //Button: go to page2
    QPushButton* btn_go_page2=new QPushButton("view all tables", page_3);
    btn_go_page2->setMinimumHeight(50);
    btn_go_page2->setMinimumWidth(100);
    layout_page->addWidget(btn_go_page2, 0, 0);
    QObject::connect(btn_go_page2, &QPushButton::clicked, [this, stack_widget](){
        GoPage2(stack_widget);
    });

    //Table
    QTableWidget* table_widget = new QTableWidget(page_3);
    layout_page->addWidget(table_widget, 1, 0, 1, 4);

    // get column title
    QString name_table=button_db->text();

    QVector<QString> columns_title=connect_db.GetColumnsTitle(name_table);
    QStringList header_labels = QStringList::fromVector(columns_title);

    QVector<QVector<QString>> matrix_rows_table = connect_db.GetElementTable(name_table, columns_title);
    int column_count = matrix_rows_table[0].length();
    int row_count = matrix_rows_table.length();
    table_widget->setColumnCount(column_count);
    table_widget->setRowCount(row_count);
    table_widget->setHorizontalHeaderLabels(header_labels);


    for(int row=0; row<row_count; row++)
    {
        for(int col=0; col<column_count; col++)
        {
            table_widget->setItem(row, col, new QTableWidgetItem(matrix_rows_table[row][col]));
        }
    }


    //Button: create Element
    QPushButton* btn_create_element=new QPushButton("Create Element", page_3);
    btn_create_element->setMinimumHeight(50);
    btn_create_element->setMinimumWidth(100);
    layout_page->addWidget(btn_create_element, 0, 3);
    QObject::connect(btn_create_element, &QPushButton::clicked, [this, stack_widget, button_db, columns_title](){
        PageCreateElement(stack_widget, button_db, columns_title);
    });


}


/*
 * Page for creating an element in a table
*/
void MainPages::PageCreateElement(QStackedWidget* stack_widget, QPushButton* button_db, const QVector<QString>& columns_title){
    QWidget* page_create = stack_widget->widget(3);
    ClearLayoutAndChildren(page_create);

    QString name_table=button_db->text();

    stack_widget->setCurrentIndex(3);

    page_create->setWindowTitle("AssemblyComputer");
    page_create->setMinimumHeight(window_MinimumHeight);
    page_create->setMinimumWidth(window_MinimumWidth);
    QGridLayout* layout_page=new QGridLayout(page_create);


    /*============================*/
    /*=== Button "view tables" ===*/
    /*============================*/
    QPushButton* btn_view_tables=new QPushButton(QString("view \"%1\"").arg(name_table), page_create);
    QObject::connect(btn_view_tables, &QPushButton::clicked, [=](){
        GoPage3(stack_widget, button_db);
    });
    layout_page->addWidget(btn_view_tables, 0, 0);


    /*===================*/
    /*=== Input field ===*/
    /*===================*/
    // Get list Elements table
    qDebug() << "columns title: " << columns_title;
    QVector<QString> autoincrement_title = connect_db.GetAutoincrementElements(name_table);
    qDebug() << "autoincrement columns title: " << autoincrement_title;
    QVector<QString> clean_title = helper_func_HS.ExcludeListInList(columns_title, autoincrement_title);
    qDebug() << "clean title: " << clean_title;

    QVector<QLineEdit*> input_fields;   // List Input Field
    for(QString title : clean_title)
    {
        QLineEdit* input_field=new QLineEdit();
        input_field->setPlaceholderText(title);
        input_fields.append(input_field);
    }
    int size_input_fields = input_fields.length();

    for(int i=0; i<size_input_fields; i++)
    {
        layout_page->addWidget(input_fields[i], i+1, 0, 1, 4);
    }


    /*===============================*/
    /*=== Button "Create element" ===*/
    /*===============================*/
    QPushButton* btn_create_element = new QPushButton("CREATE", page_create);
    layout_page->addWidget(btn_create_element, size_input_fields+2, 0, 1, 4);
    QObject::connect(btn_create_element, &QPushButton::clicked, [this, input_fields, name_table, columns_title](){
        CreateElement(input_fields, name_table, columns_title);
    });
}

void MainPages::CreateElement(QVector<QLineEdit*> input_fields, const QString& name_table, const QVector<QString>& columns_title){
    QVector<QString> query_columns;
    for(QLineEdit* input_field : input_fields)
    {
        query_columns.append(input_field->text());
    }

    // QueryCreateElement(query_columns, name_table, columns_title);
}
