#ifndef IMAGEMANAGERGENERATOR_H
#define IMAGEMANAGERGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"

class ImageManagerGenerator : public FileGeneratorBase
{
public:
    explicit ImageManagerGenerator(Project* project);
    ~ImageManagerGenerator();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

private:
    QString m_imageManagerHeaderPath;
    QString m_imageManagerSourcePath;
    QString m_imageRoot;
    QStringList m_imageSourceFiles;
    QList<quint16> m_imageIndexes;

    Project* m_project;

    void generateImageManagerHeader(void);
    void generateImageManagerSource(void);
    void generateImageIncludeHeader(void);
    void generateImageSources(void);
    void addMenuItemImages(const QList<MenuItem*>& menuItems);
};

#endif // IMAGEMANAGERGENERATOR_H
