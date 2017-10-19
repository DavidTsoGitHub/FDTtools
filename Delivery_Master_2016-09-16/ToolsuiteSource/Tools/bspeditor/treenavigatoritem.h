#ifndef TREENAVIGATORITEM_H
#define TREENAVIGATORITEM_H

#include <QDomNode>
#include <QList>

class TreeNavigatorItem
{
public:
    TreeNavigatorItem(QDomNode node, TreeNavigatorItem *parent = 0);
    ~TreeNavigatorItem();

    void appendChild(TreeNavigatorItem *child);
    void insertChild(int row, TreeNavigatorItem *child);
    void removeChild(int row);
    void removeChildren();
    TreeNavigatorItem *child(int row) const;
    int childCount() const;
    TreeNavigatorItem *parent() const;
    void sortFunctionChildren();
    int row() const;
    int childRow(TreeNavigatorItem *child) const;
    QDomNode node() const;

private:
    QDomNode m_node;

    TreeNavigatorItem *m_parent;
    QList<TreeNavigatorItem*> m_children;

};

#endif // TREENAVIGATORITEM_H
