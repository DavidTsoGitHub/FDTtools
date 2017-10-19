#ifndef BSPSPECIFICATIONPROPERTIESWIDGET_H
#define BSPSPECIFICATIONPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class BSPSpecification;
class Project;

class QLineEdit;
class QTreeWidgetItem;
class QWidget;

class BSPSpecificationPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit BSPSpecificationPropertiesWidget(Project* project, QWidget* parent = 0);
    ~BSPSpecificationPropertiesWidget();

    void SetBSPSpecification(BSPSpecification* bspSpecification);

private:
    BSPSpecification* m_bspSpecification;

    QTreeWidgetItem* m_displayPropertiesRoot;
    QTreeWidgetItem* m_segmentDisplayPropertiesRoot;

    QLineEdit* m_renderingTypeLineEdit;
    QLineEdit* m_heightLineEdit;
    QLineEdit* m_widthLineEdit;
    QLineEdit* m_useGRAMLineEdit;
    QLineEdit* m_gramAvailableMemoryLineEdit;
    QLineEdit* m_numberOfLayersLineEdit;
    QLineEdit* m_renderingOrientationLineEdit;

    QLineEdit* m_segmentDisplayNbrOfSegments;


    // ProjectObjectPropertiesWidget interface
protected:
    void beforePopulateSignals();
    void afterPopulateSignals();
};

#endif // BSPSPECIFICATIONPROPERTIESWIDGET_H
