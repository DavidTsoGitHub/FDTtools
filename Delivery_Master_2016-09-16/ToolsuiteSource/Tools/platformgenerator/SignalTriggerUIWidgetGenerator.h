#ifndef SIGNALTRIGGERUIWIDGETGENERATOR_H
#define SIGNALTRIGGERUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "UIWidgetBase.h"
#include "SignalTriggerWidget.h"

class SignalTriggerUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit SignalTriggerUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList);
    ~SignalTriggerUIWidgetGenerator();

private:
    QList<UIWidgetBase*> m_uiWidgetList;
    void generateConfigurationSourceFile();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();
};

#endif // SIGNALTRIGGERUIWIDGETGENERATOR_H
