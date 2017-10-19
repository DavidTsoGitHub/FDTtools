#include "ConfigurationParameterSpinBox.h"

#include "ConfigurationParameter.h"
#include "FunctionBlock.h"
#include <limits>
#include <QSpinBox>
#include <QKeyEvent>


ConfigurationParameterSpinBox::ConfigurationParameterSpinBox(FunctionBlock* functionBlock, const ConfigurationParameter* configurationParameter) :
    m_functionBlock(functionBlock),
    m_configurationParameter(configurationParameter)
{
    setValueRange();

    setPrefix("0x");
    setDisplayIntegerBase(16);

    if (functionBlock->HasConfigurationParameter(configurationParameter->GetParameterName()))
    {
        qint32 value = functionBlock->GetConfigurationParameterValue(configurationParameter->GetParameterName()).toInt();
        setValue(value);
    }
    else
    {
        setValue(minimum());
        setSpecialValueText("Unset");
    }

    connect(this, static_cast<void(ConfigurationParameterSpinBox::*)(int)>(&ConfigurationParameterSpinBox::valueChanged), [=](int value)
    {
        setSpecialValueText("");
        setSuffix(QString("  (%1)").arg(QString::number(value)));
        m_functionBlock->SetConfigurationParameter(m_configurationParameter->GetParameterName(), QString::number(value));
    });
}

void ConfigurationParameterSpinBox::setValueRange()
{
    if (m_configurationParameter->GetDataType() == "int8")
    {
        setMinimum(std::numeric_limits<qint8>::min());
        setMaximum(std::numeric_limits<qint8>::max());
    }
    else if (m_configurationParameter->GetDataType() == "uint8")
    {
        setMinimum(std::numeric_limits<quint8>::min());
        setMaximum(std::numeric_limits<quint8>::max());
    }
    else if (m_configurationParameter->GetDataType() == "int16")
    {
        setMinimum(std::numeric_limits<qint16>::min());
        setMaximum(std::numeric_limits<qint16>::max());
    }
    else if (m_configurationParameter->GetDataType() == "uint16")
    {
        setMinimum(std::numeric_limits<quint16>::min());
        setMaximum(std::numeric_limits<quint16>::max());
    }
    else if (m_configurationParameter->GetDataType() == "int32")
    {
        setMinimum(std::numeric_limits<qint32>::min());
        setMaximum(std::numeric_limits<qint32>::max());
    }
    else if (m_configurationParameter->GetDataType() == "uint32")
    {
        setMinimum(0);
        // Unfortunately the Qt spinbox does not support values higher than int32 max
        setMaximum(std::numeric_limits<qint32>::max());
    }
}

void ConfigurationParameterSpinBox::keyReleaseEvent(QKeyEvent *event)
{
    if (specialValueText() == "Unset")
    {
        int key = event->key();
        if ((key >= 0x30 && key <= 0x39) || (key >= 0x41 && key <= 0x46))
        {
            setSpecialValueText("");
            bool status = false;
            setValue(QString(QChar(key)).toInt(&status, 16));
            setSuffix(QString("  (%1)").arg(QString::number(QString(QChar(key)).toInt(&status, 16))));
            m_functionBlock->SetConfigurationParameter(m_configurationParameter->GetParameterName(), QString(QChar(key)));
        }
    }
}
