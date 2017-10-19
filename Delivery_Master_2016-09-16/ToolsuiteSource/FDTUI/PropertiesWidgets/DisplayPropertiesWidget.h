#ifndef DISPLAYPROPERTIESWIDGET_H
#define DISPLAYPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class BSPDisplay;
class LayoutDefinition;
class Project;

class QCheckBox;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QWidget;
class RangePickerWidget;
class ColorRangePreviewWidget;

class DisplayPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit DisplayPropertiesWidget(Project* project, QWidget *parent = 0);
    ~DisplayPropertiesWidget();

    void SetLayoutDefinition(LayoutDefinition* layoutDefinition);

private slots:
    void slot_StartupEnabledCheckBox_toggled(bool checked);
    void slot_MainLayout_currentIndexChanged(int index);

private:
    void updateChromaKeyPreview();
    void populateLayoutComboBox();
    void setStartupItemsState(bool checked);

    const BSPDisplay* m_display;
    LayoutDefinition* m_layoutDefinition;

    QLineEdit* m_renderingTypeLineEdit;
    QLineEdit* m_heightLineEdit;
    QLineEdit* m_widthLineEdit;
    QLineEdit* m_bitsPerPixelLineEdit;
    QLineEdit* m_useDCUMemoryLineEdit;
    QLineEdit* m_dcuMemorySizeLineEdit;
    QLineEdit* m_dcuNbrOfLayersLineEdit;
    QSpinBox* m_cropLeftSpinBox;
    QSpinBox* m_cropRightSpinBox;
    QSpinBox* m_cropTopSpinBox;
    QSpinBox* m_cropBottomSpinBox;
    QComboBox* m_MainLayoutComboBox;
    QCheckBox* m_StartupEnabledCheckBox;
    QSpinBox* m_StartupTimeoutSpinBox;
    QTreeWidgetItem* m_chromaKeyRoot;
    RangePickerWidget* m_chromaRedPicker;
    RangePickerWidget* m_chromaGreenPicker;
    RangePickerWidget* m_chromaBluePicker;
    ColorRangePreviewWidget* m_chromaColorRangePreview;
    QCheckBox* m_rotateScreenCounterClockwiseEnabledCheckBox;

    // ProjectObjectPropertiesWidget interface
protected:
    void beforePopulateSignalSinks();
};

#endif // DISPLAYPROPERTIESWIDGET_H
