#ifndef IMAGESINUSEDIALOG_H
#define IMAGESINUSEDIALOG_H

#include <QDialog>
class QItemSelection;


namespace Ui {
class ImagesInUseDialog;
}

class ImagesInUseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImagesInUseDialog(QString content, QWidget *parent = 0);
    ~ImagesInUseDialog();

private:
    Ui::ImagesInUseDialog *ui;

};

#endif // IMAGESINUSEDIALOG_H
