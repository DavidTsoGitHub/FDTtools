#include "treenavigatormodel.h"

#include "treenavigatoritem.h"

TreeNavigatorModel::TreeNavigatorModel(std::unique_ptr<TreeNavigatorFilter> filter) : m_filter(std::move(filter))
{
    m_rootItem = new TreeNavigatorItem(QDomNode(), 0);
}

void TreeNavigatorModel::populate(QDomNode root)
{
    delete m_rootItem;

    beginResetModel();
    m_rootItem = new TreeNavigatorItem(root, 0);
    createChildren(m_rootItem, root);
    endResetModel();
}

TreeNavigatorModel::~TreeNavigatorModel()
{
    delete m_rootItem;
}

QModelIndex TreeNavigatorModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNavigatorItem *parentItem = getItem(parent);
    TreeNavigatorItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeNavigatorModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeNavigatorItem *childItem = getItem(index);
    TreeNavigatorItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeNavigatorModel::rowCount(const QModelIndex &parent) const
{
    return getItem(parent)->childCount();
}

int TreeNavigatorModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant TreeNavigatorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeNavigatorItem* modelItem = getItem(index);
    if(modelItem->node().isNull())
        return QVariant();

    // Name
    if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.column() == 0)
    {
        return m_filter->nodeName(modelItem->node());
    }

    return QVariant();
}

QDomNode TreeNavigatorModel::node(const QModelIndex &index)
{
    return getItem(index)->node();
}

QModelIndexList TreeNavigatorModel::getPersistentModelIndices()
{
    return persistentIndexList();
}

TreeNavigatorItem *TreeNavigatorModel::getItem(const QModelIndex &index) const
{
    if(!index.isValid())
        return m_rootItem;
    else
        return static_cast<TreeNavigatorItem*>(index.internalPointer());
}

void TreeNavigatorModel::createChildren(TreeNavigatorItem *root, QDomNode node)
{
    for(int i = 0; i < node.childNodes().size(); ++i)
    {
        QDomNode child = node.childNodes().at(i);
        if(!m_filter->includeNode(child))
            continue;

        TreeNavigatorItem *item = new TreeNavigatorItem(child, root);
        createChildren(item, child);
        if (root->node().nodeName().compare("BSPFunctions") == 0)
        {
            root->sortFunctionChildren();
        }
    }
}
