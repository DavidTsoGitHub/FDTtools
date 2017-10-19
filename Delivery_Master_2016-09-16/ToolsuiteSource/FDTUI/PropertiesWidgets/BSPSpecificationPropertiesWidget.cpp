#include "BSPSpecificationPropertiesWidget.h"

#include "BSPDisplay.h"
#include "BSPFunction.h"
#include "BSPSegmentDisplay.h"
#include "BSPSpecification.h"
#include "GCLSignalEndpoint.h"
#include "Project.h"

#include <QLineEdit>
#include <QTreeWidgetItem>
#include <QWidget>

BSPSpecificationPropertiesWidget::BSPSpecificationPropertiesWidget(Project* project, QWidget* parent) :
    ProjectObjectPropertiesWidget(project, parent),
    m_bspSpecification(NULL),
    m_displayPropertiesRoot(NULL),
    m_segmentDisplayPropertiesRoot(NULL),
    m_renderingTypeLineEdit(NULL),
    m_heightLineEdit(NULL),
    m_widthLineEdit(NULL),
    m_useGRAMLineEdit(NULL),
    m_gramAvailableMemoryLineEdit(NULL),
    m_numberOfLayersLineEdit(NULL),
    m_renderingOrientationLineEdit(NULL),
    m_segmentDisplayNbrOfSegments(NULL)
{
}

BSPSpecificationPropertiesWidget::~BSPSpecificationPropertiesWidget()
{
}

void BSPSpecificationPropertiesWidget::SetBSPSpecification(BSPSpecification* bspSpecification)
{
    m_bspSpecification = bspSpecification;
    SetProjectObject(m_bspSpecification);
}

void BSPSpecificationPropertiesWidget::beforePopulateSignals()
{
    if (m_bspSpecification->HasDisplay())
    {
        m_displayPropertiesRoot = AddContainer("Display Properties");

        m_renderingTypeLineEdit = new QLineEdit(this);
        m_renderingTypeLineEdit->setEnabled(false);
        m_renderingTypeLineEdit->setText((m_bspSpecification->GetDisplay()->GetRenderingType() == BSPDisplay::HW) ? "HW" : "SW");
        AddEditableData("Rendering Type", m_renderingTypeLineEdit, m_displayPropertiesRoot);

        m_heightLineEdit = new QLineEdit(this);
        m_heightLineEdit->setEnabled(false);
        m_heightLineEdit->setText(QString::number(m_bspSpecification->GetDisplay()->GetHeight()));
        AddEditableData("Height", m_heightLineEdit, m_displayPropertiesRoot);

        m_widthLineEdit = new QLineEdit(this);
        m_widthLineEdit->setEnabled(false);
        m_widthLineEdit->setText(QString::number(m_bspSpecification->GetDisplay()->GetWidth()));
        AddEditableData("Width", m_widthLineEdit, m_displayPropertiesRoot);

        m_useGRAMLineEdit = new QLineEdit(this);
        m_useGRAMLineEdit->setEnabled(false);
        m_useGRAMLineEdit->setText((m_bspSpecification->GetDisplay()->GetUseGRAM() == true) ? "Yes" : "No");
        AddEditableData("Use GRAM", m_useGRAMLineEdit, m_displayPropertiesRoot);

        if (m_bspSpecification->GetDisplay()->GetUseGRAM())
        {
            m_gramAvailableMemoryLineEdit = new QLineEdit(this);
            m_gramAvailableMemoryLineEdit->setEnabled(false);
            m_gramAvailableMemoryLineEdit->setText(QString::number(m_bspSpecification->GetDisplay()->GetAvailableMemory()) + " bytes");
            AddEditableData("GRAM Memory", m_gramAvailableMemoryLineEdit, m_displayPropertiesRoot);

            m_numberOfLayersLineEdit = new QLineEdit(this);
            m_numberOfLayersLineEdit->setEnabled(false);
            m_numberOfLayersLineEdit->setText(QString::number(m_bspSpecification->GetDisplay()->GetNumberOfLayers()));
            AddEditableData("Number of Layers", m_numberOfLayersLineEdit, m_displayPropertiesRoot);

            m_renderingOrientationLineEdit = new QLineEdit(this);
            m_renderingOrientationLineEdit->setEnabled(false);
            m_renderingOrientationLineEdit->setText((m_bspSpecification->GetDisplay()->GetRenderingOrientation() == BSPDisplay::CounterClockWise) ? "Counter Clock Wise" : "No Rotation");
            AddEditableData("Rendering Orientation", m_renderingOrientationLineEdit, m_displayPropertiesRoot);
        }
    }

    if (m_bspSpecification->HasSegmentDisplay())
    {
        m_segmentDisplayPropertiesRoot = AddContainer("Segment display Properies");

        m_segmentDisplayNbrOfSegments = new QLineEdit(this);
        m_segmentDisplayNbrOfSegments->setEnabled(false);
        m_segmentDisplayNbrOfSegments->setText(QString::number(m_bspSpecification->SegmentDisplay()->GetSegmentBitMappings().count()));
        AddEditableData("Number of Segments", m_segmentDisplayNbrOfSegments, m_segmentDisplayPropertiesRoot);
    }
}

void BSPSpecificationPropertiesWidget::afterPopulateSignals()
{
    QList<const BSPFunction*> unknownSignals;
    foreach (const BSPFunction* function, m_bspSpecification->Functions())
    {
        if (function->GCLSignalType == BSPFunction::Undefined && m_bspSpecification->GetSignalsForFunction(function).count() > 0)
        {
            unknownSignals.append(function);
        }
    }

    if (unknownSignals.count() > 0)
    {
        QTreeWidgetItem* undefinedSignalsContainer = AddContainer("Undefined signals");
        undefinedSignalsContainer->setToolTip(0, "These are functions that had a GCL signal name in the BSP specification but did not have a GCL signal type.");

        foreach (const BSPFunction* function, unknownSignals)
        {
            QTreeWidgetItem* undefinedSignalContainer = AddContainer(m_bspSpecification->GetSignalsForFunction(function).first()->SignalName(), undefinedSignalsContainer);


            if (function->Arguments.count() > 0)
            {
                AddDisplayData("DataType", function->Arguments.first().Type, undefinedSignalContainer);
            }
            else
            {
                AddDisplayData("DataType", "void", undefinedSignalContainer);
            }
        }
    }
}
