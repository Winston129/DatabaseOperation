#ifndef HELPERFUNC_H
#define HELPERFUNC_H

#include <QString>
#include <QVector>


class HelperFuncHS
{
public:
    HelperFuncHS();
    QVector<QString> ExcludeListInList(const QVector<QString>& target_list, const QVector<QString>& exclude_list);
};

#endif // HELPERFUNC_H
