#ifndef BLOCKBARUIWIDGETGENERATOR_H
#define BLOCKBARUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "BlockBarWidget.h"

class BlockBarUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit BlockBarUIWidgetGenerator(QList<UIWidgetBase*> uiWidgets);

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles(void);

private:
    QList<UIWidgetBase*> m_uiWidgets;
    QString m_Path;

    void generateSource(void);
};

#endif // BLOCKBARUIWIDGETGENERATOR_H
