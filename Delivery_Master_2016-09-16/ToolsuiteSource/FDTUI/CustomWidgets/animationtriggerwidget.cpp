#include "AnimationTriggerWidget.h"

#include "Layout.h"
#include "ProjectObjectPropertiesWidget.h"
#include "SignalMappingWidget.h"
#include "TransitionCondition.h"
#include "UIWidgetBase.h"

#include <QComboBox>
#include <QPushButton>
#include <QWidget>

AnimationTriggerWidget::AnimationTriggerWidget(TransitionClause* transition, QTreeWidgetItem* transitionRootItem, ProjectObject *projectObject, Project* project, ProjectObjectPropertiesWidget *parent, quint8 index) :
    m_projectObject(projectObject),
    m_project(project),
    m_parent(parent),
    m_index(index),
    m_trigger(transition),
    m_triggerRootItem(transitionRootItem)
{
    m_uiWidget = static_cast<UIWidgetBase*>(m_projectObject);

    addHeader();
    addFirstCondition();
    addSecondCondition();
}

AnimationTriggerWidget::~AnimationTriggerWidget()
{

}

void AnimationTriggerWidget::addHeader()
{
    m_headerItem = new QTreeWidgetItem(m_triggerRootItem);

    QFont font;
    font.setItalic(true);
    font.setBold(true);
    m_headerItem->setFont(0, font);
    m_headerItem->setText(0, "Trigger");
    m_headerItem->setExpanded(true);

    QPushButton* deleteButton = new QPushButton(this);
    deleteButton->setText("Delete");
    deleteButton->setFixedWidth(90);
    connect(deleteButton, &QPushButton::clicked, this, &AnimationTriggerWidget::removeTrigger);
    m_parent->AddTreeWidgetItem(m_headerItem, deleteButton, 1);

}

void AnimationTriggerWidget::addSignalSinkWidget(QTreeWidgetItem* parent, TransitionCondition *condition)
{
    GCLSignalSink* sink = NULL;
    if (condition != NULL)
    {
        sink = condition->GetSignalSinks().first();
    }
    QTreeWidgetItem* mappingItem = new QTreeWidgetItem(parent);
    mappingItem->setFirstColumnSpanned(true);
    m_parent->AddTreeWidgetItem(mappingItem, new SignalMappingWidget(sink, m_project));
}

void AnimationTriggerWidget::addConditionCombobox(QTreeWidgetItem* parent, TransitionCondition* transitionCondition, QComboBox* combobox)
{
    combobox = new QComboBox(this);
    combobox->addItems(QStringList() << "Equals" << "Greater than" << "Greater or Equal to" << "Less than" << "Less or Equal to");
    combobox->setFixedWidth(150);
    combobox->setCurrentIndex(transitionCondition->GetCompareOperator());
    connect(combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int currentIndex) {
        conditionCombobox_currentIndexChanged(currentIndex, transitionCondition);
    });

    m_parent->AddEditableData("Condition", combobox, parent);
}

void AnimationTriggerWidget::addCompareValueLineEdit(QTreeWidgetItem* parent, TransitionCondition* transitionCondition, QLineEdit* lineEdit)
{
    lineEdit = new QLineEdit(this);
    lineEdit->insert(QString::number(transitionCondition->GetConditionValue()));
    connect(lineEdit, &QLineEdit::textChanged, [=](QString valueText) {
        transitionCondition->SetConditionValue(valueText.toInt());
    });
    m_parent->AddEditableData("Value", lineEdit, parent);
}

void AnimationTriggerWidget::addAdditionalCompareCombobox(QTreeWidgetItem *parent)
{
    m_additionalCompareComboBox = new QComboBox(this);
    m_additionalCompareComboBox->addItems(QStringList() << "NONE" << "AND" << "XOR");
    m_additionalCompareComboBox->setFixedWidth(70);
    connect(m_additionalCompareComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &AnimationTriggerWidget::additionalCompareComboBox_currentIndexChanged);
    m_additionalCompareComboBox->setCurrentIndex(m_trigger->GetConnective());
    m_parent->AddEditableData("", m_additionalCompareComboBox, parent);
}

void AnimationTriggerWidget::addFirstCondition()
{
    QFont font;
    font.setItalic(true);
    QTreeWidgetItem* firstConditionHeader = new QTreeWidgetItem(m_headerItem);
    firstConditionHeader->setFont(0, font);
    firstConditionHeader->setText(0, "First Condition");
    firstConditionHeader->setExpanded(true);
    m_parent->AddTreeWidgetItem(firstConditionHeader, NULL);

    addSignalSinkWidget(firstConditionHeader, m_trigger->GetFirstCondition());
    addConditionCombobox(firstConditionHeader, m_trigger->GetFirstCondition(), m_firstConditionComboBox);
    addCompareValueLineEdit(firstConditionHeader, m_trigger->GetFirstCondition(), m_firstConditionLineEdit);
}

void AnimationTriggerWidget::addSecondCondition()
{
    QFont font;
    font.setItalic(true);
    QTreeWidgetItem* secondConditionHeader = new QTreeWidgetItem(m_headerItem);
    secondConditionHeader->setFont(0, font);
    secondConditionHeader->setText(0, "Second Condition");
    m_parent->AddTreeWidgetItem(secondConditionHeader, NULL);
    addAdditionalCompareCombobox(secondConditionHeader);

    if (m_trigger->GetConnective() != LayoutTransitionTypes::NONE)
    {
        addSignalSinkWidget(secondConditionHeader,  m_trigger->GetSecondCondition());
        addConditionCombobox(secondConditionHeader, m_trigger->GetSecondCondition(), m_secondConditionComboBox);
        addCompareValueLineEdit(secondConditionHeader, m_trigger->GetSecondCondition(), m_secondConditionLineEdit);
        secondConditionHeader->setExpanded(true);
    }
    else
    {
        secondConditionHeader->setExpanded(false);
    }
}

void AnimationTriggerWidget::removeTrigger()
{
    if (m_uiWidget != NULL)
    {
        m_uiWidget->RemoveAnimationTrigger(m_index, m_trigger);
        delete m_trigger;

        quint8 index = 0;
        foreach (TransitionClause* trigger, m_uiWidget->GetAnimationTriggers(m_index))
        {
            GCLSignalSink* firstSink = trigger->GetFirstCondition()->GetSignalSinks().first();
            QString newSignalName = "Transition_" + QString::number(index);

            if (firstSink != NULL)
            {
                GCLSignalSink* newSink = new GCLSignalSink(newSignalName + "_1", firstSink->ComponentName(), "DynamicWidgetValue");
                newSink->ConnectTo(firstSink->GetSignalSource());
                firstSink->DisconnectFromSource();
                trigger->GetFirstCondition()->SetSignalSink(newSink);
            }

            if (trigger->GetSecondCondition() != NULL)
            {
                GCLSignalSink* secondSink = trigger->GetSecondCondition()->GetSignalSinks().first();
                GCLSignalSink* newSink = new GCLSignalSink(newSignalName + "_2", secondSink->ComponentName(), "DynamicWidgetValue");
                newSink->ConnectTo(secondSink->GetSignalSource());
                secondSink->DisconnectFromSource();
                trigger->GetSecondCondition()->SetSignalSink(newSink);
            }

            trigger->SetSignalName(newSignalName);
            ++index;
        }

        m_projectObject->NotifyPropertyUpdated();
        m_projectObject->NotifyPropertyUpdatedRedrawNeeded();
    }
}

void AnimationTriggerWidget::additionalCompareComboBox_currentIndexChanged(int currentIndex)
{
    if (currentIndex != m_trigger->GetConnective())
    {
        m_trigger->SetConnective(static_cast<LayoutTransitionTypes::TransitionConnective>(m_additionalCompareComboBox->currentIndex()));

        if (m_trigger->GetConnective() == LayoutTransitionTypes::NONE)
        {
            m_trigger->RemoveSecondCondition();
        }
        else
        {
            m_trigger->AddSecondCondition();
        }
        m_projectObject->NotifyPropertyUpdatedRedrawNeeded();
    }

}

void AnimationTriggerWidget::conditionCombobox_currentIndexChanged(int currentIndex, TransitionCondition* transitionCondition)
{
    transitionCondition->SetCompareOperator(static_cast<LayoutTransitionTypes::TransitionCompare>(currentIndex));
}
