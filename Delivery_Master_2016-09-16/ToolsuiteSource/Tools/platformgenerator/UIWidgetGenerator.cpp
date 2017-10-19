#include "UIWidgetGenerator.h"

#include "DynamicTextLabelUIWidgetGenerator.h"
#include "DynamicValueLabelUIWidgetGenerator.h"
#include "BlockBarUIWidgetGenerator.h"
#include "LabelUIWidgetGenerator.h"
#include "Layout.h"
#include "MenuItem.h"
#include "IconUIWidgetGenerator.h"
#include "ImageUIWidgetGenerator.h"
#include "SelectableListUIWidgetGenerator.h"
#include "DynamicIconUIWidgetGenerator.h"
#include "DynamicImageUIWidgetGenerator.h"
#include "SignalTriggerUIWidgetGenerator.h"
#include "ColumnLevelUIWidgetGenerator.h"
#include "ListNavigationUIWidgetGenerator.h"

#include "DynamicLabelUIWidgetGenerator.h"

#include "DynamicTextLabelUIWidgetGenerator.h"
#include "Logger.h"
#include "PlatformPaths.h"
#include <QDirIterator>

UIWidgetGenerator::UIWidgetGenerator(Project& project) :
    m_project(project)
{
}

UIWidgetGenerator::~UIWidgetGenerator(void)
{
}

void UIWidgetGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    Q_INIT_RESOURCE(UIWidgets);

    readAllWidgets();
    addAllStaticUIWidgetHeaderFiles();

    if (GetGeneratedFilesCount() > 0)
    {
        addDynamicUIWidgetFiles();
    }

    Q_CLEANUP_RESOURCE(UIWidgets);
}

void UIWidgetGenerator::readAllWidgets()
{
    LayoutDefinition& layoutDefinition = m_project.GetLayoutDefinition();
    if (m_project.GetBSPSpecification().HasDisplay())
    {
        foreach (Layout* layout, layoutDefinition.Layouts())
        {
            foreach (DisplayArea* area, layout->DisplayAreas())
            {
                foreach (UIWidgetBase* uiWidget, area->UIWidgets())
                {
                    m_allWidgetsByType.insertMulti(uiWidget->GetUIWidgetTypeName(), uiWidget);

                    if (!m_widgetTypes.contains(uiWidget->GetUIWidgetTypeName()))
                    {
                        m_widgetTypes.append(uiWidget->GetUIWidgetTypeName());
                    }
                }
            }

            if (layout->HasMenu())
            {
                foreach (MenuItem* menuItem, layout->GetMenu()->MenuRoot().MenuItems())
                {
                    parseMenuWidgetsForMenuItem(menuItem);
                }
            }
        }
    }
}

void UIWidgetGenerator::parseMenuWidgetsForMenuItem(MenuItem* currentMenuItem)
{
    foreach (UIWidgetBase* uiWidget, currentMenuItem->UIWidgets())
    {
        m_allWidgetsByType.insertMulti(uiWidget->GetUIWidgetTypeName(), uiWidget);

        if (!m_widgetTypes.contains(uiWidget->GetUIWidgetTypeName()))
         {
             m_widgetTypes.append(uiWidget->GetUIWidgetTypeName());
         }
    }

    if (!currentMenuItem->SubMenuItems().isEmpty())
    {
        foreach (MenuItem* menuItem, currentMenuItem->SubMenuItems())
        {
            parseMenuWidgetsForMenuItem(menuItem);
        }
    }
}

void UIWidgetGenerator::addDynamicUIWidgetFiles()
{
    foreach (QString type, m_widgetTypes)
    {
        if (type == "Label")
        {
            LabelUIWidgetGenerator labelUIWidgetGenerator(m_allWidgetsByType.values(LabelWidget::TypeName));
            AddGeneratedFiles(labelUIWidgetGenerator.GenerateFiles());
        }
        else if (type == "Icon")
        {
            IconUIWidgetGenerator iconUIWidgetGenerator(m_allWidgetsByType.values(IconWidget::TypeName));
            AddGeneratedFiles(iconUIWidgetGenerator.GenerateFiles());
        }
        else if (type == "Image")
        {
            ImageUIWidgetGenerator imageUIWidgetGenerator(m_allWidgetsByType.values(ImageWidget::TypeName));
            AddGeneratedFiles(imageUIWidgetGenerator.GenerateFiles());
        }
        else if (type == "DynamicTextLabel")
        {
            DynamicTextLabelUIWidgetGenerator dynamicTextLabelUIWidgetGenerator(m_allWidgetsByType.values(DynamicTextLabelWidget::TypeName), m_project);
            AddGeneratedFiles(dynamicTextLabelUIWidgetGenerator.GenerateFiles());
        }
        else if (type == "DynamicValueLabel")
        {
            DynamicValueLabelUIWidgetGenerator dynamicValueLabelUIWidgetGenerator(m_allWidgetsByType.values(DynamicValueLabelWidget::TypeName), m_project);
            AddGeneratedFiles(dynamicValueLabelUIWidgetGenerator.GenerateFiles());
        }
        else if (type == "BlockBar")
        {
            BlockBarUIWidgetGenerator blockBarUIWidgetGenerator(m_allWidgetsByType.values(BlockBarWidget::TypeName));
            AddGeneratedFiles(blockBarUIWidgetGenerator.GenerateFiles());
        }
        else if (type == "SelectableList")
        {
            SelectableListUIWidgetGenerator selectableListUIWidgetGenerator(m_allWidgetsByType.values(SelectableListWidget::TypeName), m_project);
            AddGeneratedFiles(selectableListUIWidgetGenerator.GenerateFiles());
        }
        else if (type == "DynamicIcon")
        {
            DynamicIconUIWidgetGenerator dynamicIconWidgetGenerator(m_allWidgetsByType.values(DynamicIconWidget::TypeName));
            AddGeneratedFiles(dynamicIconWidgetGenerator.GenerateFiles());
        }
        else if (type == "DynamicLabel")
        {
            DynamicLabelUIWidgetGenerator DynamicLabelUIWidgetGenerator(m_allWidgetsByType.values(DynamicLabelWidget::TypeName));
            AddGeneratedFiles(DynamicLabelUIWidgetGenerator.GenerateFiles());
        }
        else if (type == "DynamicImage")
        {
            DynamicImageUIWidgetGenerator dynamicImageWidgetGenerator(m_allWidgetsByType.values(DynamicImageWidget::TypeName));
            AddGeneratedFiles(dynamicImageWidgetGenerator.GenerateFiles());
        }
        else if (type == "ColumnLevel")
        {
            ColumnLevelUIWidgetGenerator columnLevelWidgetGenerator(m_allWidgetsByType.values(ColumnLevelWidget::TypeName), m_project);
            AddGeneratedFiles(columnLevelWidgetGenerator.GenerateFiles());
        }
        else if (type == "SignalTrigger")
        {
            SignalTriggerUIWidgetGenerator signalTriggerWidgetGenerator(m_allWidgetsByType.values(SignalTriggerWidget::TypeName));
            AddGeneratedFiles(signalTriggerWidgetGenerator.GenerateFiles());
        }
        else if (type == "ListNavigation")
        {
            ListNavigationUIWidgetGenerator listNavigationWidgetGenerator(m_allWidgetsByType.values(ListNavigationWidget::TypeName), m_project);
            AddGeneratedFiles(listNavigationWidgetGenerator.GenerateFiles());
        }
    }
}

void UIWidgetGenerator::addAllStaticUIWidgetHeaderFiles()
{
    QString resourcePath(":/UIWidgets/TargetSource/");

    QDirIterator it(resourcePath, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString path = it.next();
        if(!QFileInfo(path).isFile()) continue;

        QString widgetTypeFromPath = path.split("/").at(3);

        QFile file(path);
        QFileInfo fileInfo(file.fileName());
        if (fileInfo.fileName().endsWith(".c"))
        {
            if (m_widgetTypes.contains(widgetTypeFromPath) || widgetTypeFromPath == "UIWidget")
            {
                if (!file.open(QFile::ReadOnly)) continue;
                QString relativeFilePath(path.replace(":/UIWidgets/TargetSource/", ""));
                QString targetPath = PlatformPaths::CreateUIWidgetPath(relativeFilePath);
                AddCopiedFile(targetPath, file.readAll());

                file.close();
            }
        }
        else if (fileInfo.fileName().endsWith(".h"))
        {
            if (m_widgetTypes.contains(widgetTypeFromPath) || widgetTypeFromPath == "UIWidget")
            {
                if (!file.open(QFile::ReadOnly)) continue;
                QString relativeFilePath(path.replace(":/UIWidgets/TargetSource/", ""));
                QString targetPath = PlatformPaths::CreateUIWidgetPath(relativeFilePath);
                AddCopiedFile(targetPath, file.readAll());

                file.close();
            }
        }
    }
}
