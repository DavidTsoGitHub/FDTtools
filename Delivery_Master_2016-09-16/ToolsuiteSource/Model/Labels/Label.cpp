#include "Label.h"

Label::Label() : ProjectObject("<Undefined>"),
    m_id(UndefinedLabelID),
    m_masterText("<Undefined>")
{
}

Label::~Label()
{}

QString Label::GetDescription() const
{
    return m_description;
}

void Label::SetDescription(const QString& value)
{
    m_description = value;
    NotifyPropertyUpdated();
}

QString Label::GetMasterText() const
{
    return m_masterText;
}

void Label::SetMasterText(const QString& value)
{
    m_masterText = value;
    setObjectName(value);
    NotifyPropertyUpdated();
}

quint32 Label::GetID() const
{
    return m_id;
}

void Label::SetID(const quint32& value)
{
    m_id = value;
    NotifyPropertyUpdated();
}

const QList<Translation*>& Label::Translations()
{
    return m_translations;
}

void Label::AddTranslation(Translation* translation)
{
    m_translations.append(translation);
    translation->SetParent(this);
    NotifyPropertyUpdated();
}

bool Label::HasTranslation(const QString& language) const
{
    foreach (const Translation* translation, m_translations)
    {
        if (translation->GetLanguage() == language)
        {
            return true;
        }
    }

    return false;
}

Translation* Label::GetTranslation(const QString& language) const
{
    foreach (Translation* translation, m_translations)
    {
        if (translation->GetLanguage() == language)
        {
            return translation;
        }
    }

    return NULL;
}

void Label::RemoveTranslation(const QString& language)
{
    foreach (Translation* translation, m_translations)
    {
        if (translation->GetLanguage() == language)
        {
            m_translations.removeAll(translation);
            delete translation;
            NotifyPropertyUpdated();
        }
    }
}

Label Label::UndefinedLabel;
