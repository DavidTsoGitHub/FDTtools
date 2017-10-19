#ifndef CHECKBOXSORTPROXYMODEL
#define CHECKBOXSORTPROXYMODEL

class ImageDefinition;
class Project;
template <typename T> class QList;
class QSortFilterProxyModel;

class CheckBoxSortProxyModel : public QSortFilterProxyModel
{
public:
    explicit CheckBoxSortProxyModel(Project* project, QObject* parent = 0);

    // Boolean data editable by checkboxes for this column and filtered to
    // only include images that are included in the provided usedImages list.
    void SetExternalFlashColumn(int column, QList<quint32> usedImages);

    // Boolean data editable by checkboxes for these colums, no special
    // filtering/display
    void SetCheckBoxColumns(QList<int> columns);

private:
    int m_externalFlashColumn;
    QList<int> m_columnsWithCheckBox;
    Project* m_project;
    ImageDefinition& m_imageDefinition;
    QList<quint32> m_usedImages;

    // QAbstractItemModel interface
protected:
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

};

#endif // CHECKBOXSORTPROXYMODEL

