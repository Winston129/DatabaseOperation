#include "HelperFuncHS.h"

HelperFuncHS::HelperFuncHS()
{

}

QVector<QString> HelperFuncHS::ExcludeListInList(const QVector<QString>& target_list, const QVector<QString>& exclude_list)
{
    QVector<QString> return_list;
    for(QString exclude_itam : exclude_list)
    {
        for(QString target_itam : target_list)
        {
            if(target_itam!=exclude_itam)
            {
                return_list.append(target_itam);
            }
        }
    }

    return return_list;
}
