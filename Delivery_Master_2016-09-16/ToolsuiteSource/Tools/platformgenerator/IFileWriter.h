#ifndef FILEWRITER
#define FILEWRITER

#include <QString>

class IFileWriter
{
public:

    virtual ~IFileWriter(void) {}

    virtual void WriteFile(const QString path, const QByteArray& contents) const = 0;
};

#endif // FILEWRITER

