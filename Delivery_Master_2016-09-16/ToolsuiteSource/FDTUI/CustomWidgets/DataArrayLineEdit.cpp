#include "DataArrayLineEdit.h"

#include "CalibrationParameter.h"
#include "ConfigurationParameter.h"
#include "FunctionBlock.h"
#include <limits>
#include <QVBoxLayout>

DataArrayLineEdit::DataArrayLineEdit(FunctionBlock* functionBlock, const CalibrationParameter* calibrationParameter, QWidget* parent) :
    QWidget(parent),
    m_functionBlock(functionBlock),
    m_calParameter(calibrationParameter),
    m_cfgParameter(NULL)
{
    setupUi();
    m_isUnsigned = getDataType().toLower().startsWith("u");

    m_lineEdit->setText(functionBlock->GetCalibrationParameterDefaultValue(calibrationParameter->GetParameterName()));

    connect(m_lineEdit, &QLineEdit::textChanged, this, &DataArrayLineEdit::slot_textChanged);
}

DataArrayLineEdit::DataArrayLineEdit(FunctionBlock* functionBlock, const ConfigurationParameter* configurationParameter, QWidget* parent) :
    QWidget(parent),
    m_functionBlock(functionBlock),
    m_calParameter(NULL),
    m_cfgParameter(configurationParameter)
{
    setupUi();
    m_isUnsigned = getDataType().toLower().startsWith("u");

    m_lineEdit->setText(functionBlock->GetConfigurationParameterValue(configurationParameter->GetParameterName()));

    connect(m_lineEdit, &QLineEdit::textChanged, this, &DataArrayLineEdit::slot_textChanged);
}

void DataArrayLineEdit::setupUi()
{
    setLayout(new QVBoxLayout());
    m_errorLabel = new QLabel();
    m_lineEdit = new QLineEdit();
    m_lineEdit->setToolTip("Insert a token (semi-colon or pipe) separated string of values, values can be entered in hex (prefixed with 0x), octal (prefixed with 0) or decimal.");

    layout()->setContentsMargins(0, 2, 0, 2);
    layout()->addWidget(m_lineEdit);
    layout()->addWidget(m_errorLabel);

    m_separatorsRegexp = QRegularExpression("[;\\|]");
}

void DataArrayLineEdit::slot_textChanged(QString newText)
{
    QStringList items = newText.split(m_separatorsRegexp);

    if (items.count() != getArrayLength())
    {
        m_errorLabel->setText(QString("Element count mismatch in value string (expected:%1 actual:%2)").arg(getArrayLength()).arg(items.count()));
    }
    else
    {
        QStringList trimmedItems;
        foreach (QString item, items)
        {
            bool parsedOk;
            bool isWithinRange;

            if (m_isUnsigned)
            {
                quint32 value = item.toInt(&parsedOk, 0);
                isWithinRange = isWithinRangeForDataType(value);
            }
            else
            {
                qint32 value = item.toInt(&parsedOk, 0);
                isWithinRange = isWithinRangeForDataType(value);
            }

            if (!parsedOk)
            {
                m_errorLabel->setText(QString("'%1' could not be parsed to a number").arg(item));
                return;
            }
            if (!isWithinRange)
            {
                m_errorLabel->setText(QString("'%1' is not within range for a " + getDataType()).arg(item));
                return;
            }

            trimmedItems.append(item.trimmed());
        }

        qint32 cursorPosition = m_lineEdit->cursorPosition();

        m_errorLabel->setText("");
        m_lineEdit->setText(trimmedItems.join("|"));
        m_lineEdit->setCursorPosition(cursorPosition);

        setValue(m_lineEdit->text());
    }
}

quint8 DataArrayLineEdit::getArrayLength()
{
    if (m_calParameter != NULL)
    {
        return m_calParameter->GetArrayLength();
    }
    else if (m_cfgParameter != NULL)
    {
        return m_cfgParameter->GetArrayLength();
    }
    else
    {
        // Since the constructors do not allow for this scenario to happen we just return 0
        return 0;
    }
}

QString DataArrayLineEdit::getDataType()
{
    if (m_calParameter != NULL)
    {
        return m_calParameter->GetDataType();
    }
    else if (m_cfgParameter != NULL)
    {
        return m_cfgParameter->GetDataType();
    }
    else
    {
        // Since the constructors do not allow for this scenario to happen we just return empty string
        return "";
    }
}

QString DataArrayLineEdit::getParameterName()
{
    if (m_calParameter != NULL)
    {
        return m_calParameter->GetParameterName();
    }
    else if (m_cfgParameter != NULL)
    {
        return m_cfgParameter->GetParameterName();
    }
    else
    {
        // Since the constructors do not allow for this scenario to happen we just return empty string
        return "";
    }
}

void DataArrayLineEdit::setValue(QString value)
{
    if (m_calParameter != NULL)
    {
        m_functionBlock->SetCalibrationParameterDefaultValue(m_calParameter->GetParameterName(), value);
    }
    else if (m_cfgParameter != NULL)
    {
        m_functionBlock->SetConfigurationParameter(m_cfgParameter->GetParameterName(), value);
    }
    else
    {
        // Since the constructors do not allow for this scenario to happen we simply ignore this
    }
}

bool DataArrayLineEdit::isWithinRangeForDataType(quint32 value)
{
    if (getDataType() == "uint32") return true;
    else if (getDataType() == "uint16") return value <= USHRT_MAX;
    else if (getDataType() == "uint8") return value <= UCHAR_MAX;
    return false;
}

bool DataArrayLineEdit::isWithinRangeForDataType(qint32 value)
{
    if (getDataType() == "sint32") return true;
    else if (getDataType() == "sint16") return SHRT_MIN <= value && value <= SHRT_MAX;
    else if (getDataType() == "sint8") return INT_MIN <= value && value <= INT_MAX;
    return false;
}
