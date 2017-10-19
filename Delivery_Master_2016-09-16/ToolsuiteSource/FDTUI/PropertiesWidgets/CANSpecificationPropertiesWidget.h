#ifndef CANSPECIFICATIONPROPERTIESWIDGET_H
#define CANSPECIFICATIONPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class CANSignal;
class CANSpecification;
class Project;

class QTreeWidgetItem;
class QSpinBox;
class QString;
class QWidget;

class CANSpecificationPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit CANSpecificationPropertiesWidget(Project* project, const QString& ecuName, QWidget* parent = 0);
    ~CANSpecificationPropertiesWidget();

    void SetCANSpecification(CANSpecification* canSpecification);

private:
    const QString& m_ecuName;
    CANSpecification* m_canSpecification;
    QTreeWidgetItem* m_presdivDataItem;
    Project* m_project;

    void populateDriverParameters();
    void populateDiagParameters(void);
    void populateJ1939Parameters(void);
    void setToolTip(QTreeWidgetItem* item, const CANSignal* signal);
    QSpinBox* createConfigurationSpinBox(quint32 value, quint32 min, quint32 max);

protected:
    void beforePopulateSignals();
};

#endif // CANSPECIFICATIONPROPERTIESWIDGET_H
