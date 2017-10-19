#include "FrameSelectionWidget.h"
#include "ui_FrameSelectionWidget.h"

#include "Image.h"

#include <QList>
#include <QWidget>

FrameSelectionWidget::FrameSelectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameSelectionWidget)
{
    ui->setupUi(this);

    ui->srcList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->dstList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->dstList->setDragEnabled(true);
    ui->dstList->viewport()->setAcceptDrops(true);
    ui->dstList->setDropIndicatorShown(true);
    ui->dstList->setDragDropMode(QAbstractItemView::InternalMove);

    connect(ui->addButton, &QAbstractButton::clicked, this, &FrameSelectionWidget::addSelectedFrames);
    connect(ui->removeButton, &QAbstractButton::clicked, this, &FrameSelectionWidget::removeSelectedFrames);
    connect(ui->upButton, &QAbstractButton::clicked, this, &FrameSelectionWidget::moveSelectedFrameUp);
    connect(ui->downButton, &QAbstractButton::clicked, this, &FrameSelectionWidget::moveSelectedFrameDown);
    connect(ui->clearButton, &QAbstractButton::clicked, this, &FrameSelectionWidget::removeAllSelectedFrames);
    connect(ui->dstList->selectionModel()->model(), &QAbstractItemModel::rowsMoved, this, &FrameSelectionWidget::signal_modified);
}

FrameSelectionWidget::~FrameSelectionWidget()
{
    delete ui;
}

QList<quint32> FrameSelectionWidget::GetFrames()
{
    QList<quint32> frames;
    for(int row = 0; row < ui->dstList->count(); ++row)
        frames.append(ui->dstList->item(row)->data(Qt::UserRole).toUInt());

    return frames;
}

void FrameSelectionWidget::PopulateFrames(ImageDefinition &def, const AnimationFrameByFrame &anim)
{
    ui->dstList->clear();

    foreach(quint32 id, anim.ImageIds())
    {
        Image* image = def.GetImageByID(id);
        if(image == NULL)
            continue;

        QListWidgetItem *item = new QListWidgetItem(image->GetName());
        item->setData(Qt::UserRole, QVariant(image->GetID()));
        ui->dstList->insertItem(ui->dstList->count(), item);
    }
}

void FrameSelectionWidget::PopulateAvailableImages(const ImageDefinition &def)
{
    ui->srcList->clear();
    foreach(auto* image, def.Images())
    {
        if(!image->GetUseForAnimations())
            continue;

        QListWidgetItem *item = new QListWidgetItem(image->GetName());
        item->setData(Qt::UserRole, QVariant(image->GetID()));
        ui->srcList->insertItem(ui->srcList->count(), item);
    }
}

void FrameSelectionWidget::Clear()
{
    ui->srcList->clear();
    ui->dstList->clear();
}

void FrameSelectionWidget::addSelectedFrames()
{
    if(ui->srcList->selectedItems().count() < 1)
        return;

    foreach(auto *item, ui->srcList->selectedItems())
        ui->dstList->insertItem(ui->dstList->count(), item->clone());

    emit signal_modified();
}

void FrameSelectionWidget::removeSelectedFrames()
{
    if(ui->dstList->selectedItems().count() < 1)
        return;

    foreach(auto *item, ui->dstList->selectedItems())
        ui->dstList->takeItem(ui->dstList->row(item));

    emit signal_modified();
}

void FrameSelectionWidget::removeAllSelectedFrames()
{
    ui->dstList->clear();

    emit signal_modified();
}

void FrameSelectionWidget::moveSelectedFrameUp()
{
    if(ui->dstList->selectedItems().size() != 1)
        return;

    int row = ui->dstList->row(ui->dstList->selectedItems().front());
    if(row == 0)
        return;

    auto *taken = ui->dstList->takeItem(row);
    ui->dstList->insertItem(row - 1, taken);
    ui->dstList->setCurrentItem(taken);

    emit signal_modified();
}

void FrameSelectionWidget::moveSelectedFrameDown()
{
    if(ui->dstList->selectedItems().size() != 1)
        return;

    int row = ui->dstList->row(ui->dstList->selectedItems().front());
    if(row == ui->dstList->count() - 1)
        return;

    auto *taken = ui->dstList->takeItem(row);
    ui->dstList->insertItem(row + 1, taken);
    ui->dstList->setCurrentItem(taken);

    emit signal_modified();
}

