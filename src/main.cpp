#include "mainwindow.h"
#include "assemblydb.h"
#include "callmessagehs.h"
#include "style_main.h"
#include "MainPages.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QStackedWidget* stack_widget=new QStackedWidget();

    MainPages StorkDB(stack_widget);

    stack_widget->show();
    return app.exec();
}


