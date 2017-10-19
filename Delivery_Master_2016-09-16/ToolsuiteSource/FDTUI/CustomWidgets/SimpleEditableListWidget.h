#ifndef SIMPLEEDITABLELISTWIDGET_H
#define SIMPLEEDITABLELISTWIDGET_H

class QAbstractListModel;
class QWidget;

namespace Ui {
class simpleEditableListWidget;
}

class SimpleEditableListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleEditableListWidget(QWidget *parent = 0);
    ~SimpleEditableListWidget();

    void setModel(QAbstractListModel *model);
    QAbstractListModel *model() const;
    QModelIndex selectedIndex() const;

public slots:
    void setAddEnabled(bool enabled);
    void setRemoveEnabled(bool enabled);
    void setCurrentIndex(const QModelIndex &index);

signals:
    void signal_listViewClicked();
    void signal_selectionChanged();
    void signal_AddClicked();
    void signal_RemoveClicked();

private:
    Ui::simpleEditableListWidget *ui;
};

#endif // SIMPLEEDITABLELISTWIDGET_H
