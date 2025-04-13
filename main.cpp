#include "mainwindow.h"
#include "assemblydb.h"
#include "callmessagehs.h"
#include "style_main.h"
#include <QApplication>
#include <QStackedWidget>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTableWidget>
//"AssemblyComputer.sqlite"


//CALL MASSEGER
CallMessageHS call_message_HS;
//DATABASE
AssemblyDB connect_db;
//
bool open_connect=false;
QStringList list_name_db;

void Page1(QStackedWidget* stack_widget);
void ConnectDB(const QString& path_db);
void GoPage2(QStackedWidget* stack_widget);
void Page2(QStackedWidget* stack_widget);
void ClikeButtonDB(QPushButton* button_db);
//void Page3(QStackedWidget* stack_widget);
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QStackedWidget* stack_widget=new QStackedWidget();

    //PAGES
    QWidget* page_1=new QWidget();
    QWidget* page_2=new QWidget();
    QWidget* page_3=new QWidget();
    QLabel* lol = new QLabel(page_3);
    lol->setText("LO{");

    stack_widget->addWidget(page_1);
    stack_widget->addWidget(page_2);
    stack_widget->addWidget(page_3);

    Page1(stack_widget);
    Page2(stack_widget);
//    Page3(stack_widget);

    stack_widget->show();
    return app.exec();
}

//PAGE1: DataBase Conect
void Page1(QStackedWidget* stack_widget)
{
    QWidget* page_1 = stack_widget->widget(0);

    page_1->setWindowTitle("AssemblyComputer");
    page_1->setMinimumHeight(500);
    page_1->setMinimumWidth(700);
    QGridLayout* layout_page_1=new QGridLayout(page_1);

    //Lable
    QLabel* description=new QLabel();
    description->setText("Enter DataBase");
    //Input field
    QLineEdit* input_field=new QLineEdit();
    input_field->setPlaceholderText("sub/path/db.sqlite");
    input_field->setMaxLength(30);
    //Button: DataBase Connect
    QPushButton* btn_connect=new QPushButton("Connect", page_1);
    QObject::connect(btn_connect, &QPushButton::clicked, [=](){
        ConnectDB(input_field->text());
    });
    //Button: Go to the page 2
    QPushButton* btn_go_page2=new QPushButton("Go to DataBase", page_1);
    QObject::connect(btn_go_page2, &QPushButton::clicked, [=](){
        GoPage2(stack_widget);
        list_name_db=connect_db.GetTablesName();
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

//PAGE1:: Connect db
void ConnectDB(const QString& path_db)
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

//PAGE1:: Go to the Page_2
void GoPage2(QStackedWidget* stack_widget){
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

//PAGE2:
void Page2(QStackedWidget* stack_widget)
{
    QWidget* page_2 = stack_widget->widget(1);

    if(page_2->layout()){
        delete page_2->layout();
    }

    page_2->setWindowTitle("AssemblyComputer");
    page_2->setMinimumHeight(500);
    page_2->setMinimumWidth(700);
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
        QObject::connect(button_db, &QPushButton::clicked, [button_db](){
            ClikeButtonDB(button_db);
        });
    }
}

void ClikeButtonDB(QPushButton* button_db){
    QString name_table=button_db->text();
    call_message_HS.CallMessage(name_table);

    QVector<QString> columns_title=connect_db.GetColumnsTitle(name_table);
    QString lamfa;
    for(QString title : columns_title){
        lamfa+=title + " | ";
    }
    call_message_HS.CallMessage(lamfa);

}






