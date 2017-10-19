#include "ComboBoxMetaType.h"

ComboBoxMetaType::ComboBoxMetaType()
{

}

const QString& ComboBoxMetaType::GetActive() const
{
    return m_active;
}

void ComboBoxMetaType::SetActive(QString active)
{
    m_active = active;
}

const QMap<int, QString>& ComboBoxMetaType::GetComboBoxData() const
{
    return m_comboBoxData;
}

void ComboBoxMetaType::AddToComboBoxData(int value, QString Text)
{
    m_comboBoxData.insert(value, Text);
}
