#ifndef ADDNEWLAYOUTDIALOG_H
#define ADDNEWLAYOUTDIALOG_H

#include "LayoutDefinition.h"

class QDialog;

namespace Ui {
class AddNewLayoutDialog;
}

class AddNewLayoutDialog : public QDialog
{
    Q_OBJECT

public:

    explicit AddNewLayoutDialog(LayoutDefinition& layoutDefinition, QWidget *parent = 0);
    ~AddNewLayoutDialog();

    QString GetLayoutName();
    LayoutDefinition::LayoutType GetLayoutType();

private:
    LayoutDefinition& m_layoutDefinition;
    QString m_name;
    Ui::AddNewLayoutDialog *m_ui;

    // QDialog interface
public slots:
    void accept();
};

#endif // ADDNEWLAYOUTDIALOG_H
