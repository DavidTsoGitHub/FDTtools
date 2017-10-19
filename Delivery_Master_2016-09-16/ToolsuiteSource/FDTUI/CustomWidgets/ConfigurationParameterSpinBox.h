#ifndef CONFIGURATIONPARAMETERSPINBOX_H
#define CONFIGURATIONPARAMETERSPINBOX_H

class ConfigurationParameter;
class FunctionBlock;
class QSpinBox;

class ConfigurationParameterSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    ConfigurationParameterSpinBox(FunctionBlock* functionBlock, const ConfigurationParameter* configurationParameter);

private:
    qint32 m_min;
    qint32 m_max;
    FunctionBlock* m_functionBlock;
    const ConfigurationParameter* m_configurationParameter;

    void setValueRange(void);

protected:
    void keyReleaseEvent(QKeyEvent* event);
};

#endif // CONFIGURATIONPARAMETERSPINBOX_H
