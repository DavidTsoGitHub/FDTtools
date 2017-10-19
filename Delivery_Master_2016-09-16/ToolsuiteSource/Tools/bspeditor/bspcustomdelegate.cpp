#include "bspcustomdelegate.h"

#include "bsprules.h"
#include "bspschemarules.h"
#include "propertiesmodel.h"

#include <QComboBox>

BspCustomDelegate::BspCustomDelegate(QDomDocument schema, QDomDocument spec, QObject *parent)
    : QStyledItemDelegate(parent), m_schema(schema), m_spec(spec)
{
}

QWidget *BspCustomDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    auto model = qobject_cast<const PropertiesModel*>(index.model());
    auto elementName = model->rowNode(index).nodeName();
    auto attribute = model->columnAttribute(index);
    auto howToEdit = BspRules::howToEditAttribute(attribute);

    // Extract values from schema
    if(howToEdit == BspRules::AttributeEditHint::SchemaEnumerationComboBox)
    {
        auto comboBox = new QComboBox(parent);
        auto values = BspSchemaRules::enumerationValues(m_schema, elementName, attribute);
        if(!BspSchemaRules::isAttributeRequired(m_schema, elementName, attribute))
            values.append("");
        comboBox->addItems(values);
        return comboBox;
    }
    // Custom values
    else if(howToEdit == BspRules::AttributeEditHint::CustomComboBox)
    {
        auto comboBox = new QComboBox(parent);
        auto values = BspRules::customComboBoxValues(m_spec, attribute);
        comboBox->addItems(values);
        return comboBox;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}
