#ifndef DATAARRAYLINEEDIT_H
#define DATAARRAYLINEEDIT_H

class CalibrationParameter;
class ConfigurationParameter;
class FunctionBlock;
class QLabel;
class QLineEdit;
class QRegularExpression;
class QString;
class QWidget;

class DataArrayLineEdit : public QWidget
{
    Q_OBJECT
public:
    explicit DataArrayLineEdit(FunctionBlock* functionBlock, const CalibrationParameter* calibrationParameter, QWidget* parent = 0);
    explicit DataArrayLineEdit(FunctionBlock* functionBlock, const ConfigurationParameter* configurationParameter, QWidget* parent = 0);

private slots:
    void slot_textChanged(QString newText);

private:
    QLineEdit* m_lineEdit;
    QLabel* m_errorLabel;

    FunctionBlock* m_functionBlock;

    const CalibrationParameter* m_calParameter;
    const ConfigurationParameter* m_cfgParameter;

    bool m_isUnsigned;

    QRegularExpression m_separatorsRegexp;

    quint8 getArrayLength();
    QString getDataType();
    QString getParameterName();
    void setValue(QString value);

    void setupUi();
    bool isWithinRangeForDataType(quint32 value);
    bool isWithinRangeForDataType(qint32 value);
};

#endif // DATAARRAYLINEEDIT_H
