#ifndef IMPORTCOMPILERDIALOG_H
#define IMPORTCOMPILERDIALOG_H

class QDialog;
class QWidget;
class QString;
class QPushButton;

namespace Ui {
class ImportCompilerDialog;
}

class ImportCompilerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportCompilerDialog(QWidget *parent = 0);
    ~ImportCompilerDialog();

    QString GetCompilerData();

private:
    Ui::ImportCompilerDialog* m_ui;

    QPushButton* m_okButton;
};

#endif // IMPORTCOMPILERDIALOG_H
