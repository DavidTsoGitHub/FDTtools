#include "SinksOverviewProxyModel.h"

#include "FunctionBlock.h"
#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "Project.h"

#include <QObject>
#include <QRegExp>
#include <QSortFilterProxyModel>

SinksOverviewProxyModel::SinksOverviewProxyModel(Project *project, SinksOverviewModel* model, QObject *parent) :
    QSortFilterProxyModel(parent),
    m_project(project),
    m_sinksOverviewModel(model),
    m_filterRegExp(NULL)
{
}

void SinksOverviewProxyModel::SetFilter(const QString &filterString)
{
    m_filterRegExp = QRegExp(filterString, Qt::CaseInsensitive, QRegExp::Wildcard);
    setFilterRegExp(m_filterRegExp);
}

void SinksOverviewProxyModel::SetShowCAN(const bool showCAN)
{
    m_showCAN = showCAN;
    invalidate();
}

void SinksOverviewProxyModel::SetShowHAL(const bool showHAL)
{
    m_showHAL = showHAL;
    invalidate();
}

void SinksOverviewProxyModel::SetShowFunctionBlocks(const bool showFunctionBlocks)
{
    m_showFunctionBlocks = showFunctionBlocks;
    invalidate();
}

void SinksOverviewProxyModel::SetShowWidgets(const bool showWidgets)
{
    m_showWidgets = showWidgets;
    invalidate();
}

void SinksOverviewProxyModel::SetShowDisplay(const bool showDisplay)
{
    m_showDisplay = showDisplay;
    invalidate();
}

void SinksOverviewProxyModel::SetShowUnmappedSinks(const bool showUnmappedSinks)
{
    m_showUnmappedSinks = showUnmappedSinks;
    invalidate();
}

void SinksOverviewProxyModel::SetShowMappedSinks(const bool showMappedSinks)
{
    m_showMappedSinks = showMappedSinks;
    invalidate();
}

void SinksOverviewProxyModel::SetSearchSources(const bool searchSources)
{
    m_searchSources = searchSources;
    invalidate();
}

void SinksOverviewProxyModel::Invalidate()
{
    invalidate();
}

bool SinksOverviewProxyModel::acceptsCAN(const GCLSignalSink* sink, const GCLSignalSource* source) const
{
    if (sink->ComponentName().compare("CAN") == 0)
        return true;

    if (m_searchSources && source != NULL && source->ComponentName().compare("CAN") == 0)
        return true;

    return false;
}

bool SinksOverviewProxyModel::acceptsHAL(const GCLSignalSink *sink, const GCLSignalSource *source) const
{
    if (sink->ComponentName().compare("HAL") == 0)
        return true;

    if (m_searchSources && source != NULL && source->ComponentName().compare("HAL") == 0)
        return true;

    return false;
}

bool SinksOverviewProxyModel::acceptsFunctionBlocks(const GCLSignalSink *sink, const GCLSignalSource *source) const
{
    QStringList fbNames = QStringList();
    foreach (FunctionBlock* fbBase, m_project->GetFunctionBlocks().List())
    {
        fbNames.append(fbBase->GetComponentName());
    }

    if (fbNames.contains(sink->ComponentName()))
        return true;

    if (m_searchSources && source != NULL && fbNames.contains(source->ComponentName()))
        return true;

    return false;
}

bool SinksOverviewProxyModel::acceptsDisplay(const GCLSignalSink *sink, const GCLSignalSource *source) const
{
    if (sink->ComponentName().compare("DisplayManager") == 0)
        return true;

    if (m_searchSources && source != NULL && source->ComponentName().compare("DisplayManager") == 0)
        return true;

    return false;
}

bool SinksOverviewProxyModel::acceptsWidgets(const GCLSignalSink *sink, const GCLSignalSource *source) const
{
    if (sink->ComponentName().startsWith("Widgets"))
        return true;

    if (m_searchSources && source != NULL && source->ComponentName().startsWith("Widgets"))
        return true;

    return false;
}

bool SinksOverviewProxyModel::acceptsUnmappedSinks(const GCLSignalSink* /*sink*/, const GCLSignalSource* source) const
{
    return (source == NULL);
}

bool SinksOverviewProxyModel::acceptsMappedSinks(const GCLSignalSink* /*sink*/, const GCLSignalSource* source) const
{
    return (source != NULL);
}

bool SinksOverviewProxyModel::acceptsConfiguration(const GCLSignalSink* sink, const GCLSignalSource* source) const
{
    return (m_showCAN && acceptsCAN(sink, source))
        || (m_showHAL && acceptsHAL(sink, source))
        || (m_showFunctionBlocks && acceptsFunctionBlocks(sink, source))
        || (m_showDisplay && acceptsDisplay(sink, source))
        || (m_showWidgets && acceptsWidgets(sink, source))
        || (m_showUnmappedSinks && acceptsUnmappedSinks(sink, source))
        || (m_showMappedSinks && acceptsMappedSinks(sink, source));
}

bool SinksOverviewProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    bool filterAcceptance = false;

    QModelIndex sinkIndex = sourceModel()->index(source_row, SinksOverviewModel::SinkColumn, source_parent);

    if (sinkIndex.data().canConvert<QString>())
    {
        QString sinkLongName = sinkIndex.data().value<QString>();

        const GCLSignalSink* sink = m_sinksOverviewModel->FindSink(sinkLongName);
        filterAcceptance |= (m_filterRegExp.indexIn(sinkLongName) != -1);
        const GCLSignalSource* source = NULL;

        if (sink->GetSignalSource() != NULL)
        {
            //search in sources as well
            source = sink->GetSignalSource();
            filterAcceptance |= (m_filterRegExp.indexIn(source->LongName()) != -1);
        }

        filterAcceptance &= acceptsConfiguration(sink, source);
    }

    return filterAcceptance;
}
