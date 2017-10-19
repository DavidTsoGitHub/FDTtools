#ifndef FONTCONVERTER_H
#define FONTCONVERTER_H

#include "Font.h"

#include <QString>
#include <QSet>

typedef struct
{
    qint32 encoding;  /* glyph encoding */
    qint32 offsetX;    /* glyph x offset */
    qint32 offsetY;    /* glyph y offset */
    qint32 width;      /* glyph width in pixels */
    qint32 height;     /* glyph height in pixels */
    qint32 deviceWidthX; /* offset to next glyph, x-axis */
    qint32 deviceWidthY; /* offset to next glyph, y-axis */
    qint32 boundingWidth;
    qint32 boundingHeight;
    QString bitmap;  /* name of raw bitmap data array */
} GlyphData;

class FontConverter
{
public:
    static QString GenerateFontHeader(const Font& font);
    static QString GenerateFontSource(QString bdfFileContents, const Font &font, const QSet<quint16> utf16CharsUsedByFont, bool storeInExternalFlash);

private:
    static QString createFontString(GlyphData& glyph);
    static QString createFontString(quint16 encoding, qint8 offsetX, qint8 offsetY, quint8 width, quint8 height, qint8 deviceWidthX, qint8 deviceWidthY, quint8 boundingWidth, quint8 boundingHeight, QString bitmapName);

};

#endif // FONTCONVERTER_H
