#include "FileWriter.h"
#include <QFile>
#include <QFileInfo>


FileWriter::FileWriter(const QString rootPath)
{
    m_rootDir = new QDir(QDir::cleanPath(rootPath));
}

FileWriter::~FileWriter()
{
    delete m_rootDir;
}

void FileWriter::WriteFile(QString path, const QByteArray& contents) const
{
    QString cleanPath = QDir::cleanPath(path);
    QFileInfo absoluteFilePath(m_rootDir->absoluteFilePath(cleanPath));
    if (absoluteFilePath.isDir()) return;

    QDir absolutePath(absoluteFilePath.absoluteDir());
    if (!absolutePath.exists())
    {
        absolutePath.mkpath(".");
    }

    QFile fileToWrite(absoluteFilePath.absoluteFilePath());


    if (!fileToWrite.open(QFile::WriteOnly))
    {
        return;
    }

    fileToWrite.write(contents);

    fileToWrite.close();
}
