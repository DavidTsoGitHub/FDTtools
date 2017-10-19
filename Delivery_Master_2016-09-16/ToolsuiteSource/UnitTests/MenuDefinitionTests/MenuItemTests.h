#ifndef MENUITEMTESTS_H
#define MENUITEMTESTS_H

#include "autotest.h"

class MenuItemTests : public QObject
{
    Q_OBJECT

public:


private Q_SLOTS:
    void AddSubMenuItem_ValidMenuItem_AddsMenuItemToParent(void);
    void AddLabelWidget_ValidLabelWidget_AddsLabelWidgetToParent(void);

    void MoveSubMenuItemUp_MenuItemIsSecondInList_MenuItemBecomesFirstInList(void);
    void MoveSubMenuItemUp_MenuItemIsFirstInList_MenuItemDoesNotMove(void);

    void MoveSubMenuItemDown_MenuItemIsFirstInList_MenuItemBecomesSecondInList(void);
    void MoveSubMenuItemDown_MenuItemIsLastInList_MenuItemDoesNotMove(void);

};

DECLARE_TEST(MenuItemTests)

#endif // MENUITEMTESTS_H
