#ifndef MENUDEFINITIONDESERIALIZERTESTS_H
#define MENUDEFINITIONDESERIALIZERTESTS_H

#include "autotest.h"
#include "MenuDefinitionSerializer.h"

class MenuDefinitionDeserializerTests : public QObject
{
    Q_OBJECT

public:

private Q_SLOTS:
    // Happy path
    void Deserialize_NoData_ReturnsTrueAndEmptySpecification(void);
    void Deserialize_OneMenuItemSingleLabelWidgetAndSingleCommand_ReturnsTrueAndMenuItem(void);
    void Deserialize_OneMenuItemManyLabelWidgetsAndManyCommandMappings_ReturnsTrueAndMenuItem(void);
    void Deserialize_NestedMenuItems_ReturnsTrueAndMenuHierarchy(void);

private:
    MenuDefinitionSerializer* m_serializer;
};

DECLARE_TEST(MenuDefinitionDeserializerTests)

#endif // MENUDEFINITIONDESERIALIZERTESTS_H
