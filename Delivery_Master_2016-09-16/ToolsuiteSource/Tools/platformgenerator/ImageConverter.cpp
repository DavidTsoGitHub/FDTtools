#include "ImageConverter.h"
#include "Logger.h"

#include <QImage>
#include <QTransform>

ImageConverter::ImageConverter()
{

}

ImageConverter::~ImageConverter()
{

}

QString ImageConverter::GenerateImageSource(const QString imagePath, quint32 id, bool rotated, bool storeInExternalFlash)
{
    QString imageName = "Image" + QString::number(id);
    QStringList sourceLines;

    sourceLines.append("/********************************* Includes **********************************/");
    sourceLines.append("#include \"Image.h\"");
    sourceLines.append("");
    sourceLines.append("/**************************** Constants and Types ****************************/");
    sourceLines.append("");

    QImage image;

    image.load(imagePath);
    image = image.convertToFormat(QImage::Format_RGB16, Qt::ColorOnly|Qt::ThresholdDither|Qt::AvoidDither);

    if (rotated)
    {
        QTransform rotateImage;
        rotateImage.rotate(270);
        image = image.transformed(rotateImage);
    }

    int width = image.width();
    int height = image.height();

    QStringList byteList;
    getBytesAsHexString(image, byteList);

    QString pixelArrayString = "";

    sourceLines.append("/*** " + imageName + " ***/");

    if (storeInExternalFlash)
    {
        sourceLines.append("");
        sourceLines.append("#ifndef __WIN32");
        sourceLines.append("#pragma push // Save the current state");
        sourceLines.append("#pragma force_active on");
        sourceLines.append("#pragma section data_type \".CstData\" \".CstData\"");
        sourceLines.append("__declspec (section \".CstData\")");
        sourceLines.append("");
        sourceLines.append("#endif");
    }

    sourceLines.append("");

    sourceLines.append("static const uint16 " + imageName + "Bitmap[] = ");
    sourceLines.append("{");
    for (int i = 0, j = 0; i < byteList.count(); ++i, ++j)
    {
        pixelArrayString.append(byteList[i]);
        if(i < byteList.count()-1)
        {
            pixelArrayString.append(", ");
        }
        if(j > 14 || i >= byteList.count()-1)
        {
            sourceLines.append("    " + pixelArrayString);
            j = -1;
            pixelArrayString = "";
        }
    }
    sourceLines.append("};");
    sourceLines.append("");

    sourceLines.append("const image_t " + imageName + " = {");
    sourceLines.append("    .width = " + QString::number(width) + ",");
    sourceLines.append("    .height = " + QString::number(height) + ",");
    sourceLines.append("    .bitmap = " + imageName + "Bitmap");
    sourceLines.append("};");
    sourceLines.append("");
    
    if (storeInExternalFlash)
    {
        sourceLines.append("#ifndef __WIN32");
        sourceLines.append("#pragma pop");
        sourceLines.append("#endif");
        sourceLines.append("");
    }

    return sourceLines.join("\n");
}

void ImageConverter::getBytesAsHexString(QImage& image, QStringList& byteList)
{
    uchar* bytes = image.bits();
    for(int i = 0; i < image.byteCount(); i += 2 )
    {
        byteList.append("0x" + QString("%1").arg(bytes[i + 1], 2, 16, QChar('0')).toUpper() + QString("%1").arg(bytes[i], 2, 16, QChar('0')).toUpper());
    }
}
