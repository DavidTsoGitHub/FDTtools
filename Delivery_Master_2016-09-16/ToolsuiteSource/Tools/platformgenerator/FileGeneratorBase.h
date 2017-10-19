#ifndef FILEGENERATORBASE
#define FILEGENERATORBASE

#include "GeneratedFile.h"
#include "PlatformPaths.h"
#include <QList>
#include <QString>

class FileGeneratorBase
{
public:
    virtual ~FileGeneratorBase(void) {}
    QList<GeneratedFile>& GenerateFiles(void);

protected:
    virtual void OnGenerateFiles(void) = 0;
    void AddGeneratedFile(QString path, const QString& contents);
    void AddCopiedFile(QString path, const QByteArray contents);
    void AddFilesFromResourcePath(QString resourcePath);
    void AddGeneratedFiles(QList<GeneratedFile> generatedFiles);
    quint32 GetGeneratedFilesCount(void) const;

private:
    QList<GeneratedFile> m_generatedFiles;

    static QString CurrentDateTime(void);

};

#endif // FILEGENERATORBASE

