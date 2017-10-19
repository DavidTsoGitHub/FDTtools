#include "CompilerBase.h"
#include <QXmlStreamWriter>
#include <QDirIterator>
#include <QTextStream>
#include "Logger.h"

CompilerBase::CompilerBase(const QString typeName) :
    quickBuild(false),
    m_name("Unset"),
    m_typeName(typeName)
{
}

void CompilerBase::addCustomAttributeDefinition(const QString customAttributeName, const QString shortDescription, CompilerBase::CompilerAttributeType type)
{
    CompilerAttribute customAttribute;
    customAttribute.Name = customAttributeName;
    customAttribute.ShortDescription = shortDescription;
    customAttribute.Type = type;

    m_customAttributeDefinitions.append(customAttribute);
}

CompilerBase::~CompilerBase()
{
}

QString CompilerBase::Serialize()
{
    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.writeStartElement("CompilerData");
    writer.writeTextElement("Type", GetTypeString());
    writer.writeTextElement("Name", GetName());

    writer.writeStartElement("EnvironmentVariables");
    QMapIterator<QString, QString> iterator(m_environmentVariables);
    while (iterator.hasNext())
    {
        iterator.next();
        writer.writeStartElement("EnvironmentVariable");
        writer.writeAttribute("Key", iterator.key());
        writer.writeAttribute("Value", iterator.value());
        writer.writeEndElement();
    }
    writer.writeEndElement();

    writer.writeStartElement("CompilerSpecificData");
    QMapIterator<QString, QString> compilerSpecificData(m_customAttributes);
    while (compilerSpecificData.hasNext())
    {
        compilerSpecificData.next();
        writer.writeTextElement(compilerSpecificData.key(), compilerSpecificData.value());
    }
    writer.writeEndElement();

    writer.writeEndElement();
    return xml;
}

bool CompilerBase::Deserialize(const QString data)
{
    QXmlStreamReader xml(data);

    if (xml.readNextStartElement())
    {
        if (xml.name()=="CompilerData")
        {
            while (!(xml.name() == "CompilerData" && xml.isEndElement()) && !xml.atEnd())
            {
                xml.readNext();
                if (!xml.isStartElement()) continue;

                if (xml.name() == "Type")
                {
                    m_typeName = xml.readElementText();
                }
                else if (xml.name() == "Name")
                {
                    m_name = xml.readElementText();
                }
                else if (xml.name() == "EnvironmentVariables")
                {
                    deserializeEnvironmentVariables(xml);
                }
                else if (xml.name() == "CompilerSpecificData")
                {
                    deserializeCompilerSpecificData(xml);
                }
            }
        }
        else
        {
            xml.raiseError("The data does not have a CompilerData element");
        }
    }

    return xml.error() == QXmlStreamReader::NoError;
}

void CompilerBase::deserializeCompilerSpecificData(QXmlStreamReader& xml)
{
    while (!(xml.name() == "CompilerSpecificData" && xml.isEndElement()) && !xml.atEnd())
    {
        xml.readNext();
        if (!xml.isStartElement()) continue;

        QString key = xml.name().toString();
        QString value = xml.readElementText();
        SetCustomAttribute(key, value);
    }
}

void CompilerBase::deserializeEnvironmentVariables(QXmlStreamReader& xml)
{
    while (!(xml.name() == "EnvironmentVariables" && xml.isEndElement()) && !xml.atEnd())
    {
        xml.readNext();
        if (xml.name() == "EnvironmentVariable" && xml.isStartElement())
        {
            QString key = xml.attributes().value("Key").toString();
            QString value = xml.attributes().value("Value").toString();
            m_environmentVariables.insert(key, value);
        }
    }
}

bool CompilerBase::writeToFile(QString fileName, QString fileContent)
{
    QFileInfo fileInfo(fileName);

    QDir dir(fileInfo.path());
    if(!dir.exists())
    {
        dir.mkpath(".");
    }

    QFile qf(fileName);
    if (!qf.open(QIODevice::WriteOnly))
    {
        LOG_ERROR("Failed to open file: " + fileName + " for writing");
        return false;
    }

    QTextStream stream(&qf);

    stream << fileContent;
    qf.close();

    return true;
}

const QString& CompilerBase::GetName() const
{
    return m_name;
}

void CompilerBase::SetName(QString name)
{
    m_name = name;
}

const QString CompilerBase::GetTypeString()
{
    return m_typeName;
}

const QMap<QString, QString>& CompilerBase::GetEnvironmentVariables() const
{
    return m_environmentVariables;
}

void CompilerBase::AddEnvironmentVariable(QString key, QString value)
{
    m_environmentVariables.insert(key, value);
}

const QList<CompilerBase::CompilerAttribute> CompilerBase::GetCompilerAttributes()
{
    return m_customAttributeDefinitions;
}

const QString CompilerBase::GetCustomAttribute(const QString customAttributeName) const
{
    return m_customAttributes.value(customAttributeName);
}
void CompilerBase::SetCustomAttribute(const QString customAttributeName, const QString customAttributeValue)
{
    m_customAttributes[customAttributeName] = customAttributeValue;
}

bool CompilerBase::CompilerAttributeExist(QString compilerAttribute, QString errorDescription)
{
    if (GetCustomAttribute(compilerAttribute).isEmpty() && !QFile(GetCustomAttribute(compilerAttribute)).exists())
    {
        LOG_ERROR(errorDescription);
        return false;
    }

    return true;
}

bool CompilerBase::ValidateBuildOutput(QString buildOutput, QStringList& errors)
{
    Q_UNUSED(buildOutput);
    Q_UNUSED(errors);
    return true;
}

void addIfNotPresent(QStringList& list, QString value)
{
    if (!list.contains(value))
    {
        list.append(value);
    }
}

CompilerBase::SourceTree CompilerBase::parseSourceTree(QDir sourcePath)
{
    SourceTree tree;
    QDirIterator it(sourcePath.absolutePath(), QStringList() << "*.c" << "*.h" << "*.lib" << "*.a" << "*.s", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        it.next();

        QString absolutePath(it.fileInfo().absolutePath());
        QString absoluteFilePath(it.fileInfo().absoluteFilePath());

        if (it.fileName().endsWith(".c"))
        {
            tree.SourceFiles.append(sourcePath.relativeFilePath(absoluteFilePath));
            addIfNotPresent(tree.SourceDirectories, sourcePath.relativeFilePath(absolutePath));
        }
        else if (it.fileName().endsWith(".h"))
        {
            addIfNotPresent(tree.IncludeDirectories, sourcePath.relativeFilePath(absolutePath));
        }
        else if(it.fileName().endsWith(".s"))
        {
            tree.AsmFiles.append(sourcePath.relativeFilePath(absoluteFilePath));
            addIfNotPresent(tree.AsmDirectories, sourcePath.relativeFilePath(absolutePath));
            addIfNotPresent(tree.SourceDirectories, sourcePath.relativeFilePath(absolutePath));
        }
        else if (it.fileName().endsWith(".lib") || it.fileName().endsWith(".a"))
        {
            tree.LibraryFiles.append(sourcePath.relativeFilePath(absoluteFilePath));
            addIfNotPresent(tree.LibraryDirectories, sourcePath.relativeFilePath(absolutePath));
        }
    }

    return tree;
}
