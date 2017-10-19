#ifndef SOURCESOVERVIEWPROXYMODEL_H
#define SOURCESOVERVIEWPROXYMODEL_H

#include "ISignalMappable.h"

class QSortFilterProxyModel;
class QObject;

class Project;

class SourcesOverviewProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SourcesOverviewProxyModel(Project* project, ISignalMappable* model, QObject* parent = 0);

signals:

public slots:
    void SetFilter(const QString& filterString);

    void SetShowCAN(const bool showCAN);
    void SetShowHAL(const bool showHAL);
    void SetShowFunctionBlocks(const bool showFunctionBlocks);
    void SetShowDisplay(const bool showDisplay);
    void SetShowWidgets(const bool showWidgets);
    void SetShowAnimations(const bool showAnimations);
    void SetShowUnmappedSources(const bool showUnmappedSources);
    void SetShowMappedSources(const bool showMappedSources);

    void Invalidate();

private:
    Project* m_project;
    ISignalMappable* m_overviewModel;
    QRegExp m_filterRegExp;

    bool m_showCAN = true;
    bool m_showHAL = true;
    bool m_showFunctionBlocks = true;
    bool m_showDisplay = true;
    bool m_showWidgets = true;
    bool m_showAnimations = true;
    bool m_showUnmappedSources = true;
    bool m_showMappedSources = true;

    bool acceptsConfiguration(const QList<GCLSignalSink*>* sinks, const GCLSignalSource* source) const;

    bool acceptsCAN(const QList<GCLSignalSink*>* sinks, const GCLSignalSource* source) const;
    bool acceptsHAL(const QList<GCLSignalSink*>* sinks, const GCLSignalSource* source) const;
    bool acceptsFunctionBlocks(const QList<GCLSignalSink*>* sinks, const GCLSignalSource* source) const;
    bool acceptsDisplay(const QList<GCLSignalSink*>* sinks, const GCLSignalSource* source) const;
    bool acceptsWidgets(const QList<GCLSignalSink*>* sinks, const GCLSignalSource* source) const;
    bool acceptsAnimations(const QList<GCLSignalSink*>* sinks, const GCLSignalSource* source) const;
    bool acceptsUnmappedSources(const QList<GCLSignalSink*>* sinks, const GCLSignalSource* source) const;
    bool acceptsMappedSources(const QList<GCLSignalSink*>* sinks, const GCLSignalSource* source) const;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
};

#endif // SOURCESOVERVIEWPROXYMODEL_H
