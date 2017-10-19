#ifndef EXPORTCOMPILERDIALOG_H
#define EXPORTCOMPILERDIALOG_H

class QDialog;
class QString;
class QWidget;

namespace Ui {
class ExportCompilerDialog;
}

class ExportCompilerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportCompilerDialog(QString compilerData, QWidget *parent = 0);
    ~ExportCompilerDialog();

private:
    Ui::ExportCompilerDialog* m_ui;
};

#endif // EXPORTCOMPILERDIALOG_H
