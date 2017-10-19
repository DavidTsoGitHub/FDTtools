#ifndef FUNCTION_H
#define FUNCTION_H

#include <QString>
#include <QList>
#include <QJsonObject>

class Function
{
public:
    enum type_t
    {
        NO_TYPE,
        SET,
        GET
    };

    Function(QString functionName, type_t type, QString valueParameter = "0", qint32 indexParameter = 0);

    static Function* Deserialize(const QJsonObject& json);
    void Serialize(QJsonObject& json) const;

    QString GetName();
    qint32 GetIndex();
    QString GetValue();

private:
    QString m_functionName;
    type_t m_type;
    qint32 m_indexParameter;
    QString m_valueParameter;
};

#endif // FUNCTION_H
