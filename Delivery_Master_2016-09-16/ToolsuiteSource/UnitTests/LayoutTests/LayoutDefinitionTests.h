#ifndef LAYOUTDEFINITIONTESTS_H
#define LAYOUTDEFINITIONTESTS_H

#include "autotest.h"

class LayoutDefinitionTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void HasMenu_NoMenuDefinitionSet_ReturnsFalse(void);
    void HasMenu_MenuDefinitionSet_ReturnsTrue(void);
    void GetMenu_NoMenuDefinitionSet_ReturnsNull(void);
    void GetMenu_MenuDefinitionSet_ReturnsMenuDefinition(void);
    void SetMenu_NoDisplayArea_CreatesDisplayAreaForMenuDefinition(void);
    void SetMenu_ExistingDisplayArea_ExistingDisplayAreaHoldsMenuDefinition(void);
};

DECLARE_TEST(LayoutDefinitionTests)

#endif // LAYOUTDEFINITIONTESTS_H
