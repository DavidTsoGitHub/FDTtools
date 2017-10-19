#include "FileUtilities.h"

#include <QDir>
#include <QDirIterator>

FileUtilities::FileUtilities()
{

}

bool FileUtilities::CopyDir(const QString source, const QString dest, const bool overwrite)
{
    QDir sourceDirectory(source);
    QDir destinationDirectory(dest);

    bool error = false;

    if (!sourceDirectory.exists())
    {
        return false;
    }
    if (!destinationDirectory.exists())
    {
        destinationDirectory.mkpath(".");
    }

    QDirIterator sourceDirectoryIterator(sourceDirectory, QDirIterator::Subdirectories);

    while (sourceDirectoryIterator.hasNext())
    {
        QString path = sourceDirectoryIterator.next();

        if (path.endsWith('.') || path.endsWith("..")) continue;

        QString relativePath = QString(path).replace(source, "");

        while (relativePath.startsWith("/"))
        {
            relativePath.remove(0, 1);
        }

        QFileInfo fileInfo(sourceDirectoryIterator.fileInfo());

        if (fileInfo.isDir())
        {
            destinationDirectory.mkpath(relativePath);
        }
        else if (fileInfo.isFile())
        {
            QFile sourceFile(fileInfo.absoluteFilePath());
            QString destinationPath = destinationDirectory.absoluteFilePath(relativePath);

            qDebug("%s -> %s", fileInfo.absoluteFilePath().toStdString().c_str(), destinationPath.toStdString().c_str());

            QFile destinationFile(destinationPath);
            if (destinationFile.exists() && overwrite)
            {
                destinationFile.remove();
            }

            if (!sourceFile.copy(destinationPath))
            {
                error = true;
            }

        }
    }

    return !error;
}

