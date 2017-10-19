#include "JailedPathSelectorDelegate.h"

#include "JailedDirectorySelector.h"
#include <QDir>
#include <QStyledItemDelegate>

JailedPathSelectorDelegate::JailedPathSelectorDelegate(const QString& rootPath, QObject* parent) : QStyledItemDelegate(parent),
    m_rootPath(rootPath)
{
}

JailedPathSelectorDelegate::~JailedPathSelectorDelegate()
{
}

QWidget* JailedPathSelectorDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    JailedDirectorySelector* editor = new JailedDirectorySelector(m_rootPath.absolutePath(), parent);
    return editor;
}

void JailedPathSelectorDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    JailedDirectorySelector* pathSelector = static_cast<JailedDirectorySelector*>(editor);
    QString currentPath = m_rootPath.absoluteFilePath(index.data().toString());
    pathSelector->SetSelectedDirectory(currentPath);
}

void JailedPathSelectorDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    qDebug(__FUNCTION__);
    JailedDirectorySelector* pathSelector = static_cast<JailedDirectorySelector*>(editor);

    if (pathSelector->result() == JailedDirectorySelector::Accepted)
    {
        QString newPath = pathSelector->GetSelectedDirectoryAsRelative();
        qDebug("accepted! %s", qUtf8Printable(newPath));

        model->setData(index, newPath);
    }
}

QString JailedPathSelectorDelegate::displayText(const QVariant& value, const QLocale& /*locale*/) const
{
    if (value.toString().isEmpty()) return QString();

    return m_rootPath.absoluteFilePath(value.toString());
}


void JailedPathSelectorDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    QPoint coords = qobject_cast<QWidget*>(editor->parent())->mapToGlobal(QPoint(0,0));
    editor->move(coords);
}
