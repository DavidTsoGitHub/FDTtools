#ifndef DISPLAYAREAPROPERTIESWIDGET_H
#define DISPLAYAREAPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class DisplayArea;
class NameChangeWidget;
class Project;

class QCheckBox;
class QLineEdit;
class QSpinBox;
class QWidget;

class DisplayAreaPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit DisplayAreaPropertiesWidget(Project* project, QWidget *parent = 0);
    ~DisplayAreaPropertiesWidget();

    void SetDisplayArea(DisplayArea* displayArea);

private:
    DisplayArea* m_displayArea;
    Project* m_project;

    NameChangeWidget* m_nameLineEdit;
    QSpinBox* m_xSpinBox;
    QSpinBox* m_ySpinBox;
    QSpinBox* m_widthSpinBox;
    QSpinBox* m_heightSpinBox;
    QCheckBox* m_borderCheckBox;
    QSpinBox* m_rgbFGRed;
    QSpinBox* m_rgbFGGreen;
    QSpinBox* m_rgbFGBlue;
    QLineEdit* m_foregroundColorPreview;
    QCheckBox* m_transparentBackground;
    QSpinBox* m_rgbBGRed;
    QSpinBox* m_rgbBGGreen;
    QSpinBox* m_rgbBGBlue;
    QLineEdit* m_backgroundColorPreview;

    void updateForegroundColorPreview();
    void updateBackgroundColorPreview();

private slots:
    void slot_backgroundTransparent_toggled(bool checked);

    // ProjectObjectPropertiesWidget interface
protected:
    void beforePopulateSignals();
};

#endif // DISPLAYAREAPROPERTIESWIDGET_H
