#include "bsprules.h"

QMap<QString, QString> BspRules::s_elementDisplayNames =
{
    {"BSPRunnables", "Runnables"},
    {"BSPRunnable", "Runnable"},
    {"BSPCalibrations", "Calibrations"},
    {"BSPCalibration", "Calibration"},
    {"BSPTypes", "Types"},
    {"BSPEnumerations", "Enumerations"},
    {"BSPEnum", "Enum"},
    {"Items", "Items"},
    {"Item", "Item"},
    {"BSPFixedPointFloats", "Fixed-point Floats"},
    {"BSPFixedPointFloat", "Fixed-point Float"},
    {"BSPFunctions", "Functions"},
    {"BSPFunction", "Function"},
    {"Arguments", "Arguments"},
    {"Argument", "Argument"},
    {"IndexToSignalMappings", "Signal Mappings"},
    {"IndexToSignalMapping", "Signal Mapping"},
    {"BSPSegmentDisplay", "Segment Display"},
    {"BSPDisplay", "Display"},
    {"BSPMemory", "Memory"},
    {"CharacterMappings", "Character Mappings"},
    {"CharacterMapping", "Character Mapping"},
    {"SpecialSegments", "Special Segments"},
    {"SpecialSegment", "Special Segment"},
    {"SegmentBitMappings", "Segment Bitmappings"},
    {"SegmentBitMapping", "Segment Bitmapping"},
};

QStringList BspRules::s_editInTable =
{
    "BSPRunnables",
    "BSPCalibrations",
    "BSPEnumerations",
    "Items",
    "BSPFixedPointFloats",
    "BSPFunctions",
    "Arguments",
    "IndexToSignalMappings",
    "CharacterMappings",
    "SpecialSegments",
    "SegmentBitMappings",
};

QStringList BspRules::s_editInSchemaEnumerationComboBox =
{
    "DataType",
    "BaseType",
    "Endianess",
    "GCLSignalType",
    "UseGRAM",
    "RenderingType",
    "RenderingOrientation"
};

QStringList BspRules::s_editInCustomComboBox =
{
    "ArgumentType",
    "Return"
};

QStringList BspRules::s_attributePriorityOrder =
{
    "Name", "FunctionName", "Order", "ArgumentName", "Value",
    "Identifier", "ID", "Init", "Runnable", "CycleTime", "InitialDelay",
    "DataType", "ArrayLength", "DefaultValue", "ArgumentType", "BaseType",
    "Unit", "Endianess", "SignificantBits", "FractionalBits", "Return",
    "GCLSignal", "GCLSignalName", "GCLSignalType", "Requirement",
    "Version", "ToImplement", "MappingType", "Scaling", "Char",
    "Segments", "Byte", "Bit0", "Bit1", "Bit2", "Bit3", "Bit4", "Bit5",
    "Bit6", "Bit7", "Width", "Height", "AvailableMemory", "UseGRAM",
    "NumberOfLayers", "RenderingType", "RenderingOrientation", "Description",
    "InternalMemorySize", "ExternalMemorySize", "ExternalMemoryAddress"
};

QMap<QString, QStringList> BspRules::s_elementsAllowedToBeAppended =
{
    {"BSP", {"BSPSegmentDisplay", "BSPDisplay", "BSPMemory"}},
    {"BSPRunnables", {"BSPRunnable"}},
    {"BSPCalibrations", {"BSPCalibration"}},
    {"BSPEnumerations", {"BSPEnum"}},
    {"BSPEnum", {"Items"}},
    {"Items", {"Item"}},
    {"BSPFixedPointFloats", {"BSPFixedPointFloat"}},
    {"BSPFunctions", {"BSPFunction"}},
    {"BSPFunction", {"Arguments", "IndexToSignalMappings"}},
    {"Arguments", {"Argument"}},
    {"IndexToSignalMappings", {"IndexToSignalMapping"}},
    {"BSPSegmentDisplay", {"CharacterMappings", "SpecialSegments", "SegmentBitMappings"}},
    {"CharacterMappings", {"CharacterMapping"}},
    {"SpecialSegments", {"SpecialSegment"}},
    {"SegmentBitMappings", {"SegmentBitMapping"}}
};

QStringList BspRules::s_elementsAllowedToBeRemoved =
{
    "BSPRunnable",
    "BSPCalibration",
    "BSPEnum",
    "Item",
    "BSPFixedPointFloat",
    "BSPFunction",
    "Arguments",
    "Argument",
    "IndexToSignalMappings",
    "IndexToSignalMapping",
    "CharacterMapping",
    "SpecialSegment",
    "SegmentBitMapping",
    "BSPSegmentDisplay",
    "BSPDisplay"
};

QString BspRules::nodeName(const QString &nodeName)
{
    if(s_elementDisplayNames.find(nodeName) != s_elementDisplayNames.end())
        return s_elementDisplayNames.value(nodeName);

    return "";
}

QString BspRules::nodeName(QDomNode node)
{
    auto name = nodeName(node.nodeName());
    if(!name.isEmpty())
        return name;

    return node.nodeName();
}

BspRules::ElementEditHint BspRules::howToEditElement(QDomNode node)
{
    if(s_editInTable.contains(node.nodeName()))
        return ElementEditHint::ChildrenInTable;

    return ElementEditHint::Default;
}

BspRules::AttributeEditHint BspRules::howToEditAttribute(const QString &attribute)
{
    if(s_editInSchemaEnumerationComboBox.contains(attribute))
        return AttributeEditHint::SchemaEnumerationComboBox;
    else if(s_editInCustomComboBox.contains(attribute))
        return AttributeEditHint::CustomComboBox;

    return AttributeEditHint::Default;
}

int BspRules::attributePriority(QString attribute)
{
    auto index = s_attributePriorityOrder.indexOf(attribute);
    if(index < 0)
        return s_attributePriorityOrder.size();

    return index;
}

QStringList BspRules::elementsAllowedToBeAppended(QDomNode node)
{
    if(node.isNull())
        return {};

    return s_elementsAllowedToBeAppended.value(node.nodeName());
}

bool BspRules::elementAllowedToBeRemoved(QDomNode node)
{
    return s_elementsAllowedToBeRemoved.contains(node.nodeName());
}

QStringList BspRules::customComboBoxValues(QDomDocument spec, const QString &attribute)
{
    QStringList types;

    // Standard types
    types.append({"int8", "uint8", "int8*", "uint8*", "const int8*", "const uint8*"});
    types.append({"int16", "uint16", "int16*", "uint16*", "const int16*", "const uint16*"});
    types.append({"int32", "uint32", "int32*", "uint32*", "const int32*", "const uint32*"});
    types.append({"dateTime_t", "dateTime_t*"});
    types.append({"DM1Records_t", "DM1Records_t*"});

    // Append enums
    auto enums = spec.elementsByTagName("BSPEnum");
    for(int i=0; i<enums.size(); ++i)
    {
        auto name = enums.at(i).toElement().attribute("Name");
        if(!name.isEmpty())
        {
            types.append(name);
            types.append(name + "*");
        }
    }

    // Append fixed-point floats
    auto fpfs = spec.elementsByTagName("BSPFixedPointFloat");
    for(int i=0; i<fpfs.size(); ++i)
    {
        auto name = fpfs.at(i).toElement().attribute("Name");
        if(!name.isEmpty())
        {
            types.append(name);
            types.append(name + "*");
        }
    }

    if(attribute == "Return")
        types.append("void");

    return types;
}

