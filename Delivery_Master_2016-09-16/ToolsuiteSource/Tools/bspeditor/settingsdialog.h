#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    bool useDefaultSchema() const;
    QString customSchema() const;
    bool autoOpenRecent() const;

    void accept();

private:
    Ui::SettingsDialog *ui;

    void setCustomSchemaEnabled(bool enabled);
    void selectCustomSchema();

};

#endif // SETTINGSDIALOG_H
