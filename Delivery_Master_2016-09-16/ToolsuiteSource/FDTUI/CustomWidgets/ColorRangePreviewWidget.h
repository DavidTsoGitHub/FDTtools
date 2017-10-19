#ifndef COLORRANGEPREVIEWWIDGET_H
#define COLORRANGEPREVIEWWIDGET_H

#include <QWidget>
class QColor;
class QLineEdit;
class QLabel;

class ColorRangePreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorRangePreviewWidget(QWidget* parent = 0);
    ~ColorRangePreviewWidget();

signals:

public slots:
    void slot_SetStartRangeColor(QColor startColor);
    void slot_SetEndRangeColor(QColor endColor);

private:
    QLabel* m_arrowLabel;
    QLineEdit* m_startColor;
    QLineEdit* m_endColor;

    void initializeColorWidgets(QLineEdit* widget);

};

#endif // COLORRANGEPREVIEWWIDGET_H
