#include "ImageManagerGenerator.h"

#include "Layout.h"
#include "FunctionBlock.h"
#include "ImageDefinition.h"
#include "ImageConverter.h"
#include "ImageWidget.h"
#include "DynamicImageWidget.h"
#include "AnimationDefinition.h"
#include "Logger.h"
#include "MenuItem.h"
#include "RegEx/RegExPatterns.h"
#include "BSPDisplay.h"
#include "bspmemory.h"

ImageManagerGenerator::ImageManagerGenerator(Project *project) :
    m_project(project)
{
    m_imageManagerHeaderPath = QString(PlatformPaths::CreateServicePath("ImageManager")).append("/ImageManager.h");
    m_imageManagerSourcePath = QString(PlatformPaths::CreateServicePath("ImageManager")).append("/ImageManager.c");
    m_imageRoot = QString(PlatformPaths::CreateServicePath("Images"));

    foreach (Layout* layout, m_project->GetLayoutDefinition().Layouts())
    {
        QList<UIWidgetBase*> widgets;
        layout->GetUIWidgets(widgets);

        foreach (UIWidgetBase* widget, widgets)
        {
            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_Image)
            {
                ImageWidget* imageWidget = static_cast<ImageWidget*>(widget);
                if (!m_imageIndexes.contains(imageWidget->GetImageID()))
                {
                    m_imageIndexes.append(imageWidget->GetImageID());
                }
            }
            else if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage)
            {
                DynamicImageWidget* dImageWidget = static_cast<DynamicImageWidget*>(widget);
                int animation1ID = dImageWidget->GetAnimationID(1);
                int animation2ID = dImageWidget->GetAnimationID(2);

                if (animation1ID != 0)
                {
                    const Animation* animation1 = m_project->GetAnimationDefinition().GetAnimationByID(animation1ID);
                    if(animation1->GetAnimationType() == Animation::Type::FrameByFrame)
                    {
                        foreach(quint32 id, static_cast<const AnimationFrameByFrame*>(animation1)->ImageIds())
                        {
                            m_imageIndexes.append(id);
                        }
                    }
                }

                if (animation2ID != 0)
                {
                    const Animation* animation2 = m_project->GetAnimationDefinition().GetAnimationByID(animation2ID);
                    if(animation2->GetAnimationType() == Animation::Type::FrameByFrame)
                    {
                        foreach(quint32 id, static_cast<const AnimationFrameByFrame*>(animation2)->ImageIds())
                        {
                            m_imageIndexes.append(id);
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

            if ((!m_imageIndexes.contains(imageID)) && (imageID > 0))
            {
                m_imageIndexes.append(imageID);
            }
        }
    }

    m_imageIndexes.removeAll(0u); // 0 means no image so we do not need to generate anything for that
}

ImageManagerGenerator::~ImageManagerGenerator()
{

}

void ImageManagerGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");
    generateImageManagerHeader();
    generateImageManagerSource();
    generateImageSources();
    generateImageIncludeHeader();
}

void ImageManagerGenerator::generateImageManagerHeader()
{
    QStringList headerLines;

    headerLines.append("#ifndef IMAGE_MANAGER_H");
    headerLines.append("#define IMAGE_MANAGER_H");
    headerLines.append("");
    headerLines.append("/********************************* Includes **********************************/");
    headerLines.append("#include \"StandardTypes.h\"");
    headerLines.append("#include \"Image.h\"");
    headerLines.append("");
    headerLines.append("/**************************** Constants and Types ****************************/");
    headerLines.append("");
    headerLines.append("/***************************** Exported Functions ****************************/");
    headerLines.append("const image_t* ImageManager_GetImage(uint32 imageID);");
    headerLines.append("");
    headerLines.append("#endif");
    headerLines.append("");

    AddGeneratedFile(m_imageManagerHeaderPath, headerLines.join("\n"));
}

void ImageManagerGenerator::generateImageManagerSource()
{
    ImageDefinition& imageDefinition = m_project->GetImageDefinition();

    QStringList sourceLines;
    sourceLines.append("//lint -save");
    sourceLines.append("/********************************* Includes **********************************/");
    sourceLines.append("#include \"ImageManager.h\"");
    if (imageDefinition.Images().count() > 0)
    {
        sourceLines.append("#include \"Images.h\"");
        sourceLines.append("");
        sourceLines.append("/**************************** Constants and Types ****************************/");
        sourceLines.append("typedef struct");
        sourceLines.append("{");
        sourceLines.append("    uint32 imageID;");
        sourceLines.append("    const image_t* image;");
        sourceLines.append("} image_definition_t;");
        sourceLines.append("");
        sourceLines.append("/********************************* Variables *********************************/");
        sourceLines.append("");
        sourceLines.append("/************************* Local Function Prototypes *************************/");
        sourceLines.append("");
        sourceLines.append("/**************************** Constants and Types ****************************/");
        sourceLines.append("#define NUMBER_OF_IMAGES " + QString::number(m_imageIndexes.count()) + "u");
        sourceLines.append("");
        sourceLines.append("//lint -esym(9003,imageDefinitions) [Supressed as we need to separate generated and static code.]");
        sourceLines.append("static const image_definition_t imageDefinitions[NUMBER_OF_IMAGES] = ");
        sourceLines.append("{");
        QStringList imageMappings;
        foreach (quint16 index, m_imageIndexes)
        {
            QString imageMapping("    { .imageID = %1, .image=&Image%1 }");
            imageMappings.append(imageMapping.arg(index));
        }
        sourceLines.append(imageMappings.join(",\n"));
        sourceLines.append("};");
        sourceLines.append("");
    }

    sourceLines.append("/***************************** Exported Functions ****************************/");
    sourceLines.append("const image_t* ImageManager_GetImage(uint32 imageID)");
    sourceLines.append("{");
    sourceLines.append("    const image_t* returnValue = NULL;");

    if (imageDefinition.Images().count() > 0)
    {
        sourceLines.append("    for (uint32 i = 0; i < NUMBER_OF_IMAGES; ++i)");
        sourceLines.append("    {");
        sourceLines.append("        if (imageDefinitions[i].imageID == imageID)");
        sourceLines.append("        {");
        sourceLines.append("            returnValue = imageDefinitions[i].image;");
        sourceLines.append("            break;");
        sourceLines.append("        }");
        sourceLines.append("    }");
        sourceLines.append("");
    }
    else
    {
        sourceLines.append("    (void)imageID;");
    }
    sourceLines.append("    return returnValue;");
    sourceLines.append("}");
    sourceLines.append("");
    sourceLines.append("//lint -restore");
    sourceLines.append("");

    AddGeneratedFile(m_imageManagerSourcePath, sourceLines.join("\n"));
}

void ImageManagerGenerator::generateImageSources()
{
    foreach (Image* image, m_project->GetImageDefinition().Images())
    {
        if (m_imageIndexes.contains(image->GetID()))
        {
            QString imageName = "Image" + QString::number(image->GetID());
            imageName = imageName.replace(RegExPatterns::NotAcceptedCodeCharacters(), "");

            QString imageFilePath(m_project->GetProjectRoot().absoluteFilePath(image->GetPath()));

            bool rotated = m_project->GetBSPSpecification().GetDisplay()->GetRenderingOrientation() == BSPDisplay::CounterClockWise ? true : false;
            bool storeInExternalFlash = (m_project->GetBSPSpecification().GetMemory()->GetExternalMemorySize() > 0) && (image->GetStoreOnExternalFlash());
            QString imageSource = ImageConverter::GenerateImageSource(imageFilePath, image->GetID(), rotated, storeInExternalFlash);

            QString path(m_imageRoot + "/" + imageName + ".c");
            AddGeneratedFile(path, imageSource);

            m_imageSourceFiles.append(imageName);
        }
    }
}

void ImageManagerGenerator::generateImageIncludeHeader()
{
    QStringList headerLines;
    QString inclusionGuardName("IMAGES_H");

    headerLines.append("#ifndef " + inclusionGuardName);
    headerLines.append("#define " + inclusionGuardName);
    headerLines.append("");
    headerLines.append("/********************************* Includes **********************************/");
    headerLines.append("");
    headerLines.append("#include \"Image.h\"");
    headerLines.append("");
    headerLines.append("/********************************* Constants and Variables **********************************/");

    foreach (QString imageSource, m_imageSourceFiles)
    {
        headerLines.append("extern const image_t " + imageSource + ";");
    }

    headerLines.append("");
    headerLines.append("#endif");
    headerLines.append("");

    AddGeneratedFile(m_imageRoot + "/Images.h", headerLines.join("\n"));
}

void ImageManagerGenerator::addMenuItemImages(const QList<MenuItem*>& menuItems)
{
    foreach (MenuItem* menuItem, menuItems)
    {
        quint32 imageID = menuItem->GetImageID();

        if ((!m_imageIndexes.contains(imageID)) && (imageID > 0))
        {
            m_imageIndexes.append(imageID);
        }

        if (!menuItem->SubMenuItems().isEmpty())
        {
            addMenuItemImages(menuItem->SubMenuItems());
        }
    }
}
