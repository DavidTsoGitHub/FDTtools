#ifndef FONTCHECKBOXPROXYMODEL
#define FONTCHECKBOXPROXYMODEL

class Project;
class QObject;
class QSortFilterProxyModel;

class FontCheckBoxProxyModel : public QSortFilterProxyModel
{
public:
    explicit FontCheckBoxProxyModel(Project* project, QObject* parent = 0);

    void SetExternalFlashColumn(int column);

private:
    int m_externalFlashColumn;
    Project* m_project;

    // QAbstractItemModel interface
protected:
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // FONTCHECKBOXPROXYMODEL

