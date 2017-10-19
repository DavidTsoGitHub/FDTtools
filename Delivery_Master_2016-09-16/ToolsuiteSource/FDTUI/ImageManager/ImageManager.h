#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QStringList>
class ImagesInUseDialog;

class CheckBoxSortProxyModel;
class ImageModel;
class MenuItem;
class Project;
class ProjectObject;
template <typename T> class QList;
class QModelIndex;
class QTableWidget;
class QWidget;
class QItemSelection;

namespace Ui {
class ImageManager;
}

class ImageManager : public QWidget
{
    Q_OBJECT

public:
    explicit ImageManager(QWidget *parent = 0);
    ~ImageManager();

signals:
    void signal_modified();
    void signal_ImageDataChanged();
    void signal_imageSelectionChanged(ProjectObject* projectObject);

public slots:
    void SetProject(Project *project);
    void ClearProject();
    void JustSelected();

private slots:
    void slot_addImageButtonClicked();
    void slot_removeImageButtonClicked();
    void slot_replaceImageButtonClicked();
    void slot_imageSelectionChanged();
    void slot_projectUpdated();
    void slot_editOtherSelected(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void slot_imageInUseDialogRemoveImages();
    void slot_imageInUseDialogClosed();

private:
    Ui::ImageManager *m_ui;
    Project* m_project;
    ImageModel* m_imageModel;
    CheckBoxSortProxyModel* m_proxyModel;
    QList<quint32> m_usedImages;
    ProjectObject* m_currentSelectedImage;
    QStringList m_imageUsedInItems;
    QStringList m_imageUsedInMenu;
    ImagesInUseDialog* m_imageInUseDialog;
    QItemSelection m_removeImagesSelection;

    void calculateMemoryUsage();
    void addMenuItemImages(const QList<MenuItem *> &menuItems);
    void findAllUsedImages();
    bool validateImageDimensions(QString fileName);
    bool copyImageToProjectDir(QFileInfo expectedProjectFile, QString fileName);

    QStringList findImageUsage(quint32 selectedImage);

    void removeImageUsage(quint32 selectedImage);
    void removeImageUsageInMenu(const QList<MenuItem *> &menuItems, int selectedImage);
    void findImageUsageInMenu(const QList<MenuItem *> &menuItems, int selectedImage);
};

#endif // IMAGEMANAGER_H
