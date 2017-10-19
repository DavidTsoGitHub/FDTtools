#ifndef LAYOUTDEFINITION
#define LAYOUTDEFINITION

#include "IGCLSignalSink.h"
#include "IRunnable.h"
#include "ISelfValidation.h"
#include "ProjectObject.h"

class DisplayArea;
class MenuDefinition;
class Layout;

class QString;

enum class ColorComponent;

template <typename T> class QList;

class LayoutDefinition : public ProjectObject, public IGCLSignalSink, public IRunnable, public ISelfValidation
{
    Q_OBJECT

    typedef enum
    {
        InputUp,
        InputDown,
        InputLeft,
        InputRight,
        InputSet,
        InputClear
    } ButtonInput;

public:

    typedef enum
    {
        Regular, Startup, CheckMessage
    }LayoutType;

    explicit LayoutDefinition(ProjectObject* parent = 0);
    virtual ~LayoutDefinition();

    quint32 GetHighestID(void) const;
    void SetHighestID(quint32 value);

    quint16 CropLeft() const;
    void SetCropLeft(quint16 value);
    quint16 CropRight() const;
    void SetCropRight(quint16 value);
    quint16 CropTop() const;
    void SetCropTop(quint16 value);
    quint16 CropBottom() const;
    void SetCropBottom(quint16 value);

    Layout *GetMainLayout() const;
    void SetMainLayout(Layout *layout);

    bool StartupEnabled() const;
    void SetStartupEnabled(bool enabled);
    quint8 StartupScreenDisplayTime() const;
    void SetStartupScreenDisplayTime(quint8 value);

    const QList<Layout*> Layouts() const;
    void AddLayout(Layout* layout);
    void RemoveLayout(Layout* layout);
    void MoveLayoutUp(Layout *layout);
    void MoveLayoutDown(Layout *layout);


    quint32 GetID(void) const;
    void SetID(quint32 value);

    Layout* GetStartupLayout() const;
    void SetStartupLayout(Layout *startupLayout);
    bool HasStartupLayout(void);
    Layout *GetLayoutFromID(quint32 id) const;
    Layout *GetLayoutFromIndex(int index) const;

    void SetCheckMessageLayout(Layout* checkmessageLayout);
    Layout* GetCheckMessageLayout(void) const;
    bool HasCheckMessageLayout(void);
    QList<Layout*> GetValidLayoutsForCheckMessages() const;
    QList<quint32> GetValidLayoutIDsForCheckMessages(void);

    bool HasMenu(void) const;
    Layout* GetMenuLayout();
    DisplayArea* GetMenuArea(void) const;
    MenuDefinition* GetMenu(void) const;

    QList<quint32> GetCLUT() const;
    quint16 appendCLUTGetIndex(quint32 value);

    QColor GetStartChromaKeyColor() const;
    QColor GetEndChromaKeyColor() const;
    void SetStartChromaKeyColorComponent(ColorComponent colorComponent, quint8 value);
    void SetEndChromaKeyColorComponent(ColorComponent colorComponent, quint8 value);
	
signals:
    void signal_LayoutPriorityUpdated(void);

private:
    void moveLayoutToHighestPriority(Layout *layout);
    void moveLayoutToLowestPriority(Layout *layout);
    void addInputToCommandMapping(ButtonInput input);
    QString getSignalNameFromButtonInput(ButtonInput input);
    void removeLayoutFromButtonTransitions(quint8 layoutID);
    void updateButtonTransitionLayoutMappings(qint32 currentIndex, qint32 newIndex);
    quint32 getNextLayoutID(void);
    void animationRemoved(quint32 animationID);

    void addGCLSignalSink(GCLSignalSink* sink);

    quint32 m_highestID;
    quint16 m_cropTop;
    quint16 m_cropBottom;
    quint16 m_cropLeft;
    quint16 m_cropRight;
    Layout* m_mainLayout;
    bool m_startupEnabled;
    quint8 m_startupScreenDisplayTime;
    bool m_checkMessageEnabled;

	QList<Layout*> m_layouts;    
    QList<const Task*> m_tasks;
    QList<quint32> m_clutList;
    QList<GCLSignalSink*> m_signalSinks;
    QColor m_startChromaKeyColor;
    QColor m_endChromaKeyColor;

    // IGCLSignalSink interface
public:
    const QList<GCLSignalSink *>& GetSignalSinks() const;
    GCLSignalSink* GetSignalSink(QString signalName) const;

    // IRunnable interface
public:
    const QList<const Task*>& Tasks() const;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);

    // ISelfValidation interface
public:
    bool objectSelfValidation(Project* project, QStringList *errorList);
};

#endif // LAYOUTDEFINITION
