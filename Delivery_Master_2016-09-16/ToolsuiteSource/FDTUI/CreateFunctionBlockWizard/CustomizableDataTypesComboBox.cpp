#include "CustomizableDataTypesComboBox.h"

#include <QComboBox>

CustomizableDataTypesComboBox::CustomizableDataTypesComboBox(QWidget* parent) : QComboBox(parent)
{
    setEditable(true);

    addItem("uint8");
    addItem("sint8");
    addItem("uint16");
    addItem("sint16");
    addItem("uint32");
    addItem("sint32");
    addItem("boolean");
    addItem("DynamicWidgetValue");
    addItem("LabelMappings*");
    addItem("dateTime_t");
    addItem("DM1Records_t*");
    addItem("void");
    addItem("PowerMode");
}
