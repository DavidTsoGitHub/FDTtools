#ifndef ICONSELECTORCOMBOBOX_H
#define ICONSELECTORCOMBOBOX_H

class FunctionBlock;
class IconDefinition;
class Project;
class QComboBox;
class QString;
class QWidget;

class IconSelectorComboBox : public QComboBox
{
    Q_OBJECT

public:
    IconSelectorComboBox(FunctionBlock* functionBlock, QString iconResourceName, Project* project, QWidget* parent = 0);

private slots:
    void populate();
    void slot_currentIndexChanged(QString);

private:
    const IconDefinition& m_iconDefinition;
    QString m_iconResourceName;
    FunctionBlock* m_functionBlock;

    bool m_signalsEnabled;
    void projectUpdated();
};

#endif // ICONSELECTORCOMBOBOX_H
