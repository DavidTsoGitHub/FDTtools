#ifndef SELECTDISPLAYAREADIALOG_H
#define SELECTDISPLAYAREADIALOG_H

class DisplayArea;
class Layout;
class Project;
class QDialog;
class QWidget;

namespace Ui {
class SelectDisplayAreaDialog;
}

class SelectDisplayAreaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDisplayAreaDialog(Project* project, quint8 currentLayoutIndex = 0, QWidget *parent = 0);
    ~SelectDisplayAreaDialog();

    DisplayArea* GetSelectedDisplayArea();

private:
    Ui::SelectDisplayAreaDialog* m_ui;
    Project* m_project;
    quint8 m_currentLayoutIndex;
    Layout* m_layout;
    DisplayArea* m_selectedDisplayArea;
    void populateDisplayAreasComboBox(void);    
    void populateLayoutsComboBox();

private slots:
    void slot_displayAreaComboBox_currentIndexChanged(int index);
    void slot_layoutsComboBox_currentIndexChanged(int index);
};

#endif // SELECTDISPLAYAREADIALOG_H
