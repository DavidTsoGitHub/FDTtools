#ifndef UIWIDGETPROPERTIESWIDGET_H
#define UIWIDGETPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"
#include "UIWidgetBase.h"

class NameChangeWidget;
class Project;

class QCheckBox;
class QComboBox;
class QLineEdit;
template <class Key, class T> class QMap;
class QSpinBox;
class QTextEdit;
class QTreeWidgetItem;
class QWidget;

class UIWidgetPropertiesWidget : public ProjectObjectPropertiesWidget
{
public:
    UIWidgetPropertiesWidget(Project* project, QWidget* parent = 0);
    ~UIWidgetPropertiesWidget();

    void SetUIWidget(UIWidgetBase* uiWidget);

private:
    Project* m_project;
    UIWidgetBase* m_uiWidget;
    NameChangeWidget* m_nameEdit;
    QSpinBox* m_x;
    QSpinBox* m_y;
    QSpinBox* m_width;
    QSpinBox* m_heigth;
    QCheckBox* m_boxed;

    QSpinBox* m_rgbPrimaryFGRed;
    QSpinBox* m_rgbPrimaryFGGreen;
    QSpinBox* m_rgbPrimaryFGBlue;
    QLineEdit* m_primaryForegroundColorPreview;

    QSpinBox* m_rgbPrimaryBGRed;
    QSpinBox* m_rgbPrimaryBGGreen;
    QSpinBox* m_rgbPrimaryBGBlue;
    QLineEdit* m_primaryBackgroundColorPreview;
    QCheckBox* m_primaryTransparentBackground;

    QSpinBox* m_rgbSecondaryFGRed;
    QSpinBox* m_rgbSecondaryFGGreen;
    QSpinBox* m_rgbSecondaryFGBlue;
    QLineEdit* m_secondaryForegroundColorPreview;

    QSpinBox* m_rgbSecondaryBGRed;
    QSpinBox* m_rgbSecondaryBGGreen;
    QSpinBox* m_rgbSecondaryBGBlue;
    QLineEdit* m_secondaryBackgroundColorPreview;
    QCheckBox* m_secondaryTransparentBackground;

    QTreeWidgetItem* m_animationContainer;
    quint32 m_nbrOfAnimation1Triggers;
    quint32 m_nbrOfAnimation2Triggers;

    QTextEdit* m_description;
    QLineEdit* m_layersNeeded;

    QMap<int, QWidget*> m_customPropertyWidgets;

    QSpinBox* getNewSpinBox(UIWidgetBase::DataType_t dataType);
    QComboBox* getNewComboBox(UIWidgetBase::DataType_t dataType, quint32 currentId);

    void updatePrimaryForegroundColorPreview();
    void updatePrimaryBackgroundColorPreview();

    void updateSecondaryForegroundColorPreview();
    void updateSecondaryBackgroundColorPreview();

    void populateTriggers(UIWidgetBase *widget, quint8 animationIndex);
    void addNewAnimation1Trigger();
    void addNewAnimation2Trigger();

private slots:
    void slot_nameChanged(const QString& newName);
    void slot_xChanged(int newValue);
    void slot_yChanged(int newValue);
    void slot_widthChanged(int newValue);
    void slot_heightChanged(int newValue);
    void slot_boxedChanged(bool checked);
    void slot_customPropertyChanged(int propertyIndex);

    void slot_rgbPrimaryFGRedChanged(int newValue);
    void slot_rgbPrimaryFGGreenChanged(int newValue);
    void slot_rgbPrimaryFGBlueChanged(int newValue);

    void slot_primaryBackgroundTransparent_toggled(bool checked);
    void slot_rgbPrimaryBGRedChanged(int newValue);
    void slot_rgbPrimaryBGGreenChanged(int newValue);
    void slot_rgbPrimaryBGBlueChanged(int newValue);

    void slot_rgbSecondaryFGRedChanged(int newValue);
    void slot_rgbSecondaryFGGreenChanged(int newValue);
    void slot_rgbSecondaryFGBlueChanged(int newValue);

    void slot_secondaryBackgroundTransparent_toggled(bool checked);
    void slot_rgbSecondaryBGRedChanged(int newValue);
    void slot_rgbSecondaryBGGreenChanged(int newValue);
    void slot_rgbSecondaryBGBlueChanged(int newValue);

protected:
    void beforePopulateSignals();
    void afterPopulateSignals();
};

#endif // UIWIDGETPROPERTIESWIDGET_H
