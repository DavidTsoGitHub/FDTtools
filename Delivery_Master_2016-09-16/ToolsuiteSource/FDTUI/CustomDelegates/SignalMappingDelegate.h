#ifndef SIGNALMAPPINGDELEGATE_H
#define SIGNALMAPPINGDELEGATE_H

class GCLSignalSink;
class ISignalMappable;
class Project;
class QStyledItemDelegate;
class QWidget;

class SignalMappingDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SignalMappingDelegate(Project* project, ISignalMappable* signalOverviewModel, QWidget *parent = 0);

    typedef enum
    {
        SinksOverview = Qt::UserRole,
        SourcesOverview
    } UserRoles;

signals:

public slots:

private:
    Project* m_project;
    ISignalMappable* m_signalOverviewModel;
    GCLSignalSink* m_sink;

    void mappingUpdated(GCLSignalSink* sink);

    // QAbstractItemDelegate interface
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif // SIGNALMAPPINGDELEGATE_H
