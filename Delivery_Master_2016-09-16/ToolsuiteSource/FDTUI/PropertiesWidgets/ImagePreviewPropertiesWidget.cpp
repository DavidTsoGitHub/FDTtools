#include "ImagePreviewPropertiesWidget.h"

#include "Icon.h"
#include "Image.h"
#include "Project.h"

#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QString>
#include <QWidget>


ImagePreviewPropertiesWidget::ImagePreviewPropertiesWidget(Project* project, QWidget *parent) :
    ProjectObjectPropertiesWidget(project, parent),
    m_project(project),
    m_icon(NULL),
    m_image(NULL),
    m_imageName(NULL),
    m_imageLabel(NULL),
    m_pixmap(NULL)
{
}

void ImagePreviewPropertiesWidget::SetIcon(Icon* icon)
{
    m_icon = icon;
    loadProperties(icon);
    SetProjectObject(icon);
}

void ImagePreviewPropertiesWidget::SetImage(Image* image)
{
    m_image = image;
    loadProperties(image);
    SetProjectObject(image);
}

void ImagePreviewPropertiesWidget::loadProperties(Image* image)
{
    m_fileName = image->GetFileName();
    m_filePath = m_project->GetImageDefinition().GetResource(m_image->GetID());
}

void ImagePreviewPropertiesWidget::loadProperties(Icon* icon)
{
    m_fileName = icon->GetFileName();
    m_filePath = m_project->GetIconDefinition().GetResource(icon->GetID());
}

void ImagePreviewPropertiesWidget::beforePopulateSignals()
{
    m_imageName = new QLineEdit();
    AddEditableData("File name", m_imageName);

    m_imageName->setText(m_fileName);
    m_imageName->setEnabled(false);

    QSizePolicy sizePolicy = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHeightForWidth(true);

    m_pixmap = new QPixmap;
    m_pixmap->load(m_filePath);

    AddEditableData("Width", new QLabel(QString::number(m_pixmap->width())));
    AddEditableData("Height", new QLabel(QString::number(m_pixmap->height())));

    m_imageLabel = new QLabel;
    m_imageLabel->setSizePolicy(sizePolicy);
    m_imageLabel->setPixmap(*m_pixmap);

    QTreeWidgetItem* imageRow = AddEditableData("Preview", m_imageLabel);
    imageRow->setSizeHint(1, QSize(m_imageLabel->size()));

    m_pixmap->scaled(m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

}

void ImagePreviewPropertiesWidget::afterPopulateUIHook()
{
    //override the standard column resizing
}

void ImagePreviewPropertiesWidget::resizeEvent(QResizeEvent* event)
{
    if (!m_pixmap->isNull())
    {
        m_imageLabel->setPixmap(m_pixmap->scaled(m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    (void)event;
}

void ImagePreviewPropertiesWidget::showEvent(QShowEvent* event)
{
    if (!m_pixmap->isNull())
    {
        m_imageLabel->setPixmap(m_pixmap->scaled(m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    (void)event;
}
