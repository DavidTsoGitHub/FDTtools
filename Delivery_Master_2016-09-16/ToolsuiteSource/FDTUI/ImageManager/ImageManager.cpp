#include "ImageManager.h"
#include "ui_ImageManager.h"

#include "Layout.h"
#include "FunctionBlock.h"
#include "ImageResource.h"
#include "BSPDisplay.h"
#include "bspmemory.h"
#include "CheckBoxSortProxyModel.h"
#include "ImageModel.h"
#include "ImageWidget.h"
#include "MenuItem.h"
#include "Project.h"
#include "ProjectObject.h"
#include "ImagesInUseDialog.h"

#include <QImageReader>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QModelIndex>
#include <QTableWidget>
#include <QWidget>

ImageManager::ImageManager(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ImageManager),
    m_project(NULL),
    m_imageModel(NULL),
    m_currentSelectedImage(NULL),
    m_imageInUseDialog(NULL)
{
    m_ui->setupUi(this);

    connect(m_ui->addImageButton, &QPushButton::clicked, this, &ImageManager::slot_addImageButtonClicked);
    connect(m_ui->removeImageButton, &QPushButton::clicked, this, &ImageManager::slot_removeImageButtonClicked);
    connect(m_ui->replaceImageButton, &QPushButton::clicked, this, &ImageManager::slot_replaceImageButtonClicked);
}

ImageManager::~ImageManager()
{
    if (m_imageInUseDialog != NULL)
    {
        delete m_imageInUseDialog;
    }
    delete m_ui;
    delete m_imageModel;
}

void ImageManager::SetProject(Project *project)
{
    m_project = project;

    m_imageModel = new ImageModel(m_project->GetImageDefinition());

    QList<int> columnsWithCheckbox;
    columnsWithCheckbox.append(ImageModel::StoreInExternalFlashColumn);

    findAllUsedImages();
    m_proxyModel = new CheckBoxSortProxyModel(m_project);
    m_proxyModel->SetExternalFlashColumn(ImageModel::StoreInExternalFlashColumn, m_usedImages);
    m_proxyModel->SetCheckBoxColumns({ImageModel::UseForNonAnimations, ImageModel::UseForAnimations});
    m_proxyModel->setSourceModel(m_imageModel);

    m_ui->imageTable->setModel(m_proxyModel);
    m_ui->imageTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_ui->imageTable->resizeColumnsToContents();
    m_ui->imageTable->resizeRowsToContents();

    calculateMemoryUsage();

    connect(m_ui->imageTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ImageManager::slot_imageSelectionChanged);
    connect(m_imageModel, &ImageModel::signal_modified, this, &ImageManager::signal_modified);
    connect(m_imageModel, &ImageModel::signal_ImageDataChanged, this, &ImageManager::calculateMemoryUsage);
    connect(m_imageModel, &ImageModel::signal_ImageDataChanged, this, &ImageManager::signal_ImageDataChanged);
    connect(m_project, &Project::PropertyUpdated, this, &ImageManager::slot_projectUpdated);

    connect(m_imageModel, &ImageModel::dataChanged, this, &ImageManager::slot_editOtherSelected);

    connect(&m_project->GetLabelDefinition(), &LabelDefinition::signal_CalculateFontMemoryUsage, this, &ImageManager::calculateMemoryUsage);
}

void ImageManager::ClearProject()
{
    m_project = NULL;
    delete m_imageModel;
    m_imageModel = NULL;
}

void ImageManager::JustSelected()
{
    if (m_currentSelectedImage != NULL)
    {
        emit signal_imageSelectionChanged(m_currentSelectedImage);
    }
}

void ImageManager::slot_projectUpdated()
{
    findAllUsedImages();
    m_proxyModel->SetExternalFlashColumn(ImageModel::StoreInExternalFlashColumn, m_usedImages);
    calculateMemoryUsage();
}

void ImageManager::slot_editOtherSelected(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(bottomRight);

    QList<int> multiEditColumns({ImageModel::StoreInExternalFlashColumn, ImageModel::UseForNonAnimations, ImageModel::UseForAnimations});
    if(!multiEditColumns.contains(topLeft.column()))
        return;

    QModelIndexList selected = m_ui->imageTable->selectionModel()->selectedRows();
    foreach(QModelIndex index, selected)
    {
        if(index.row() == topLeft.row())
            continue;

        disconnect(m_imageModel, &ImageModel::dataChanged, this, &ImageManager::slot_editOtherSelected);
        m_imageModel->setData(m_imageModel->index(index.row(),topLeft.column()), topLeft.data(Qt::EditRole), Qt::EditRole);
        connect(m_imageModel, &ImageModel::dataChanged, this, &ImageManager::slot_editOtherSelected);
    }
}

void ImageManager::ImageManager::slot_imageInUseDialogClosed()
{
    delete m_imageInUseDialog;
    m_imageInUseDialog = NULL;
}

void ImageManager::slot_imageSelectionChanged()
{
    m_ui->removeImageButton->setEnabled(m_ui->imageTable->selectionModel()->hasSelection());
    m_ui->replaceImageButton->setEnabled(m_ui->imageTable->selectionModel()->hasSelection() && m_ui->imageTable->selectionModel()->selectedRows().count() == 1);

    if (m_ui->imageTable->selectionModel()->hasSelection() && m_ui->imageTable->selectionModel()->selectedRows().count() == 1)
    {
        m_ui->replaceImageButton->setEnabled(true);
        int selectedIndex = m_ui->imageTable->selectionModel()->currentIndex().row();

        m_currentSelectedImage = static_cast<ProjectObject*>(m_imageModel->GetImageDefintion()->Images().at(selectedIndex));
        emit signal_imageSelectionChanged(m_currentSelectedImage);
    }
}

void ImageManager::calculateMemoryUsage()
{
    quint32 memoryUsedInExternalFlash = m_project->GetImageDefinition().GetImageSizeInExternalFlash();
    quint32 memoryUsedInInternalFlash = m_project->GetImageDefinition().GetImageSizeInInternalFlash();

    const BSPMemory* memory = m_project->GetBSPSpecification().GetMemory();

    m_ui->imagesInInternalFlashLabel->setText(QString::number(memoryUsedInInternalFlash) + " bytes");
    m_ui->imagesInExternalFlashLabel->setText(QString::number(memoryUsedInExternalFlash) + " bytes");

    quint32 totalInternalFlashUsage = m_project->GetLabelDefinition().GetFontSizeInInternalFlash() + memoryUsedInInternalFlash;
    quint32 totalExternalFlashUsage = m_project->GetLabelDefinition().GetFontSizeInExternalFlash() + memoryUsedInExternalFlash;

    m_ui->externalFlashUsageLabel->setText("~" + QString::number(totalExternalFlashUsage) + " / " + QString::number(memory->GetExternalMemorySize()) + " bytes");
    m_ui->internalFlashUsageLabel->setText("~" + QString::number(totalInternalFlashUsage) + " / " + QString::number(memory->GetInternalMemorySize()) + " bytes");
}

void ImageManager::slot_addImageButtonClicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
                                                          tr("Select one or more image files"),
                                                          m_project->GetProjectRoot().absolutePath(),
                                                          QString("Images (*.png *.jpg *.bmp *.gif)"));

    if(fileNames.isEmpty())
        return;

    bool addingMultipleImages = fileNames.size() > 1;
    foreach(QString fileName, fileNames) {
        QFileInfo fileInfo(fileName);
        QDir projectPath(m_project->GetProjectRoot());

        // Check dimensions
        if(!validateImageDimensions(fileName))
            continue;

        // For gif files, explode into individual frames and add each frame
        if(fileInfo.suffix() == "gif")
        {
            QImage image;
            QImageReader reader(fileName);
            int frameIdx = 0;
            while(reader.read(&image))
            {
                // Save frame to temp folder
                QString frameFileName = QDir::tempPath() + QDir::separator() +
                        fileInfo.baseName() + "_frame_" + QString::number(frameIdx) + ".png";
                image.save(frameFileName, "png");
                // Copy image file
                QFileInfo frameInfo(frameFileName);
                QFileInfo expectedProjectFile(projectPath.absoluteFilePath(Project::ImagesDirectoryName +  "/" + frameInfo.fileName()));
                QString path = expectedProjectFile.absoluteFilePath().replace(m_project->GetProjectRoot().absolutePath() + "/","");
                bool success = copyImageToProjectDir(expectedProjectFile, frameFileName);
                // Remove temporary frame file
                QFile tmpFile(frameFileName);
                tmpFile.remove();
                if(!success)
                    break;

                // Add frame to model
                Image* newImage = m_imageModel->AddImage(frameInfo.fileName(), path);
                newImage->SetUseForAnimations(true);
                newImage->SetUseForNonAnimations(false);
                frameIdx++;
            }

            continue;
        }

        // Regular image files
        // Copy image file
        QFileInfo expectedProjectFile(projectPath.absoluteFilePath(Project::ImagesDirectoryName +  "/" + fileInfo.fileName()));
        QString path = expectedProjectFile.absoluteFilePath().replace(m_project->GetProjectRoot().absolutePath() + "/","");
        if(!copyImageToProjectDir(expectedProjectFile, fileName))
            continue;

        // Add to model
        Image *newImage = m_imageModel->AddImage(fileInfo.fileName(), path);
        newImage->SetUseForNonAnimations(true);
        newImage->SetUseForAnimations(addingMultipleImages);
    }
}

void ImageManager::slot_removeImageButtonClicked()
{
    if (m_imageInUseDialog != NULL)
    {
        disconnect(m_imageInUseDialog, &QDialog::accepted, this, &ImageManager::slot_imageInUseDialogRemoveImages);
        disconnect(m_imageInUseDialog, &QDialog::accepted, this, &ImageManager::slot_imageInUseDialogClosed);
        m_imageInUseDialog->close();
        delete m_imageInUseDialog;
    }

    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Delete image(s)", "You are about to delete image(s). The image(s) will be removed from the project and the hard drive! Do you want to proceed?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        return;
    }

    m_removeImagesSelection = m_ui->imageTable->selectionModel()->selection();

    m_imageUsedInItems.clear();

    foreach (QItemSelectionRange range, m_removeImagesSelection)
    {
        for (int i = range.top(); i <= range.bottom(); ++i)
        {
            quint32 selectedImage = (quint32)i + 1u;
            QStringList usedInItems = findImageUsage(selectedImage);

            if (usedInItems.count() > 0)
            {
                QString fileName = m_imageModel->GetImageDefintion()->Images().at(i)->GetName();
                usedInItems.insert(0, fileName);
                m_imageUsedInItems.append(usedInItems);
                m_imageUsedInItems.append("\n");
            }
        }
    }

    if (m_imageUsedInItems.count() > 0)
    {
        QString messageString = "The following image(s) are still used in the project in the following items:\n\n" + m_imageUsedInItems.join("\n") + "\n\nDo you still want to remove the image(s)?";
        m_imageInUseDialog = new ImagesInUseDialog(messageString);
        m_imageInUseDialog->setWindowTitle("Images in use");
        m_imageInUseDialog->setModal(false);
        connect(m_imageInUseDialog, &QDialog::accepted, this, &ImageManager::slot_imageInUseDialogRemoveImages);
        connect(m_imageInUseDialog, &QDialog::rejected, this, &ImageManager::slot_imageInUseDialogClosed);
        m_imageInUseDialog->show();

    }
    else
    {
        foreach(QModelIndex index, m_removeImagesSelection.indexes())
        {
            int selectedIndex = index.row();

            QString fileName = m_project->GetProjectRoot().absoluteFilePath(m_imageModel->GetImageDefintion()->Images().at(selectedIndex)->GetPath());
            QFile file(fileName);
            if (file.exists())
            {
                bool fileRemoved = file.remove();
                if (!fileRemoved)
                {
                    QMessageBox::warning(this, "File removal failed", "The file: " + fileName + " could not be removed. Please try closing any application that could be using the file and remove the file manually.", QMessageBox::Ok);
                }
            }
        }

        foreach (QItemSelectionRange range, m_removeImagesSelection)
        {
            m_imageModel->removeRows(range.top(), range.height(), range.parent());
        }

        m_currentSelectedImage = NULL;
        emit signal_imageSelectionChanged(NULL);
    }
}

void ImageManager::slot_replaceImageButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select new image file"), m_project->GetProjectRoot().absolutePath(), QString("Images (*.png *.jpg *.bmp)"));

    if(!fileName.isEmpty())
    {
        QFileInfo imageFileInfo(fileName);
        QDir projectPath(m_project->GetProjectRoot());
        QFileInfo expectedProjectImageFile(projectPath.absoluteFilePath(Project::ImagesDirectoryName +  "/" + imageFileInfo.fileName()));

        if (!expectedProjectImageFile.absoluteDir().exists())
        {
           expectedProjectImageFile.absoluteDir().mkpath(".");
        }

        if (!expectedProjectImageFile.exists())
        {
           QFile fontFile(fileName);

           if (!fontFile.open(QFile::ReadOnly))
           {
               QMessageBox::warning(this, "Error opening image file!", "Couldn't open image file for copying:\n" + fontFile.errorString());
               return;
           }

           if (!fontFile.copy(expectedProjectImageFile.absoluteFilePath()))
           {
               QMessageBox::warning(this, "Error copying image file!", "Couldn't copy image file!\n" + fontFile.fileName());
           }
        }

        QString imagePath = expectedProjectImageFile.absoluteFilePath().replace(m_project->GetProjectRoot().absolutePath() + "/","");

        QModelIndex selectedImageIndex = m_ui->imageTable->selectionModel()->currentIndex();

        if (selectedImageIndex.isValid())
        {
            QModelIndex imagePathIndex = m_imageModel->index(selectedImageIndex.row(), ImageModel::PathColumn);
            m_imageModel->setData(imagePathIndex, imagePath, Qt::EditRole);
        }
    }
}

void ImageManager::slot_imageInUseDialogRemoveImages()
{
    foreach(QModelIndex index, m_removeImagesSelection.indexes())
    {
        int selectedIndex = index.row();
        int selectedImage = selectedIndex + 1;

        QString fileName = m_project->GetProjectRoot().absoluteFilePath(m_imageModel->GetImageDefintion()->Images().at(selectedIndex)->GetPath());
        QFile file(fileName);
        if (file.exists())
        {
            bool fileRemoved = file.remove();
            if (!fileRemoved)
            {
                QMessageBox::warning(this, "File removal failed", "The file: " + fileName + " could not be removed. Please try closing any application that could be using the file and remove the file manually.", QMessageBox::Ok);
            }
            else
            {
                removeImageUsage(selectedImage);
            }
        }
    }

    foreach (QItemSelectionRange range, m_removeImagesSelection)
    {
        m_imageModel->removeRows(range.top(), range.height(), range.parent());
    }

    m_currentSelectedImage = NULL;
    emit signal_imageSelectionChanged(NULL);
}

void ImageManager::removeImageUsage(quint32 selectedImage)
{
    foreach (Layout* layout, m_project->GetLayoutDefinition().Layouts())
    {
        QList<UIWidgetBase*> widgets;
        layout->GetUIWidgets(widgets);

        foreach (UIWidgetBase* widget, widgets)
        {
            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_Image)
            {
                ImageWidget* imageWidget = static_cast<ImageWidget*>(widget);

                if (imageWidget->GetImageID() == selectedImage)
                {
                    imageWidget->SetHeight(0u);
                    imageWidget->SetWidth(0u);
                    imageWidget->SetImageID(std::numeric_limits<quint32>::max());
                }
            }

            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_Image || widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage)
            {
                if (widget->GetAnimationID(1) > 0)
                {
                    foreach (AnimationFrameByFrame* fbfAnimation, m_project->GetAnimationDefinition().FrameByFrameAnimations())
                    {
                        if (widget->GetAnimationID(1) == fbfAnimation->GetID())
                        {
                            foreach (quint32 imageId, fbfAnimation->ImageIds())
                            {
                                if (imageId == selectedImage)
                                {
                                    fbfAnimation->RemoveImageId(imageId);
                                }
                            }
                            break;
                        }
                    }
                }

                if (widget->GetAnimationID(2) > 0)
                {
                    foreach (AnimationFrameByFrame* fbfAnimation, m_project->GetAnimationDefinition().FrameByFrameAnimations())
                    {
                        if (widget->GetAnimationID(2) == fbfAnimation->GetID())
                        {
                            foreach (quint32 imageId, fbfAnimation->ImageIds())
                            {
                                if (imageId == selectedImage)
                                {
                                    fbfAnimation->RemoveImageId(imageId);
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }

        if (layout->HasMenu())
        {
            removeImageUsageInMenu(layout->GetMenu()->MenuRoot().MenuItems(), selectedImage);
        }
    }

    foreach (FunctionBlock* functionBlock, m_project->GetFunctionBlocks().List())
    {
        foreach (const ImageResource imageResource, functionBlock->GetFunctionDefinition().ImageResources())
        {
            quint32 imageID = functionBlock->GetImageResourceID(imageResource.Name);

            if (imageID == selectedImage)
            {
                functionBlock->SetImageResourceID(imageResource.Name, std::numeric_limits<quint32>::max());
            }
        }
    }
}

void ImageManager::removeImageUsageInMenu(const QList<MenuItem*>& menuItems, int selectedImage)
{
    foreach (MenuItem* menuItem, menuItems)
    {
        quint32 imageID = menuItem->GetImageID();

        if (imageID == (quint32)selectedImage)
        {
            menuItem->setImageID(std::numeric_limits<quint32>::max());
        }

        if (!menuItem->SubMenuItems().isEmpty())
        {
            removeImageUsageInMenu(menuItem->SubMenuItems(), selectedImage);
        }
    }
}

void ImageManager::findAllUsedImages()
{
    m_usedImages.clear();
    foreach (Layout* layout, m_project->GetLayoutDefinition().Layouts())
    {
        QList<UIWidgetBase*> widgets;
        layout->GetUIWidgets(widgets);

        foreach (UIWidgetBase* widget, widgets)
        {
            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_Image)
            {
                ImageWidget* imageWidget = static_cast<ImageWidget*>(widget);
                if (!m_usedImages.contains(imageWidget->GetImageID()))
                {
                    m_usedImages.append(imageWidget->GetImageID());
                }
            }

            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_Image || widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage)
            {
                if (widget->GetAnimationID(1) > 0)
                {
                    foreach (AnimationFrameByFrame* fbfAnimation, m_project->GetAnimationDefinition().FrameByFrameAnimations())
                    {
                        if (widget->GetAnimationID(1) == fbfAnimation->GetID())
                        {
                            foreach (quint32 imageId, fbfAnimation->ImageIds())
                            {
                                m_usedImages.append(imageId);
                            }
                            break;
                        }
                    }
                }

                if (widget->GetAnimationID(2) > 0)
                {
                    foreach (AnimationFrameByFrame* fbfAnimation, m_project->GetAnimationDefinition().FrameByFrameAnimations())
                    {
                        if (widget->GetAnimationID(2) == fbfAnimation->GetID())
                        {
                            foreach (quint32 imageId, fbfAnimation->ImageIds())
                            {
                                m_usedImages.append(imageId);
                            }
                            break;
                        }
                    }
                }
            }
        }

        if (layout->HasMenu())
        {
            addMenuItemImages(layout->GetMenu()->MenuRoot().MenuItems());
        }
    }

    foreach (FunctionBlock* functionBlock, m_project->GetFunctionBlocks().List())
    {
        foreach (const ImageResource imageResource, functionBlock->GetFunctionDefinition().ImageResources())
        {
            quint32 imageID = functionBlock->GetImageResourceID(imageResource.Name);

            if (!m_usedImages.contains(imageID))
            {
                m_usedImages.append(imageID);
            }
        }
    }

    m_project->GetImageDefinition().SetImagesUsed(m_usedImages);
}

void ImageManager::addMenuItemImages(const QList<MenuItem*>& menuItems)
{
    foreach (MenuItem* menuItem, menuItems)
    {
        quint32 imageID = menuItem->GetImageID();

        if (!m_usedImages.contains(imageID))
        {
            m_usedImages.append(imageID);
        }

        if (!menuItem->SubMenuItems().isEmpty())
        {
            addMenuItemImages(menuItem->SubMenuItems());
        }
    }
}

bool ImageManager::validateImageDimensions(QString fileName)
{
    QImage image(fileName);
    if(image.isNull())
        return false;

    if (m_project->GetBSPSpecification().GetDisplay()->GetRenderingOrientation() == BSPDisplay::CounterClockWise)
    {
        if ((image.height() % 2) == 1)
        {
            QMessageBox::warning(this, "Incorrect image dimensions", "The height of " + fileName + " needs to be divisible by 2. Resize the image and try to add it again.\n");
            return false;
        }
    }
    else
    {
        if ((image.width() % 2) == 1)
        {
            QMessageBox::warning(this, "Incorrect image dimensions", "The width of " + fileName + " needs to be divisible by 2. Resize the image and try to add it again.\n");
            return false;
        }
    }
    return true;
}

bool ImageManager::copyImageToProjectDir(QFileInfo expectedProjectFile, QString fileName)
{
    if (!expectedProjectFile.absoluteDir().exists())
        expectedProjectFile.absoluteDir().mkpath(".");

    if (!expectedProjectFile.exists())
    {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly))
        {
            QMessageBox::warning(this, "Error opening image file!", "Couldn't open image file for copying:\n" + file.errorString());
            return false;
        }

        if (!file.copy(expectedProjectFile.absoluteFilePath()))
        {
            QMessageBox::warning(this, "Error copying image file!", "Couldn't copy image file!\n" + file.fileName());
            return false;
        }
    }

    return true;
}

QStringList ImageManager::findImageUsage(quint32 selectedImage)
{
    m_imageUsedInMenu.clear();
    QStringList usedInItems;
    QStringList usedInWidgets;
    QStringList usedInAnimations;
    QStringList usedInFunctionBlocks;
    QStringList usedInCheckMessages;

    foreach (Layout* layout, m_project->GetLayoutDefinition().Layouts())
    {
        QList<UIWidgetBase*> widgets;
        layout->GetUIWidgets(widgets);

        foreach (UIWidgetBase* widget, widgets)
        {
            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_Image)
            {
                ImageWidget* imageWidget = static_cast<ImageWidget*>(widget);

                if (imageWidget->GetImageID() == selectedImage)
                {
                    if (usedInWidgets.count() == 0)
                    {
                        usedInWidgets.append("\tWidgets:");
                    }
                    DisplayArea* parentArea = static_cast<DisplayArea*>(imageWidget->GetParent());
                    usedInWidgets.append("\t\t" + layout->Name() + "->" + parentArea->GetName() + "->" + imageWidget->GetUserDefinedName());
                }
            }
        }

        if (layout->HasMenu())
        {
            findImageUsageInMenu(layout->GetMenu()->MenuRoot().MenuItems(), selectedImage);
        }

        if (layout->IsCheckMessageScreen())
        {
            foreach (UIWidgetBase* widget, widgets)
            {
                if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage)
                {
                    foreach (CheckMessage* checkMessage, m_project->GetCheckMessagesDefinition().CheckMessages())
                    {
                        foreach (CheckMessageItem* checkMessageItem, checkMessage->Items())
                        {
                            if (checkMessageItem->AssociatedWidgetID == widget->GetUIWidgetID())
                            {
                                if (checkMessageItem->ResourceID == selectedImage)
                                {
                                    if (usedInCheckMessages.count() == 0)
                                    {
                                        usedInCheckMessages.append("\tCheck messages:");
                                    }
                                    usedInCheckMessages.append("\t\t" + checkMessage->GetName());
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    foreach (const AnimationFrameByFrame* animFbf, m_project->GetAnimationDefinition().FrameByFrameAnimations())
    {
        foreach (quint32 fbfImageId, animFbf->ImageIds())
        {
            if (fbfImageId == selectedImage)
            {
                if (usedInAnimations.count() == 0)
                {
                    usedInAnimations.append("\tFrameByFrame animations:");
                }
                usedInAnimations.append("\t\t" + animFbf->GetName());
            }
        }
    }

    foreach (FunctionBlock* functionBlock, m_project->GetFunctionBlocks().List())
    {
        foreach (const ImageResource imageResource, functionBlock->GetFunctionDefinition().ImageResources())
        {
            quint32 imageID = functionBlock->GetImageResourceID(imageResource.Name);

            if (imageID == selectedImage)
            {
                if (usedInFunctionBlocks.count() == 0)
                {
                    usedInFunctionBlocks.append("\tFunction Blocks:");
                }

                usedInFunctionBlocks.append("\t\t" + functionBlock->GetFunctionDefinition().GetName());
            }
        }
    }

    if (usedInWidgets.count() > 0)
    {
        usedInItems.append(usedInWidgets);
    }

    if (usedInAnimations.count() > 0)
    {
        usedInItems.append(usedInAnimations);
    }

    if (m_imageUsedInMenu.count() > 0)
    {
        usedInItems.append(m_imageUsedInMenu);
    }

    if (usedInFunctionBlocks.count() > 0)
    {
        usedInItems.append(usedInFunctionBlocks);
    }

    if (usedInCheckMessages.count() > 0)
    {
        usedInItems.append(usedInCheckMessages);
    }

    return usedInItems;
}

void ImageManager::findImageUsageInMenu(const QList<MenuItem*>& menuItems, int selectedImage)
{
    foreach (MenuItem* menuItem, menuItems)
    {
        quint32 imageID = menuItem->GetImageID();

        if (imageID == (quint32)selectedImage)
        {
            if (m_imageUsedInMenu.count() == 0)
            {
                m_imageUsedInMenu.append("\tMenu Items:");
            }
            m_imageUsedInMenu.append("\t\t" + menuItem->GetName() + "->MenuItem Image");
        }

        if (!menuItem->SubMenuItems().isEmpty())
        {
            findImageUsageInMenu(menuItem->SubMenuItems(), selectedImage);
        }
    }
}
