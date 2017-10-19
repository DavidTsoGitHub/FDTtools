#include "AddLanguageDialog.h"
#include "ui_AddLanguageDialog.h"

#include <QDialog>
#include <QList>
#include <QListWidget>
#include <QString>
#include <QStringList>
#include <QWidget>

AddLanguageDialog::AddLanguageDialog(const QStringList& usedLanguages, QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::AddLanguageDialog),
    m_usedLanguages(usedLanguages)
{
    m_ui->setupUi(this);
    setupLanguageList();
    setupLanguageListWidget();
    setupSignals();
}

AddLanguageDialog::~AddLanguageDialog()
{
    delete m_ui;
}

void AddLanguageDialog::setupSignals()
{
    connect(m_ui->addLanguageOKButton, &QAbstractButton::clicked, this, &AddLanguageDialog::slot_buttonOk_clicked);
    connect(m_ui->addLanguageCancelButton, &QAbstractButton::clicked, this, &AddLanguageDialog::slot_buttonCancel_clicked);
    connect(m_ui->addSelectedLanguagesButton, &QAbstractButton::clicked, this, &AddLanguageDialog::slot_addSelectedLanguagesButton_clicked);
    connect(m_ui->removeSelectedLanguagesButton, &QAbstractButton::clicked, this, &AddLanguageDialog::slot_removeSelectedLanguagesButton_clicked);
    connect(m_ui->removeAllSelectedLanguages, &QAbstractButton::clicked, this, &AddLanguageDialog::slot_removeAllSelectedLanguages_clicked);
}

void AddLanguageDialog::setupLanguageListWidget()
{
    foreach (iso639_1Language isoLanguage, m_languages)
    {
        if (m_usedLanguages.contains(isoLanguage.Iso639_1Abbreviation))
        {
            m_ui->selectedLanguagesListWidget->addItem(isoLanguage.LanguageName.trimmed() + " - " + isoLanguage.Iso639_1Abbreviation);
            continue;
        }

        m_ui->availableLanguagesListWidget->addItem(isoLanguage.LanguageName.trimmed() + " - " + isoLanguage.Iso639_1Abbreviation);
    }
}

void AddLanguageDialog::moveSelectedFromWidgetToWidget(QListWidget& fromWidget, QListWidget& toWidget)
{
    foreach(QListWidgetItem* language, fromWidget.selectedItems())
    {
        if (!listWidgetContainsText(toWidget, language->text()))
        {
            toWidget.addItem(language->text());
        }

        delete language;
    }

    toWidget.sortItems();
}

bool AddLanguageDialog::listWidgetContainsText(QListWidget& widget, QString text)
{
    for (int i = 0; i < widget.count(); ++i)
    {
        if (widget.item(i)->text() == text)
        {
            return true;
        }
    }

    return false;
}

void AddLanguageDialog::slot_removeAllSelectedLanguages_clicked()
{
    int lastIndex = m_ui->selectedLanguagesListWidget->count() - 1;
    for(; lastIndex >= 0; --lastIndex)
    {
        if (!listWidgetContainsText(*m_ui->availableLanguagesListWidget, m_ui->selectedLanguagesListWidget->item(lastIndex)->text()))
        {
            m_ui->availableLanguagesListWidget->addItem(m_ui->selectedLanguagesListWidget->item(lastIndex)->text());
        }

        delete m_ui->selectedLanguagesListWidget->takeItem(lastIndex);
    }

    m_ui->availableLanguagesListWidget->sortItems();
}

void AddLanguageDialog::slot_addSelectedLanguagesButton_clicked()
{
    moveSelectedFromWidgetToWidget(*m_ui->availableLanguagesListWidget, *m_ui->selectedLanguagesListWidget);
}

void AddLanguageDialog::slot_removeSelectedLanguagesButton_clicked()
{
    moveSelectedFromWidgetToWidget(*m_ui->selectedLanguagesListWidget, *m_ui->availableLanguagesListWidget);
}

void AddLanguageDialog::slot_buttonOk_clicked()
{
    QStringList* languages = new QStringList();

    for(int i = 0; i < m_ui->selectedLanguagesListWidget->count(); ++i)
    {
        QListWidgetItem* language = m_ui->selectedLanguagesListWidget->item(i);
        languages->append(language->text().split(" ").last());
    }

    emit signal_buttonOK_clicked(languages);
}

void AddLanguageDialog::slot_buttonCancel_clicked()
{
    emit signal_buttonCancel_clicked();
}


void AddLanguageDialog::setupLanguageList()
{
    m_languages.append(iso639_1Language("ab","Abkhaz"));
    m_languages.append(iso639_1Language("aa","Afar"));
    m_languages.append(iso639_1Language("af","Afrikaans"));
    m_languages.append(iso639_1Language("ak","Akan"));
    m_languages.append(iso639_1Language("sq","Albanian"));
    m_languages.append(iso639_1Language("am","Amharic"));
    m_languages.append(iso639_1Language("ar","Arabic"));
    m_languages.append(iso639_1Language("an","Aragonese"));
    m_languages.append(iso639_1Language("hy","Armenian"));
    m_languages.append(iso639_1Language("as","Assamese"));
    m_languages.append(iso639_1Language("av","Avaric"));
    m_languages.append(iso639_1Language("ae","Avestan"));
    m_languages.append(iso639_1Language("ay","Aymara"));
    m_languages.append(iso639_1Language("az","Azerbaijani"));
    m_languages.append(iso639_1Language("bm","Bambara"));
    m_languages.append(iso639_1Language("ba","Bashkir"));
    m_languages.append(iso639_1Language("eu","Basque"));
    m_languages.append(iso639_1Language("be","Belarusian"));
    m_languages.append(iso639_1Language("bn","Bengali, Bangla"));
    m_languages.append(iso639_1Language("bh","Bihari"));
    m_languages.append(iso639_1Language("bi","Bislama"));
    m_languages.append(iso639_1Language("bs","Bosnian"));
    m_languages.append(iso639_1Language("br","Breton"));
    m_languages.append(iso639_1Language("bg","Bulgarian"));
    m_languages.append(iso639_1Language("my","Burmese"));
    m_languages.append(iso639_1Language("ca","Catalan"));
    m_languages.append(iso639_1Language("ch","Chamorro"));
    m_languages.append(iso639_1Language("ce","Chechen"));
    m_languages.append(iso639_1Language("ny","Chichewa, Chewa, Nyanja"));
    m_languages.append(iso639_1Language("zh","Chinese"));
    m_languages.append(iso639_1Language("zh_hans","Chinese (Simplified)")); //not iso639 -- recommended W3C tag
    m_languages.append(iso639_1Language("zh_hant","Chinese (Traditional)")); //not iso639 -- recommended W3C tag
    m_languages.append(iso639_1Language("cv","Chuvash"));
    m_languages.append(iso639_1Language("kw","Cornish"));
    m_languages.append(iso639_1Language("co","Corsican"));
    m_languages.append(iso639_1Language("cr","Cree"));
    m_languages.append(iso639_1Language("hr","Croatian"));
    m_languages.append(iso639_1Language("cs","Czech"));
    m_languages.append(iso639_1Language("da","Danish"));
    m_languages.append(iso639_1Language("dv","Divehi, Dhivehi, Maldivian"));
    m_languages.append(iso639_1Language("nl","Dutch"));
    m_languages.append(iso639_1Language("dz","Dzongkha"));
    m_languages.append(iso639_1Language("en","English"));
    m_languages.append(iso639_1Language("en_uk", "English (United Kingdom)")); //not iso639!
    m_languages.append(iso639_1Language("en_us", "English (United States)")); //not iso639!
    m_languages.append(iso639_1Language("eo","Esperanto"));
    m_languages.append(iso639_1Language("et","Estonian"));
    m_languages.append(iso639_1Language("ee","Ewe"));
    m_languages.append(iso639_1Language("fo","Faroese"));
    m_languages.append(iso639_1Language("fj","Fijian"));
    m_languages.append(iso639_1Language("fi","Finnish"));
    m_languages.append(iso639_1Language("fr","French"));
    m_languages.append(iso639_1Language("ff","Fula, Fulah, Pulaar, Pular"));
    m_languages.append(iso639_1Language("gl","Galician"));
    m_languages.append(iso639_1Language("ka","Georgian"));
    m_languages.append(iso639_1Language("de","German"));
    m_languages.append(iso639_1Language("el","Greek (modern)"));
    m_languages.append(iso639_1Language("gn","Guaraní"));
    m_languages.append(iso639_1Language("gu","Gujarati"));
    m_languages.append(iso639_1Language("ht","Haitian, Haitian Creole"));
    m_languages.append(iso639_1Language("ha","Hausa"));
    m_languages.append(iso639_1Language("he","Hebrew (modern)"));
    m_languages.append(iso639_1Language("hz","Herero"));
    m_languages.append(iso639_1Language("hi","Hindi"));
    m_languages.append(iso639_1Language("ho","Hiri Motu"));
    m_languages.append(iso639_1Language("hu","Hungarian"));
    m_languages.append(iso639_1Language("ia","Interlingua"));
    m_languages.append(iso639_1Language("id","Indonesian"));
    m_languages.append(iso639_1Language("ie","Interlingue"));
    m_languages.append(iso639_1Language("ga","Irish"));
    m_languages.append(iso639_1Language("ig","Igbo"));
    m_languages.append(iso639_1Language("ik","Inupiaq"));
    m_languages.append(iso639_1Language("io","Ido"));
    m_languages.append(iso639_1Language("is","Icelandic"));
    m_languages.append(iso639_1Language("it","Italian"));
    m_languages.append(iso639_1Language("iu","Inuktitut"));
    m_languages.append(iso639_1Language("ja","Japanese"));
    m_languages.append(iso639_1Language("jv","Javanese"));
    m_languages.append(iso639_1Language("kl","Kalaallisut, Greenlandic"));
    m_languages.append(iso639_1Language("kn","Kannada"));
    m_languages.append(iso639_1Language("kr","Kanuri"));
    m_languages.append(iso639_1Language("ks","Kashmiri"));
    m_languages.append(iso639_1Language("kk","Kazakh"));
    m_languages.append(iso639_1Language("km","Khmer"));
    m_languages.append(iso639_1Language("ki","Kikuyu, Gikuyu"));
    m_languages.append(iso639_1Language("rw","Kinyarwanda"));
    m_languages.append(iso639_1Language("ky","Kyrgyz"));
    m_languages.append(iso639_1Language("kv","Komi"));
    m_languages.append(iso639_1Language("kg","Kongo"));
    m_languages.append(iso639_1Language("ko","Korean"));
    m_languages.append(iso639_1Language("ku","Kurdish"));
    m_languages.append(iso639_1Language("kj","Kwanyama, Kuanyama"));
    m_languages.append(iso639_1Language("la","Latin"));
    m_languages.append(iso639_1Language("lb","Luxembourgish, Letzeburgesch"));
    m_languages.append(iso639_1Language("lg","Ganda"));
    m_languages.append(iso639_1Language("li","Limburgish, Limburgan, Limburger"));
    m_languages.append(iso639_1Language("ln","Lingala"));
    m_languages.append(iso639_1Language("lo","Lao"));
    m_languages.append(iso639_1Language("lt","Lithuanian"));
    m_languages.append(iso639_1Language("lu","Luba-Katanga"));
    m_languages.append(iso639_1Language("lv","Latvian"));
    m_languages.append(iso639_1Language("gv","Manx"));
    m_languages.append(iso639_1Language("mk","Macedonian"));
    m_languages.append(iso639_1Language("mg","Malagasy"));
    m_languages.append(iso639_1Language("ms","Malay"));
    m_languages.append(iso639_1Language("ml","Malayalam"));
    m_languages.append(iso639_1Language("mt","Maltese"));
    m_languages.append(iso639_1Language("mi","Māori"));
    m_languages.append(iso639_1Language("mr","Marathi (Marāṭhī)"));
    m_languages.append(iso639_1Language("mh","Marshallese"));
    m_languages.append(iso639_1Language("mn","Mongolian"));
    m_languages.append(iso639_1Language("na","Nauru"));
    m_languages.append(iso639_1Language("nv","Navajo, Navaho"));
    m_languages.append(iso639_1Language("nd","Northern Ndebele"));
    m_languages.append(iso639_1Language("ne","Nepali"));
    m_languages.append(iso639_1Language("ng","Ndonga"));
    m_languages.append(iso639_1Language("nb","Norwegian Bokmål"));
    m_languages.append(iso639_1Language("nn","Norwegian Nynorsk"));
    m_languages.append(iso639_1Language("no","Norwegian"));
    m_languages.append(iso639_1Language("ii","Nuosu"));
    m_languages.append(iso639_1Language("nr","Southern Ndebele"));
    m_languages.append(iso639_1Language("oc","Occitan"));
    m_languages.append(iso639_1Language("oj","Ojibwe, Ojibwa"));
    m_languages.append(iso639_1Language("cu","Old Church Slavonic, Church Slavonic, Old Bulgarian"));
    m_languages.append(iso639_1Language("om","Oromo"));
    m_languages.append(iso639_1Language("or","Oriya"));
    m_languages.append(iso639_1Language("os","Ossetian, Ossetic"));
    m_languages.append(iso639_1Language("pa","Panjabi, Punjabi"));
    m_languages.append(iso639_1Language("pi","Pāli"));
    m_languages.append(iso639_1Language("fa","Persian (Farsi)"));
    m_languages.append(iso639_1Language("pl","Polish"));
    m_languages.append(iso639_1Language("ps","Pashto, Pushto"));
    m_languages.append(iso639_1Language("pt","Portuguese"));
    m_languages.append(iso639_1Language("qu","Quechua"));
    m_languages.append(iso639_1Language("rm","Romansh"));
    m_languages.append(iso639_1Language("rn","Kirundi"));
    m_languages.append(iso639_1Language("ro","Romanian"));
    m_languages.append(iso639_1Language("ru","Russian"));
    m_languages.append(iso639_1Language("sa","Sanskrit (Saṁskṛta)"));
    m_languages.append(iso639_1Language("sc","Sardinian"));
    m_languages.append(iso639_1Language("sd","Sindhi"));
    m_languages.append(iso639_1Language("se","Northern Sami"));
    m_languages.append(iso639_1Language("sm","Samoan"));
    m_languages.append(iso639_1Language("sg","Sango"));
    m_languages.append(iso639_1Language("sr","Serbian"));
    m_languages.append(iso639_1Language("gd","Scottish Gaelic, Gaelic"));
    m_languages.append(iso639_1Language("sn","Shona"));
    m_languages.append(iso639_1Language("si","Sinhala, Sinhalese"));
    m_languages.append(iso639_1Language("sk","Slovak"));
    m_languages.append(iso639_1Language("sl","Slovene"));
    m_languages.append(iso639_1Language("so","Somali"));
    m_languages.append(iso639_1Language("st","Southern Sotho"));
    m_languages.append(iso639_1Language("es","Spanish"));
    m_languages.append(iso639_1Language("su","Sundanese"));
    m_languages.append(iso639_1Language("sw","Swahili"));
    m_languages.append(iso639_1Language("ss","Swati"));
    m_languages.append(iso639_1Language("sv","Swedish"));
    m_languages.append(iso639_1Language("ta","Tamil"));
    m_languages.append(iso639_1Language("te","Telugu"));
    m_languages.append(iso639_1Language("tg","Tajik"));
    m_languages.append(iso639_1Language("th","Thai"));
    m_languages.append(iso639_1Language("ti","Tigrinya"));
    m_languages.append(iso639_1Language("bo","Tibetan Standard, Tibetan, Central"));
    m_languages.append(iso639_1Language("tk","Turkmen"));
    m_languages.append(iso639_1Language("tl","Tagalog"));
    m_languages.append(iso639_1Language("tn","Tswana"));
    m_languages.append(iso639_1Language("to","Tonga (Tonga Islands)"));
    m_languages.append(iso639_1Language("tr","Turkish"));
    m_languages.append(iso639_1Language("ts","Tsonga"));
    m_languages.append(iso639_1Language("tt","Tatar"));
    m_languages.append(iso639_1Language("tw","Twi"));
    m_languages.append(iso639_1Language("ty","Tahitian"));
    m_languages.append(iso639_1Language("ug","Uyghur"));
    m_languages.append(iso639_1Language("uk","Ukrainian"));
    m_languages.append(iso639_1Language("ur","Urdu"));
    m_languages.append(iso639_1Language("uz","Uzbek"));
    m_languages.append(iso639_1Language("ve","Venda"));
    m_languages.append(iso639_1Language("vi","Vietnamese"));
    m_languages.append(iso639_1Language("vo","Volapük"));
    m_languages.append(iso639_1Language("wa","Walloon"));
    m_languages.append(iso639_1Language("cy","Welsh"));
    m_languages.append(iso639_1Language("wo","Wolof"));
    m_languages.append(iso639_1Language("fy","Western Frisian"));
    m_languages.append(iso639_1Language("xh","Xhosa"));
    m_languages.append(iso639_1Language("yi","Yiddish"));
    m_languages.append(iso639_1Language("yo","Yoruba"));
    m_languages.append(iso639_1Language("za","Zhuang, Chuang"));
    m_languages.append(iso639_1Language("zu","Zulu"));
}
