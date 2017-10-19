#ifndef CALIBRATIONPARAMETERSPINBOX_H
#define CALIBRATIONPARAMETERSPINBOX_H

class CalibrationParameter;
class FunctionBlock;
class QSpinBox;

class CalibrationParameterSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    CalibrationParameterSpinBox(FunctionBlock* functionBlock, const CalibrationParameter* calibrationParameter);

private:
    qint32 m_min;
    qint32 m_max;
    FunctionBlock* m_functionBlock;
    const CalibrationParameter* m_calibrationParameter;

    void setValueRange(void);

protected:
    void keyReleaseEvent(QKeyEvent* event);
};

#endif // CALIBRATIONPARAMETERSPINBOX_H
