#ifndef DYNAMICLABELUIWIDGETGENERATOR_H
#define DYNAMICLABELUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "UIWidgetBase.h"
#include "DynamicLabelWidget.h"

class DynamicLabelUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit DynamicLabelUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList);
    ~DynamicLabelUIWidgetGenerator();

private:
    QList<UIWidgetBase*> m_uiWidgetList;
    void generateConfigurationSourceFile();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

};


#endif // DYNAMICLABELUIWIDGETGENERATOR_H

