#ifndef LABELPROXYMODEL_H
#define LABELPROXYMODEL_H

class QSortFilterProxyModel;

class LabelModel;
class QRegExp;

class LabelProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LabelProxyModel(QObject *parent = 0);

signals:

public slots:
    void SetFilter(const QString& filterString);

private:
    QRegExp m_filterRegExp;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
};

#endif // LABELPROXYMODEL_H
