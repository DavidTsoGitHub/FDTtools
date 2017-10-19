#ifndef BSPENUM_H
#define BSPENUM_H

#include "BSPEnumItem.h"

#include <QString>
#include <QMap>

class BSPEnum
{
public:
    QString Name;
    QString Description;
    QMap<quint32, BSPEnumItem> EnumItems;

    BSPEnum();
};

#endif // BSPENUM_H
