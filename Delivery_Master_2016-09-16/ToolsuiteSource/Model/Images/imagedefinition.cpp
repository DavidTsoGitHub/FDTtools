#include "ImageDefinition.h"

#include "bspmemory.h"
#include "Image.h"
#include "Logger.h"
#include "Exceptions/ObjectNotFoundException.h"
#include "ProjectObjectVisitor.h"

#include <limits>

ImageDefinition::ImageDefinition(ProjectObject *parent) : UIWidgetDefinitionBase(QString("Images"), parent),
    m_parent(parent)
{
    m_undefinedImage = new Image();
    m_undefinedImage->SetID(std::numeric_limits<quint32>::max());
    m_undefinedImage->SetName("Undefined");
}

ImageDefinition::~ImageDefinition()
{
    delete m_undefinedImage;
}

quint32 ImageDefinition::GetNextAvailableImageId() const
{
    quint32 highestOccupiedImageId = 0;

    foreach (Image* image, m_images)
    {
        if(image->GetID() >= highestOccupiedImageId)
        {
            highestOccupiedImageId = image->GetID();
        }
    }

    return highestOccupiedImageId + 1;
}

const QList<Image*>&ImageDefinition::Images() const
{
    return m_images;
}

Image *ImageDefinition::GetImageByID(quint32 id)
{
    foreach (Image* image, m_images)
    {
        if(image->GetID() == id)
        {
            return image;
        }
    }

    return &Image::UndefinedImage;
}

void ImageDefinition::AddImage(Image* image)
{
    m_images.append(image);
    image->SetParent(this);
    NotifyPropertyUpdated();
}

void ImageDefinition::RemoveImageAtIndex(int index)
{
    Image* imageToRemove = m_images.at(index);

    m_images.removeAll(imageToRemove);
    delete imageToRemove;
    NotifyPropertyUpdated();
    NotifyPropertyUpdatedRedrawNeeded();
}

QString ImageDefinition::GetResource(quint32 id)
{
    return GetImageFolderPath() + "/" + GetImageByID(id)->GetFileName();
}

QString ImageDefinition::GetResourceName(quint32 id)
{
    return GetImageByID(id)->GetName();
}

const QString ImageDefinition::GetImageFolderPath() const
{
    return m_imageFolderPath;
}

void ImageDefinition::SetImageFolderPath(QString imageFolderPath)
{
    m_imageFolderPath = imageFolderPath;
    NotifyPropertyUpdated();
}

void ImageDefinition::SetImagesUsed(QList<quint32> images)
{
    foreach (quint32 imageID, images)
    {
        m_usedImages.append(imageID);
    }
}

quint32 ImageDefinition::GetImageSizeInInternalFlash()
{
    quint32 internalFlashUsage = 0;
    Project* project = static_cast<Project*>(m_parent);

    const BSPMemory* memory = project->GetBSPSpecification().GetMemory();

    foreach (Image* image, Images())
    {
        if (!m_usedImages.contains(image->GetID())) continue;

        if ((memory->GetExternalMemorySize() == 0) || (!image->GetStoreOnExternalFlash()))
        {
            internalFlashUsage += image->GetSize();
        }
    }

    return internalFlashUsage;
}

quint32 ImageDefinition::GetImageSizeInExternalFlash()
{
    quint32 externalFlashUsage = 0;
    Project* project = static_cast<Project*>(m_parent);

    const BSPMemory* memory = project->GetBSPSpecification().GetMemory();

    if (memory->GetExternalMemorySize() > 0)
    {
        foreach (Image* image, Images())
        {
            if (!m_usedImages.contains(image->GetID())) continue;

            if (image->GetStoreOnExternalFlash())
            {
                externalFlashUsage += image->GetSize();
            }
        }
    }

    return externalFlashUsage;
}

void ImageDefinition::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitImageDefinition(this);
}
