#include "ProjectValidator.h"

#include <qmath.h>
#include <QString>

#include "BSPDisplay.h"
#include "BSPEnum.h"
#include "IBSP_DCU_BPPModes.h"
#include "DisplayArea.h"
#include "DynamicTextLabelWidget.h"
#include "DynamicValueLabelWidget.h"
#include "GCLSignalSink.h"
#include "GenericObjectVisitor.h"
#include "LabelObjectVisitor.h"
#include "Layout.h"
#include "MenuItem.h"
#include "Project.h"
#include "RootMenuItemContainer.h"
#include "SelfValidationVisitor.h"

ProjectValidator::ProjectValidator(Project& project) :
    m_project(project),
    m_validateWidth(true),
    m_mandatorySinkUnmapped(false),
    m_allowedWidthMultiple(16)
{
}

bool ProjectValidator::ProjectIsValidForSimulator()
{
    m_warnings.clear();
    m_errors.clear();

    bool projectIsValid = true;
    projectIsValid &= setupWidthLimitations();
    projectIsValid &= pcicFunctionsValid();
    projectIsValid &= layoutDefinitionIsValid();
    projectIsValid &= commonValidatorsAreValid();
    projectIsValid &= functionBlocksContainLibraryOrSourceFilesForTarget(false);
    return projectIsValid;
}

bool ProjectValidator::ProjectIsValidForTarget()
{
    m_warnings.clear();
    m_errors.clear();

    bool projectIsValid = true;
    projectIsValid &= setupWidthLimitations();
    projectIsValid &= commonValidatorsAreValid();
    projectIsValid &= layoutDefinitionIsValid();
    projectIsValid &= functionBlocksContainLibraryOrSourceFilesForTarget(false);
    projectIsValid &= targetSourceFolderExist();
    return projectIsValid;
}

bool ProjectValidator::setupWidthLimitations()
{
    const BSPDisplay* display = m_project.GetBSPSpecification().GetDisplay();

    if (display->GetRenderingType() == BSPDisplay::SW)
    {
        // If software rendering is used, we do not have to consider the dimensions of the areas.
        // The limitation comes from the DCU layer configuration, which is not applicable in this case
        return true;
    }

    m_validateWidth = display->GetRenderingOrientation() == BSPDisplay::Normal;

    switch (display->GetBpp())
    {
        case 32: m_allowedWidthMultiple = 1; break;
        case 24: m_allowedWidthMultiple = 4; break;
        case 16: m_allowedWidthMultiple = 2; break;
        case 8: m_allowedWidthMultiple = 4; break;
        case 4: m_allowedWidthMultiple = 8; break;
        case 2: m_allowedWidthMultiple = 16; break;
        case 1: m_allowedWidthMultiple = 16; break;
        default:
            m_errors.append("Unknown BPP value for display: " + QString::number(display->GetBpp()));
            return false;
    }

    return true;
}

bool ProjectValidator::commonValidatorsAreValid()
{
    bool valid = true;

    valid &= menuHasSubMenuItem();
    valid &= allMandatorySinksAreMapped();
    valid &= schedulerCycleTimesAreValid();
    valid &= widgetSizesAreValid();
    valid &= noBordersOnHWRenderedAreas();
    valid &= diagnosticsValid();
    valid &= labelFontsAreValid();
    valid &= layoutsLayersAndMemoryUsagesAreOk();
    valid &= layoutsHaveUniqueNames();
    valid &= chromaKeyRangeIsValid();
    valid &= menuItemsAreUnique();
    valid &= selfValidation();

    return valid;
}

QString ProjectValidator::Errors() const
{
    return m_errors.join("\n");
}

QString ProjectValidator::Warnings() const
{
    return m_warnings.join("\n");
}

//This function returns GRAM numbers accounted for double-buffering
ProjectValidator::layerAndGramUsage_t ProjectValidator::CalculateLayerAndGramUsage(Project* project, Layout *layout)
{
    layerAndGramUsage_t layoutUsage;
    layoutUsage.gramUsage = 0;
    layoutUsage.layerCount = 0;

    if (project->GetBSPSpecification().GetDisplay()->GetRenderingType() != BSPDisplay::HW)
        return layoutUsage;

    QList<DisplayArea*> areasToProcess = layout->DisplayAreas();

    if (layout == NULL)
        return layoutUsage;

    if (layout->HasMenu()) //If the layout contains the menu area
    {
        layoutUsage.layerCount++;

        quint32 numberOfLayersNeeded = 0;
        MenuDefinition* menuDefinition = layout->GetMenu();
        ProjectValidator::CheckNumberOfLayersNeededByMenu(menuDefinition->MenuRoot().MenuItems(), numberOfLayersNeeded);

        layoutUsage.layerCount += numberOfLayersNeeded;

        layoutUsage.gramUsage += (layout->GetMenuArea()->GetWidth() * layout->GetMenuArea()->GetHeight());

        areasToProcess.removeAll(layout->GetMenuArea()); //no double-counting
    }

    if (layout->IsCheckMessagesEnabled() && !layout->IsCheckMessageScreen())
    {
        Layout* checkMessageLayout = project->GetLayoutDefinition().GetCheckMessageLayout();
        if (checkMessageLayout != NULL)
        {
            ProjectValidator::layerAndGramUsage_t checkMessageUsage = CalculateLayerAndGramUsage(project, checkMessageLayout);
            layoutUsage.layerCount += checkMessageUsage.layerCount;
            layoutUsage.gramUsage += (checkMessageUsage.gramUsage / 2); //don't calculate double-buffering twice, recursive call
        }
    }

    foreach (DisplayArea* area, areasToProcess)
    {
        bool areaIsSoftwareRendered = false;

        foreach (UIWidgetBase* widget, area->UIWidgets())
        {
            if (widget->GetNbrOfLayersNeeded() > 0)
            {
                layoutUsage.layerCount += widget->GetNbrOfLayersNeeded();
            }
            else
            {
                areaIsSoftwareRendered = true;
            }
        }

        if (areaIsSoftwareRendered)
        {
            layoutUsage.layerCount++;
            layoutUsage.gramUsage += area->GetWidth() * area->GetHeight();
        }
    }

    //Double buffering
    layoutUsage.gramUsage *= 2;

    return layoutUsage;
}

void ProjectValidator::CheckNumberOfLayersNeededByMenu(const QList<MenuItem*>& menuItems, quint32& layersNeeded)
{
    foreach (MenuItem* menuItem, menuItems)
    {
        quint32 layerCount = 0;
        foreach (UIWidgetBase* widget, menuItem->UIWidgets())
        {
            if (widget->GetNbrOfLayersNeeded() > 0)
            {
                ++layerCount;
            }
        }

        if (layerCount > layersNeeded)
        {
            layersNeeded = layerCount;
        }

        if (!menuItem->SubMenuItems().isEmpty())
        {
            CheckNumberOfLayersNeededByMenu(menuItem->SubMenuItems(), layersNeeded);
        }
    }
}

QRegExp ProjectValidator::ValidCIdentifierRegExp()
{
    return QRegExp("[_a-zA-Z][ _a-zA-Z0-9]{0,30}");
}

bool ProjectValidator::pcicFunctionsValid()
{
    bool pcicFunctionsOk = true;

    BSPSpecification& bspSpecification = m_project.GetBSPSpecification();
    foreach (QString functionName, bspSpecification.GetConstants())
    {
        const BSPFunction* bspFunction = m_project.GetBSPSpecification().GetFunctionByName(functionName);
        if (bspFunction == NULL)
        {
            m_errors.append("Mandatory BSP function missing for PC BSP generation: " + functionName);
            pcicFunctionsOk = false;
        }
    }

    return pcicFunctionsOk;
}

bool ProjectValidator::layoutDefinitionIsValid()
{
    const BSPDisplay* display = m_project.GetBSPSpecification().GetDisplay();

    bool isValid = true;

    if (m_project.GetLayoutDefinition().Layouts().count() == 0)
    {
        m_errors.append("Layout(s) missing! There is has to be at least one layout defined!");
        isValid = false;
    }

    if (display->GetRenderingType() == BSPDisplay::HW)
    {
        foreach (Layout* layout, m_project.GetLayoutDefinition().Layouts())
        {
            foreach (DisplayArea* area, layout->DisplayAreas())
            {
                quint16 dimension = m_validateWidth ? area->GetWidth(): area->GetHeight();
                if (dimension % m_allowedWidthMultiple != 0)
                {
                    QString errorText("%1->%2 has an invalid %3 of %4 (must be a multiple of %5 due to DCU restrictions");

                    m_errors.append(errorText.arg(layout->Name()).arg(area->GetName()).arg((m_validateWidth ? "width" : "height")).arg(dimension).arg(m_allowedWidthMultiple));
                    isValid = false;
                }

                if (area != layout->GetMenuArea() && area->UIWidgets().count() == 0)
                {
                    m_errors.append("Area '" + area->GetName() + "' is empty. Empty areas are not allowed. Please remove the area, or add a widget to it!");
                    isValid = false;
                }
            }
        }
    }

    return isValid;
}

bool ProjectValidator::menuHasSubMenuItem()
{
    bool isValid = false;

    if (m_project.GetLayoutDefinition().HasMenu())
    {
        MenuDefinition* menu = m_project.GetLayoutDefinition().GetMenu();
        if (menu->MenuRoot().MenuItems().count() > 0)
        {
            isValid = true;
        }
        else
        {
            m_errors.append("The menu must have sub menu items, or be removed");
        }

    }
    else
    {
        isValid = true;
    }

    return isValid;
}

bool ProjectValidator::allMandatorySinksAreMapped()
{
    m_mandatorySinkUnmapped = false;
    QList<GCLSignalSink*> allSinks;
    m_project.GetAllGCLSinks(allSinks);

    foreach (GCLSignalSink* sink, allSinks)
    {
        if (sink->GetSignalSource() == NULL && sink->IsMandatory())
        {
            bool locationFound = false;
            foreach (Layout* layout, m_project.GetLayoutDefinition().Layouts())
            {
                QList<GCLSignalSink*> layoutSinks;
                layout->GetAllSignalSinks(layoutSinks);

                if (layoutSinks.contains(sink))
                {
                    foreach (DisplayArea* area, layout->DisplayAreas())
                    {
                        const QList<UIWidgetBase*> widgets = area->UIWidgets();

                        foreach (UIWidgetBase* widget, widgets)
                        {
                            if (widget->GetSignalSinks().contains(sink))
                            {
                                m_errors.append("Mandatory input signal not mapped: Layout Editor->" + layout->Name() + "->" + area->GetName() + "->" + sink->LongName());
                                m_mandatorySinkUnmapped = true;
                                locationFound = true;
                                break;
                            }
                        }
                    }
                }
            }

            if (!locationFound)
            {
                MenuDefinition* menuDefinition = m_project.GetLayoutDefinition().GetMenu();

                if (menuDefinition != NULL)
                {
                    locationFound = findUnmappedSinkInMenu(sink);
                }
            }

            if (!locationFound)
            {
                locationFound = findUnmappedSinkInCheckMessage(sink);
            }

            if (!locationFound)
            {
                m_errors.append("Mandatory input signal not mapped: " + sink->LongName());
                m_mandatorySinkUnmapped = true;
            }
        }
    }

    if (m_mandatorySinkUnmapped)
    {
        m_errors.append("Project was missing mandatory input signal sources");
        return false;
    }

    return true;
}

bool ProjectValidator::chromaKeyRangeIsValid()
{
    bool validTest = true;

    QColor start = m_project.GetLayoutDefinition().GetStartChromaKeyColor();
    QColor end = m_project.GetLayoutDefinition().GetEndChromaKeyColor();

    if (start.red() > end.red())
    {
        m_errors.append(QString("Erroneous chroma-key range (RED): start value of the chroma key range must not be greater than the end value (0x%1 =< 0x%2)")
                        .arg(QString::number(start.red(), 16), QString::number(end.red(), 16)));
        validTest = false;
    }

    if (start.green() > end.green())
    {
        m_errors.append(QString("Erroneous chroma-key range (GREEN): start value of the chroma key range must not be greater than the end value (0x%1 =< 0x%2)")
                        .arg(QString::number(start.green(), 16), QString::number(end.green(), 16)));
        validTest = false;
    }

    if (start.blue() > end.blue())
    {
        m_errors.append(QString("Erroneous chroma-key range (BLUE): start value of the chroma key range must not be greater than the end value (0x%1 =< 0x%2)")
                        .arg(QString::number(start.blue(), 16), QString::number(end.blue(), 16)));
        validTest = false;
    }

    return validTest;
}

bool ProjectValidator::findUnmappedSinkInCheckMessage(GCLSignalSink* sink)
{
    bool hasCheckMessageLayout = false;

    foreach (Layout* layout, m_project.GetLayoutDefinition().Layouts())
    {
        if (layout->IsCheckMessageScreen())
        {
            hasCheckMessageLayout = true;
            break;
        }
    }

    bool sinkFound = m_project.GetCheckMessagesDefinition().GetSignalSinks().contains(sink);
    if (sinkFound)
    {
        if (hasCheckMessageLayout)
        {
            m_mandatorySinkUnmapped = true;
            m_errors.append("Mandatory input signal not mapped: CheckMessages->" + sink->LongName());
        }
        else
        {
            // If there is no CheckMessage layout then unmapped sinks here should not fail the validation
        }
    }

    return sinkFound;
}

bool ProjectValidator::findUnmappedSinkInMenuItem(MenuItem* item, GCLSignalSink* sink, QString menuItemLocation)
{
    bool sinkFound = false;
    foreach (MenuItem* subMenuItem, item->SubMenuItems())
    {
        QString subMenuItemLocation = menuItemLocation + "->" + subMenuItem->GetName();
        sinkFound = findUnmappedSinkInMenuItem(subMenuItem, sink, subMenuItemLocation);

        if (sinkFound)
        {
            break;
        }
    }

    if (!sinkFound)
    {
        foreach (UIWidgetBase* widget, item->UIWidgets())
        {
            sinkFound = widget->GetSignalSinks().contains(sink);

            if (sinkFound)
            {
                m_errors.append("Mandatory input signal not mapped: " + menuItemLocation + "->" + sink->LongName());
                m_mandatorySinkUnmapped = true;
                break;
            }
        }
    }

    return sinkFound;
}

bool ProjectValidator::findUnmappedSinkInMenu(GCLSignalSink* sink)
{
    bool sinkFound = false;

    RootMenuItemContainer* root = m_project.GetLayoutDefinition().GetMenu()->MenuRootPointer();

    foreach (MenuItem* item, root->MenuItems())
    {
        QString menuItemLocation = "Menu Editor->Menu Root->" + item->GetName();
        sinkFound = findUnmappedSinkInMenuItem(item, sink, menuItemLocation);

        if (sinkFound)
        {
            break;
        }
    }

    return sinkFound;
}

bool ProjectValidator::schedulerCycleTimesAreValid()
{
    bool allCycleTimesOk = true;

    foreach (const BSPRunnable* runnable, m_project.GetBSPSpecification().Runnables())
    {
        if (runnable->GetCycleTime() < 5)
        {
            m_errors.append("BSP runnable: " + runnable->GetRunnableFunction() + " has to low cycle time (" + QString::number(runnable->GetCycleTime()) + "ms)!");
            allCycleTimesOk = false;
        }
    }

    foreach (FunctionBlock* function, m_project.GetFunctionBlocks().List())
    {
        foreach (const Task* task, function->GetFunctionDefinition().Tasks())
        {
            if (task->Periodicity < 5)
            {
                m_errors.append("Function block runnable: " + function->GetComponentName() + " has to low cycle time (" + QString::number(task->Periodicity) + "ms)!");
                allCycleTimesOk = false;
            }
        }
    }

    return allCycleTimesOk;
}

bool ProjectValidator::widgetSizesAreValid()
{
    bool allWidgetSizesOk = true;

    foreach (Layout* layout, m_project.GetLayoutDefinition().Layouts())
    {
        foreach (DisplayArea* area, layout->DisplayAreas())
        {
            foreach (UIWidgetBase* widget, area->UIWidgets())
            {
                if (widget->GetBoxed() == true && (widget->GetWidth() < 4 || widget->GetHeight() < 4))
                {
                    m_errors.append("Boxed widget (" + widget->GetComponentName() + ") width or height less than 4 px!");
                    allWidgetSizesOk = false;
                }
                if ( (widget->GetX() + widget->GetWidth()) > area->GetWidth() )
                {
                    m_errors.append("Widget (" + widget->GetComponentName() + ") exceeds areas boundary for X coordinates in area: " + area->GetName());
                    allWidgetSizesOk = false;
                }
                if ( (widget->GetY() + widget->GetHeight() ) > area->GetHeight() )
                {
                    m_errors.append("Widget (" + widget->GetComponentName() + ") exceeds areas boundary for Y coordinates");
                    allWidgetSizesOk = false;
                }
                if (widget->GetNbrOfLayersNeeded() > 0)
                {
                    quint16 dimension = m_validateWidth ? widget->GetWidth(): widget->GetHeight();
                    if (dimension % 2 != 0)
                    {
                        QString errorText("%1 has an invalid %2 of %3 (must be a multiple of %4 due to DCU restrictions)");

                        m_errors.append(errorText.arg(widget->GetComponentName()).arg((m_validateWidth ? "width" : "height")).arg(dimension).arg(QString::number(2)));
                        allWidgetSizesOk = false;
                    }
                }
            }
        }

        if (layout->HasMenu())
        {
            MenuDefinition* menuDefinition = layout->GetMenu();
            DisplayArea* menuArea = layout->GetMenuArea();
            checkMenuItemWidgetsValidSizeAndPos(menuDefinition->MenuRoot().MenuItems(), menuArea, allWidgetSizesOk);
        }
    }

    return allWidgetSizesOk;
}

bool ProjectValidator::noBordersOnHWRenderedAreas()
{
    bool retVal = true;

    if (m_project.GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        foreach (Layout* layout, m_project.GetLayoutDefinition().Layouts())
        {
            foreach (DisplayArea* area, layout->DisplayAreas())
            {

                if (area->GetBorder() > 0)
                {
                    bool borderOk = true;
                    foreach (UIWidgetBase* widget, area->UIWidgets())
                    {
                        borderOk = false;
                        if (widget->GetNbrOfLayersNeeded() == 0)
                        {
                            borderOk = true;
                            break;
                        }
                    }

                    if (!borderOk)
                    {
                        m_errors.append(layout->Name() + "->" + area->GetName() + " has a border but no SW rendered content!");
                        retVal = false;
                    }
                }
            }
        }
    }
    return retVal;
}

void ProjectValidator::checkMenuItemWidgetsValidSizeAndPos(const QList<MenuItem*>& menuItems, DisplayArea* area, bool& allWidgetSizesOk)
{
    foreach (MenuItem* menuItem, menuItems)
    {
        foreach (UIWidgetBase* widget, menuItem->UIWidgets())
        {
            if ( (widget->GetX() + widget->GetWidth()) > area->GetWidth() )
            {
                m_errors.append("Widget (" + widget->GetComponentName() + ") exceeds menu area boundary for X coordinates");
                allWidgetSizesOk = false;
            }
            if ( (widget->GetY() + widget->GetHeight() ) > area->GetHeight() )
            {
                m_errors.append("Widget (" + widget->GetComponentName() + ") exceeds menu area boundary for Y coordinates");
                allWidgetSizesOk = false;
            }
            if (widget->GetNbrOfLayersNeeded() > 0)
            {
                quint16 dimension = m_validateWidth ? widget->GetWidth(): widget->GetHeight();
                if (dimension % 2 != 0)
                {
                    QString errorText("%1 has an invalid %2 of %3 (must be a multiple of %4 due to DCU restrictions)");

                    m_errors.append(errorText.arg(widget->GetComponentName()).arg((m_validateWidth ? "width" : "height")).arg(dimension).arg(2));
                    allWidgetSizesOk = false;
                }
            }
        }

        if (!menuItem->SubMenuItems().isEmpty())
        {
            checkMenuItemWidgetsValidSizeAndPos(menuItem->SubMenuItems(), area, allWidgetSizesOk);
        }
    }
}

bool ProjectValidator::functionBlocksContainLibraryOrSourceFilesForTarget(bool validateForSimulation)
{
    bool neededFilesExists = true;
    foreach (FunctionBlock* block, m_project.GetFunctionBlocks().List())
    {
        QFileInfo functionDefinitionFile(m_project.GetFunctionLibraryRoot().absoluteFilePath(block->GetPathInFunctionLibrary()));
        QString currentFunctionBlockRoot = functionDefinitionFile.absolutePath();
        QString currentFunctionBlockName = block->GetFunctionDefinition().GetName();
        bool libFound = false;
        if (validateForSimulation)
        {
            QDir pcLibDir(currentFunctionBlockRoot + "/libs/PCLib");
            if (getFiles(pcLibDir) > 0)
            {
                libFound = true;
            }
        }
        else
        {
            QDir cwLibDir(currentFunctionBlockRoot + "/libs/CWLib");
            if (getFiles(cwLibDir) > 0)
            {
                libFound = true;
            }
        }

        if (!libFound)
        {
            int nbrOfFiles = 0;
            QDir srcDir(currentFunctionBlockRoot + "/src");
            nbrOfFiles += getFiles(srcDir);
            QDir incDir(currentFunctionBlockRoot + "/inc");
            nbrOfFiles += getFiles(incDir);

            if (nbrOfFiles == 0)
            {
                neededFilesExists = false;
                m_errors.append("FunctionBlock \"" + currentFunctionBlockName + "\"has no library or source code for this platform");
                break;
            }
        }
    }
    return neededFilesExists;
}

bool ProjectValidator::targetSourceFolderExist()
{
    bool folderExists = true;

    QString source = m_project.GetProjectRoot().absoluteFilePath("TargetSource");

    if (!QDir(source).exists())
    {
        folderExists = false;
        m_errors.append("BSP source folder not found! (Expected location: " + source + ")");
    }

    return folderExists;
}

bool ProjectValidator::diagnosticsValid()
{
    bool diagnosticsOK = true;

    DiagDefinition& diagDefinition = m_project.GetDiagDefinition();
    QStringList allCALParameters = m_project.GetAllCALParameters();
    QList<quint16> usedDIDs;
    foreach (DIDToCALMapping* mapping, diagDefinition.GetDIDToCALMappings())
    {
        if (usedDIDs.contains(mapping->GetDID()))
        {
            m_errors.append(QString("Duplicate DID 0x%1 defined in Diagnostics!").arg(mapping->GetDID(), 4, 16, QChar('0')));
            diagnosticsOK = false;
        }

        usedDIDs.append(mapping->GetDID());
        if (mapping->GetCALParameter().trimmed().isEmpty() || !allCALParameters.contains(mapping->GetCALParameter()))
        {
            m_errors.append(QString("Calibration Parameter for DID 0x%1 is invalid!").arg(mapping->GetDID(), 4, 16, QChar('0')));
            diagnosticsOK = false;
        }
    }

    QList<quint32> usedDTCs;
    foreach (DTCLabelMapping* mapping, diagDefinition.GetDTCLabelMappings())
    {
        if (usedDTCs.contains(mapping->GetDTC()))
        {
            m_errors.append(QString("Duplicate DTC 0x%1 defined in Diagnostics!").arg(mapping->GetDTC(), 6, 16, QChar('0')));
            diagnosticsOK = false;
        }
        usedDTCs.append(mapping->GetDTC());

        if (mapping->GetLabelID() == UndefinedLabelID)
        {
            m_warnings.append(QString("DTC 0x%1 is not mapped to a valid label!").arg(mapping->GetDTC(), 6, 16, QChar('0')));
        }
    }

    return diagnosticsOK;
}

bool ProjectValidator::labelFontsAreValid()
{
    bool validTest = true;
    LabelObjectVisitor labelObjectVisitor;
    m_project.Accept(labelObjectVisitor);

    QPair<ProjectObject*, ILabelHasFont*> pair;
    foreach(pair, labelObjectVisitor.GetFoundObjectsList())
    {
        if (pair.second->GetFontID() == 0)
        {
            //Font unset
            m_errors.append(QString("Unset font in the project object named '%1'").arg(pair.first->objectName()));
            validTest = false;
        }
    }

    return validTest;
}

bool ProjectValidator::selfValidation()
{
    SelfValidationVisitor selfValidationVisitor(&m_project);
    m_project.Accept(selfValidationVisitor);

    m_errors << selfValidationVisitor.GetErrors();

    return selfValidationVisitor.GetStatus();
}

bool ProjectValidator::layoutsHaveUniqueNames()
{
    bool validTest = true;
    int iterationCounter = 0;

    QMap<QString, Layout*> layoutMap;
    foreach(Layout* layout, m_project.GetLayoutDefinition().Layouts())
    {
        iterationCounter++;
        layoutMap.insert(layout->Name(), layout);
        if (iterationCounter != layoutMap.count())
        {
            m_errors.append("Non-unique layout name: '" + layout->Name() + "'");
            validTest = false;
            iterationCounter--;
        }
    }

    return validTest;
}

bool ProjectValidator::layoutsLayersAndMemoryUsagesAreOk()
{
    bool isValid = true;
    quint16 bspNbrOfLayers = m_project.GetBSPSpecification().GetDisplay()->GetNumberOfLayers();
    quint32 totalGRAM = m_project.GetBSPSpecification().GetDisplay()->GetAvailableMemory();

    if (m_project.GetBSPSpecification().GetDisplay()->GetRenderingType() != BSPDisplay::HW)
    {
        return isValid;
    }

    foreach (Layout* layout, m_project.GetLayoutDefinition().Layouts())
    {
        layerAndGramUsage_t layoutUsage = CalculateLayerAndGramUsage(&m_project, layout);
        if (layoutUsage.gramUsage > totalGRAM)
        {
            m_errors.append("Layout '" + layout->Name() + "' exceeds the available GRAM.");
            isValid = false;
        }

        if (layoutUsage.layerCount > bspNbrOfLayers)
        {
            m_errors.append("Layout '" + layout->Name() + "' exceeds the available DCU Layers.");
            isValid = false;
        }
    }

    return isValid;
}

bool ProjectValidator::menuItemsAreUnique()
{
    bool validTest = true;

    if (m_project.GetLayoutDefinition().HasMenu())
    {
        int iterationCounter = 0;

        QMap<QString, MenuItem*> menuMap;
        QList<MenuItem*> menuItemFlatList;
        findMenuItemsRecursively(&menuItemFlatList, m_project.GetLayoutDefinition().GetMenu()->MenuRoot());

        foreach (MenuItem* menuItem, menuItemFlatList)
        {
            iterationCounter++;
            menuMap.insert(menuItem->GetName(), menuItem);
            if (iterationCounter != menuMap.count())
            {
                m_errors.append("Non-unique menu item name: '" + menuItem->GetName() + "'");
                validTest = false;
                iterationCounter--;
            }
        }
    }

    return validTest;
}

//adds itself and children
void ProjectValidator::findMenuItemsRecursively(QList<MenuItem*>* itemList, MenuItem* beginningMenuItem)
{
    foreach(MenuItem* loopMenuItem, beginningMenuItem->SubMenuItems())
    {
        findMenuItemsRecursively(itemList, loopMenuItem);
    }

    itemList->append(beginningMenuItem);
}

void ProjectValidator::findMenuItemsRecursively(QList<MenuItem*>* itemList, RootMenuItemContainer& rootMenuItem)
{
    Q_UNUSED(itemList);

    foreach (MenuItem* loopMenuItem, rootMenuItem.MenuItems())
    {
        findMenuItemsRecursively(itemList, loopMenuItem);
    }
}

int ProjectValidator::getFiles(const QDir& currentDir)
{
    int nbrOfFiles = 0;
    foreach (QFileInfo directory, currentDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        getFiles(directory.absoluteDir());
    }

    foreach (QFileInfo file, currentDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
    {
        QFile fileToRead(file.absoluteFilePath());
        if (!fileToRead.exists() || !fileToRead.open(QFile::ReadOnly))
        {
            continue;
        }

        ++nbrOfFiles;
    }

    return nbrOfFiles;
}
