#ifndef COMBOBOXMETATYPE_H
#define COMBOBOXMETATYPE_H

#include <QObject>
#include <QMap>
#include <QMetaType>

class ComboBoxMetaType
{
public:
    ComboBoxMetaType();
    ~ComboBoxMetaType(void) {}

    const QString& GetActive(void) const;
    void SetActive(QString active);

    const QMap<int, QString>& GetComboBoxData(void) const;
    void AddToComboBoxData(int value, QString Text);

private:
    QString m_active;
    QMap<int, QString> m_comboBoxData;
};

Q_DECLARE_METATYPE(ComboBoxMetaType)

#endif // COMBOBOXMETATYPE_H
