#include "SignalMappingDelegate.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "Project.h"
#include "SourcesOverviewModel.h"

#include <QAbstractTableModel>
#include <QMap>
#include <QObject>

SourcesOverviewModel::SourcesOverviewModel(Project *project, QObject *parent) : QAbstractTableModel(parent),
    m_project(project)
{
    PopulateModel();
}

void SourcesOverviewModel::PopulateModel()
{
    m_allSources = QMap<QString, GCLSignalSource*>();
    QList<GCLSignalSource*> sourceList;
    m_project->GetAllGCLSources(sourceList);

    foreach (GCLSignalSource* source, sourceList)
    {
        m_allSources.insert(source->LongName(), source);
        m_maxNbrOfSources = qMax(source->GetConnectedSinks().count(), m_maxNbrOfSources);
    }
}

int SourcesOverviewModel::GetMaxNbrOfSources() const
{
    return m_maxNbrOfSources;
}

int SourcesOverviewModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_allSources.count();
}

int SourcesOverviewModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 1 + m_maxNbrOfSources;
}

QVariant SourcesOverviewModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    int column = index.column();
    GCLSignalSource* source = GetSourceList().at(row);

    switch(role)
    {
        case Qt::DisplayRole:
        {
            if (column == SourceColumn)
            {
                return QVariant(source->LongName());
            }
            else if (column >= FirstSinkColumn)
            {
                int listIndexToShow = column-1;

                if (source->GetConnectedSinks().count() > listIndexToShow)
                {
                    return QVariant(source->GetConnectedSinks().at(listIndexToShow)->LongName());
                }
                else
                {
                    return QVariant("<unmapped>");
                }
            }
        }
        case SignalMappingDelegate::SourcesOverview:
        {
            return QVariant(source->LongName());
        }
        case Qt::ToolTipRole:
        {
            return QVariant("Double-click to map the signal to a source");
        }
        default:
            break;
    }

    return QVariant();

}

QVariant SourcesOverviewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();

    switch(role)
    {
        case Qt::DisplayRole:
        {
            if (section == SourceColumn)
            {
                return QVariant("Source");
            }
            else
            {
                return QVariant(QString("Sink #%1").arg(section));
            }
        }
    }

    return QVariant();
}

Qt::ItemFlags SourcesOverviewModel::flags(const QModelIndex& index) const
{
    int flags = Qt::ItemIsEnabled;

    if (index.column() >= FirstSinkColumn)
    {
        flags |= SignalMappingDelegate::SourcesOverview;
        flags |= Qt::ItemIsEditable;
    }

    return static_cast<Qt::ItemFlags>(flags);
}

GCLSignalSink* SourcesOverviewModel::FindSink(const QString& /*longName*/) const
{
    return NULL;
}

QList<GCLSignalSink*> SourcesOverviewModel::GetSinkList() const
{
    return QList<GCLSignalSink*>();
}

QList<GCLSignalSource*> SourcesOverviewModel::GetSourceList() const
{
    return m_allSources.values();
}

GCLSignalSource* SourcesOverviewModel::FindSource(const QString& longName) const
{
    return m_allSources.value(longName, NULL);
}
