#include "LabelManagerGenerator.h"

#include "bspmemory.h"
#include "FontConverter.h"
#include "Label.h"
#include "Logger.h"

#include <QTextStream>

LabelManagerGenerator::LabelManagerGenerator(Project* project) :
    m_project(project),
    m_labelDefinition(project->GetLabelDefinition())
{    
    m_labelManagerHeaderPath = QString(PlatformPaths::CreateServicePath("LabelManager")).append("/LabelManager.h");
    m_labelManagerSourcePath = QString(PlatformPaths::CreateServicePath("LabelManager")).append("/LabelManager.c");
    m_stringDBHeaderPath = QString(PlatformPaths::CreateServicePath("LabelManager")).append("/StringDB.h");
    m_fontRoot = QString(PlatformPaths::CreateServicePath("Fonts"));
}

void LabelManagerGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");
    generateLabelManagerHeader();
    generateLabelManagerSource();
    generateFonts();
    generateStringDBHeader();
}


void LabelManagerGenerator::generateLabelManagerHeader()
{
    QStringList headerLines;
    headerLines.append("#ifndef LABEL_MANAGER_H");
    headerLines.append("#define LABEL_MANAGER_H");
    headerLines.append("");
    headerLines.append("/********************************* Includes **********************************/");
    headerLines.append("#include \"StandardTypes.h\"");
    headerLines.append("#include \"Font.h\"");
    headerLines.append("");
    headerLines.append("/**************************** Constants and Types ****************************/");
    headerLines.append("");
    headerLines.append("#define NUMBER_OF_LANGUAGES " + QString::number(m_labelDefinition.Languages().count()) + "u");
    headerLines.append("");
    headerLines.append("typedef struct");
    headerLines.append("{");
    headerLines.append("    const wchar* text;");
    headerLines.append("    const font_t* font;");
    headerLines.append("    boolean useMaster;");
    headerLines.append("} label_t;");
    headerLines.append("");
    headerLines.append("typedef struct");
    headerLines.append("{");
    headerLines.append("    uint32 labelID;");
    headerLines.append("    label_t master;");
    headerLines.append("    label_t translations[NUMBER_OF_LANGUAGES];");
    headerLines.append("} label_definition_t;");
    headerLines.append("");
    headerLines.append("/***************************** Exported Functions ****************************/");
    headerLines.append("");
    headerLines.append("void LabelManager_SetLanguage(uint8 languageID);");
    headerLines.append("const label_t* LabelManager_GetLabel(uint32 labelID);");
    headerLines.append("const font_t* LabelManager_GetDefaultFont(void);");
    headerLines.append("");
    headerLines.append("#endif");
    headerLines.append("");

    AddGeneratedFile(m_labelManagerHeaderPath, headerLines.join("\n"));
}

void LabelManagerGenerator::generateLabelManagerSource()
{
    QStringList sourceLines;
    sourceLines.append("//lint -save");
    sourceLines.append("/********************************* Includes **********************************/");
    sourceLines.append("#include \"LabelManager.h\"");
    sourceLines.append("#include \"StringDB.h\"");
    sourceLines.append("#include \"fonts.h\"");
    sourceLines.append("/********************************* Variables *********************************/");
    sourceLines.append("static uint8 m_currentLanguage = 0;");
    sourceLines.append("");
    sourceLines.append("/************************* Local Function Prototypes *************************/");
    sourceLines.append("");
    sourceLines.append("/**************************** Constants and Types ****************************/");
    sourceLines.append("#define NUMBER_OF_LABELS " + QString::number(m_labelDefinition.Labels().count()) + "u");
    sourceLines.append("");

    if (m_labelDefinition.Fonts().count() == 0)
    {
        sourceLines.append("// This is a result of there being no fonts defined in the project.");
        sourceLines.append("// The LabelManager requires at least one font to function properly.");
        sourceLines.append("#include <assert.h>");
        sourceLines.append("const label_t* LabelManager_GetLabel(uint32 labelID)");
        sourceLines.append("{");
        sourceLines.append("    m_currentLanguage = 0;");
        sourceLines.append("    return (label_t*)0;");
        sourceLines.append("}");
        AddGeneratedFile(m_labelManagerSourcePath, sourceLines.join("\n"));
        return;
    }

    quint32 defaultFontActualID = m_labelDefinition.GetDefaultFont()->ID;

    sourceLines.append("static const label_t undefinedLabel = ");
    sourceLines.append("{");
    sourceLines.append("    .text = str" + QString::number(generateStringDBEntry("UNDEFINED")) + ",");
    sourceLines.append("    .font = &font" + QString::number(defaultFontActualID) + ",");
    sourceLines.append("    .useMaster = FALSE");
    sourceLines.append("};");
    sourceLines.append("");
    sourceLines.append("//lint -esym(9003, labelDefinitions) [Supressed as we need to separate generated and static code.]");
    sourceLines.append("static const label_definition_t labelDefinitions[NUMBER_OF_LABELS] = ");
    sourceLines.append("{");

    foreach (Label* label, m_labelDefinition.Labels())
    {
        sourceLines.append("    {");
        sourceLines.append("        .labelID = " + QString::number(label->GetID()) + ",");
        sourceLines.append("        .master = ");
        sourceLines.append("        {");
        sourceLines.append("            .text = str" + QString::number(generateStringDBEntry(label->GetMasterText())) + ",");
        sourceLines.append("            .font = &font" + QString::number(defaultFontActualID) + ",");
        sourceLines.append("            .useMaster = FALSE");
        sourceLines.append("        },");
        sourceLines.append("        .translations = ");
        sourceLines.append("        {");

        foreach (QString language, m_labelDefinition.Languages())
        {
            sourceLines.append("            {");
            if (label->HasTranslation(language) && !label->GetTranslation(language)->GetText().isEmpty())
            {
                const Translation* translation = label->GetTranslation(language);
                sourceLines.append("                .text = str" + QString::number(generateStringDBEntry(translation->GetText())) + ",");
                quint32 fontId = translation->GetFontID();
                if (fontId == UndefinedFontID)
                {
                    fontId = defaultFontActualID;
                }
                sourceLines.append("                .font = &font" + QString::number(fontId) + ",");
                sourceLines.append("                .useMaster = FALSE");
            }
            else
            {
                sourceLines.append("                .useMaster = TRUE");
            }
            sourceLines.append("            },");
        }

        sourceLines.append("        }");
        sourceLines.append("    },");
    }

    sourceLines.append("};");

    sourceLines.append("");
    sourceLines.append("/***************************** Exported Functions ****************************/");
    sourceLines.append("void LabelManager_SetLanguage(uint8 languageID)");
    sourceLines.append("{");
    sourceLines.append("    if (languageID >= NUMBER_OF_LANGUAGES)");
    sourceLines.append("    {");
    sourceLines.append("        m_currentLanguage = 0u;");
    sourceLines.append("    }");
    sourceLines.append("    else");
    sourceLines.append("    {");
    sourceLines.append("        m_currentLanguage = languageID;");
    sourceLines.append("    }");
    sourceLines.append("}");
    sourceLines.append("");
    sourceLines.append("const label_t* LabelManager_GetLabel(uint32 labelID)");
    sourceLines.append("{");
    sourceLines.append("    const label_t* returnValue = &undefinedLabel;");
    sourceLines.append("    for (uint32 i = 0u; i < NUMBER_OF_LABELS; ++i)");
    sourceLines.append("    {");
    sourceLines.append("        if (labelDefinitions[i].labelID == labelID)");
    sourceLines.append("        {");
    sourceLines.append("            if (labelDefinitions[i].translations[m_currentLanguage].useMaster == TRUE)");
    sourceLines.append("            {");
    sourceLines.append("                returnValue = &labelDefinitions[i].master;");
    sourceLines.append("            }");
    sourceLines.append("            else");
    sourceLines.append("            {");
    sourceLines.append("                returnValue = &labelDefinitions[i].translations[m_currentLanguage];");
    sourceLines.append("            }");
    sourceLines.append("        }");
    sourceLines.append("    }");
    sourceLines.append("");
    sourceLines.append("    return returnValue;");
    sourceLines.append("}");
    sourceLines.append("");
    sourceLines.append("const font_t* LabelManager_GetDefaultFont(void)");
    sourceLines.append("{");
    sourceLines.append("    return undefinedLabel.font;");
    sourceLines.append("}");
    sourceLines.append("");
    sourceLines.append("//lint -restore");
    sourceLines.append("");

    AddGeneratedFile(m_labelManagerSourcePath, sourceLines.join("\n"));
}

void LabelManagerGenerator::generateStringDBHeader()
{
    QStringList stringLines;
    QStringList stringNameLines;
    QStringList stringDBHeaderLines;

    for (int i = 0; i < m_stringVector.size(); ++i)
    {
        stringLines.append("static const wchar str" + QString::number(i) + "[] = " + m_stringVector.at(i) + ";");
        stringNameLines.append("    str" + QString::number(i) +",");
    }

    stringDBHeaderLines.append("#ifndef STRING_DB_H");
    stringDBHeaderLines.append("#define STRING_DB_H");
    stringDBHeaderLines.append("/**************************** Constants and Types ****************************/");
    stringDBHeaderLines.append("");
    foreach (QString stringLine, stringLines)
    {
        stringDBHeaderLines.append(stringLine);
    }
    stringDBHeaderLines.append("");
    stringDBHeaderLines.append("#endif");
    stringDBHeaderLines.append("");

    AddGeneratedFile(m_stringDBHeaderPath, stringDBHeaderLines.join("\n"));
}

void LabelManagerGenerator::generateFonts()
{
    QStringList fontsHeader;

    fontsHeader.append("#ifndef _FONTS_H");
    fontsHeader.append("#define _FONTS_H");
    fontsHeader.append("");
    fontsHeader.append("/********************************* Includes **********************************/");
    fontsHeader.append("");

    foreach (Font* font, m_labelDefinition.Fonts())
    {
        QString fontFilePath(m_project->GetProjectRoot().absoluteFilePath(font->BDFPath));
        bool storeInExternalFlash;

        if (m_project->GetBSPSpecification().GetMemory()->GetExternalMemorySize() > 0)
        {
            storeInExternalFlash = font->StoreInExternalFlash;
        }
        else
        {
            storeInExternalFlash = false;
        }

        QFile fontFile(fontFilePath);
        if (!fontFile.open(QFile::ReadOnly))
        {
            LOG_ERROR("Couldn't open font file for reading! [" + fontFilePath + "] " + fontFile.errorString());
            continue;
        }

        QSet<quint16> utf16CharsUsedByFont = getUTF16CharactersUsedByFont(font);

        QTextStream fontFileContents(&fontFile);
        QString fontHeader = FontConverter::GenerateFontHeader(*font).toLocal8Bit();
        QString fontSource = FontConverter::GenerateFontSource(fontFileContents.readAll(), *font, utf16CharsUsedByFont, storeInExternalFlash).toLocal8Bit();
        fontFile.close();

        QString headerPath(m_fontRoot);
        headerPath.append("/font" + QString::number(font->ID) + ".h");

        QString sourcePath(m_fontRoot);
        sourcePath.append("/font" + QString::number(font->ID) + ".c");

        AddGeneratedFile(headerPath, fontHeader);
        AddGeneratedFile(sourcePath, fontSource);

        fontsHeader.append("#include \"font" + QString::number(font->ID) + ".h\" // " + font->BDFPath + " - " + font->Description + " - FontID: " + QString::number(font->ID));
    }

    fontsHeader.append("#endif /* _FONTS_H */");
    fontsHeader.append("");
    QString fontsPath(m_fontRoot);
    fontsPath.append("/fonts.h");
    AddGeneratedFile(fontsPath, fontsHeader.join("\n"));
}

void AddStringCharsAsUTF16(QSet<quint16>& collection, QString string)
{
    const ushort* text = string.utf16();
    for (int i=0; i<string.length(); ++i)
    {
        collection.insert(text[i]);
    }
}

QSet<quint16> LabelManagerGenerator::getUTF16CharactersUsedByFont(const Font* font)
{
    QSet<quint16> utf16CharactersUsedByFont;
    QString mandatoryCharacters = m_labelDefinition.GetMandatoryCharacters();

    // Add all printable characters in the ASCII table, for dynamic texts
    foreach (QChar c, mandatoryCharacters)
    {
        quint16 asciiValue = c.unicode();
        utf16CharactersUsedByFont.insert(asciiValue);
    }

    if (m_labelDefinition.GetDefaultFont() == font)
    {
        // Add all master text characters
        foreach (Label* label, m_labelDefinition.Labels())
        {
            AddStringCharsAsUTF16(utf16CharactersUsedByFont, label->GetMasterText());
        }

        AddStringCharsAsUTF16(utf16CharactersUsedByFont, "UNDEFINED");
    }

    // Check all labels
    foreach (Label* label, m_labelDefinition.Labels())
    {
        foreach (Translation* translation, label->Translations())
        {
            if (translation->GetFontID() == font->ID)
            {
                AddStringCharsAsUTF16(utf16CharactersUsedByFont, translation->GetText());
            }
        }
    }

    return utf16CharactersUsedByFont;
}

int LabelManagerGenerator::generateStringDBEntry(QString string)
{
    QString array = "{";
    for (int i = 0; i < string.length(); i++)
    {
        array += "0x" + QString::number(string.at(i).unicode(), 16) + ", ";
    }

    // add a closing null byte and the closing braces:
    array += "0x0000}";

    if (!m_stringVector.contains(array))
    {
        m_stringVector.append(array);
    }

    return m_stringVector.indexOf(array);
}
