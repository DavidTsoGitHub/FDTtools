#include "DTCSelectorComboBox.h"

#include "DiagDefinition.h"
#include "FunctionBlock.h"
#include "Project.h"
#include <QComboBox>
#include <QString>
#include <QWidget>

DTCSelectorComboBox::DTCSelectorComboBox(FunctionBlock* functionBlock, QString dtcParameterName, Project* project, QWidget* parent) :
    QComboBox(parent),
    m_diagDefinition(project->GetDiagDefinition()),
    m_dtcParameterName(dtcParameterName),
    m_functionBlock(functionBlock)
{
    connect(project, &Project::PropertyUpdated, this, &DTCSelectorComboBox::projectUpdated);

    populate();

    connect(this, &DTCSelectorComboBox::currentTextChanged, this, &DTCSelectorComboBox::slot_currentTextChanged);
}

void DTCSelectorComboBox::populate()
{
    quint32 dtcParameterID = m_functionBlock->GetDTCParameterID(m_dtcParameterName);

    m_signalsEnabled = false;

    clear();

    addItem("Not selected", QVariant(0));
    foreach (DTCLabelMapping* mapping, m_diagDefinition.GetDTCLabelMappings())
    {
        addItem(QString("0x%1").arg(mapping->GetDTC(), 6, 16, QChar('0')), QVariant(mapping->GetDTC()));
    }

    for (int i=0; i<count(); ++i)
    {
        if (itemData(i).toUInt() == dtcParameterID)
        {
            setCurrentIndex(i);
            break;
        }
    }

    m_signalsEnabled = true;
}

void DTCSelectorComboBox::projectUpdated()
{
    if (m_diagDefinition.IsDirty())
    {
        blockSignals(true);
        populate();
        blockSignals(false);
    }
}


void DTCSelectorComboBox::slot_currentTextChanged(QString)
{
    if (!m_signalsEnabled) return;

    QVariant resourceID = currentData();
    m_functionBlock->SetDTCParameterID(m_dtcParameterName, resourceID.toInt());
}
