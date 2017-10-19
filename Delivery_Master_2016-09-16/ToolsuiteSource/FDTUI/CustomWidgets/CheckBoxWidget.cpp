#include "CheckBoxWidget.h"

CheckBoxWidget::CheckBoxWidget(bool value)
{
    this->setChecked(value);
    connect(this, &QAbstractButton::toggled, [=] (bool value) { emit signal_ValueChanged(value); });
}

