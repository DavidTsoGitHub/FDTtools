#ifndef ICONCONVERTER_H
#define ICONCONVERTER_H

#include <QString>
#include <QList>
#include <QImage>

typedef struct
{
    quint8 width;
    quint8 height;
    quint8 bitDepth;
    quint8 bitPlanes;
    QString bitmap;
    QString pixels;
} IconData;


class IconConverter
{
public:
    static QString GenerateIconHeader(const QString iconPath, quint32 id);
private:
    static void getBytesAsHexString(QImage& image, QStringList& byteList);
    //static void printByteListDebugPicture(QImage& image);
};

#endif // ICONCONVERTER_Hl
