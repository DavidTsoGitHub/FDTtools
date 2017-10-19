#include "IconConverter.h"
#include "Logger.h"

#include <QStringList>
#include <QDebug>

QString IconConverter::GenerateIconHeader(const QString iconPath, quint32 id)
{
    QString iconName = "Icon" + QString::number(id);
    QStringList headerLines;
    QString inclusionGuardName(iconName.toUpper() + "_H");

    LOG_DEBUG("Converting Icon '" + iconName + "'");

    headerLines.append("#ifndef " + inclusionGuardName);
    headerLines.append("#define " + inclusionGuardName);
    headerLines.append("");
    headerLines.append("/********************************* Includes **********************************/");
    headerLines.append("#include \"Icon.h\"");
    headerLines.append("");
    headerLines.append("/**************************** Constants and Types ****************************/");
    headerLines.append("");

    QImage image;

    image.load(iconPath);
    image = image.convertToFormat(QImage::Format_MonoLSB, Qt::MonoOnly|Qt::ThresholdDither|Qt::AvoidDither);

    int width = image.width();
    int height = image.height();
    int bitDepth = image.depth();
    int bitPlanes = image.bitPlaneCount();
    int bytesPerLine = image.bytesPerLine();

    QStringList byteList;

    getBytesAsHexString(image, byteList);
    QString pixelArrayString = "";

    headerLines.append("/*** " + iconName + " ***/");
    headerLines.append("static const uint8 " + iconName + "Bitmap[] = ");
    headerLines.append("{");
    for (int i = 0, j = 0; i < byteList.count(); ++i, ++j)
    {
        pixelArrayString.append(byteList[i]);
        if(i < byteList.count()-1)
        {
            pixelArrayString.append(", ");
        }
        if(j > 20 || i >= byteList.count()-1)
        {
            headerLines.append("    " + pixelArrayString);
            j = 0;
            pixelArrayString = "";
        }
    }
    headerLines.append("};");
    headerLines.append("");

    headerLines.append("const icon_t " + iconName + " = {");
    headerLines.append("    .width = " + QString::number(width) + ",");
    headerLines.append("    .height = " + QString::number(height) + ",");
    headerLines.append("    .bitDepth = " + QString::number(bitDepth) + ",");
    headerLines.append("    .bitPlanes = " + QString::number(bitPlanes) + ",");
    headerLines.append("    .bytesPerLine = " + QString::number(bytesPerLine) + ",");
    headerLines.append("    .bitmap = " + iconName + "Bitmap");
    headerLines.append("};");
    headerLines.append("");

    headerLines.append("#endif /* " + inclusionGuardName + "*/");
    headerLines.append("");

    return headerLines.join("\n");
}

void IconConverter::getBytesAsHexString(QImage& image, QStringList& byteList)
{
    uchar* bytes = image.bits();
    for(int i = 0; i < image.byteCount(); ++i )
    {
        byteList.append("0x" + QString::number(bytes[i],16));
    }

    //printByteListDebugPicture(image);
}

//void IconConverter::printByteListDebugPicture(QImage& image)
//{
//    QString pixelString;

//    int width = image.width();
//    int bitDepth = image.depth();
//    int bitPlanes = image.bitPlaneCount();
//    int bytesPerLine = image.bytesPerLine();
//    int byteCount = image.byteCount();

//    uchar* bytes = image.bits();

//    int bytesPerBitplane = bitPlanes / 8;
//    int bytesPerPixel = bitDepth / 8;

//    for(int i = 0; i < byteCount; i += bytesPerLine)
//    {
//        for(int j = 0; j < width; ++j)
//        {
//            int value = 0;
//            if(bytesPerBitplane > 0)
//            {
//                quint8 valueArray[bytesPerBitplane] = {0};
//                for(int h = 0; h < bytesPerBitplane; ++h)
//                {
//                    valueArray[h] = bytes[h+i+(j*bytesPerPixel)];
//                }
//                for(int h = 0; h < bytesPerBitplane; ++h)
//                {
//                    value += valueArray[h];
//                }
//            }
//            else
//            {
//                int bitPosition = j%8;
//                int arrayPosition = i+(j/8);
//                value = (bytes[arrayPosition] >> bitPosition) & 0x01;
//            }
//            pixelString.append(QString::number(value, 16));
//        }
//        pixelString.append("\n");
//    }

//    qDebug() << pixelString;
//}
