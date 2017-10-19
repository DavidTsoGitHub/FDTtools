#ifndef DYNAMICIMAGEUIWIDGETGENERATOR
#define DYNAMICIMAGEUIWIDGETGENERATOR

#include "FileGeneratorBase.h"
#include "UIWidgetBase.h"
#include "DynamicImageWidget.h"

class DynamicImageUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit DynamicImageUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList);
    ~DynamicImageUIWidgetGenerator();

private:
    void generateConfigurationSourceFile();
    QString getConnectiveString(LayoutTransitionTypes::TransitionConnective connective);
    QString getCompareString(LayoutTransitionTypes::TransitionCompare compareCondition);

    QList<UIWidgetBase*> m_uiWidgetList;

    // FileGeneratorBase interface
protected:
        void OnGenerateFiles();
};
#endif // DYNAMICIMAGEUIWIDGETGENERATOR

