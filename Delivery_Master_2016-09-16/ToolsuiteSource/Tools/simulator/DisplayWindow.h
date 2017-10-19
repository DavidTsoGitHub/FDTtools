#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QGraphicsScene>
#include <QWidget>

namespace Ui {
    class DisplayWindow;
}

class DisplayWindow : public QWidget
{
    Q_OBJECT


public:
    typedef enum
    {
        NORMAL,
        CCW
    }Rotation_e;

    explicit DisplayWindow(QWidget *parent = 0);
    ~DisplayWindow();

    void SetRedrawText(QString string);
    void SetScene(QGraphicsScene* scene);
    void SetSceneSize(quint16 width, quint16 height, Rotation_e rotation);
    void SetBackgroundBrush(QBrush brush);
    QGraphicsView *GetGraphicsView(void);
    void Rotate(Rotation_e rotation);

private:
    Ui::DisplayWindow *m_ui;
    QSize m_size;

    // QWidget interface
public:
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
};

#endif // DISPLAYWINDOW_H
