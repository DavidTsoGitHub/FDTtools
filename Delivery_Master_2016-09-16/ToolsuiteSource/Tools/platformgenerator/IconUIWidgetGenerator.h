#ifndef ICONUIWIDGETGENERATOR_H
#define ICONUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "UIWidgetBase.h"
#include "IconWidget.h"

class IconUIWidgetGenerator : public FileGeneratorBase
{    
public:
    explicit IconUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList);
    ~IconUIWidgetGenerator();

private:
    QList<UIWidgetBase*> m_uiWidgetList;

    void generateIconConfigurationSourceFile();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

};

#endif // ICONUIWIDGETGENERATOR_H
