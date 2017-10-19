#ifndef COMPILER_H
#define COMPILER_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QDir>
#include <QXmlStreamReader>

#define CREATE_CUSTOM_ATTRIBUTE_ACCESSORS(CLASS, NAME, KEY) \
    void  CLASS::Set##NAME(QString value) { SetCustomAttribute(KEY, value); } \
    QString CLASS::Get##NAME(void) { return GetCustomAttribute(KEY); }

#define CREATE_CUSTOM_ATTRIBUTE_ACCESSOR_DEFINITIONS(NAME) \
    void Set##NAME(QString value); \
    QString Get##NAME(void);

class CompilerBase
{
public:
    ~CompilerBase(void);

    typedef enum
    {
        Text,
        File,
        Directory,
        Checkbox,
        BSPPropertyExternalMemoryAddress
    } CompilerAttributeType;

    typedef struct
    {
        QString Name;
        QString ShortDescription;
        CompilerAttributeType Type;
    } CompilerAttribute;

    const QString& GetName(void) const;
    void SetName(QString name);
    const QString GetTypeString(void);

    const QMap<QString, QString>& GetEnvironmentVariables(void) const;
    void AddEnvironmentVariable(QString key, QString value);

    const QList<CompilerAttribute> GetCompilerAttributes(void);

    virtual bool GenerateBuildEnvironment(QString projectRootPath) = 0;
    virtual bool Build(QString projectRootPath) = 0;

    /*!
     * \brief Serialize the compiler data to a transmittable string.
     * \return The serialized data in QString format.
     */
    QString Serialize(void);
    bool Deserialize(const QString data);

    const QString GetCustomAttribute(const QString customAttributeName) const;
    void SetCustomAttribute(const QString customAttributeName, const QString customAttributeValue);

    bool CompilerAttributeExist(QString compilerAttribute, QString errorDescription);
    virtual bool HasMandatoryCompilerSpecificData(void) = 0;

    virtual bool ValidateBuildOutput(QString buildOutput, QStringList& errors);

    bool quickBuild;

protected:
    typedef struct
    {
        QStringList SourceFiles;
        QStringList SourceDirectories;
        QStringList AsmFiles;
        QStringList AsmDirectories;
        QStringList IncludeDirectories;
        QStringList LibraryFiles;
        QStringList LibraryDirectories;
    } SourceTree;

    explicit CompilerBase(const QString typeName);
    SourceTree parseSourceTree(QDir sourcePath);

    void addCustomAttributeDefinition(const QString customAttributeName, const QString shortDescription, CompilerBase::CompilerAttributeType);
    bool writeToFile(QString fileName, QString fileContent);

private:
    void deserializeCompilerSpecificData(QXmlStreamReader& compilerSpecificData);
    void deserializeEnvironmentVariables(QXmlStreamReader& environmentVariables);
    QString m_name;
    QString m_typeName;
    QMap<QString, QString> m_environmentVariables;
    QMap<QString, QString> m_customAttributes;
    QList<CompilerAttribute> m_customAttributeDefinitions;
};

#endif
