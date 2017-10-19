#ifndef FRAMESELECTIONWIDGET_H
#define FRAMESELECTIONWIDGET_H

#include "AnimationDefinition.h"
#include "imagedefinition.h"

template <typename T> class QList;
class QWidget;

namespace Ui {
class FrameSelectionWidget;
}

class FrameSelectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FrameSelectionWidget(QWidget *parent = 0);
    ~FrameSelectionWidget();

    QList<quint32> GetFrames();

signals:
    void signal_modified();

public slots:
    void PopulateFrames(ImageDefinition& def, const AnimationFrameByFrame &anim);
    void PopulateAvailableImages(const ImageDefinition& def);
    void Clear();

private:
    Ui::FrameSelectionWidget *ui;

    void addSelectedFrames();
    void removeSelectedFrames();
    void removeAllSelectedFrames();
    void moveSelectedFrameUp();
    void moveSelectedFrameDown();
};

#endif // FRAMESELECTIONWIDGET_H
