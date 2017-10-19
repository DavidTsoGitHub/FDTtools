#ifndef ADDLANGUAGEDIALOG_H
#define ADDLANGUAGEDIALOG_H

class QDialog;
template <typename T> class QList;
class QListWidget;
class QString;
class QStringList;
class QWidget;

namespace Ui {
class AddLanguageDialog;
}

class AddLanguageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddLanguageDialog(const QStringList& usedLanguages, QWidget* parent = 0);
    ~AddLanguageDialog(void);

    void SetUsedLanguages(QStringList* usedLanguages);

signals:
    void signal_buttonOK_clicked(QStringList*);
    void signal_buttonCancel_clicked(void);

private slots:
    void slot_buttonOk_clicked(void);
    void slot_buttonCancel_clicked(void);
    void slot_addSelectedLanguagesButton_clicked(void);
    void slot_removeSelectedLanguagesButton_clicked(void);
    void slot_removeAllSelectedLanguages_clicked(void);

private:
    struct iso639_1Language
    {
      QString Iso639_1Abbreviation;
      QString LanguageName;

      iso639_1Language(QString iso, QString lang) : Iso639_1Abbreviation(iso), LanguageName(lang)
      {}
    };

    Ui::AddLanguageDialog* m_ui;
    QList<iso639_1Language> m_languages;
    const QStringList& m_usedLanguages;

    void setupLanguageList(void);
    void setupLanguageListWidget(void);
    void setupSignals(void);
    void moveSelectedFromWidgetToWidget(QListWidget& fromWidget, QListWidget& toWidget);
    bool listWidgetContainsText(QListWidget& widget, QString text);
};

#endif // ADDLANGUAGEDIALOG_H
