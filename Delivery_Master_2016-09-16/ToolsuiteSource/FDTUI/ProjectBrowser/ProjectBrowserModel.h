#ifndef PROJECTBROWSERMODEL_H
#define PROJECTBROWSERMODEL_H

class Project;
class ProjectObject;

class QAbstractItemModel;
class QModelIndex;
class QObject;

class ProjectBrowserModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ProjectBrowserModel(Project* project, QObject* parent = 0);

    void RemoveFunctionBlock(const QModelIndex& indexToRemove);
    QModelIndex FindIndexForObject(ProjectObject* object, const QModelIndex& parent = QModelIndex());

    void Reset();

signals:
    void signal_modified();

private:
    Project* m_project;
    bool isFilteredNode(ProjectObject* node) const;

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex& parent) const;
    QModelIndex parent(const QModelIndex& child) const;
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
};

#endif // PROJECTBROWSERMODEL_H
