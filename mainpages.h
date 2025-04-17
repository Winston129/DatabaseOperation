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
#include "assemblydb.h"
#include "callmessagehs.h"
#include "style_main.h"
#include "HelperFuncHS.h"
#include "mainwindow.h"


class MainPages
{
private:
    //CALL MASSEGER
    CallMessageHS call_message_HS;
    //DATABASE
    AssemblyDB connect_db;
    //HELPER FUNTION
    HelperFuncHS helper_func_HS;
    // SIZE WINDOW
    const int window_MinimumHeight=500;
    const int window_MinimumWidth=700;
    //
    bool open_connect=false;
    QStringList list_name_db;

public:
    MainPages(QStackedWidget* stack_widget);
    ~MainPages();

    void ClearLayoutAndChildren(QWidget* page);
    void Page1(QStackedWidget* stack_widget);
    void ConnectDB(const QString& path_db);
    void GoPage2(QStackedWidget* stack_widget);
    void Page2(QStackedWidget* stack_widget);
    void GoPage3(QStackedWidget* stack_widget, QPushButton* button_db);
    void Page3(QStackedWidget* stack_widget, QPushButton* button_db);
    void CreateElement(QStackedWidget* stack_widget, QPushButton* button_db, const QVector<QString>& columns_title);
};

#endif // MAINPAGES_H
