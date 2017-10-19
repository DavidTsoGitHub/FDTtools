#include "Image.h"
#include "ProjectObjectVisitor.h"

#include <QFileInfo>
#include <QImage>

Image::Image() : ProjectObject(QString("Image")),
  m_storeOnExternalFlash(false),
  m_useForNonAnimations(true),
  m_useForAnimations(false)
{}

quint32 Image::GetID() const
{
    return m_id;
}

void Image::SetID(const quint32 id)
{
    m_id = id;
    NotifyPropertyUpdated();
}

const QString Image::GetName() const
{
    return m_name;
}

void Image::SetName(const QString& name)
{
    m_name = name;
    setObjectName(name);
    NotifyPropertyUpdated();
}

const QString Image::GetFileName() const
{
    return QFileInfo(m_filePath).fileName();
}

bool Image::GetStoreOnExternalFlash() const
{
    return m_storeOnExternalFlash;
}

void Image::SetStoreOnExternalFlash(bool state)
{
    m_storeOnExternalFlash = state;
    NotifyPropertyUpdated();
}

bool Image::GetUseForNonAnimations() const
{
    return m_useForNonAnimations;
}

void Image::SetUseForNonAnimations(bool state)
{
    m_useForNonAnimations = state;
    NotifyPropertyUpdated();
}

bool Image::GetUseForAnimations() const
{
    return m_useForAnimations;
}

void Image::SetUseForAnimations(bool state)
{
    m_useForAnimations = state;
    NotifyPropertyUpdated();
}

const QString Image::GetPath() const
{
    return m_filePath;
}

void Image::SetPath(const QString& path)
{
    m_filePath = path;
    NotifyPropertyUpdated();
}

const QString Image::GetDescription() const
{
    return m_description;
}

void Image::SetDescription(const QString& description)
{
    m_description = description;
    NotifyPropertyUpdated();
}

quint32 Image::GetWidth() const
{
    return m_width;
}

void Image::SetWidth(quint32 width)
{
    m_width = width;
}

quint32 Image::GetHeight() const
{
    return m_height;
}

void Image::SetHeight(quint32 height)
{
    m_height = height;
}

quint32 Image::GetSize() const
{
    return (m_width * m_height * 2); // * 2 => 16BPP
}

void Image::AcceptVisitor(ProjectObjectVisitor &visitor)
{
    visitor.VisitImage(this);
}

Image Image::UndefinedImage;
