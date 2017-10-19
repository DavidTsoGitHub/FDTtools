#include "bsptreefilter.h"

#include "bsprules.h"

QStringList BSPTreeFilter::s_excludedFromTree =
{
    "BSPRunnable",
    "BSPCalibration",
    "Item",
    "BSPFixedPointFloat",
    "Argument",
    "IndexToSignalMapping",
    "CharacterMapping",
    "SpecialSegment",
    "SegmentBitMapping"
};

bool BSPTreeFilter::includeNode(QDomNode node)
{
    if(!node.isElement())
        return false;

    if(s_excludedFromTree.contains(node.nodeName()))
        return false;

    return true;
}

QString BSPTreeFilter::nodeName(QDomNode node)
{
    QString displayName = BspRules::nodeName(node);

    // For certain elements, use a name attribute for the display name
    QDomNode nameAttr;
    if(node.nodeName() == "BSPEnum" || node.nodeName() == "BSPFixedPointFloat")
        nameAttr = node.attributes().namedItem("Name");
    else if(node.nodeName() == "BSPFunction")
        nameAttr = node.attributes().namedItem("FunctionName");
    if(!nameAttr.isNull())
    {
        if(nameAttr.nodeValue().isEmpty())
            displayName = "Unnamed " + BspRules::nodeName(node);
        else
            displayName = nameAttr.nodeValue();
    }

    // Append suffixes with the child cound when content is edited in a table
    if(BspRules::howToEditElement(node) == BspRules::ElementEditHint::ChildrenInTable)
    {
        QString suffix = " [" + QString::number(node.childNodes().size()) + "]";
        displayName.append(suffix);
    }

    return displayName;
}
