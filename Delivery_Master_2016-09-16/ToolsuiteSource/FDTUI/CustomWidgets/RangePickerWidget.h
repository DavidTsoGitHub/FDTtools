#ifndef RANGEPICKERWIDGET_H
#define RANGEPICKERWIDGET_H

#include <QWidget>
class QSpinBox;
class QLabel;

class RangePickerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RangePickerWidget(QWidget* parent = 0);
    ~RangePickerWidget();

signals:
    void signal_SelectedRangeStart(int rangeStart);
    void signal_SelectedRangeEnd(int rangeEnd);

public slots:
    void slot_SetStartValue(int startValue);
    void slot_SetEndValue(int endValue);

private:
    QSpinBox* m_rangeStart;
    QSpinBox* m_rangeEnd;
    QLabel* m_arrowLabel;

    void initializeSpinBox(QSpinBox* spinBox);
};

#endif // RANGEPICKERWIDGET_H
