#include "CANSpecificationPropertiesWidget.h"

#include "CANSignal.h"
#include "CANSpecification.h"
#include "Project.h"

#include <QLabel>
#include <QTreeWidgetItem>
#include <QSpinBox>
#include <QString>
#include <QWidget>

CANSpecificationPropertiesWidget::CANSpecificationPropertiesWidget(Project* project, const QString& ecuName, QWidget* parent) :
    ProjectObjectPropertiesWidget(project, parent),
    m_ecuName(ecuName),
    m_canSpecification(NULL),
    m_presdivDataItem(NULL),
    m_project(project)
{
}

CANSpecificationPropertiesWidget::~CANSpecificationPropertiesWidget()
{
}

void CANSpecificationPropertiesWidget::SetCANSpecification(CANSpecification* canSpecification)
{
    m_canSpecification = canSpecification;
    SetProjectObject(m_canSpecification);
}

void CANSpecificationPropertiesWidget::populateDriverParameters()
{
    CANSpecification::DriverParameters driverParameters = m_canSpecification->GetDriverParameters();

    QTreeWidgetItem* driverParametersRoot = AddContainer("Driver Parameters");

    // Spinbox min/max values are taken from MPC5606S reference manual (MPC5606SRM.pdf)
    QSpinBox* presDivSpinBox = createConfigurationSpinBox(driverParameters.PRESDIV, 0, 255);
    m_presdivDataItem = AddEditableData("PRESDIV", presDivSpinBox, driverParametersRoot);
    m_presdivDataItem->setToolTip(0, "Baudrate = " + QString::number(16000000/(1 + driverParameters.PRESDIV)/16) + "bit/s");
    connect(presDivSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value)
    {
        CANSpecification::DriverParameters driverParameters = m_canSpecification->GetDriverParameters();
        driverParameters.PRESDIV = value;
        m_canSpecification->SetDriverParameters(driverParameters);
        m_presdivDataItem->setToolTip(0, "Bitrate = " + QString::number(16000000/(1 + value)/16) + "kbit/s");
    });


    QSpinBox* rjwSpinBox = createConfigurationSpinBox(driverParameters.RJW, 0, 3);
    AddEditableData("RJW", rjwSpinBox, driverParametersRoot);
    connect(rjwSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value)
    {
        CANSpecification::DriverParameters driverParameters = m_canSpecification->GetDriverParameters();
        driverParameters.RJW = value;
        m_canSpecification->SetDriverParameters(driverParameters);
    });

    QSpinBox* pseg1SpinBox = createConfigurationSpinBox(driverParameters.PSEG1, 0, 7);
    AddEditableData("PSEG1", pseg1SpinBox, driverParametersRoot);
    connect(pseg1SpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value)
    {
        CANSpecification::DriverParameters driverParameters = m_canSpecification->GetDriverParameters();
        driverParameters.PSEG1 = value;
        m_canSpecification->SetDriverParameters(driverParameters);
    });

    QSpinBox* pseg2SpinBox = createConfigurationSpinBox(driverParameters.PSEG2, 1, 7);
    AddEditableData("PSEG2", pseg2SpinBox, driverParametersRoot);
    connect(pseg2SpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value)
    {
        CANSpecification::DriverParameters driverParameters = m_canSpecification->GetDriverParameters();
        driverParameters.PSEG2 = value;
        m_canSpecification->SetDriverParameters(driverParameters);
    });


    QSpinBox* propSegSpinBox = createConfigurationSpinBox(driverParameters.PROPSEG, 0, 7);
    AddEditableData("PROPSEG", propSegSpinBox, driverParametersRoot);
    connect(propSegSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value)
    {
        CANSpecification::DriverParameters driverParameters = m_canSpecification->GetDriverParameters();
        driverParameters.PROPSEG = value;
        m_canSpecification->SetDriverParameters(driverParameters);
    });

    driverParametersRoot->setToolTip(0, "Set the driver parameters for the CAN controller");
    driverParametersRoot->setExpanded(true);
}

void CANSpecificationPropertiesWidget::populateDiagParameters()
{
    QTreeWidgetItem* diagParametersRoot = AddContainer("Diagnostic CAN ID configuration");

    QSpinBox* DiagFuncReqIDSpinBox = createConfigurationSpinBox(m_canSpecification->USDTFuncReqID(), 0, 536870912);
    AddEditableData("FuncReqID", DiagFuncReqIDSpinBox, diagParametersRoot);
    connect(DiagFuncReqIDSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_canSpecification->SetUSDTFuncReqID(value); });

    QSpinBox* DiagReqIDSpinBox = createConfigurationSpinBox(m_canSpecification->USDTReqID(), 0, 536870912);
    AddEditableData("ReqID", DiagReqIDSpinBox, diagParametersRoot);
    connect(DiagReqIDSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_canSpecification->SetUSDTReqID(value); });

    QSpinBox* DiagRespIDSpinBox = createConfigurationSpinBox(m_canSpecification->USDTRespID(), 0, 536870912);
    AddEditableData("RespID", DiagRespIDSpinBox, diagParametersRoot);
    connect(DiagRespIDSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_canSpecification->SetUSDTRespID(value);});

    diagParametersRoot->setToolTip(0, "Configure the CAN ID's for the diagnostic manager");
    diagParametersRoot->setExpanded(true);

}

void CANSpecificationPropertiesWidget::populateJ1939Parameters()
{
     QTreeWidgetItem* J1939Root = AddContainer("J1939 DM1 configuration");

     QTreeWidgetItem* J1939SFRoot = AddContainer("Single frame configuraton",J1939Root);

     QSpinBox* SFIDSpinBox = createConfigurationSpinBox(m_canSpecification->J1939SFID(), 0, 536870912);
     AddEditableData("ID", SFIDSpinBox, J1939SFRoot);
     connect(SFIDSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_canSpecification->SetJ1939SFID(value); });

     QSpinBox* SFMaskSpinBox = createConfigurationSpinBox(m_canSpecification->J1939SFMask(), 0, 536870912);
     AddEditableData("Mask", SFMaskSpinBox, J1939SFRoot);
     connect(SFMaskSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_canSpecification->SetJ1939SFMask(value); });

     QTreeWidgetItem* J1939FFRoot = AddContainer("First frame configuraton",J1939Root);
     QSpinBox* FFIDSpinBox = createConfigurationSpinBox(m_canSpecification->J1939FFID(), 0, 536870912);
     AddEditableData("ID", FFIDSpinBox, J1939FFRoot);
     connect(FFIDSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_canSpecification->SetJ1939FFID(value); });

     QSpinBox* FFMaskSpinBox = createConfigurationSpinBox(m_canSpecification->J1939FFMask(), 0, 536870912);
     AddEditableData("Mask", FFMaskSpinBox, J1939FFRoot);
     connect(FFMaskSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_canSpecification->SetJ1939FFMask(value); });

     QTreeWidgetItem* J1939CFRoot = AddContainer("Consecutive frame configuraton",J1939Root);
     QSpinBox* CFIDSpinBox = createConfigurationSpinBox(m_canSpecification->J1939CFID(), 0, 536870912);
     AddEditableData("ID", CFIDSpinBox, J1939CFRoot);
     connect(CFIDSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_canSpecification->SetJ1939CFID(value); });

     QSpinBox* CFMaskSpinBox = createConfigurationSpinBox(m_canSpecification->J1939CFMask(), 0, 536870912);
     AddEditableData("Mask", CFMaskSpinBox, J1939CFRoot);
     connect(CFMaskSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_canSpecification->SetJ1939CFMask(value); });

     J1939Root->setToolTip(0, "Mailbox configuration for DM1 message reception");
     J1939Root->setExpanded(true);
}

QSpinBox* CANSpecificationPropertiesWidget::createConfigurationSpinBox(quint32 value, quint32 min, quint32 max)
{
    QSpinBox* spinBox = new QSpinBox();

    spinBox->setMaximum(max);
    spinBox->setMinimum(min);
    spinBox->setDisplayIntegerBase(16);
    spinBox->setPrefix("0x");
    spinBox->setValue(value);
    return spinBox;
}

void CANSpecificationPropertiesWidget::beforePopulateSignals()
{
    if (!m_project->GetCANSpecificationPath().isEmpty())
    {
        populateDriverParameters();
        populateDiagParameters();
        populateJ1939Parameters();
    }
    else
    {
        QLabel* canInfoLabel = new QLabel();
        canInfoLabel->setText("No CAN specification defined!");
        AddEditableData("Info:", canInfoLabel);
    }
}


