#ifndef TRANSLATION
#define TRANSLATION

#include "ProjectObject.h"

class QString;

class Translation : public ProjectObject
{
public:
    Translation(void);

    quint32 GetFontID() const;
    void SetFontID(quint32 fontID);

    QString GetLanguage() const;
    void SetLanguage(const QString& value);

    QString GetText() const;
    void SetText(const QString& value);

private:
    void updateObjectName();

    quint32 m_fontID;
    QString m_language;
    QString m_text;
};

#endif // TRANSLATION

