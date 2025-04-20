#include "callmessagehs.h"
#include <QWidget>
#include <QMessageBox>

CallMessageHS::CallMessageHS(){}

void CallMessageHS::CallMessage(const QString &message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}
