#include "AnimationEditor.h"
#include "ui_AnimationEditor.h"

#include "AnimationListModel.h"
#include "AnimationFrameByFrame.h"
#include "OpacityCurveWidget.h"
#include "FrameSelectionWidget.h"
#include "Project.h"
#include "ProjectObject.h"
#include "SimpleEditableListWidget.h"
#include "TranslationCurveWidget.h"

#include <QPushButton>
#include <QWidget>

AnimationEditor::AnimationEditor(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::AnimationEditor),
    m_project(NULL),
    m_opacityCurveWidget(NULL),
    m_translationCurveWidget(NULL)
{
    m_ui->setupUi(this);

    m_frameAnimList = new SimpleEditableListWidget(this);
    m_ui->animationTypeFrameListWidgetLayout->addWidget(m_frameAnimList);
    m_opacityAnimList = new SimpleEditableListWidget(this);
    m_ui->animationTypeOpacityListWidgetLayout->addWidget(m_opacityAnimList);
    m_translationAnimList = new SimpleEditableListWidget(this);
    m_ui->animationTypeTranslationListWidgetLayout->addWidget(m_translationAnimList);
    m_frameSelectionWidget = new FrameSelectionWidget(this);
    m_ui->animationTypeFrameSelectionLayout->addWidget(m_frameSelectionWidget);
}

AnimationEditor::~AnimationEditor()
{
    delete m_ui;
}

void AnimationEditor::SetProject(Project* project)
{
    m_project = project;

    // Common setup for all animation types
    auto& animDef = project->GetAnimationDefinition();
    m_frameAnimModel = new AnimationListModel(animDef, AnimationListModel::Type::FrameByFrame, this);
    m_opacityAnimModel = new AnimationListModel(animDef, AnimationListModel::Type::Opacity, this);
    m_translationAnimModel = new AnimationListModel(animDef, AnimationListModel::Type::Translation, this);

    // Frame-by-frame specific setup
    connect(m_frameAnimList, &SimpleEditableListWidget::signal_selectionChanged, this, &AnimationEditor::populateFrameSelection);
    connect(&m_project->GetImageDefinition(), &ImageDefinition::PropertyUpdatedRedrawNeeded, this, &AnimationEditor::populateFrameSelection);
    connect(m_frameSelectionWidget, &FrameSelectionWidget::signal_modified, this, &AnimationEditor::updateFrameByFrameAnimationDefinition);
    populateFrameSelection();

    // Opacity specific setup
    connect(m_opacityAnimList, &SimpleEditableListWidget::signal_selectionChanged, [=]()
    {
        QModelIndex index = m_opacityAnimList->selectedIndex();

        if(m_opacityCurveWidget != NULL)
        {
            delete m_opacityCurveWidget;
            m_opacityCurveWidget = NULL;
        }

        if(!index.isValid())
            return;

        auto *animation = m_opacityAnimModel->GetAnimationOpacity(index);
        m_opacityCurveWidget = new OpacityCurveWidget(animation, this);
        m_ui->animationTypeOpacityCurveLayout->addWidget(m_opacityCurveWidget);
    });


    // Translation specific setup
    connect(m_translationAnimList, &SimpleEditableListWidget::signal_selectionChanged, [=]()
    {
        QModelIndex index = m_translationAnimList->selectedIndex();

        if(m_translationCurveWidget != NULL)
        {
            delete m_translationCurveWidget;
            m_translationCurveWidget = NULL;
        }

        if(!index.isValid())
            return;

        auto *animation = m_translationAnimModel->GetAnimationTranslation(index);
        m_translationCurveWidget = new TranslationCurveWidget(animation, this);
        m_ui->animationTypeTranslationCurveLayout->addWidget(m_translationCurveWidget, 0, 0);

        // Play button
        auto *translationPlayButton = new QPushButton("Play", m_translationCurveWidget);
        m_ui->animationTypeTranslationCurveLayout->addWidget(translationPlayButton, 1, 0, Qt::AlignLeft);
        connect(translationPlayButton, &QAbstractButton::clicked, m_translationCurveWidget, &TranslationCurveWidget::Play);
    });

    // Animation lists
    initAnimationList(m_frameAnimList, m_frameAnimModel);
    initAnimationList(m_opacityAnimList, m_opacityAnimModel);
    initAnimationList(m_translationAnimList, m_translationAnimModel);
}

void AnimationEditor::ClearProject()
{
    if(m_project == NULL)
        return;

    // Frame-by-frame specific teardown
    disconnect(m_frameSelectionWidget, &FrameSelectionWidget::signal_modified, 0, 0);

    // Common teardown for all animation types
    deinitAnimationList(m_frameAnimList, m_frameAnimModel);
    deinitAnimationList(m_opacityAnimList, m_opacityAnimModel);
    deinitAnimationList(m_translationAnimList, m_translationAnimModel);

    delete m_frameAnimModel;
    delete m_opacityAnimModel;
    delete m_translationAnimModel;

    m_project = NULL;
}

void AnimationEditor::RefreshAvailableImages()
{
    if(m_project == NULL)
        return;

    m_frameSelectionWidget->PopulateAvailableImages(m_project->GetImageDefinition());
}

void AnimationEditor::JustSelected()
{
    // Notify others of currently selected animation in current tab
    auto *widget = m_ui->AnimationEditorTabWidget->currentWidget();
    auto *list = widget->findChild<SimpleEditableListWidget*>();
    auto *model = qobject_cast<AnimationListModel*>(list->model());
    auto index = list->selectedIndex();
    if(model != NULL && index.isValid())
        emit signal_AnimationEditorItemSelected(model->GetAnimation(index));
}

void AnimationEditor::initAnimationList(SimpleEditableListWidget *list, AnimationListModel *model)
{
    list->setModel(model);
    list->setAddEnabled(true);

    connect(model, &AnimationListModel::signal_modified, this, &AnimationEditor::signal_modified);

    connect(list, &SimpleEditableListWidget::signal_AddClicked, [=]()
    {
        model->AddAnimation();
        list->setCurrentIndex(model->index(model->rowCount() - 1));
    });

    connect(list, &SimpleEditableListWidget::signal_RemoveClicked, [=]()
    {        
        model->RemoveAnimation(list->selectedIndex());
    });

    connect(list, &SimpleEditableListWidget::signal_selectionChanged, [=]()
    {
        QModelIndex index = list->selectedIndex();
        list->setRemoveEnabled(index.isValid());
        emit signal_AnimationEditorItemSelected(index.isValid() ? model->GetAnimation(index) : NULL);
    });

    connect(list, &SimpleEditableListWidget::signal_listViewClicked, [=]()
    {
        auto *widget = m_ui->AnimationEditorTabWidget->currentWidget();
        if(list == widget->findChild<SimpleEditableListWidget*>())
        {
            QModelIndex index = list->selectedIndex();
            emit signal_AnimationEditorItemSelected(index.isValid() ? model->GetAnimation(index) : NULL);
        }
    });

    connect(m_ui->AnimationEditorTabWidget, &QTabWidget::currentChanged, [=]() {
        auto *widget = m_ui->AnimationEditorTabWidget->currentWidget();
        if(list == widget->findChild<SimpleEditableListWidget*>())
        {
            QModelIndex index = list->selectedIndex();
            emit signal_AnimationEditorItemSelected(index.isValid() ? model->GetAnimation(index) : NULL);
        }
    });
}

void AnimationEditor::deinitAnimationList(SimpleEditableListWidget *list, AnimationListModel *model)
{
    disconnect(model, 0, 0, 0);
    disconnect(list, &SimpleEditableListWidget::signal_AddClicked, 0, 0);
    disconnect(list, &SimpleEditableListWidget::signal_RemoveClicked, 0, 0);
    disconnect(list, &SimpleEditableListWidget::signal_selectionChanged, 0, 0);
    list->setModel(NULL);
    list->setAddEnabled(false);
    list->setRemoveEnabled(false);
}

void AnimationEditor::populateFrameSelection()
{
    QModelIndex index = m_frameAnimList->selectedIndex();
    m_frameSelectionWidget->setEnabled(index.isValid());
    if(!index.isValid())
    {
        m_frameSelectionWidget->Clear();
        return;
    }

    const AnimationFrameByFrame& anim = *m_frameAnimModel->GetAnimationFrameByFrame(index);
    m_frameSelectionWidget->PopulateFrames(m_project->GetImageDefinition(), anim);
    RefreshAvailableImages();
}

void AnimationEditor::updateFrameByFrameAnimationDefinition()
{
    QModelIndex index = m_frameAnimList->selectedIndex();
    if(!index.isValid())
        return;

    AnimationFrameByFrame& anim = *m_frameAnimModel->GetAnimationFrameByFrame(index);
    anim.Clear();
    foreach(quint32 imageId, m_frameSelectionWidget->GetFrames())
        anim.AddImageId(imageId);
}
