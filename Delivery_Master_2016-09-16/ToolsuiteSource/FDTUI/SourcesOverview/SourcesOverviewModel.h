#ifndef SOURCESOVERVIEWMODEL_H
#define SOURCESOVERVIEWMODEL_H

#include "ISignalMappable.h"
class Project;

class QAbstractTableModel;
template <class Key, class T> class QMap;
class QObject;

class SourcesOverviewModel : public QAbstractTableModel, public ISignalMappable
{
    Q_OBJECT

public:
    explicit SourcesOverviewModel(Project* project, QObject *parent = 0);
    void PopulateModel();

    int GetMaxNbrOfSources() const;

    typedef enum
    {
        SourceColumn,
        FirstSinkColumn
    } SourcesOverviewColumns;

signals:

public slots:

private:
    Project* m_project;
    QMap<QString, GCLSignalSource*> m_allSources;
    int m_maxNbrOfSources = 0;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;

    // ISignalMappable interface
public:
    GCLSignalSink* FindSink(const QString& longName) const;
    QList<GCLSignalSink*> GetSinkList() const;
    QList<GCLSignalSource*> GetSourceList() const;
    GCLSignalSource* FindSource(const QString &longName) const;
};

#endif // SOURCESOVERVIEWMODEL_H
