#include "treenavigatoritem.h"

TreeNavigatorItem::TreeNavigatorItem(QDomNode node, TreeNavigatorItem *parent) :
    m_node(node),
    m_parent(parent)
{
    if(m_parent != nullptr)
        m_parent->appendChild(this);
}

TreeNavigatorItem::~TreeNavigatorItem()
{
    qDeleteAll(m_children);
}

void TreeNavigatorItem::appendChild(TreeNavigatorItem *child)
{
    m_children.append(child);
}

void TreeNavigatorItem::insertChild(int row, TreeNavigatorItem *child)
{
    m_children.insert(row, child);
}

QDomNode TreeNavigatorItem::node() const
{
    return m_node;
}

void TreeNavigatorItem::removeChild(int row) {
    if(row <0 || row >= m_children.size())
        return;

    delete m_children.takeAt(row);
}

void TreeNavigatorItem::removeChildren() {
    while(childCount() > 0)
        removeChild(0);
}

TreeNavigatorItem *TreeNavigatorItem::child(int row) const {
    return m_children.value(row);
}

int TreeNavigatorItem::childCount() const {
    return m_children.count();
}

TreeNavigatorItem *TreeNavigatorItem::parent() const {
    return m_parent;
}

void TreeNavigatorItem::sortFunctionChildren() {
    qSort(m_children.begin(), m_children.end(), [](const TreeNavigatorItem* a1, const TreeNavigatorItem* a2)
    {
        int result = QString::compare(a1->node().attributes().namedItem("FunctionName").nodeValue(), a2->node().attributes().namedItem("FunctionName").nodeValue(), Qt::CaseInsensitive);
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

int TreeNavigatorItem::row() const {
    if (m_parent)
        return m_parent->m_children.indexOf(const_cast<TreeNavigatorItem*>(this));

    return 0;
}

int TreeNavigatorItem::childRow(TreeNavigatorItem *item) const {
    return m_children.indexOf(item);
}

