#ifndef DYNAMICICONUIWIDGETGENERATOR_H
#define DYNAMICICONUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "UIWidgetBase.h"
#include "DynamicIconWidget.h"

class DynamicIconUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit DynamicIconUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList);
    ~DynamicIconUIWidgetGenerator();

private:
    QList<UIWidgetBase*> m_uiWidgetList;
    void generateConfigurationSourceFile();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

};

#endif // DYNAMICICONUIWIDGETGENERATOR_H
