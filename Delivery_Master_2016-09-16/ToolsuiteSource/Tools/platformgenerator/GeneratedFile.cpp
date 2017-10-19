#include "GeneratedFile.h"


GeneratedFile::GeneratedFile(QString relativePath, QString contents) :
    m_relativePath(relativePath)
{
    m_contents = contents.toLocal8Bit();
}

GeneratedFile::GeneratedFile(QString relativePath, QByteArray contents) :
    m_contents(contents),
    m_relativePath(relativePath)
{
}

const QString GeneratedFile::RelativePath() const
{
    return m_relativePath;
}

const QByteArray& GeneratedFile::Contents() const
{
    return m_contents;
}
