#include "SinksOverviewModel.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "Project.h"
#include "SignalMappingDelegate.h"

#include <QAbstractTableModel>
#include <QMap>
#include <QObject>

SinksOverviewModel::SinksOverviewModel(Project* project, QObject *parent) : QAbstractTableModel(parent),
    m_project(project)
{
    PopulateModel();
}


void SinksOverviewModel::PopulateModel()
{
    m_allSinks = QMap<QString, GCLSignalSink*>();
    QList<GCLSignalSink*> sinkList;
    m_project->GetAllGCLSinks(sinkList);

    foreach(GCLSignalSink* sink, sinkList)
    {
        m_allSinks.insert(sink->LongName(), sink);
    }
}

int SinksOverviewModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_allSinks.count();
}

int SinksOverviewModel::columnCount(const QModelIndex& /*parent*/) const
{
    return ColumnCount;
}

QVariant SinksOverviewModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int column = index.column();
    GCLSignalSink* sink = GetSinkList().at(row);

    switch (role)
    {
    case Qt::DisplayRole:
    {
        switch(column)
        {
        case SinkColumn:
            return QVariant(sink->LongName());
        case SourceColumn:
        {
            if (sink->GetSignalSource() == NULL) return QVariant(QString("<unmapped>"));

            return QVariant(sink->GetSignalSource()->LongName());
        }
        default:
            return QVariant();
        }
    }
    case SignalMappingDelegate::SinksOverview:
    {
        return QVariant(sink->LongName());
    }
    case Qt::ToolTipRole:
    {
        return QVariant("Double-click to map the signal to a source");
    }
    default:
        return QVariant();
    }
}

QVariant SinksOverviewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();

    switch(role)
    {
        case Qt::DisplayRole:
        {
            switch(section)
            {
                case SinkColumn:
                    return QVariant(QString("Sink"));
                case SourceColumn:
                    return QVariant(QString("Source"));
                default:
                    return QVariant();
            }
        }
        default:
            return QVariant();
    }
}

Qt::ItemFlags SinksOverviewModel::flags(const QModelIndex &index) const
{
    int flags = Qt::ItemIsEnabled;

    switch(index.column())
    {
    case SourceColumn:
    {
        flags |= Qt::ItemIsEditable;
        flags |= SignalMappingDelegate::SinksOverview;
        break;
    }
    default:
        break;
    }

    return static_cast<Qt::ItemFlags>(flags);
}

GCLSignalSink* SinksOverviewModel::FindSink(const QString& longName) const
{
    return m_allSinks.value(longName, NULL);
}

QList<GCLSignalSink *> SinksOverviewModel::GetSinkList() const
{
    return m_allSinks.values();
}

QList<GCLSignalSource*> SinksOverviewModel::GetSourceList() const
{
    return QList<GCLSignalSource*>();
}

GCLSignalSource* SinksOverviewModel::FindSource(const QString& /*longName*/) const
{
    return NULL;
}
