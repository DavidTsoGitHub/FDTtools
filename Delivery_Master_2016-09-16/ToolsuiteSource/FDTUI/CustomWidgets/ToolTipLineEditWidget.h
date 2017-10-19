#ifndef TOOLTIPLINEEDITWIDGET_H
#define TOOLTIPLINEEDITWIDGET_H

class QLineEdit;
class QString;
class QWidget;

class ToolTipLineEditWidget : public QLineEdit
{
    Q_OBJECT

public:
    explicit ToolTipLineEditWidget(const QString& text, QWidget* parent = 0);
};

#endif // TOOLTIPLINEEDITWIDGET_H
