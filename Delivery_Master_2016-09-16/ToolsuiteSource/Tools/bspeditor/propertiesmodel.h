#ifndef PROPERTIESMODEL_H
#define PROPERTIESMODEL_H

#include <QAbstractItemModel>
#include <QDomAttr>
#include <QDomDocument>
#include <QDomElement>
#include <QList>
#include <QStringList>

class PropertiesModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit PropertiesModel(QDomDocument schema);
    void addElement(QDomElement node);
    void clearNodes();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QDomNode rowNode(const QModelIndex &index) const;
    QString columnAttribute(const QModelIndex& index) const;

signals:
    void modified();

private:
    QDomDocument m_schema;
    QList<QDomElement> m_elements;
    QStringList m_attributes;

    QDomNode getNode(const QModelIndex& index) const;


    // QAbstractItemModel interface
public:
    void sort(int column, Qt::SortOrder order);
};

#endif // PROPERTIESMODEL_H
