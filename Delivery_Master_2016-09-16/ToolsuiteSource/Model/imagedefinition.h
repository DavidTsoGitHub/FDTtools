#ifndef IMAGEDEFINITION_H
#define IMAGEDEFINITION_H

#include "UIWidgetDefinitionBase.h"

class Image;

class ImageDefinition : public UIWidgetDefinitionBase
{
    Q_OBJECT

public:
    explicit ImageDefinition(ProjectObject* parent = 0);
    ~ImageDefinition();

    quint32 GetNextAvailableImageId() const;

    const QList<Image*>& Images(void) const;
    Image* GetImageByID(quint32 id);
    void AddImage(Image* image);
    void RemoveImageAtIndex(int index);

    const QString GetImageFolderPath() const;
    void SetImageFolderPath(QString imageFolderPath);

    void SetImagesUsed(QList<quint32> images);
    quint32 GetImageSizeInInternalFlash();
    quint32 GetImageSizeInExternalFlash();

signals:
    void signal_CalculateImageMemoryUsage();

    // UIWidgetDefinitionBase interface
public:
    QString GetResource(quint32 id);
    QString GetResourceName(quint32 id);

private:
    ProjectObject* m_parent;
    QList<Image*> m_images;
    QList<quint32> m_usedImages;

    Image* m_undefinedImage;
    QString m_imageFolderPath;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor &visitor);
};

#endif // IMAGEDEFINITION_H

