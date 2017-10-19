#include "UIWidgetsModel.h"

#include "UIWidgetBase.h"

UIWidgetsModel::UIWidgetsModel(QList<UIWidgetBase*>& widgets) :
    m_widgets(widgets)
{
    connect(this, &UIWidgetsModel::rowsAboutToBeInserted, this, &UIWidgetsModel::signal_modified);
    connect(this, &UIWidgetsModel::rowsAboutToBeRemoved, this, &UIWidgetsModel::signal_modified);
    connect(this, &UIWidgetsModel::rowsAboutToBeMoved, this, &UIWidgetsModel::signal_modified);
    connect(this, &UIWidgetsModel::modelAboutToBeReset, this, &UIWidgetsModel::signal_modified);
}

UIWidgetBase* UIWidgetsModel::GetUIWidget(int row)
{
    return m_widgets.value(row);
}

int UIWidgetsModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
    {
        return m_widgets.count();
    }

    return 0;
}

QVariant UIWidgetsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    switch (role)
    {
        case Qt::ToolTipRole:
            return m_widgets.at(index.row())->ShortDescription();
        case Qt::DisplayRole:
            return m_widgets.at(index.row())->GetUIWidgetTypeName();
        case Qt::WhatsThisRole:
            return m_widgets.at(index.row())->LongDescription();
        case Qt::DecorationRole:
        {
            UIWidgetBase* uiWidget = m_widgets.at(index.row());

            const QString iconFileName = ":/ToolIcons/" + uiWidget->GetUIWidgetTypeName() + "Icon.bmp";
            // If the file is not found, the pixmap will become a null pixmap (0x0px)
            return QPixmap(iconFileName);
        }
        default:
            return QVariant();
    }
}

QVariant UIWidgetsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);

    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    return QString("Widgets");
}
