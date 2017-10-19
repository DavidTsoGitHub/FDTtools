#ifndef LABELUIWIDGETGENERATOR_H
#define LABELUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "UIWidgetBase.h"
#include "LabelWidget.h"

class LabelUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit LabelUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList);
    ~LabelUIWidgetGenerator();

private:
    QList<UIWidgetBase*> m_uiWidgetList;

    void generateLabelConfigurationSourceFile();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();
};

#endif // LABELUIWIDGETGENERATOR_H
