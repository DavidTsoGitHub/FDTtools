#include "LabelDefinition.h"

#include "bspmemory.h"
#include "Font.h"
#include "Label.h"
#include "Logger.h"
#include "Project.h"
#include "ProjectObjectVisitor.h"

#include <QTextStream>
#include <QtMath>
#include <QTime>
#include <QtConcurrent/QtConcurrent>

#define MAX_CHAR_SET_SIZE 65536

LabelDefinition::LabelDefinition(ProjectObject* parent) : UIWidgetDefinitionBase(QString("Labels"), parent),
    m_defaultFont(NULL),
    m_mandatoryCharacters(""),
    m_highestLabelID(0),
    m_highestFontID(0)
{
    m_defaultFontObject.Description = "<default font>";
    m_defaultFontObject.ID = 0;

    connect(this, &LabelDefinition::signal_CalculateFontMemoryUsage, this, &LabelDefinition::CalculateFontMemoryUsage);
}

LabelDefinition::~LabelDefinition()
{
}

const QList<Label*> LabelDefinition::Labels() const
{
    return m_labelsByID.values();
}

const QStringList& LabelDefinition::Languages() const
{
    return m_languages;
}

QList<Font*> LabelDefinition::Fonts()
{
    return m_fonts.values();
}

void LabelDefinition::AddFont(Font* font)
{
    SetHighestFontID(font->ID);
    m_fonts.insert(font->ID, font);
    emit signal_CalculateFontMemoryUsage();
    NotifyPropertyUpdated();
}

void LabelDefinition::AddLabel(Label* label)
{
    foreach (const Translation* translation, label->Translations())
    {
        if (m_languages.contains(translation->GetLanguage()))
        {
            continue;
        }

        m_languages.append(translation->GetLanguage());
    }

    if (label->GetID() == UndefinedLabelID)
    {
        label->SetID(getNextLabelID());
    }
    else
    {
        SetHighestLabelID(label->GetID());
    }
    m_labelsByID.insert(label->GetID(), label);
    label->SetParent(this);
    emit signal_CalculateFontMemoryUsage();
    NotifyPropertyUpdated();
}

Label* LabelDefinition::GetLabel(quint32 id)
{
    return m_labelsByID.value(id, &Label::UndefinedLabel);
}

void LabelDefinition::RemoveLabels(quint32 index, quint32 count)
{
    for (int i = count; i>0; --i)
    {
        quint32 keyToRemove = m_labelsByID.keys().at(index);
        Label* label = m_labelsByID.value(keyToRemove);
        m_labelsByID.remove(keyToRemove);
        delete label;
    }

    emit signal_CalculateFontMemoryUsage();
    NotifyPropertyUpdated();
}

void LabelDefinition::AddLanguage(const QString& language)
{
    m_languages.append(language);
    NotifyPropertyUpdated();
}

void LabelDefinition::RemoveLanguage(const QString& language)
{
    foreach (Label* label, m_labelsByID.values())
    {
        label->RemoveTranslation(language);
    }
    m_languages.removeAll(language);
    emit signal_CalculateFontMemoryUsage();
    NotifyPropertyUpdated();
}

const Font* LabelDefinition::GetDefaultFont() const
{
    return m_defaultFont;
}

void LabelDefinition::SetDefaultFont(Font* font)
{
    Font* previousDefaultFont = m_defaultFont;
    m_defaultFont = font;

    if (previousDefaultFont != NULL)
    {
        UpdateFontMemoryUsage(previousDefaultFont->ID);
        UpdateFontMemoryUsage(font->ID);
    }

    emit signal_CalculateFontMemoryUsage();

    NotifyPropertyUpdated();
}

Font* LabelDefinition::GetFontByID(quint32 fontID)
{
    if (m_fonts.count() < 1)
    {
        LOG_ERROR("No fonts set in Label Definition XML.");
        return NULL;
    }

    if (fontID == UndefinedFontID)
    {
        return &m_defaultFontObject;
    }

    Font* font = GetFontFromMap(fontID);
    if (font == NULL)
    {
        return m_defaultFont;
    }

    return font;
}

Font *LabelDefinition::GetFontByBDFPath(const QString &bdfPath)
{
    foreach (Font* font, m_fonts)
    {
        if (font->BDFPath.compare(bdfPath) == 0) return font;
    }

    return NULL;
}

void LabelDefinition::RemoveFontID(const qint32 id)
{
    Font* fontToRemove = GetFontFromMap(id);
    if (fontToRemove != NULL)
    {
        m_fonts.remove(id);

        if (m_defaultFont == fontToRemove)
        {
            m_defaultFont = NULL;
        }

        removeFontFromLabels(fontToRemove->ID);

        delete fontToRemove;
        emit signal_CalculateFontMemoryUsage();
        NotifyPropertyUpdated();
    }
}

void LabelDefinition::SetFontBDFPath(const qint32 id, const QString& bdfPath)
{
    Font* fontToChange = GetFontFromMap(id);
    if (fontToChange != NULL)
    {
        fontToChange->BDFPath = bdfPath;
        NotifyPropertyUpdated();
    }
}

void LabelDefinition::SetFontDescription(const qint32 id, const QString& description)
{
    Font* fontToChange = GetFontFromMap(id);
    if (fontToChange != NULL)
    {
        fontToChange->Description = description;
        NotifyPropertyUpdated();
    }
}

void LabelDefinition::SetFontLineSpacing(const qint32 id, const quint8 lineSpacing)
{
    Font* fontToChange = GetFontFromMap(id);
    if (fontToChange != NULL)
    {
        fontToChange->LineSpacing = lineSpacing;
        NotifyPropertyUpdated();
    }
}

void LabelDefinition::SetFontStoreInExternalFlash(const qint32 id, bool storeInExternalFlash)
{
    Font* fontToChange = GetFontFromMap(id);
    if (fontToChange != NULL)
    {
        fontToChange->StoreInExternalFlash = storeInExternalFlash;

        emit signal_FontRelocatedInMemory();
        NotifyPropertyUpdated();
    }
}

QString LabelDefinition::GetMandatoryCharacters() const
{
    return m_mandatoryCharacters;
}

void LabelDefinition::SetMandatoryCharacters(QString mandatoryCharacters)
{
    m_mandatoryCharacters = mandatoryCharacters;
    NotifyPropertyUpdated();
}

QString LabelDefinition::GetResource(quint32 id)
{
    return GetLabel(id)->GetMasterText();
}

QString LabelDefinition::GetResourceName(quint32 id)
{
    return GetLabel(id)->GetMasterText();
}

quint32 LabelDefinition::GetHighestLabelID(void)
{
    return m_highestLabelID;
}

quint32 LabelDefinition::getNextLabelID()
{
    ++m_highestLabelID;

    return m_highestLabelID;
}

void LabelDefinition::removeFontFromLabels(quint32 removedFontID)
{
    if (removedFontID == UndefinedFontID) return;

    foreach (Label* label, m_labelsByID)
    {
        foreach (Translation* translation, label->Translations())
        {
            if(translation->GetFontID() == removedFontID)
            {
                translation->SetFontID(UndefinedFontID);
            }
        }
    }
    emit signal_CalculateFontMemoryUsage();
    emit signal_FontRemoved(removedFontID);

    NotifyPropertyUpdated();
}

Font *LabelDefinition::GetFontFromMap(quint32 fontID)
{
    if (m_fonts.contains(fontID))
    {
        return m_fonts.value(fontID);
    }
    else
    {
        return NULL;
    }
}

Font *LabelDefinition::GetFontByDescription(const QString &description)
{
    foreach (Font* font, m_fonts)
    {
        if (font->Description.compare(description) == 0)
        {
            return font;
        }
    }
    return NULL;
}

void LabelDefinition::UpdateFontMemoryUsage(quint32 fontID)
{
    Font* font = GetFontFromMap(fontID);
    if (font != NULL)
    {
        QSet<quint16> utf16CharsUsedByFont = getUTF16CharactersUsedByFont(font);
        QFuture<quint32> future = QtConcurrent::run(this, &LabelDefinition::getMemoryUsageFromBDFFile, font, utf16CharsUsedByFont);

    }
}

QSet<quint16> LabelDefinition::getUTF16CharactersUsedByFont(Font* font)
{
    QSet<quint16> utf16CharactersUsedByFont;
    QString mandatoryCharacters = GetMandatoryCharacters();

    // Add all printable characters in the ASCII table, for dynamic texts
    foreach (QChar c, mandatoryCharacters)
    {
        quint16 asciiValue = c.unicode();
        utf16CharactersUsedByFont.insert(asciiValue);
    }

    if (GetDefaultFont() == font)
    {
        // Add all master text characters
        foreach (Label* label, Labels())
        {
            addStringCharsAsUTF16(utf16CharactersUsedByFont, label->GetMasterText());
        }

        addStringCharsAsUTF16(utf16CharactersUsedByFont, "UNDEFINED");
    }

    // Check all labels
    foreach (Label* label, Labels())
    {
        foreach (Translation* translation, label->Translations())
        {
            if (translation->GetFontID() == font->ID)
            {
                addStringCharsAsUTF16(utf16CharactersUsedByFont, translation->GetText());
            }
        }
    }

    return utf16CharactersUsedByFont;
}

void LabelDefinition::addStringCharsAsUTF16(QSet<quint16>& collection, QString string)
{
    const ushort* text = string.utf16();
    for (int i=0; i<string.length(); ++i)
    {
        collection.insert(text[i]);
    }
}

quint32 LabelDefinition::getMemoryUsageFromBDFFile(Font* const font, QSet<quint16> asciiChars)
{
    Project* project = static_cast<Project*>(this->GetParent());

    if (project == NULL)
    {
        return 0u;
    }

    QString fontFilePath(project->GetProjectRoot().absoluteFilePath(font->BDFPath));

    QFile fontFile(fontFilePath);
    if (!fontFile.open(QFile::ReadOnly))
    {
        LOG_ERROR("Couldn't open font file for reading! [" + fontFilePath + "] " + fontFile.errorString());
        return 0u;
    }

    QTextStream contents(&fontFile);

    quint8 boundingBoxWidth = 0;
    quint8 boundingBoxHeight = 0;

    quint8 glyphSizeArray[4];
    quint8 glyphSizeIndex = 0;
    quint8 paddedGlyphSizeArray[4];
    quint32 memoryUsage = 0;
    while (!contents.atEnd())
    {
        QString line = contents.readLine();

        if (line.startsWith("FONTBOUNDINGBOX"))
        {
            QStringList parts = line.split(QRegExp("\\s+"));
            boundingBoxWidth = parts.at(1).toInt();
            boundingBoxHeight = parts.at(2).toInt();
        }

        if (!line.contains("STARTCHAR")) continue;
        if (line.contains("ENDFONT")) break;

        quint32 encoding = 0;

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
                    ok = false;
                }

                if (ok)
                {
                    if (!asciiChars.contains(encoding))
                    {
                        break;
                    }
                }
            }
            else if (line.contains("BBX"))
            {
                QStringList parts = line.split(QRegExp("\\s+"));
                quint32 multiplierWidth = 0;
                quint32 multiplierHeight = 0;

                quint32 glyphWidth = parts.at(1).toUInt();
                quint32 glyphHeight = parts.at(2).toUInt();

                multiplierWidth = (glyphWidth / 8) + (glyphWidth % 8 == 0 ? 0 : 1);
                multiplierHeight = (glyphHeight / 8) + (glyphHeight % 8 == 0 ? 0 : 1);

                glyphSizeArray[glyphSizeIndex] = multiplierWidth * glyphHeight;

                if (glyphSizeArray[glyphSizeIndex] == 0)
                {
                    glyphSizeArray[glyphSizeIndex] = qCeil((boundingBoxHeight*boundingBoxWidth)/8.0);
                    paddedGlyphSizeArray[glyphSizeIndex] = qCeil(glyphSizeArray[glyphSizeIndex] / 8.0) * 8;
                }
                else
                {
                    paddedGlyphSizeArray[glyphSizeIndex] = (multiplierWidth * multiplierHeight) * 8;
                }

                if (glyphSizeIndex > 2)
                {
                    quint8 firstCharPacked = qCeil(glyphSizeArray[0] / 4.0) * 4;
                    quint8 secondCharPacked = qCeil(glyphSizeArray[1] / 4.0) * 4;
                    quint8 thirdCharPacked = qCeil(glyphSizeArray[2] / 4.0) * 4;
                    quint8 fourthCharPacked = qCeil(glyphSizeArray[3] / 4.0) * 4;

                    Char_Groups bestGroup = getBestGrouping(firstCharPacked, secondCharPacked, thirdCharPacked, fourthCharPacked, paddedGlyphSizeArray);

                    switch (bestGroup)
                    {
                        case FirstTwoCharsPacked:
                            memoryUsage +=  (qCeil((firstCharPacked + secondCharPacked) / 8.0) * 8);
                            glyphSizeArray[0] = glyphSizeArray[2];
                            glyphSizeArray[1] = glyphSizeArray[3];
                            glyphSizeArray[2] = 0;
                            glyphSizeArray[3] = 0;

                            paddedGlyphSizeArray[0] = paddedGlyphSizeArray[2];
                            paddedGlyphSizeArray[1] = paddedGlyphSizeArray[3];
                            paddedGlyphSizeArray[2] = 0;
                            paddedGlyphSizeArray[3] = 0;
                            glyphSizeIndex = 2;
                            break;
                        case MidTwoCharsPacked:
                            memoryUsage +=  (qCeil((secondCharPacked + thirdCharPacked) / 8.0) * 8) + paddedGlyphSizeArray[0];
                            glyphSizeArray[0] = glyphSizeArray[3];
                            glyphSizeArray[1] = 0;
                            glyphSizeArray[2] = 0;
                            glyphSizeArray[3] = 0;

                            paddedGlyphSizeArray[0] = paddedGlyphSizeArray[3];
                            paddedGlyphSizeArray[1] = 0;
                            paddedGlyphSizeArray[2] = 0;
                            paddedGlyphSizeArray[3] = 0;
                            glyphSizeIndex = 1;
                            break;
                        case LastTwoCharsPacked:
                            memoryUsage +=  paddedGlyphSizeArray[0] + paddedGlyphSizeArray[1];
                            glyphSizeArray[0] = glyphSizeArray[2];
                            glyphSizeArray[1] = glyphSizeArray[3];
                            glyphSizeArray[2] = 0;
                            glyphSizeArray[3] = 0;

                            paddedGlyphSizeArray[0] = paddedGlyphSizeArray[2];
                            paddedGlyphSizeArray[1] = paddedGlyphSizeArray[3];
                            paddedGlyphSizeArray[2] = 0;
                            paddedGlyphSizeArray[3] = 0;
                            glyphSizeIndex = 2;
                            break;
                        case FirstThreeCharsPacked:
                            memoryUsage +=  (qCeil((firstCharPacked + secondCharPacked + thirdCharPacked) / 8.0) * 8);
                            glyphSizeArray[0] = glyphSizeArray[3];
                            glyphSizeArray[1] = 0;
                            glyphSizeArray[2] = 0;
                            glyphSizeArray[3] = 0;

                            paddedGlyphSizeArray[0] = paddedGlyphSizeArray[3];
                            paddedGlyphSizeArray[1] = 0;
                            paddedGlyphSizeArray[2] = 0;
                            paddedGlyphSizeArray[3] = 0;
                            glyphSizeIndex = 1;
                            break;
                        case LastThreeCharsPacked:
                            memoryUsage += paddedGlyphSizeArray[0];
                            glyphSizeArray[0] = glyphSizeArray[1];
                            glyphSizeArray[1] = glyphSizeArray[2];
                            glyphSizeArray[2] = glyphSizeArray[3];
                            glyphSizeArray[3] = 0;

                            paddedGlyphSizeArray[0] = paddedGlyphSizeArray[1];
                            paddedGlyphSizeArray[1] = paddedGlyphSizeArray[2];
                            paddedGlyphSizeArray[2] = paddedGlyphSizeArray[3];
                            paddedGlyphSizeArray[3] = 0;
                            glyphSizeIndex = 3;
                            break;
                        case AllCharsPacked:
                        {
                            memoryUsage += qCeil((firstCharPacked + secondCharPacked + thirdCharPacked + fourthCharPacked) / 8.0) * 8;
                            glyphSizeArray[0] = 0;
                            glyphSizeArray[1] = 0;
                            glyphSizeArray[2] = 0;
                            glyphSizeArray[3] = 0;

                            paddedGlyphSizeArray[0] = 0;
                            paddedGlyphSizeArray[1] = 0;
                            paddedGlyphSizeArray[2] = 0;
                            paddedGlyphSizeArray[3] = 0;
                            glyphSizeIndex = 0;
                        }
                            break;
                        case NoGoodGroupsFound:
                            memoryUsage += paddedGlyphSizeArray[0];
                            glyphSizeArray[0] = glyphSizeArray[1];
                            glyphSizeArray[1] = glyphSizeArray[2];
                            glyphSizeArray[2] = glyphSizeArray[3];
                            glyphSizeArray[3] = 0;

                            paddedGlyphSizeArray[0] = paddedGlyphSizeArray[1];
                            paddedGlyphSizeArray[1] = paddedGlyphSizeArray[2];
                            paddedGlyphSizeArray[2] = paddedGlyphSizeArray[3];
                            paddedGlyphSizeArray[3] = 0;
                            glyphSizeIndex = 3;
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    ++glyphSizeIndex;
                }
            }
            else if (line.contains("BITMAP"))
            {
                break;
            }
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        memoryUsage += paddedGlyphSizeArray[i];
    }

    font->MemoryUsage = memoryUsage;
    return memoryUsage;
}

LabelDefinition::Char_Groups LabelDefinition::getBestGrouping(quint8 firstCharPacked, quint8 secondCharPacked, quint8 thirdCharPacked, quint8 fourthCharPacked, quint8* paddedChars)
{
    Char_Groups retVal = NoGoodGroupsFound;
    quint32 sizeAllCharsPacked = firstCharPacked + secondCharPacked + thirdCharPacked + fourthCharPacked;
    quint32 sizeFirstTwoCharsPacked = qCeil((firstCharPacked + secondCharPacked) / 8.0) * 8 + thirdCharPacked + fourthCharPacked;
    quint32 sizeMidTwoCharsPacked = qCeil((secondCharPacked + thirdCharPacked) / 8.0) * 8 + firstCharPacked + fourthCharPacked;
    quint32 sizeLastTwoCharsPacked = qCeil((thirdCharPacked + fourthCharPacked) / 8.0) * 8 + firstCharPacked + secondCharPacked;
    quint32 wtf = qCeil((firstCharPacked + secondCharPacked + thirdCharPacked) / 8.0);
    quint32 sizeFirstThreeCharsPacked = wtf * 8 + fourthCharPacked;
    quint32 sizeLastThreeCharsPacked = qCeil((secondCharPacked + thirdCharPacked + fourthCharPacked) / 8.0) * 8 + firstCharPacked;

    quint32 minSize = 0xFFFFFFFF;

    quint32 sizeAllCharsPadded = 0;

    for (int i = 0; i < 4; ++i)
    {
        sizeAllCharsPadded += paddedChars[i];
    }

    if (sizeAllCharsPacked < sizeFirstTwoCharsPacked && sizeAllCharsPacked < sizeMidTwoCharsPacked && sizeAllCharsPacked < sizeLastTwoCharsPacked && sizeAllCharsPacked < sizeFirstThreeCharsPacked && sizeAllCharsPacked < sizeLastThreeCharsPacked)
    {
        return AllCharsPacked;
    }

    if (sizeFirstTwoCharsPacked <= sizeMidTwoCharsPacked && sizeFirstTwoCharsPacked <= sizeLastThreeCharsPacked && sizeFirstTwoCharsPacked <= sizeFirstThreeCharsPacked && sizeFirstTwoCharsPacked <= sizeLastThreeCharsPacked)
    {
        minSize = sizeFirstTwoCharsPacked;
        retVal = FirstTwoCharsPacked;
    }

    if (sizeMidTwoCharsPacked <= sizeLastTwoCharsPacked && sizeMidTwoCharsPacked <= sizeFirstThreeCharsPacked && sizeMidTwoCharsPacked <= sizeLastThreeCharsPacked)
    {
        if (sizeMidTwoCharsPacked < minSize)
        {
            minSize = sizeMidTwoCharsPacked;
            retVal = MidTwoCharsPacked;
        }
    }

    if (sizeLastTwoCharsPacked <= sizeFirstThreeCharsPacked && sizeLastTwoCharsPacked <= sizeLastThreeCharsPacked)
    {
        if (sizeLastTwoCharsPacked < minSize)
        {
            minSize = sizeLastTwoCharsPacked;
            retVal = LastTwoCharsPacked;
        }
    }

    if (sizeFirstThreeCharsPacked <= sizeLastThreeCharsPacked)
    {
        if (sizeFirstThreeCharsPacked < minSize)
        {
            minSize = sizeFirstThreeCharsPacked;
            retVal = FirstThreeCharsPacked;
        }
    }

    if (sizeLastThreeCharsPacked < minSize)
    {
        retVal = LastThreeCharsPacked;
    }

    return retVal;
}

void LabelDefinition::SetHighestLabelID(quint32 ID)
{
    if(m_highestLabelID < ID)
    {
        m_highestLabelID = ID;
    }
}

quint32 LabelDefinition::GetHighestFontID()
{
    return m_highestFontID;
}

void LabelDefinition::SetHighestFontID(quint32 ID)
{
    if(m_highestFontID < ID)
    {
        m_highestFontID = ID;
    }
}

quint32 LabelDefinition::GetNextFontID()
{
    m_highestFontID++;

    return m_highestFontID;
}

void LabelDefinition::CalculateFontMemoryUsage()
{
    QFuture<void> futureResult = QtConcurrent::run(this, &LabelDefinition::threadedCalculateFontMemoryUsage);
}

void LabelDefinition::threadedCalculateFontMemoryUsage()
{
    foreach (Font* font, Fonts())
    {
        QSet<quint16> utf16CharsUsedByFont = getUTF16CharactersUsedByFont(font);
        getMemoryUsageFromBDFFile(font, utf16CharsUsedByFont);
    }
    emit signal_CalculationDone();

}

quint32 LabelDefinition::GetFontSizeInExternalFlash()
{
    quint32 externalFlashUsage = 0;
    Project* project = static_cast<Project*>(this->GetParent());

    if (project->GetBSPSpecification().GetMemory()->GetExternalMemorySize() > 0)
    {
        foreach (Font* font, Fonts())
        {
            if (font->StoreInExternalFlash)
            {
                externalFlashUsage += font->MemoryUsage;
            }
        }
    }

    return externalFlashUsage;
}

quint32 LabelDefinition::GetFontSizeInInternalFlash()
{
    quint32 internalFlashUsage = 0;
    Project* project = static_cast<Project*>(this->GetParent());

    foreach (Font* font, Fonts())
    {
        if ((!font->StoreInExternalFlash) || (project->GetBSPSpecification().GetMemory()->GetExternalMemorySize() == 0))
        {
            internalFlashUsage += font->MemoryUsage;
        }
    }

    return internalFlashUsage;
}

void LabelDefinition::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitLabelDefinition(this);
}


bool LabelDefinition::objectSelfValidation(Project* project, QStringList* errorList)
{
    Q_UNUSED(project);

    bool validation = true;

    if (Fonts().count() == 0)
    {
        errorList->append("Font(s) missing! At least one font has to be defined!");
        validation = false;
    }

    if (Languages().count() == 0)
    {
        errorList->append("Language(s) missing! At least one language has to be defined!");
        validation = false;
    }

    foreach (Label* label, Labels())
    {
        if (label->GetMasterText().compare("<Undefined>") == 0)
        {
            errorList->append("Label: " + QString::number(label->GetID()) + " is missing master text!");
            validation = false;
        }
    }

    return validation;
}
