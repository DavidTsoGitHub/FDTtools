#ifndef UITOOLBOXDIALOG_H
#define UITOOLBOXDIALOG_H

class DisplayArea;
class MenuItem;
class UIWidgetBase;
class UIWidgetContainerBase;
class UIWidgetsModel;
class QDialog;
class QWidget;

class Project;

namespace Ui {
class UIToolboxDialog;
}

class UIToolboxDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UIToolboxDialog(Project* project, UIWidgetContainerBase* widgetContainer, QWidget* parent = 0);
    ~UIToolboxDialog();

public slots:

signals:
    void signal_widgetAdded(UIWidgetBase* uiWidget);

private:
    Ui::UIToolboxDialog* m_ui;

    Project* m_project;
    UIWidgetContainerBase* m_selectedUIWidgetContainerBase;
    UIWidgetsModel* m_uiWidgetsModel;
    UIWidgetBase* m_activeWidget;

    bool checkMessageHook();
    bool validation();
    bool validateListNavigationWidget();
    bool validateMenuItem(const MenuItem* menuItem);
    bool trySelectedWidget(const QModelIndex& currentIndex, const QModelIndex& previous);
    UIWidgetBase* createSelectedWidget();
};

#endif // UITOOLBOXDIALOG_H
