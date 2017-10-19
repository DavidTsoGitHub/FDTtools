#ifndef MAPPINGSDIALOG_H
#define MAPPINGSDIALOG_H

class Label;
class LabelMappings;
class QDialog;
template <class T> class QList;
class QWidget;

namespace Ui {
class MappingsDialog;
}

class MappingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MappingsDialog(LabelMappings& labelMappings, const QList<Label*> &availableLabels, QWidget *parent = 0);

    QList<Label*> GetSelectedLabels(void);

private slots:
    void slot_onCurrentRowChangedSelectedLabels(int row);
    void slot_moveLabelUp(void);
    void slot_moveLabelDown(void);
    void slot_addLabel(void);
    void slot_removeLabel(void);

private:

    Ui::MappingsDialog *m_ui;
    QList<Label*> m_availableLabels;
    QList<Label*> m_selectedLabels;

    void updateListWidgets(void);
    void setupLabelLists(LabelMappings& labelMappings, const QList<Label*> &labels);
    void setupSignals();
};

#endif // MAPPINGSDIALOG_H
