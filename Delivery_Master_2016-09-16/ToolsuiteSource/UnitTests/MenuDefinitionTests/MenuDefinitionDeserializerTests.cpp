#include "MenuDefinitionDeserializerTests.h"

#include "MenuDefinitionDeserializer.h"
#include "MenuDefinitionSerializer.h"
#include "MenuItem.h"
#include "IconDefinition.h"
#include "LabelWidget.h"
#include "Project.h"

#include <QXmlStreamWriter>

void MenuDefinitionDeserializerTests::Deserialize_NoData_ReturnsTrueAndEmptySpecification()
{
    // Arrange
    MenuDefinitionDeserializer target("<?xml version=\"1.0\" encoding=\"utf-8\"?><Menu></Menu>");
    MenuDefinition menuDefinition;
    Project project;

    // Act
    bool result = target.Deserialize(menuDefinition, project);

    // Assert
    QCOMPARE(result, true);
    QCOMPARE(menuDefinition.InputToCommandMappings().count(), 0);
    QCOMPARE(menuDefinition.MenuRoot().MenuItems().count(), 0);
}

void MenuDefinitionDeserializerTests::Deserialize_OneMenuItemSingleLabelWidgetAndSingleCommand_ReturnsTrueAndMenuItem()
{
    Project project;
    // Arrange
    LabelDefinition labelDefinition;
    LabelWidget* expectedLabelWidget = new LabelWidget(labelDefinition, project.GetLayoutDefinition());
    expectedLabelWidget->SetWidth(10);
    expectedLabelWidget->SetHeight(20);
    expectedLabelWidget->SetX(30);
    expectedLabelWidget->SetY(40);
    expectedLabelWidget->SetLabelID(50);

    MenuItem* expectedMenuItem = new MenuItem("expected");
    expectedMenuItem->AddUIWidget(expectedLabelWidget);

    MenuDefinition expected;
    expected.MenuRoot().AddMenuItem(expectedMenuItem);


    // Act
    m_serializer = new MenuDefinitionSerializer(expected);
    MenuDefinitionDeserializer target(m_serializer->Serialize());
    MenuDefinition menuDefinition;
    bool result = target.Deserialize(menuDefinition, project);

    // Assert
    QCOMPARE(result, true);
    QCOMPARE(menuDefinition.MenuRoot().MenuItems().count(), 1);
    QCOMPARE(menuDefinition.MenuRoot().MenuItems().at(0)->GetName(), expectedMenuItem->GetName());
    QCOMPARE(menuDefinition.MenuRoot().MenuItems().at(0)->UIWidgets().count(), 1);
    LabelWidget* actualWidget = dynamic_cast<LabelWidget*>(menuDefinition.MenuRoot().MenuItems().at(0)->UIWidgets().at(0));
    QCOMPARE(actualWidget->GetX(), expectedLabelWidget->GetX());
    QCOMPARE(actualWidget->GetY(), expectedLabelWidget->GetY());
    QCOMPARE(actualWidget->GetWidth(), expectedLabelWidget->GetWidth());
    QCOMPARE(actualWidget->GetHeight(), expectedLabelWidget->GetHeight());
    QCOMPARE(actualWidget->GetLabelID(), expectedLabelWidget->GetLabelID());

    delete m_serializer;
}

void MenuDefinitionDeserializerTests::Deserialize_OneMenuItemManyLabelWidgetsAndManyCommandMappings_ReturnsTrueAndMenuItem()
{
    Project project;
    // Arrange
    LabelDefinition labelDefinition;

    MenuItem* expectedMenuItem = new MenuItem("expected");
    expectedMenuItem->AddUIWidget(new LabelWidget(labelDefinition, project.GetLayoutDefinition()));
    expectedMenuItem->AddUIWidget(new LabelWidget(labelDefinition, project.GetLayoutDefinition()));

    MenuDefinition expected;
    expected.MenuRoot().AddMenuItem(expectedMenuItem);


    // Act
    m_serializer = new MenuDefinitionSerializer(expected);
    MenuDefinitionDeserializer target(m_serializer->Serialize());
    MenuDefinition menuDefinition;
    bool result = target.Deserialize(menuDefinition, project);

    // Assert
    QCOMPARE(result, true);
    QCOMPARE(menuDefinition.MenuRoot().MenuItems().count(), 1);
    QCOMPARE(menuDefinition.MenuRoot().MenuItems().at(0)->UIWidgets().count(), 2);

    delete m_serializer;
}

void MenuDefinitionDeserializerTests::Deserialize_NestedMenuItems_ReturnsTrueAndMenuHierarchy()
{
    // Arrange
    MenuItem* parent = new MenuItem("parent");
    MenuItem* child1 = new MenuItem("child1");
    MenuItem* child2 = new MenuItem("child2");

    parent->AddSubMenuItem(child1);
    parent->AddSubMenuItem(child2);

    MenuDefinition expected;
    expected.MenuRoot().AddMenuItem(parent);
    m_serializer = new MenuDefinitionSerializer(expected);
    QString xml = m_serializer->Serialize();

    Project project;

    // Act
    MenuDefinitionDeserializer target(xml);
    MenuDefinition menuDefinition;
    bool result = target.Deserialize(menuDefinition, project);

    // Assert
    QCOMPARE(result, true);
    QCOMPARE(menuDefinition.MenuRoot().MenuItems().count(), 1);
    QCOMPARE(menuDefinition.MenuRoot().MenuItems().at(0)->SubMenuItems().count(), 2);

    delete m_serializer;
}
