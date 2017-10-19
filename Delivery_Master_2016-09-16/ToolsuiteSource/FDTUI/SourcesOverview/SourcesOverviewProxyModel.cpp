#include "SourcesOverviewProxyModel.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "FunctionBlock.h"
#include "Project.h"
#include "SignalMappingDelegate.h"

#include <QObject>
#include <QSortFilterProxyModel>

SourcesOverviewProxyModel::SourcesOverviewProxyModel(Project* project, ISignalMappable* model, QObject* parent) : QSortFilterProxyModel(parent),
    m_project(project),
    m_overviewModel(model),
    m_filterRegExp(NULL)
{
}

void SourcesOverviewProxyModel::SetFilter(const QString& filterString)
{
    m_filterRegExp = QRegExp(filterString, Qt::CaseInsensitive, QRegExp::Wildcard);
    setFilterRegExp(m_filterRegExp);
}

void SourcesOverviewProxyModel::SetShowCAN(const bool showCAN)
{
    m_showCAN = showCAN;
    invalidate();
}

void SourcesOverviewProxyModel::SetShowHAL(const bool showHAL)
{
    m_showHAL = showHAL;
    invalidate();
}

void SourcesOverviewProxyModel::SetShowFunctionBlocks(const bool showFunctionBlocks)
{
    m_showFunctionBlocks = showFunctionBlocks;
    invalidate();
}

void SourcesOverviewProxyModel::SetShowDisplay(const bool showDisplay)
{
    m_showDisplay = showDisplay;
    invalidate();
}

void SourcesOverviewProxyModel::SetShowWidgets(const bool showWidgets)
{
    m_showWidgets = showWidgets;
    invalidate();
}

void SourcesOverviewProxyModel::SetShowAnimations(const bool showAnimations)
{
    m_showAnimations = showAnimations;
    invalidate();
}

void SourcesOverviewProxyModel::SetShowUnmappedSources(const bool showUnmappedSources)
{
    m_showUnmappedSources = showUnmappedSources;
    invalidate();
}

void SourcesOverviewProxyModel::SetShowMappedSources(const bool showMappedSources)
{
    m_showMappedSources = showMappedSources;
    invalidate();
}

void SourcesOverviewProxyModel::Invalidate()
{
    invalidate();
}

bool SourcesOverviewProxyModel::acceptsConfiguration(const QList<GCLSignalSink *>* sinks, const GCLSignalSource* source) const
{
    return (m_showCAN && acceptsCAN(sinks, source))
        || (m_showHAL && acceptsHAL(sinks, source))
        || (m_showFunctionBlocks && acceptsFunctionBlocks(sinks, source))
        || (m_showDisplay && acceptsDisplay(sinks, source))
        || (m_showWidgets && acceptsWidgets(sinks, source))
        || (m_showAnimations && acceptsAnimations(sinks, source))
        || (m_showUnmappedSources && acceptsUnmappedSources(sinks, source))
        || (m_showMappedSources && acceptsMappedSources(sinks, source));
}

bool SourcesOverviewProxyModel::acceptsCAN(const QList<GCLSignalSink *>* /*sinks*/, const GCLSignalSource* source) const
{
    return (source->ComponentName() == "CAN");
}

bool SourcesOverviewProxyModel::acceptsHAL(const QList<GCLSignalSink *>* /*sinks*/, const GCLSignalSource* source) const
{
    return (source->ComponentName() == "HAL");
}

bool SourcesOverviewProxyModel::acceptsFunctionBlocks(const QList<GCLSignalSink *>* /*sinks*/, const GCLSignalSource* source) const
{
    QStringList fbNames = QStringList();
    foreach (FunctionBlock* fbBase, m_project->GetFunctionBlocks().List())
    {
        fbNames.append(fbBase->GetComponentName());
    }

    return (fbNames.contains(source->ComponentName()));
}

bool SourcesOverviewProxyModel::acceptsDisplay(const QList<GCLSignalSink *>* /*sinks*/, const GCLSignalSource* source) const
{
    return (source->ComponentName().startsWith("DisplayManager", Qt::CaseInsensitive));
}

bool SourcesOverviewProxyModel::acceptsWidgets(const QList<GCLSignalSink *>* /*sinks*/, const GCLSignalSource* source) const
{
    return (source->ComponentName().startsWith("Widgets", Qt::CaseInsensitive));
}

bool SourcesOverviewProxyModel::acceptsAnimations(const QList<GCLSignalSink *>* /*sinks*/, const GCLSignalSource* source) const
{
    return (source->ComponentName().startsWith("Animation", Qt::CaseInsensitive));
}

bool SourcesOverviewProxyModel::acceptsUnmappedSources(const QList<GCLSignalSink *>* sinks, const GCLSignalSource* /*source*/) const
{
    return (sinks->count() == 0);
}

bool SourcesOverviewProxyModel::acceptsMappedSources(const QList<GCLSignalSink *>* sinks, const GCLSignalSource* /*source*/) const
{
    return (sinks->count() > 0);
}

bool SourcesOverviewProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    bool filterAcceptance = false;

    QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);

    if (sourceIndex.data(SignalMappingDelegate::SourcesOverview).canConvert<QString>())
    {
        QString sourceLongName = sourceIndex.data().value<QString>();
        const GCLSignalSource* source = m_overviewModel->FindSource(sourceLongName);

        filterAcceptance |= (m_filterRegExp.indexIn(sourceLongName) != -1);

        QList<GCLSignalSink*> sinks = source->GetConnectedSinks();

        filterAcceptance &= acceptsConfiguration(&sinks, source);
    }

    return filterAcceptance;
}
