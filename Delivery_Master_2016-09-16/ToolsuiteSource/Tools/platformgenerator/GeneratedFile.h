#ifndef GENERATEDFILE_H
#define GENERATEDFILE_H

#include <QString>

class GeneratedFile
{
public:
    explicit GeneratedFile(QString relativePath, QString contents);
    explicit GeneratedFile(QString relativePath, QByteArray contents);
    const QString RelativePath(void) const;
    const QByteArray& Contents(void) const;

private:
    QByteArray m_contents;
    QString m_relativePath;
};

#endif // GENERATEDFILE_H
