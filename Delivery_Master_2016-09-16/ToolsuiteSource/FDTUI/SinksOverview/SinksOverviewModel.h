#ifndef SINKSOVERVIEWMODEL_H
#define SINKSOVERVIEWMODEL_H

#include "ISignalMappable.h"

class Project;
class GCLSignalSource;

class QAbstractTableModel;
template <class T> class QList;
template <class Key, class T> class QMap;
class QObject;

class SinksOverviewModel : public QAbstractTableModel, public ISignalMappable
{
    Q_OBJECT

public:
    explicit SinksOverviewModel(Project* project, QObject *parent = 0);
    void PopulateModel();

    typedef enum
    {
        SinkColumn,
        SourceColumn,
        ColumnCount //keep this item last
    } SinksOverviewColumns;

private:
    Project* m_project;
    QMap<QString, GCLSignalSink*> m_allSinks;

signals:

public slots:

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    // ISignalMappable interface
public:
    GCLSignalSink* FindSink(const QString& longName) const;
    QList<GCLSignalSink *> GetSinkList() const;
    QList<GCLSignalSource*> GetSourceList() const;
    GCLSignalSource* FindSource(const QString& longName) const;
};

#endif // SINKSOVERVIEWMODEL_H
