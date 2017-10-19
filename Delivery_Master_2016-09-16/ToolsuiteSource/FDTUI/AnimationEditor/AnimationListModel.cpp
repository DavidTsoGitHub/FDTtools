#include "AnimationListModel.h"

#include "Animation.h"
#include "AnimationDefinition.h"

AnimationListModel::AnimationListModel(AnimationDefinition &animDef, Type type, QObject *parent)
    : QAbstractListModel(parent), m_animationDefinition(animDef), m_type(type)
{
    connect(this, &AnimationListModel::rowsAboutToBeInserted, this, &AnimationListModel::signal_modified);
    connect(this, &AnimationListModel::rowsAboutToBeRemoved, this, &AnimationListModel::signal_modified);
    connect(this, &AnimationListModel::rowsAboutToBeMoved, this, &AnimationListModel::signal_modified);
    connect(this, &AnimationListModel::modelAboutToBeReset, this, &AnimationListModel::signal_modified);
}

AnimationListModel::~AnimationListModel()
{
}

Animation *AnimationListModel::AddAnimation()
{
    Animation* animation = NULL;

    if(m_type == Type::FrameByFrame)
    {
        int count = m_animationDefinition.FrameByFrameAnimations().count();
        beginInsertRows(QModelIndex(), count, count);
        animation = m_animationDefinition.AddAnimationFrameByFrame();
        endInsertRows();
    }
    else if(m_type == Type::Opacity)
    {
        int count = m_animationDefinition.OpacityAnimations().count();
        beginInsertRows(QModelIndex(), count, count);
        animation = m_animationDefinition.AddAnimationOpacity();
        endInsertRows();
    }
    else if(m_type == Type::Translation)
    {
        int count = m_animationDefinition.TranslationAnimations().count();
        beginInsertRows(QModelIndex(), count, count);
        animation = m_animationDefinition.AddAnimationTranslation();
        endInsertRows();
    }

    return animation;
}

Animation *AnimationListModel::GetAnimation(const QModelIndex &index)
{
    if(m_type == Type::FrameByFrame)
        return m_animationDefinition.FrameByFrameAnimations().at(index.row());

    else if(m_type == Type::Opacity)
        return m_animationDefinition.OpacityAnimations().at(index.row());

    else if(m_type == Type::Translation)
        return m_animationDefinition.TranslationAnimations().at(index.row());

    return NULL;
}

AnimationFrameByFrame *AnimationListModel::GetAnimationFrameByFrame(const QModelIndex &index) const
{
    if(!index.isValid())
        return NULL;

    return m_animationDefinition.FrameByFrameAnimations().at(index.row());
}

AnimationOpacity *AnimationListModel::GetAnimationOpacity(const QModelIndex &index) const
{
    if(!index.isValid())
        return NULL;

    return m_animationDefinition.OpacityAnimations().at(index.row());
}

AnimationTranslation *AnimationListModel::GetAnimationTranslation(const QModelIndex &index) const
{
    if(!index.isValid())
        return NULL;

    return m_animationDefinition.TranslationAnimations().at(index.row());
}

void AnimationListModel::RemoveAnimation(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    beginRemoveRows(QModelIndex(), index.row(), index.row());

    quint32 animationID = GetAnimation(index)->GetID();

    if(m_type == Type::FrameByFrame)
        m_animationDefinition.RemoveFrameByFrameAnimation(m_animationDefinition.FrameByFrameAnimations().at(index.row()));

    else if(m_type == Type::Opacity)
        m_animationDefinition.RemoveOpacityAnimation(m_animationDefinition.OpacityAnimations().at(index.row()));

    else if(m_type == Type::Translation)
        m_animationDefinition.RemoveTranslationAnimation(m_animationDefinition.TranslationAnimations().at(index.row()));

    endRemoveRows();

    emit m_animationDefinition.signal_AnimationRemoved(animationID);
}

void AnimationListModel::Refresh()
{
    beginResetModel();
    endResetModel();
}

int AnimationListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if(m_type == Type::FrameByFrame)
        return m_animationDefinition.FrameByFrameAnimations().size();

    else if(m_type == Type::Opacity)
        return m_animationDefinition.OpacityAnimations().size();

    else if(m_type == Type::Translation)
        return m_animationDefinition.TranslationAnimations().size();

    return 0;
}

QVariant AnimationListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    // Name
    if (role == Qt::DisplayRole) {
        if(m_type == Type::FrameByFrame)
            return QVariant(m_animationDefinition.FrameByFrameAnimations().at(index.row())->GetName());

        else if(m_type == Type::Opacity)
            return QVariant(m_animationDefinition.OpacityAnimations().at(index.row())->GetName());

        else if(m_type == Type::Translation)
            return QVariant(m_animationDefinition.TranslationAnimations().at(index.row())->GetName());

    }

    return QVariant();
}

QVariant AnimationListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();
}
