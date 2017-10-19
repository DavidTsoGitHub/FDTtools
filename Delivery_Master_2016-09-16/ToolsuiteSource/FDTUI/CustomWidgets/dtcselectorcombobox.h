#ifndef DTCSELECTORCOMBOBOX
#define DTCSELECTORCOMBOBOX

class DiagDefinition;
class FunctionBlock;
class Project;
class QComboBox;
class QString;
class QWidget;

class DTCSelectorComboBox : public QComboBox
{
    Q_OBJECT

public:
    DTCSelectorComboBox(FunctionBlock* functionBlock, QString dtcParameterName, Project* project, QWidget* parent = 0);

private slots:
    void populate();
    void slot_currentTextChanged(QString);
    void projectUpdated();

private:
    const DiagDefinition& m_diagDefinition;
    QString m_dtcParameterName;
    FunctionBlock* m_functionBlock;

    bool m_signalsEnabled;
};

#endif // DTCSELECTORCOMBOBOX

