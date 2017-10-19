#include "ImageSelectorComboBox.h"

#include "FunctionBlock.h"
#include "Image.h"
#include "ImageDefinition.h"
#include "Project.h"
#include <QComboBox>
#include <QString>

ImageSelectorComboBox::ImageSelectorComboBox(FunctionBlock* functionBlock, QString imageResourceName, Project* project, QWidget* parent) :
    QComboBox(parent),
    m_imageDefinition(project->GetImageDefinition()),
    m_imageResourceName(imageResourceName),
    m_functionBlock(functionBlock)
{
    connect(project, &Project::PropertyUpdated, this, &ImageSelectorComboBox::projectUpdated);

    populate();
    connect(this, static_cast<void(ImageSelectorComboBox::*)(const QString&)>(&ImageSelectorComboBox::currentIndexChanged), this, &ImageSelectorComboBox::slot_currentIndexChanged);
}

void ImageSelectorComboBox::populate()
{
    quint32 imageResourceID = m_functionBlock->GetImageResourceID(m_imageResourceName);
    QString selectedItem("Not selected");

    m_signalsEnabled = false;

    clear();

    addItem("Not selected", QVariant(0));
    foreach (Image* image, m_imageDefinition.Images())
    {
        if(!image->GetUseForNonAnimations())
            continue;

        if(image->GetID() == imageResourceID)
            selectedItem = image->GetName();

        addItem(image->GetName(), QVariant(image->GetID()));
    }

    setCurrentText(selectedItem);

    m_signalsEnabled = true;
}

void ImageSelectorComboBox::projectUpdated()
{
    if (m_imageDefinition.IsDirty())
    {
        populate();
    }
}

void ImageSelectorComboBox::slot_currentIndexChanged(QString)
{
    if (!m_signalsEnabled) return;

    QVariant resourceID = currentData();
    m_functionBlock->SetImageResourceID(m_imageResourceName, resourceID.toInt());
}
