#include "FontConverter.h"
#include "Logger.h"

#include <QStringList>
#include <QTextStream>
#include <QList>
#include <QtMath>

#define MAX_NAME_LEN 100
#define MAX_CHAR_SET_SIZE 65536
#define MAX_LINE_LENGTH 500


QString FontConverter::GenerateFontHeader(const Font& font)
{
    QString simplifiedFontName = "font" + QString::number(font.ID);
    QStringList headerLines;
    QString inclusionGuardName(simplifiedFontName.toUpper() + "_H");

    LOG_DEBUG("Converting font '" + simplifiedFontName + "' from " + font.BDFPath);

    headerLines.append("#ifndef " + inclusionGuardName);
    headerLines.append("#define " + inclusionGuardName);
    headerLines.append("");
    headerLines.append("/*** Generated font file for " + font.BDFPath + "***/");
    headerLines.append("/********************************* Includes **********************************/");
    headerLines.append("#include \"Font.h\"");
    headerLines.append("");
    headerLines.append("/**************************** Constants and Types ****************************/");
    headerLines.append("");
    headerLines.append("extern const font_t " + simplifiedFontName + ";");
    headerLines.append("");
    headerLines.append("#endif /* " + inclusionGuardName + "*/");
    headerLines.append("");

    return headerLines.join("\n");
}

QString FontConverter::GenerateFontSource(QString bdfFileContents, const Font& font, const QSet<quint16> utf16CharsUsedByFont, bool storeInExternalFlash)
{
    QString simplifiedFontName = "font" + QString::number(font.ID);
    QList<GlyphData> glyphData;
    QTextStream contents(&bdfFileContents);
    QStringList sourceLines;
    quint32 size = 0;
    quint32 boundingBoxWidth = 0;
    quint32 boundingBoxHeight = 0;

    sourceLines.append("");
    sourceLines.append("/*** Generated font source file for " + font.BDFPath + "***/");
    sourceLines.append("/********************************* Includes **********************************/");
    sourceLines.append("#include \"" + simplifiedFontName + ".h\"");
    sourceLines.append("");
    sourceLines.append("/********************************* Externals **********************************/");

    if (storeInExternalFlash)
    {
        sourceLines.append("");
        sourceLines.append("#ifndef __WIN32");
        sourceLines.append("#pragma push // Save the current state");
        sourceLines.append("#pragma force_active on");
        sourceLines.append("#pragma section data_type \".CstData\" \".CstData\"");
        sourceLines.append("");
        sourceLines.append("#endif");
    }

    while (!contents.atEnd())
    {
        QString line = contents.readLine();

        if (line.startsWith("SIZE"))
        {
            QStringList parts = line.split(QRegExp("\\s+"));
            size = parts.at(1).toInt();
        }

        if (line.startsWith("FONTBOUNDINGBOX"))
        {
            QStringList parts = line.split(QRegExp("\\s+"));
            boundingBoxWidth = parts.at(1).toInt();
            boundingBoxHeight = parts.at(2).toInt();
        }

        if (!line.contains("STARTCHAR")) continue;
        if (line.contains("ENDFONT")) break;

        QString glyphName = line.split(QRegExp("\\s+")).at(1);

        quint32 encoding=0, deviceWidthX=0, deviceWidthY=0, glyphWidth=0, glyphHeight=0;
        qint32 offsetX=0, offsetY=0;

        while (!contents.atEnd())
        {
            line = contents.readLine();
            if (line.contains("ENCODING"))
            {
                bool ok;
                encoding = line.split(QRegExp("\\s+")).at(1).toInt(&ok);
                if (!ok)
                {
                    LOG_ERROR("Error parsing encoding: '" + line + "'");
                }
                if (encoding >= MAX_CHAR_SET_SIZE)
                {
                    LOG_ERROR("Encoding out of bounds: '" + line + "'");
                }
            }
            else if (line.contains("DWIDTH"))
            {
                QStringList parts = line.split(QRegExp("\\s+"));
                deviceWidthX = parts.at(1).toInt();
                deviceWidthY = parts.at(2).toInt();
            }
            else if (line.contains("BBX"))
            {
                QStringList parts = line.split(QRegExp("\\s+"));
                glyphWidth = parts.at(1).toInt();
                glyphHeight = parts.at(2).toInt();
                offsetX = parts.at(3).toInt();
                offsetY = parts.at(4).toInt();
            }
            else if (line.contains("BITMAP"))
            {
                if (utf16CharsUsedByFont.contains(encoding))
                {
                    line = contents.readLine();
                    int glyphLength = 0;
                    QString glyphBitmapName(simplifiedFontName + "_" + QString::number(encoding));

                    QStringList bytes;
                    while (!line.contains("ENDCHAR"))
                    {
                        line = line.trimmed();
                        while (line.length() > 0) {
                            bytes.append("0x" + line.left(2));
                            line.remove(0, 2);
                            ++glyphLength;
                        }
                        line = contents.readLine();
                    }

                    if (bytes.length() < 1)
                    {
                        glyphLength = qCeil((boundingBoxHeight*boundingBoxWidth)/8.0);
                        for (int i = 0; i < glyphLength; ++i)
                        {
                            bytes.append("0x00");
                        }
                    }

                    sourceLines.append("/*** " + glyphName + " ***/");

                    if (storeInExternalFlash)
                    {
                        sourceLines.append("");
                        sourceLines.append("#ifndef __WIN32");
                        sourceLines.append("__declspec (section \".CstData\")");
                        sourceLines.append("#endif");
                    }

                    sourceLines.append("static const uint8 " + glyphBitmapName + "[" + QString::number(glyphLength) + "] = ");
                    sourceLines.append("{");
                    sourceLines.append("    " + bytes.join(", "));

                    sourceLines.append("};");
                    sourceLines.append("");

                    GlyphData gd;
                    gd.encoding = encoding;
                    gd.deviceWidthX = deviceWidthX;
                    gd.deviceWidthY = deviceWidthY;
                    gd.height = glyphHeight;
                    gd.width = glyphWidth;
                    gd.offsetX = offsetX;
                    gd.offsetY = offsetY;
                    gd.boundingWidth = boundingBoxWidth;
                    gd.boundingHeight = boundingBoxHeight;
                    gd.bitmap = glyphBitmapName;
                    glyphData.append(gd);
                }

                break;
            }
        }
    }

    // Generate empty glyph
    sourceLines.append("");
    qint8 emptyGlyphHeight = size;
    qint8 emptyGlyphWidth = size/2;
    qint32 glyphBitmapSize = qCeil((emptyGlyphHeight*emptyGlyphWidth)/8.0);
    sourceLines.append("static const uint8 " + simplifiedFontName + "_emptyGlyph[" + QString::number(glyphBitmapSize) + "] =");
    sourceLines.append("{");
    QStringList bytes;
    for (int i = 0; i < glyphBitmapSize; ++i)
    {
        bytes.append("0xFF");
    }
    sourceLines.append("    " + bytes.join(", "));
    sourceLines.append("};");

    sourceLines.append("");
    sourceLines.append("const font_t " + simplifiedFontName + " = {");
    sourceLines.append("    .size = " + QString::number(size) + ",");
    sourceLines.append("    .linespacing = " + QString::number(font.LineSpacing) + ",");
    sourceLines.append("    .numGlyphs = " + QString::number(glyphData.size()) + ",");
    sourceLines.append("    .emptyGlyph = " + createFontString(0, 0, 0, emptyGlyphWidth, emptyGlyphHeight, emptyGlyphWidth+2, emptyGlyphHeight, emptyGlyphWidth, emptyGlyphHeight, simplifiedFontName + "_emptyGlyph") + ",");
    sourceLines.append("    .glyphs = {");

    QStringList fontGlyphArrayLines;
    for (int i = 0; i < glyphData.size(); i++)
    {
        GlyphData gd = glyphData.at(i);
        if (!gd.bitmap.isEmpty()) {
            QString line;
            line.append("        " + createFontString(gd));
            fontGlyphArrayLines.append(line);
        }
    }

    sourceLines.append(fontGlyphArrayLines.join(",\n"));
    sourceLines.append("    }");
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

QString FontConverter::createFontString(GlyphData& glyph)
{
        return createFontString(glyph.encoding, glyph.offsetX, glyph.offsetY, glyph.width, glyph.height, glyph.deviceWidthX, glyph.deviceWidthY, glyph.boundingWidth, glyph.boundingHeight, glyph.bitmap);
}

QString FontConverter::createFontString(quint16 encoding, qint8 offsetX, qint8 offsetY, quint8 width, quint8 height, qint8 deviceWidthX, qint8 deviceWidthY, quint8 boundingWidth, quint8 boundingHeight, QString bitmapName)
{
    return "{ "
           ".encoding=" + QString::number(encoding) + ", " +
           ".offsetX=" + QString::number(offsetX) + ", " +
           ".offsetY=" + QString::number(offsetY) + ", " +
           ".width=" + QString::number(width) + ", " +
           ".height=" + QString::number(height) + ", " +
           ".deviceWidthX=" + QString::number(deviceWidthX) + ", " +
           ".deviceWidthY=" + QString::number(deviceWidthY) + ", " +
           ".boundingWidth=" + QString::number(boundingWidth) + ", " +
           ".boundingHeight=" + QString::number(boundingHeight) + ", " +
           ".bitmap=" + bitmapName +
           "}";
}
