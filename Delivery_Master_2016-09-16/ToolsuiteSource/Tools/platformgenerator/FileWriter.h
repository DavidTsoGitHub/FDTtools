#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QString>
#include <QDir>
#include "IFileWriter.h"

class FileWriter : public IFileWriter
{

public:
    explicit FileWriter(const QString rootPath);
    ~FileWriter(void);

    // IFileWriter interface
public:
    void WriteFile(QString path, const QByteArray& contents) const;

private:
    QDir* m_rootDir;
};

#endif // FILEWRITER_H
