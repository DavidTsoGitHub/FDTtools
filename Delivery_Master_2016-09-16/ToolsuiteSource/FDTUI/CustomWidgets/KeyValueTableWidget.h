#ifndef KEYVALUETABLEWIDGET_H
#define KEYVALUETABLEWIDGET_H

template <class Key, class T> class QMap;
class QTableWidget;
class QString;

class KeyValueTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit KeyValueTableWidget(QWidget* parent = 0);
    void SetData(const QMap<QString, QString>& data);
    const QMap<QString, QString> GetData();

signals:
    void signal_DataChanged();

private slots:
    void slot_afterCellChanged(int row, int column);

private:
    void addHeader(int column, QString header);
    int insertEmptyRow();
    void insertDataRow(QString key, QString value);
    bool rowIsEmpty(int row);

    // QWidget interface
protected:
    void keyReleaseEvent(QKeyEvent* event);
};

#endif // KEYVALUETABLEWIDGET_H
