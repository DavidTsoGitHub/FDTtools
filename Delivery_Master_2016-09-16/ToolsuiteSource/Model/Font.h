#ifndef FONT
#define FONT

#include <QString>

class Font
{
public:

    quint32 ID;
    QString BDFPath;
    QString Description;
    quint32 MemoryUsage;
    quint8  LineSpacing;
    bool StoreInExternalFlash;
};

#endif // FONT

