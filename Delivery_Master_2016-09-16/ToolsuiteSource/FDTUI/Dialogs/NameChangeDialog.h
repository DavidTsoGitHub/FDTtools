#ifndef NAMECHANGEDIALOG_H
#define NAMECHANGEDIALOG_H

class QDialog;
class QRegExpValidator;

namespace Ui {
class NameChangeDialog;
}

class NameChangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NameChangeDialog(QString *name, QWidget *parent = 0);
    ~NameChangeDialog();

private:
    Ui::NameChangeDialog *m_ui;
    QString* m_name;
    QRegExpValidator* m_cNameRegExpValidator;

    // QDialog interface
public slots:
    void accept();
};

#endif // NAMECHANGEDIALOG_H
