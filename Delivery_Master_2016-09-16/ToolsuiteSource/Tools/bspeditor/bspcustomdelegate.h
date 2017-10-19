#ifndef BSPCUSTOMDELEGATE_H
#define BSPCUSTOMDELEGATE_H

#include <QDomDocument>
#include <QStyledItemDelegate>

class BspCustomDelegate : public QStyledItemDelegate
{
public:
    explicit BspCustomDelegate(QDomDocument schema, QDomDocument spec, QObject *parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QDomDocument m_schema;
    QDomDocument m_spec;
};

#endif // BSPCUSTOMDELEGATE_H
