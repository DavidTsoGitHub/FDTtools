#include "DataTypeComboBox.h"

#include <QComboBox>

BasicDataTypesComboBox::BasicDataTypesComboBox(QWidget* parent) : QComboBox(parent)
{
    setEditable(false);

    addItem("uint8");
    addItem("sint8");
    addItem("uint16");
    addItem("sint16");
    addItem("uint32");
    addItem("sint32");
}

