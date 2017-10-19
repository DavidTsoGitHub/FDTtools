#ifndef IMAGEUIWIDGETGENERATOR_H
#define IMAGEUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "UIWidgetBase.h"
#include "ImageWidget.h"

class ImageUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit ImageUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList);
    ~ImageUIWidgetGenerator();

private:
    QList<UIWidgetBase*> m_uiWidgetList;

    void generateImageConfigurationSourceFile();
    QString getConnectiveString(LayoutTransitionTypes::TransitionConnective connective);
    QString getCompareString(LayoutTransitionTypes::TransitionCompare compareCondition);

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();
};

#endif // IMAGEUIWIDGETGENERATOR_H
