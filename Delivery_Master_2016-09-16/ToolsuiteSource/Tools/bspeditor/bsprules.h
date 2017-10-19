#ifndef BSPRULES_H
#define BSPRULES_H

#include <QDomDocument>
#include <QDomNode>
#include <QMap>
#include <QStringList>

// BSP-specific logic
class BspRules
{
public:
    enum class ElementEditHint { Default, ChildrenInTable };
    enum class AttributeEditHint { Default, SchemaEnumerationComboBox, CustomComboBox };

    static QString nodeName(const QString& nodeName);
    static QString nodeName(QDomNode node);
    static ElementEditHint howToEditElement(QDomNode node);
    static AttributeEditHint howToEditAttribute(const QString& attribute);
    static int attributePriority(QString attribute);
    static QStringList elementsAllowedToBeAppended(QDomNode node);
    static bool elementAllowedToBeRemoved(QDomNode node);
    static QStringList customComboBoxValues(QDomDocument spec, const QString& attribute);

private:
    static QMap<QString, QString> s_elementDisplayNames;
    static QStringList s_editInTable;
    static QStringList s_editInSchemaEnumerationComboBox;
    static QStringList s_editInCustomComboBox;
    static QStringList s_attributePriorityOrder;
    static QMap<QString, QStringList> s_elementsAllowedToBeAppended;
    static QStringList s_elementsAllowedToBeRemoved;
};

#endif // BSPRULES_H
