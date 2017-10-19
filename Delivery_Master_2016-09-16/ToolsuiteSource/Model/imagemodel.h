#ifndef IMAGEMODEL
#define IMAGEMODEL

#include "ImageDefinition.h"
#include "Project.h"

#include <QAbstractTableModel>

class ImageModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ImageModel(ImageDefinition& images, QObject* parent = 0);

    Image* AddImage(QString name, QString path);

    ImageDefinition* GetImageDefintion() const;

    typedef enum
    {
        NameColumn,
        PathColumn,
        DescriptionColumn,
        ResolutionColumn,
        SizeColumn,
        StoreInExternalFlashColumn,
        UseForNonAnimations,
        UseForAnimations,
        ImageModelColumnCount
    } ImageModelColumns;

signals:
    void signal_modified();
    void signal_ImageDataChanged();

private:
    ImageDefinition& m_imageDefinition;
    Project* m_project;
    QFont m_headerFont;

    Image *createImage(QString name, QString path);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool removeRows(int row, int count, const QModelIndex& parent);

};

#endif // IMAGEMODEL
