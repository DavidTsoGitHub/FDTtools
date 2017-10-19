#ifndef LABELDEFINITION_H
#define LABELDEFINITION_H

#include "Font.h"
#include "ISelfValidation.h"
#include "UIWidgetDefinitionBase.h"

class GCLSignalSink;
class Label;
class Project;

template <class Key, class T> class QMap;
class QStringList;

const quint32 UndefinedFontID = 0;

class LabelDefinition : public UIWidgetDefinitionBase, public ISelfValidation
{
    Q_OBJECT

public:

    typedef enum
    {
        FirstTwoCharsPacked,
        MidTwoCharsPacked,
        LastTwoCharsPacked,
        FirstThreeCharsPacked,
        LastThreeCharsPacked,
        AllCharsPacked,
        NoGoodGroupsFound
    } Char_Groups;

    explicit LabelDefinition(ProjectObject* parent = 0);
    ~LabelDefinition(void);

    const QList<Label*> Labels() const;
    void AddLabel(Label* label);
    Label* GetLabel(quint32 id);
    void RemoveLabels(quint32 index, quint32 count);

    void AddLanguage(const QString& language);
    void RemoveLanguage(const QString& language);
    const QStringList& Languages() const;

    QList<Font*> Fonts();
    void AddFont(Font* font);
    const Font* GetDefaultFont() const;
    void SetDefaultFont(Font *font);
    bool ApplyFontsToTranslations(void);
    Font *GetFontByID(quint32 fontID);
    Font* GetFontByBDFPath(const QString& bdfPath);
    void RemoveFontID(const qint32 id);
    void SetFontBDFPath(const qint32 id, const QString& bdfPath);
    void SetFontDescription(const qint32 id, const QString& description);
    void SetFontLineSpacing(const qint32 id, const quint8 lineSpacing);
    void SetFontStoreInExternalFlash(const qint32 id, bool storeInExternalFlash);

    QString GetMandatoryCharacters() const;
    void SetMandatoryCharacters(QString mandatoryCharacters);
    quint32 GetHighestLabelID(void);
    void SetHighestLabelID(quint32 ID);
    quint32 GetHighestFontID(void);
    void SetHighestFontID(quint32 ID);
    quint32 GetNextFontID(void);
    Font* GetFontFromMap(quint32 fontID);
    Font* GetFontByDescription(const QString &description);

    void CalculateFontMemoryUsage();
    quint32 GetFontSizeInExternalFlash();
    quint32 GetFontSizeInInternalFlash();
    void UpdateFontMemoryUsage(quint32 fontID);

signals:
    void signal_CalculateFontMemoryUsage();
    void signal_CalculationDone();
    void signal_FontRelocatedInMemory();
    void signal_FontRemoved(quint32 fontID);

private:
    Font m_defaultFontObject;
    Font* m_defaultFont;
    QMap<quint32, Font*> m_fonts;
    QMap<quint32, Label*> m_labelsByID;
    QStringList m_languages;
    QList<const GCLSignalSink*> m_signalSinks;
    QString m_mandatoryCharacters;
    quint32 m_highestLabelID;
    quint32 m_highestFontID;

    quint32 getNextLabelID();
    void removeFontFromLabels(quint32 removedFontID);


    QSet<quint16> getUTF16CharactersUsedByFont(Font *font);
    quint32 getMemoryUsageFromBDFFile(Font * const font, QSet<quint16> asciiChars);
    void addStringCharsAsUTF16(QSet<quint16> &collection, QString string);
    Char_Groups getBestGrouping(quint8 firstCharPacked, quint8 secondCharPacked, quint8 thirdCharPacked, quint8 fourthCharPacked, quint8 *paddedChars);

    void threadedCalculateFontMemoryUsage();
public:
    QString GetResource(quint32 id);
    QString GetResourceName(quint32 id);

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);

    // ISelfValidation interface
public:
    bool objectSelfValidation(Project* project, QStringList* errorList);
};

#endif // LABELDEFINITION_H
