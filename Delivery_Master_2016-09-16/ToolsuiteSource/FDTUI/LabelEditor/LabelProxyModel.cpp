#include "LabelProxyModel.h"

#include <QRegExp>
#include <QSortFilterProxyModel>

static const QString ADD_LINE_VERTICAL_HEADER = "+";

LabelProxyModel::LabelProxyModel(QObject* parent) :
    QSortFilterProxyModel(parent),
    m_filterRegExp(NULL)
{
}

void LabelProxyModel::SetFilter(const QString &filterString)
{
    m_filterRegExp = QRegExp(filterString, Qt::CaseInsensitive, QRegExp::Wildcard);
    setFilterRegExp(m_filterRegExp);
}

bool LabelProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    bool filterAcceptance = false;

    for (int i = 0; i < sourceModel()->columnCount(); i++)
    {
        QString columnString = sourceModel()->index(source_row, i, source_parent).data().value<QString>();
        if (m_filterRegExp.indexIn(columnString) != -1)
        {
            filterAcceptance = true;
            break;
        }
    }

    if (filterAcceptance == false)
    {
        //also try to match on row indexes
        filterAcceptance |= (m_filterRegExp.indexIn(QString::number(source_row+1)) != -1);
    }

    return filterAcceptance;
}
