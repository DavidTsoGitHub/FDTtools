#ifndef CHECKMESSAGEMODEL_H
#define CHECKMESSAGEMODEL_H

#include "CheckMessagesDefinition.h"
#include "LabelDefinition.h"
#include "ImageDefinition.h"
#include "IconDefinition.h"
#include "LayoutDefinition.h"

#include <QAbstractTableModel>

class CheckMessageModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CheckMessageModel(CheckMessageDefinition& definition, LabelDefinition &labelDefinition, ImageDefinition &imageDefinition, IconDefinition &iconDefinition, LayoutDefinition& layoutDefinition, QObject* parent = 0);

    typedef enum
    {
        NameColumn = 0,
        CheckMessageColumnCount
    } CheckMessageColumns;

    CheckMessageDefinition& GetCheckMessageDefinition(void);
    void RemoveWidgetSink(GCLSignalSink* sink);

signals:
    void signal_modified();
    void signal_checkMessageAdded(const QModelIndex& index);

private:
    CheckMessageDefinition& m_checkMessageDefinition;
    LabelDefinition& m_labelDefinition;
    ImageDefinition& m_imageDefinition;
    IconDefinition& m_iconDefinition;
    LayoutDefinition& m_layoutDefinition;


    QString getItemType(quint32 item);
    void reCalcPriority(void);
    QList<QString> getDynamicWidgetNames(void) const;

    // QAbstractItemModel interface
public:
    //QModelIndex index(int row, int column, const QModelIndex& parent) const;
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool removeRows(int row, int count, const QModelIndex& parent);
    Qt::ItemFlags flags(const QModelIndex& index) const;

public slots:
    void slot_moveSelectedItemUp(const QModelIndex& index);
    void slot_moveSelectedItemDown(const QModelIndex& index);
    void slot_layoutModelChanged(void);
};

#endif // CHECKMESSAGEMODEL_H
