#ifndef MENUITEMPREVIEWWIDGET_H
#define MENUITEMPREVIEWWIDGET_H

class MenuItem;
class Project;
class UIWidgetBase;

class QGraphicsScene;
class QWidget;

namespace Ui
{
class MenuItemPreviewWidget;
}

class MenuItemPreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuItemPreviewWidget(quint16 displayHeight, quint16 displayWidth, QWidget *parent = 0);
    ~MenuItemPreviewWidget(void);

    void SetProject(Project* project);
    void SetMenuItem(MenuItem* menuItem);    
    void HighLightUIWidget(UIWidgetBase* uiWidget);
    void Redraw(void);

private:
    Ui::MenuItemPreviewWidget *ui;
    QGraphicsScene* m_graphicsScene;
    quint16 m_menuAreaHeight;
    quint16 m_menuAreaWidth;
    quint16 m_menuAreaPosX;
    quint16 m_menuAreaPosY;
    quint16 m_displayHeight;
    quint16 m_displayWidth;
    MenuItem* m_menuItem;
    Project* m_project;
    UIWidgetBase* m_highLightedUIWidget;
    bool m_showChromaKey;
};

#endif // MENUITEMPREVIEWWIDGET_H
