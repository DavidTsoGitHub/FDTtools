#ifndef TREENAVIGATORMODEL_H
#define TREENAVIGATORMODEL_H

#include <QAbstractItemModel>
#include <QDomNode>
#include <QModelIndexList>

#include <memory>

#include "treenavigatorfilter.h"

class TreeNavigatorItem;

class TreeNavigatorModel : public QAbstractItemModel
{
public:
    explicit TreeNavigatorModel(std::unique_ptr<TreeNavigatorFilter> filter);
    ~TreeNavigatorModel();

    void populate(QDomNode root);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QDomNode node(const QModelIndex &index);
    QModelIndexList getPersistentModelIndices();

private:
    TreeNavigatorItem* getItem(const QModelIndex &index) const;
    void createChildren(TreeNavigatorItem *root, QDomNode node);

    std::unique_ptr<TreeNavigatorFilter> m_filter;
    TreeNavigatorItem* m_rootItem;

};

#endif // TREENAVIGATORMODEL_H
