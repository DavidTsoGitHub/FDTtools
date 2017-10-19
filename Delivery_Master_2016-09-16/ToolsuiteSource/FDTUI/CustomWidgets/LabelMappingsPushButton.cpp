#include "LabelMappingsPushButton.h"

#include "LabelMappings.h"
#include <QPushButton>
#include <QString>
#include <QWidget>

LabelMappingsPushButton::LabelMappingsPushButton(LabelMappings* labelMappings, QString text, QWidget* parent) : QPushButton(text, parent),
    m_labelMappings(labelMappings)
{
    connect(this, &QAbstractButton::clicked, [=] { emit signal_Clicked(m_labelMappings); });
}

LabelMappingsPushButton::~LabelMappingsPushButton()
{

}

