#ifndef ANIMATION
#define ANIMATION

#include "ProjectObject.h"
#include "IGCLSignalSource.h"

#include <QList>

class GCLSignalSink;

class Animation : public ProjectObject, public IGCLSignalSource
{
public:
    enum class Type { FrameByFrame, Opacity, Translation };
    enum class RepeatMode { ONESHOT, LOOP, BOUNCE, BOUNCE_LOOP };

    typedef QList<Animation*> List;
    explicit Animation();
    virtual ~Animation() = 0;

    quint32 GetID(void) const;
    void SetID(const quint32 id);

    const QString GetName(void) const;
    void SetName(const QString& name);

    void SetRepeatMode(RepeatMode mode);
    RepeatMode GetRepeatMode() const;

    void SetDefaultStep(int value);
    int GetDefaultStep(void) const;

    void UpdateAllInternalSinks();
    void UpdateAllConnectedSinks();
    void AddGCLSignalSink(GCLSignalSink *sink);
    void AddGCLSignalSource(GCLSignalSource *source);

    static quint32 GetNewID();

    static quint32 GetAnimationCount();
    static void SetAnimationCount(quint32 id);
    static void ResetAnimationCount();

    virtual Type GetAnimationType() const = 0;

    // IGCLSignalSource interface
    const QString GetComponentName() const override;
    const QList<GCLSignalSource *> &GetSignalSources() const override;
    GCLSignalSource *GetSignalSource(QString signalName) const override;

protected:
    void AddSignalSource(QString signalName, QString dataType);
    void RemoveSignalSource(QString signalName);

private:
    quint32 m_id;
    QString m_name;
    QString m_gclComponentName;
    RepeatMode m_repeatMode;
    quint32 m_defaultStep;

    static quint32 animationCount;
    QList<GCLSignalSource *> m_sources;


    // IGCLSignalSource interface
public:
};


#endif // ANIMATION

