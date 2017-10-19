#ifndef SINKSOVERVIEWPROXYMODEL_H
#define SINKSOVERVIEWPROXYMODEL_H

#include "SinksOverviewModel.h"

class QObject;
class QRegExp;
class QSortFilterProxyModel;

class SinksOverviewProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SinksOverviewProxyModel(Project* project, SinksOverviewModel* model, QObject *parent = 0);

signals:

public slots:
    void SetFilter(const QString& filterString);

    void SetShowCAN(const bool showCAN);
    void SetShowHAL(const bool showHAL);
    void SetShowFunctionBlocks(const bool showFunctionBlocks);
    void SetShowDisplay(const bool showDisplay);
    void SetShowWidgets(const bool showWidgets);
    void SetShowUnmappedSinks(const bool showUnmappedSinks);
    void SetShowMappedSinks(const bool showMappedSinks);
    void SetSearchSources(const bool searchSources);

    void Invalidate();

private:
    Project* m_project;
    SinksOverviewModel* m_sinksOverviewModel;
    QRegExp m_filterRegExp;

    bool m_showCAN = true;
    bool m_showHAL = true;
    bool m_showFunctionBlocks = true;
    bool m_showDisplay = true;
    bool m_showWidgets = true;
    bool m_showUnmappedSinks = true;
    bool m_showMappedSinks = true;
    bool m_searchSources = false;

    bool acceptsConfiguration(const GCLSignalSink* sink, const GCLSignalSource* source) const;

    bool acceptsCAN(const GCLSignalSink *sink, const GCLSignalSource *source) const;
    bool acceptsHAL(const GCLSignalSink *sink, const GCLSignalSource *source) const;
    bool acceptsFunctionBlocks(const GCLSignalSink *sink, const GCLSignalSource *source) const;
    bool acceptsDisplay(const GCLSignalSink *sink, const GCLSignalSource *source) const;
    bool acceptsWidgets(const GCLSignalSink *sink, const GCLSignalSource *source) const;
    bool acceptsUnmappedSinks(const GCLSignalSink *sink, const GCLSignalSource *source) const;
    bool acceptsMappedSinks(const GCLSignalSink *sink, const GCLSignalSource *source) const;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // SINKSOVERVIEWPROXYMODEL_H

