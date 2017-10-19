#ifndef LABEL
#define LABEL

#include "ProjectObject.h"
#include "Translation.h"
#include <QString>
#include <QList>

const quint32 UndefinedLabelID = std::numeric_limits<quint32>::max();

class Label : public ProjectObject
{
public:
    static Label UndefinedLabel;

    Label();
    ~Label();

    quint32 GetID() const;
    void SetID(const quint32& value);

    QString GetMasterText() const;
    void SetMasterText(const QString& value);

    QString GetDescription() const;
    void SetDescription(const QString& value);

    const QList<Translation*>& Translations();
    void AddTranslation(Translation* translation);
    bool HasTranslation(const QString& language) const;
    Translation* GetTranslation(const QString& language) const;
    void RemoveTranslation(const QString& language);


private:
    QList<Translation*> m_translations;
    quint32 m_id;
    QString m_masterText;
    QString m_description;
};

#endif // LABEL
