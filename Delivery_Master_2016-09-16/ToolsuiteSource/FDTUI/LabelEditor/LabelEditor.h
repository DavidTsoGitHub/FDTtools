#ifndef LABELEDITOR_H
#define LABELEDITOR_H

class AddLanguageDialog;
class FontCheckBoxProxyModel;
class FontModel;
class LabelModel;
class LabelProxyModel;
class Project;
class ProjectObject;

class QDir;
class QModelIndex;
class QItemSelection;
class QTableWidgetItem;
class QTimer;
class QString;
class QStringList;
class QWidget;

namespace Ui
{
class LabelEditor;
}

class LabelEditor : public QWidget
{
    Q_OBJECT

public:
    explicit LabelEditor(QWidget* parent = 0);
    ~LabelEditor(void);

signals:
    void signal_modified();
    void signal_LabelEditorItemSelected(ProjectObject* selectedItem);

public slots:
    void SetProject(Project* project);
    void ClearProject();
    void JustSelected();
    void JustDeselected();

private:
    Ui::LabelEditor* m_ui;
    AddLanguageDialog* m_addLanguageDialog;
    Project* m_project;
    LabelModel* m_labelModel;
    LabelProxyModel* m_labelProxyModel;
    FontModel* m_fontModel;
    FontCheckBoxProxyModel* m_proxyModel;
    QDir m_projectRoot;
    QTimer* m_updateMemoryUsageTimer;
    bool m_tabShowing;
    bool m_refreshNeeded;

    void setupSignals(void);
    void addNewLanguages(QStringList& selectedLanguages);
    void deleteRemovedLanguages(QStringList& selectedLanguages);    
    void updateMemoryUsage();
    void startTimerUpdateMemoryUsage();
    void setRefreshNeeded();

public slots:
    void calculateMemoryUsage();

private slots:
    void slot_addLanguageButton_pressed(void);
    void slot_addLanguageDialog_accepted(QStringList* languagesToAdd);
    void slot_addLanguageDialog_rejected(void);

    void slot_labelSelectionChanged(const QItemSelection& selected, const QItemSelection&);
    void slot_removeSelectedLabels();
    void slot_removeFontFromLabels(quint32 removedFontID);

    void slot_fontSelectionChanged(const QItemSelection&, const QItemSelection&);
    void slot_addFontButton_clicked();
    void slot_addLabelButton_clicked();
    void slot_updateMemoryUsageButtonClicked();
    void slot_removeSelectedFonts();
    void slot_replaceFontButton_clicked();
    void slot_defaultFontComboBoxSelectionChanged(const QString &text);
    void slot_mandatoryGlyphsTextEdit_changed();
};

#endif // LABELEDITOR_H
