#include "propertiesmodel.h"

#include "bsprules.h"
#include "bspschemarules.h"

#include <QtAlgorithms>
#include <QSet>

PropertiesModel::PropertiesModel(QDomDocument schema) : m_schema(schema)
{
    connect(this, &QAbstractItemModel::dataChanged, this, &PropertiesModel::modified);
}

void PropertiesModel::addElement(QDomElement element)
{
    if(element.isNull())
        return;

    QSet<QString> uniqueAttributes;
    uniqueAttributes.fromList(m_attributes);
    auto howToEdit = BspRules::howToEditElement(element);
    beginResetModel();

    // Add this element's chldren as rows in the table
    if(howToEdit == BspRules::ElementEditHint::ChildrenInTable)
    {        
        for(int i = 0; i < element.childNodes().size(); ++i)
        {
            QDomElement child = element.childNodes().at(i).toElement();
            if(child.isNull())
                continue;

            // Assemble union of attributes for all elements to be added
            for(int i = 0; i < child.attributes().size(); ++i)
                uniqueAttributes.insert(child.attributes().item(i).nodeName());

            m_elements.append(child);
        }
    }

    // Add this element as the one row in the table
    else if(howToEdit == BspRules::ElementEditHint::Default)
    {
        // Assemble union of attributes for all elements to be added
        for(int i = 0; i < element.attributes().size(); ++i)
            uniqueAttributes.insert(element.attributes().item(i).nodeName());

        m_elements.append(element);
    }

    m_attributes = uniqueAttributes.toList();

    // Sort according to priority
    qSort(m_attributes.begin(), m_attributes.end(),
          [](const QString& a1, const QString& a2)
    {
        return BspRules::attributePriority(a1) <
                BspRules::attributePriority(a2);
    });

    if (element.nodeName().compare("BSPFunctions") == 0)
        sort(0, Qt::AscendingOrder);

    endResetModel();
}

void PropertiesModel::clearNodes()
{
    beginResetModel();
    m_elements.clear();
    m_attributes.clear();
    endResetModel();
}

QModelIndex PropertiesModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column, nullptr);
}

QModelIndex PropertiesModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int PropertiesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_elements.size();
}

int PropertiesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_attributes.size();
}

QVariant PropertiesModel::data(const QModelIndex &index, int role) const
{
    auto node = getNode(index);
    if(role == Qt::DisplayRole || role == Qt::EditRole)
        return node.isNull() ? QVariant() : node.nodeValue();

    return QVariant();
}

Qt::ItemFlags PropertiesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool PropertiesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole || !index.isValid())
        return false;

    auto attrNode = getNode(index);
    auto attrStr = m_attributes.at(index.column());
    auto elementNode = m_elements.at(index.row());

    // No such attribute in element (yet)
    if(attrNode.isNull())
    {
        if(value.toString().isEmpty())
            return false;

        elementNode.setAttribute(attrStr, value.toString());
    }

    // Attribute already exists
    else
    {
        // Remove optional attribute if an empty string is entered
        if(value.toString().isEmpty() && !BspSchemaRules::isAttributeRequired
                (m_schema, elementNode.nodeName(), attrStr))
            elementNode.removeAttribute(attrStr);
        else
            attrNode.setNodeValue(value.toString());
    }

    emit dataChanged(index, index);
    return true;
}

QVariant PropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    return m_attributes.at(section);
}

QDomNode PropertiesModel::rowNode(const QModelIndex &index) const
{
    if(!index.isValid())
        return QDomNode();

    if(index.row() > m_elements.size())
        return QDomNode();

    return m_elements.at(index.row());
}

QString PropertiesModel::columnAttribute(const QModelIndex &index) const
{
    if(!index.isValid())
        return "";

    if(index.column() > m_attributes.size())
        return "";

    return m_attributes.at(index.column());
}

QDomNode PropertiesModel::getNode(const QModelIndex &index) const
{
    if(!index.isValid())
        return QDomNode();

    if(index.column() > m_attributes.size() || index.row() > m_elements.size())
        return QDomNode();

    auto attrStr = m_attributes.at(index.column());
    auto element = m_elements.at(index.row());
    return element.attributeNode(attrStr);
}


void PropertiesModel::sort(int column, Qt::SortOrder order)
{
    if (m_elements.count() > 0)
    {
        if (column == 0 && (m_elements.first().nodeName().compare("BSPFunction") == 0))
        {
            beginResetModel();
            if (order == Qt::AscendingOrder)
            {
                qSort(m_elements.begin(), m_elements.end(), [](const QDomElement& a, const QDomElement& b)
                {
                    int result = QString::compare(a.attributes().namedItem("FunctionName").nodeValue(), b.attributes().namedItem("FunctionName").nodeValue(), Qt::CaseInsensitive);
                    if (result > 0)
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                });
            }
            else
            {
                qSort(m_elements.begin(), m_elements.end(), [](const QDomElement& a, const QDomElement& b)
                {
                    int result = QString::compare(a.attributes().namedItem("FunctionName").nodeValue(), b.attributes().namedItem("FunctionName").nodeValue(), Qt::CaseInsensitive);
                    if (result > 0)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                });
            }
            endResetModel();
        }
    }
}
