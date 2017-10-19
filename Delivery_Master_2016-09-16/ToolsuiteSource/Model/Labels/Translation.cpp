#include "Translation.h"

#include <QString>

Translation::Translation() : ProjectObject("Translation"),
    m_fontID(0)
{
}

QString Translation::GetText() const
{
    return m_text;
}

void Translation::SetText(const QString& value)
{
    m_text = value;

    updateObjectName();
    NotifyPropertyUpdated();
}

QString Translation::GetLanguage() const
{
    return m_language;
}

void Translation::SetLanguage(const QString& value)
{
    m_language = value;
    updateObjectName();
    NotifyPropertyUpdated();
}

quint32 Translation::GetFontID() const
{
    return m_fontID;
}

void Translation::SetFontID(quint32 fontID)
{
    m_fontID = fontID;
    NotifyPropertyUpdated();
}

void Translation::updateObjectName()
{
    setObjectName(m_language + " - " + m_text);
}
