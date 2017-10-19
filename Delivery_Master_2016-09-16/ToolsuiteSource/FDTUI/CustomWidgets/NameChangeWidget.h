#ifndef NAMECHANGEWIDGET_H
#define NAMECHANGEWIDGET_H

class Animation;
class CheckMessage;
class DisplayArea;
class Layout;
class ProjectObject;
class UIWidgetBase;
class ProjectObject;
class ProjectObject;
class QLabel;
class QString;
class QWidget;

class NameChangeWidget : public QWidget
{
    Q_OBJECT
public:
    NameChangeWidget(ProjectObject *object, QWidget* parent = 0);

private:
    ProjectObject* m_projectObject;
    UIWidgetBase* m_widget;
    DisplayArea* m_displayArea;
    Layout* m_layout;
    Animation* m_animation;
    CheckMessage* m_checkMessage;

    QLabel* m_nameLabel;

    void showNameChangeDialog();
    void getProjectObjectType();
    void setLineEditString();
    void updateName(QString name);

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent* event);
};

#endif // NAMECHANGEWIDGET_H
