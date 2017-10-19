#include "ImageModel.h"

#include "Image.h"
#include "Project.h"

#include <QFont>
#include <QImage>

ImageModel::ImageModel(ImageDefinition& images, QObject* parent) : QAbstractTableModel(parent),
    m_imageDefinition(images)
{
    m_project = static_cast<Project*>(m_imageDefinition.GetParent());

    m_headerFont.setBold(true);

    connect(this, &ImageModel::rowsAboutToBeInserted, this, &ImageModel::signal_modified);
    connect(this, &ImageModel::rowsAboutToBeRemoved, this, &ImageModel::signal_modified);
    connect(this, &ImageModel::rowsAboutToBeMoved, this, &ImageModel::signal_modified);
    connect(this, &ImageModel::modelAboutToBeReset, this, &ImageModel::signal_modified);
}

Image *ImageModel::AddImage(QString name, QString path)
{
    Image* newImage = createImage(name, path);
    beginInsertRows(QModelIndex(), m_imageDefinition.Images().count(), m_imageDefinition.Images().count());
    m_imageDefinition.AddImage(newImage);
    endInsertRows();
    emit signal_ImageDataChanged();
    return newImage;
}

Image* ImageModel::createImage(QString name, QString path)
{
    quint32 nextAvailableId = m_imageDefinition.GetNextAvailableImageId();
    QString imagePath = m_project->GetProjectRoot().absoluteFilePath(path);
    QImage image;
    image.load(imagePath);
    Image* newImage = new Image();
    newImage->SetName(name);
    newImage->SetPath(path);
    newImage->SetID(nextAvailableId);
    newImage->SetWidth(image.width());
    newImage->SetHeight(image.height());
    return newImage;
}

int ImageModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return m_imageDefinition.Images().count();
}

int ImageModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return ImageModelColumnCount;
}

QVariant ImageModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (index.row() >= m_imageDefinition.Images().count()) return QVariant();

    Image* image = m_imageDefinition.Images().at(index.row());
    switch (role)
    {
        case Qt::ForegroundRole:
            switch (index.column())
            {
                case PathColumn:
                case ResolutionColumn:
                case SizeColumn:
                    return QBrush(QColor(0x78, 0x78, 0x78));
                default:
                    return QBrush();
            }
        case Qt::DisplayRole:
        case Qt::EditRole:
            switch (index.column())
            {
                case NameColumn:
                    return QVariant::fromValue(image->GetName());
                case PathColumn:
                    return QVariant::fromValue(image->GetPath());
                case DescriptionColumn:
                    return QVariant::fromValue(image->GetDescription());
                case ResolutionColumn:
                {
                    QString resolution = QString::number(image->GetWidth()) + "x" + QString::number(image->GetHeight());
                    return QVariant::fromValue(resolution);
                }
                case SizeColumn:
                    return QVariant::fromValue(image->GetSize());
                case StoreInExternalFlashColumn:
                    return QVariant::fromValue(image->GetStoreOnExternalFlash());
                case UseForNonAnimations:
                    return QVariant::fromValue(image->GetUseForNonAnimations());
                case UseForAnimations:
                    return QVariant::fromValue(image->GetUseForAnimations());
                default:
                    return QVariant();
            }
        default:
            return QVariant();
    }
}

ImageDefinition* ImageModel::GetImageDefintion() const
{
    return &m_imageDefinition;
}

bool ImageModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) return false;

    bool dataWasChanged = false;

    Image* image = m_imageDefinition.Images().at(index.row());

    switch (index.column())
    {
        case NameColumn:
            if (image->GetName().compare(value.toString()) != 0 && !value.toString().isEmpty())
            {
                image->SetName(value.toString());
                dataWasChanged = true;
            }
            break;
        case DescriptionColumn:
            if (image->GetDescription().compare(value.toString()) != 0)
            {
                image->SetDescription(value.toString());
                dataWasChanged = true;
            }
           break;
        case PathColumn:
            if (image->GetPath().compare(value.toString()) != 0)
            {
                image->SetPath(value.toString());
                dataWasChanged = true;
            }
            break;
        case StoreInExternalFlashColumn:
            if (image->GetStoreOnExternalFlash() != value.toBool())
            {
                image->SetStoreOnExternalFlash(value.toBool());
                dataWasChanged = true;
                emit m_imageDefinition.signal_CalculateImageMemoryUsage();
            }
            break;
        case UseForAnimations:
            if (image->GetUseForAnimations() != value.toBool())
            {
                image->SetUseForAnimations(value.toBool());
                dataWasChanged = true;
            }
            break;
        case UseForNonAnimations:
            if (image->GetUseForNonAnimations() != value.toBool())
            {
                image->SetUseForNonAnimations(value.toBool());
                dataWasChanged = true;
            }
            break;
        default:
            break;
    }

    if (dataWasChanged)
    {
        emit dataChanged(index, index, QVector<int>() << role);
        emit signal_ImageDataChanged();
    }

    return dataWasChanged;
}

QVariant ImageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
    {
        if (section >= m_imageDefinition.Images().count()) return QVariant("");
        if (role != Qt::DisplayRole) return QVariant();

        return QVariant::fromValue(m_imageDefinition.Images().at(section)->GetID());
    }
    else
    {
        switch (role)
        {
            case Qt::FontRole:
                return QVariant::fromValue(m_headerFont);
            case Qt::DisplayRole:
                switch (section)
                {
                    case NameColumn:
                        return QVariant("Name");
                    case DescriptionColumn:
                        return QVariant("Description");
                    case PathColumn:
                        return QVariant("Path");
                    case ResolutionColumn:
                        return QVariant("Resolution");
                    case SizeColumn:
                        return QVariant("Size in bytes");
                    case StoreInExternalFlashColumn:
                        return QVariant("Store in External Flash");
                    case UseForAnimations:
                        return QVariant("Use for Animations");
                    case UseForNonAnimations:
                        return QVariant("Use for Layouts/Menus");
                    default:
                        return QVariant();
                }
            case Qt::TextAlignmentRole:
                switch (section)
                {
                    case DescriptionColumn:
                        return Qt::AlignLeft;
                    default:
                        return QVariant();
                }
            default:
                return QVariant();
        }
    }

}

Qt::ItemFlags ImageModel::flags(const QModelIndex& index) const
{
    switch (index.column())
    {
        case NameColumn:
        case DescriptionColumn:
        case StoreInExternalFlashColumn:
        case UseForNonAnimations:
        case UseForAnimations:
            return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
        default:
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
}

bool ImageModel::removeRows(int row, int count, const QModelIndex& parent)
{
    int lastRowIndex = row + count - 1;

    beginRemoveRows(parent, row, lastRowIndex);
    int indexToRemove = lastRowIndex;
    for (; indexToRemove >= row; --indexToRemove)
    {
        m_imageDefinition.RemoveImageAtIndex(indexToRemove);
    }
    endRemoveRows();

    emit signal_ImageDataChanged();

    return true;

}
