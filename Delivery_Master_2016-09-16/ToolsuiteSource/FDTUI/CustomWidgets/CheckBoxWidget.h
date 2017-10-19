#ifndef CHECKBOXWIDGET_H
#define CHECKBOXWIDGET_H

class QCheckBox;
class QVariant;

class CheckBoxWidget : public QCheckBox
{
    Q_OBJECT

public:
    explicit CheckBoxWidget(bool value);

signals:
    void signal_ValueChanged(QVariant value);
};

#endif // CHECKBOXWIDGET_H
