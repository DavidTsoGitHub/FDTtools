#include "NameChangeWidget.h"
#include "NameChangeDialog.h"

#include "Animation.h"
#include "DisplayArea.h"
#include "Layout.h"
#include "UIWidgetBase.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>

NameChangeWidget::NameChangeWidget(ProjectObject* object, QWidget *parent) :
    QWidget(parent),
    m_projectObject(object),
    m_widget(NULL),
    m_displayArea(NULL),
    m_layout(NULL),
    m_animation(NULL),
    m_checkMessage(NULL),
    m_nameLabel(new QLabel())
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_nameLabel, 1, Qt::AlignLeft);

    getProjectObjectType();
    setLineEditString();

    QPushButton* mappingButton = new QPushButton("...");
    mappingButton->setFixedWidth(30);
    layout->addWidget(mappingButton);

    connect(mappingButton, &QPushButton::clicked, this, &NameChangeWidget::showNameChangeDialog);
}

void NameChangeWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;
    showNameChangeDialog();
}

void NameChangeWidget::showNameChangeDialog()
{
    if (m_projectObject != NULL)
    {
        QString name = "";
        if (m_widget != NULL)
        {
            name = m_widget->GetUserDefinedName();
        }
        else if (m_displayArea != NULL)
        {
            name = m_displayArea->GetName();
        }
        else if (m_layout != NULL)
        {
            name = m_layout->Name();
        }
        else if (m_animation != NULL)
        {
            name = m_animation->GetName();
        }
        else if (m_checkMessage != NULL)
        {
            name = m_checkMessage->GetName();
        }

        NameChangeDialog dialog(&name, this);
        int res = dialog.exec();

        if (res == QDialog::Rejected) return;

        updateName(name);
    }
}

void NameChangeWidget::getProjectObjectType()
{
    if (dynamic_cast<UIWidgetBase*>(m_projectObject) != NULL)
    {
        m_widget = dynamic_cast<UIWidgetBase*>(m_projectObject);
        return;
    }

    if (dynamic_cast<DisplayArea*>(m_projectObject) != NULL)
    {
        m_displayArea = dynamic_cast<DisplayArea*>(m_projectObject);
        return;
    }

    if (dynamic_cast<Layout*>(m_projectObject) != NULL)
    {
        m_layout = dynamic_cast<Layout*>(m_projectObject);
        return;
    }

    if (dynamic_cast<Animation*>(m_projectObject) != NULL)
    {
        m_animation = dynamic_cast<Animation*>(m_projectObject);
        return;
    }

    if (dynamic_cast<CheckMessage*>(m_projectObject) != NULL)
    {
        m_checkMessage = dynamic_cast<CheckMessage*>(m_projectObject);
        return;
    }
}

void NameChangeWidget::setLineEditString()
{
    if (m_widget != NULL)
    {
        m_nameLabel->setText(m_widget->GetUserDefinedName());
        return;
    }

    if (m_displayArea != NULL)
    {
        m_nameLabel->setText(m_displayArea->GetName());
        return;
    }

    if (m_layout != NULL)
    {
        m_nameLabel->setText(m_layout->Name());
        return;
    }

    if (m_animation != NULL)
    {
        m_nameLabel->setText(m_animation->GetName());
        return;
    }

    if (m_checkMessage != NULL)
    {
        m_nameLabel->setText(m_checkMessage->GetName());
        return;
    }
}

void NameChangeWidget::updateName(QString name)
{
    if (m_widget != NULL)
    {
        if (QString::compare(name, m_widget->GetUserDefinedName()) != 0)
        {
            m_widget->SetUserDefinedName(name);
            m_projectObject->PropertyUpdatedRedrawNeeded();
        }
        return;
    }

    if (m_displayArea != NULL)
    {
        if (QString::compare(name, m_displayArea->GetName()) != 0)
        {
            m_displayArea->SetName(name);
            m_projectObject->PropertyUpdatedRedrawNeeded();
        }
        return;
    }

    if (m_layout != NULL)
    {
        if (QString::compare(name, m_layout->Name()) != 0)
        {
            m_layout->SetName(name);
            m_projectObject->PropertyUpdatedRedrawNeeded();
        }
        return;
    }

    if (m_animation != NULL)
    {
        if (QString::compare(name, m_animation->GetName()) != 0)
        {
            m_animation->SetName(name);
            m_projectObject->PropertyUpdatedRedrawNeeded();
        }
        return;
    }

    if (m_checkMessage != NULL)
    {
        if (QString::compare(name, m_checkMessage->GetName()) != 0)
        {
            m_checkMessage->SetName(name);
            m_projectObject->PropertyUpdatedRedrawNeeded();
        }
    }
}
