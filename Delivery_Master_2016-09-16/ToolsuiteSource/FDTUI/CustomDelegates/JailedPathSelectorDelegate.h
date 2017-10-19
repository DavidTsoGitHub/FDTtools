#ifndef JAILEDPATHSELECTORDELEGATE_H
#define JAILEDPATHSELECTORDELEGATE_H

class QStyledItemDelegate;
class QDir;

class JailedPathSelectorDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit JailedPathSelectorDelegate(const QString& rootPath, QObject* parent = 0);
    ~JailedPathSelectorDelegate();

private:
    QDir m_rootPath;

    // QAbstractItemDelegate interface
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    // QStyledItemDelegate interface
public:
    QString displayText(const QVariant& value, const QLocale& locale) const;
};

#endif // JAILEDPATHSELECTORDELEGATE_H
