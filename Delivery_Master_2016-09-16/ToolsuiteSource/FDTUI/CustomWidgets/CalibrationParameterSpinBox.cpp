#include "CalibrationParameterSpinBox.h"

#include "FunctionBlock.h"
#include <limits>
#include <QSpinBox>
#include <QKeyEvent>

CalibrationParameterSpinBox::CalibrationParameterSpinBox(FunctionBlock* functionBlock, const CalibrationParameter* calibrationParameter) :
    m_functionBlock(functionBlock),
    m_calibrationParameter(calibrationParameter)
{
    setValueRange();

    setPrefix("0x");
    setDisplayIntegerBase(16);

    if (functionBlock->HasCalibrationParameterDefaultValue(calibrationParameter->GetParameterName()))
    {
        setValue(functionBlock->GetCalibrationParameterDefaultValue(calibrationParameter->GetParameterName()).toInt());
    }
    else
    {
        setValue(minimum());
        setSpecialValueText("Unset");
    }

    connect(this, static_cast<void(CalibrationParameterSpinBox::*)(int)>(&CalibrationParameterSpinBox::valueChanged), [=](int value)
    {
        setSpecialValueText("");
        setSuffix(QString("  (%1)").arg(QString::number(value)));
        m_functionBlock->SetCalibrationParameterDefaultValue(m_calibrationParameter->GetParameterName(), QString::number(value));
    });
}

void CalibrationParameterSpinBox::setValueRange()
{
    if (m_calibrationParameter->GetDataType() == "int8")
    {
        setMinimum(std::numeric_limits<qint8>::min());
        setMaximum(std::numeric_limits<qint8>::max());
    }
    else if (m_calibrationParameter->GetDataType() == "uint8")
    {
        setMinimum(std::numeric_limits<quint8>::min());
        setMaximum(std::numeric_limits<quint8>::max());
    }
    else if (m_calibrationParameter->GetDataType() == "int16")
    {
        setMinimum(std::numeric_limits<qint16>::min());
        setMaximum(std::numeric_limits<qint16>::max());
    }
    else if (m_calibrationParameter->GetDataType() == "uint16")
    {
        setMinimum(std::numeric_limits<quint16>::min());
        setMaximum(std::numeric_limits<quint16>::max());
    }
    else if (m_calibrationParameter->GetDataType() == "int32")
    {
        setMinimum(std::numeric_limits<qint32>::min());
        setMaximum(std::numeric_limits<qint32>::max());
    }
    else if (m_calibrationParameter->GetDataType() == "uint32")
    {
        setMinimum(0);
        // Unfortunately the Qt spinbox does not support values higher than int32 max
        setMaximum(std::numeric_limits<qint32>::max());
    }
}

void CalibrationParameterSpinBox::keyReleaseEvent(QKeyEvent *event)
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
            m_functionBlock->SetCalibrationParameterDefaultValue(m_calibrationParameter->GetParameterName(), QString(QChar(key)));
        }
    }
}
