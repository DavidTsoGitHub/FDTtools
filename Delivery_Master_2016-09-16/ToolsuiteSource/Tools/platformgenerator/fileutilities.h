#ifndef FILEUTILITIES_H
#define FILEUTILITIES_H

#include <QString>

class FileUtilities
{
public:
    static bool CopyDir(const QString source, const QString dest, const bool overwrite);

private:
    FileUtilities(void);
};

#endif // FILEUTILITIES_H
