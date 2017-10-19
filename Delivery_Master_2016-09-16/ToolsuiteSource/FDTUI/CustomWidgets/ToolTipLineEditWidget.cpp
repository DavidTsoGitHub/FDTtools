#include "ToolTipLineEditWidget.h"

#include <QLineEdit>
#include <QString>
#include <QWidget>

ToolTipLineEditWidget::ToolTipLineEditWidget(const QString& text, QWidget* parent) : QLineEdit(text, parent)
{
    setToolTip(text);
    connect(this, &QLineEdit::textChanged, [=](const QString& text) { setToolTip(text); });
}
