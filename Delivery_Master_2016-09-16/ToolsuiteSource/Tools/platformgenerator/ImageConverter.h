#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

#include "Project.h"

#include <QString>
#include <QImage>

class ImageConverter
{
public:
    ImageConverter();
    ~ImageConverter();

    static QString GenerateImageSource(const QString imagePath, quint32 id, bool rotated, bool storeInExternalFlash);

private:
    static void getBytesAsHexString(QImage &image, QStringList &byteList);
};

#endif // IMAGECONVERTER_H
