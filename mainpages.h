#ifndef MAINPAGES_H
#define MAINPAGES_H

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


class MainPages
{
private:
    //CALL MASSEGER
    CallMessageHS call_message_HS;
    //DATABASE
    AssemblyDB connect_db;
    // SIZE WINDOW
    const int window_MinimumHeight=500;
    const int window_MinimumWidth=700;
    //
    bool open_connect=false;
    QStringList list_name_db;

public:
    MainPages(QStackedWidget* stack_widget);
    ~MainPages();

    void Page1(QStackedWidget* stack_widget);
    void ConnectDB(const QString& path_db);
    void GoPage2(QStackedWidget* stack_widget);
    void Page2(QStackedWidget* stack_widget);
    void GoPage3(QStackedWidget* stack_widget, QPushButton* button_db);
    void Page3(QStackedWidget* stack_widget, QPushButton* button_db);
    void CreateElement(QStackedWidget* stack_widget, QString name_table, const QVector<QString>& columns_title);
};

#endif // MAINPAGES_H
