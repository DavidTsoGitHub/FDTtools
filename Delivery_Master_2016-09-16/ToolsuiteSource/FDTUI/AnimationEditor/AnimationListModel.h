#ifndef PROJECTOBJECTLISTMODEL_H
#define PROJECTOBJECTLISTMODEL_H

#include <QAbstractListModel>

class Animation;
class AnimationDefinition;
class AnimationFrameByFrame;
class AnimationOpacity;
class AnimationTranslation;

class AnimationListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum class Type { FrameByFrame, Opacity, Translation };

    AnimationListModel(AnimationDefinition &animDef, Type type, QObject *parent = Q_NULLPTR);
    ~AnimationListModel();

    Animation* AddAnimation();
    Animation* GetAnimation(const QModelIndex &index);
    AnimationFrameByFrame* GetAnimationFrameByFrame(const QModelIndex &index) const;
    AnimationOpacity *GetAnimationOpacity(const QModelIndex &index) const;
    AnimationTranslation *GetAnimationTranslation(const QModelIndex &index) const;
    void RemoveAnimation(const QModelIndex &index);
    void Refresh();

signals:
    void signal_modified();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    AnimationDefinition& m_animationDefinition;
    Type m_type;

};

#endif // PROJECTOBJECTLISTMODEL_H
