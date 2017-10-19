#ifndef BSPFIXEDPOINTFLOAT_H
#define BSPFIXEDPOINTFLOAT_H

#include <QString>
class BSPFixedPointFloat
{
public:
    typedef enum
    {
        UnknownEndian,
        BigEndian,
        LittleEndian
    } endianess_t;

    BSPFixedPointFloat();
    QString Name;
    QString BaseType;
    QString Unit;
    endianess_t Endianess;
    quint8 SignificantBits;
    quint8 FractionalBits;
};

#endif // BSPFIXEDPOINTFLOAT_H
