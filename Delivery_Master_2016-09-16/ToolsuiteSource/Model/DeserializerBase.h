#ifndef DESERIALIZERBASE_H
#define DESERIALIZERBASE_H

class QString;
class QXmlStreamReader;

class DeserializerBase
{
public:

    typedef enum
    {
        DeserializationOK,
        ErrorNoFile,
        ErrorFileEmpty,
        ErrorTagNotFound,
        ErrorIncorrectStructure,
        ErrorPointsToMissingFile,
        ErrorGeneral,
        ErrorMissingRequiredTag,
        ErrorAIVNotFound,
        ErrorWrongAIV,
        ErrorUnknown
    } DeserializerRespons;

    virtual DeserializerRespons Deserialize(const QString& xml) = 0;
    const char* GetEncoding(void) const;
    const QString Error(void) const;

protected:
    explicit DeserializerBase(const char* encoding);
    virtual ~DeserializerBase(void);
    static bool isEndOfElement(QXmlStreamReader& xmlReader, QString elementName);

    QString m_error;

private:
    const char* m_encoding;
};

#endif // DESERIALIZERBASE_H
