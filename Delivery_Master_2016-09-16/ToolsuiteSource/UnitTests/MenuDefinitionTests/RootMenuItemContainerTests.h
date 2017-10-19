#ifndef ROOTMENUITEMCONTAINERTESTS_H
#define ROOTMENUITEMCONTAINERTESTS_H

#include "autotest.h"



class RootMenuItemContainerTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void AddMenuItem_MenuItemIsNull_NoMenuItemAdded(void);
    void AddMenuItem_ValidMenuItem_MenuItemAdded(void);

    void RemoveMenuItem_KnownMenuItem_MenuItemIsRemoved(void);
    void RemoveMenuItem_UnknownMenuItem_NoMenuItemRemoved(void);
    void RemoveMenuItem_MenuItemIsNull_NoMenuItemRemoved(void);

    void MoveMenuItemUp_MenuItemIsSecondInList_MenuItemBecomesFirstInList(void);
    void MoveMenuItemUp_MenuItemIsFirstInList_MenuItemDoesNotMove(void);

    void MoveMenuItemDown_MenuItemIsFirstInList_MenuItemBecomesSecondInList(void);
    void MoveMenuItemDown_MenuItemIsLastInList_MenuItemDoesNotMove(void);

};

DECLARE_TEST(RootMenuItemContainerTests)

#endif // ROOTMENUITEMCONTAINERTESTS_H
