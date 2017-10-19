#include "FileGeneratorBase.h"
#include <QDateTime>
#include <QDirIterator>

QList<GeneratedFile>& FileGeneratorBase::GenerateFiles()
{
    m_generatedFiles.clear();

    OnGenerateFiles();

    return m_generatedFiles;
}

void FileGeneratorBase::AddFilesFromResourcePath(QString resourcePath)
{
    if (!resourcePath.endsWith('/'))
    {
        resourcePath.append('/');
    }

    QDirIterator it(resourcePath, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString path = it.next();
        if(!QFileInfo(path).isFile())
        {
            continue;
        }

        QFile file(path);
        if (!file.open(QFile::ReadOnly)) continue;

        QString relativePath(path.replace(resourcePath, ""));
        m_generatedFiles.append(GeneratedFile(relativePath, file.readAll()));
    }
}

void FileGeneratorBase::AddGeneratedFiles(QList<GeneratedFile> generatedFiles)
{
    m_generatedFiles.append(generatedFiles);
}

quint32 FileGeneratorBase::GetGeneratedFilesCount() const
{
    return m_generatedFiles.count();
}

void FileGeneratorBase::AddGeneratedFile(QString path, const QString& contents)
{
    QString timeStampedContents;
    timeStampedContents.append("/*** File generated " + CurrentDateTime() + " ***/\n");
    timeStampedContents.append(contents);
    m_generatedFiles.append(GeneratedFile(path, timeStampedContents));
}

void FileGeneratorBase::AddCopiedFile(QString path, const QByteArray contents)
{
    m_generatedFiles.append(GeneratedFile(path, contents));
}

QString FileGeneratorBase::CurrentDateTime()
{
    QDateTime now = QDateTime::currentDateTime();
    QString ret = now.toString("yyyy-MM-dd hh:mm:ss");
    return ret;
}
