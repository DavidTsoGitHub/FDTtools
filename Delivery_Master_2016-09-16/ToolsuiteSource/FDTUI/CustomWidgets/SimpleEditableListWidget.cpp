#include "SimpleEditableListWidget.h"
#include "ui_SimpleEditableListWidget.h"

#include <QAbstractListModel>
#include <QWidget>

SimpleEditableListWidget::SimpleEditableListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::simpleEditableListWidget)
{
    ui->setupUi(this);
    ui->simpleEditableListWidgetListView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->simpleEditableListWidgetButtonAdd, &QAbstractButton::clicked, this, &SimpleEditableListWidget::signal_AddClicked);
    connect(ui->simpleEditableListWidgetButtonRemove, &QAbstractButton::clicked, this, &SimpleEditableListWidget::signal_RemoveClicked);
}

SimpleEditableListWidget::~SimpleEditableListWidget()
{
    delete ui;
}

void SimpleEditableListWidget::setModel(QAbstractListModel *model)
{
    ui->simpleEditableListWidgetListView->setModel(model);
    connect(ui->simpleEditableListWidgetListView, &QListView::clicked, this, &SimpleEditableListWidget::signal_listViewClicked);

    auto* selectionModel = ui->simpleEditableListWidgetListView->selectionModel();
    if(selectionModel != NULL)
        connect(selectionModel, &QItemSelectionModel::selectionChanged,
                this, &SimpleEditableListWidget::signal_selectionChanged);
}

QAbstractListModel *SimpleEditableListWidget::model() const
{
    return qobject_cast<QAbstractListModel*>(ui->simpleEditableListWidgetListView->model());
}

QModelIndex SimpleEditableListWidget::selectedIndex() const
{
    if(ui->simpleEditableListWidgetListView->selectionModel() == NULL)
        return QModelIndex();

    auto rows = ui->simpleEditableListWidgetListView->selectionModel()->selectedRows();
    return rows.size() > 0 ? rows.first() : QModelIndex();
}

void SimpleEditableListWidget::setAddEnabled(bool enabled)
{
    ui->simpleEditableListWidgetButtonAdd->setEnabled(enabled);
}

void SimpleEditableListWidget::setRemoveEnabled(bool enabled)
{
    ui->simpleEditableListWidgetButtonRemove->setEnabled(enabled);
}

void SimpleEditableListWidget::setCurrentIndex(const QModelIndex &index)
{
    ui->simpleEditableListWidgetListView->setCurrentIndex(index);

}
