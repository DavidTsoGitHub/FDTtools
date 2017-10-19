#include "SignalMappingDelegate.h"
#include "SignalMappingWidget.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "ISignalMappable.h"
#include "Project.h"
#include <QStyledItemDelegate>
#include <QStylePainter>
#include <QPushButton>
#include <QWidget>

SignalMappingDelegate::SignalMappingDelegate(Project* project, ISignalMappable *signalOverviewModel, QWidget *parent) : QStyledItemDelegate(parent),
    m_project(project),
    m_signalOverviewModel(signalOverviewModel),
    m_sink(NULL)
{
}

void SignalMappingDelegate::mappingUpdated(GCLSignalSink* /*sink*/)
{
    //called when SignalMappingWidget finsihed mapping
}

QWidget* SignalMappingDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.data(UserRoles::SinksOverview).canConvert<QString>())
    {
        QString sinkSignalLongName = index.data(UserRoles::SinksOverview).value<QString>();

        GCLSignalSink* sink = m_signalOverviewModel->FindSink(sinkSignalLongName);

        QString displayedText = QString("<unset>");
        GCLSignalSource* source = NULL;

        if (sink->GetSignalSource() != NULL)
        {
            source = sink->GetSignalSource();
            displayedText = source->LongName();
        }

        SignalMappingWidget* mappingWidget = new SignalMappingWidget(sink, m_project, parent);
        connect(mappingWidget, &SignalMappingWidget::signal_MappingUpdated, this, &SignalMappingDelegate::mappingUpdated);

        return mappingWidget;
    }
    else if (index.data(UserRoles::SourcesOverview).canConvert<QString>())
    {
        QString sourceSignalLongName = index.data(UserRoles::SourcesOverview).value<QString>();
        int sinkIndex = index.column() - 1;

        GCLSignalSource* source = m_signalOverviewModel->FindSource(sourceSignalLongName);

        QString displayedText = QString("<unset>");
        GCLSignalSink* sink = NULL;

        if (source->GetConnectedSinks().count()-1 < sinkIndex)
        {
            QLabel* label = new QLabel(parent);
            label->setText("<no corresponding sink>");
            label->setAutoFillBackground(true);

            return label;
        }

        sink = source->GetConnectedSinks().at(sinkIndex);
        displayedText = sink->LongName();

        SignalMappingWidget* mappingWidget = new SignalMappingWidget(sink, m_project, parent);
        connect(mappingWidget, &SignalMappingWidget::signal_MappingUpdated, this, &SignalMappingDelegate::mappingUpdated);

        return mappingWidget;
    }
    else
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void SignalMappingDelegate::setEditorData(QWidget* /*editor*/, const QModelIndex& /*index*/) const
{
    //the model is driven externally -- do nothing
}

void SignalMappingDelegate::setModelData(QWidget* /*editor*/, QAbstractItemModel* /*model*/, const QModelIndex& /*index*/) const
{
    //the model is driven externally -- do nothing
}
