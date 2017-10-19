#ifndef UIWIDGETBASE_H
#define UIWIDGETBASE_H

#include "UIWidgetDefinitionBase.h"
#include "GraphicsItemBase.h"
#include "IGCLSignalSink.h"
#include "IGCLSignalSource.h"
#include "ISelfValidation.h"
#include "TransitionClause.h"

class ColorPair;
class Project;

#include <QColor>
#include <QString>

class UIWidgetBase : public ProjectObject, public IGCLSignalSink, public IGCLSignalSource, public ISelfValidation
{

public:

    typedef enum
    {
        Not_Available = 1ul << 0,
        LineEdit_Editable = 1ul << 1,
        LineEdit_NonEditable = 1ul << 2,
        SpinBox_Uint8 = 1ul << 3,
        SpinBox_Uint16 = 1ul << 4,
        SpinBox_Uint32 = 1ul << 5,
        SpinBox_Int8 = 1ul << 6,
        SpinBox_Int16 = 1ul << 7,
        SpinBox_Int32 = 1ul << 8,
        SpinBox_DynamicValueType = 1ul << 9,
        SignalSink = 1ul << 10,
        CheckBox = 1ul << 11,
        CompilerComboBox = 1ul << 12,
        IconsComboBox = 1ul << 13,
        ImagesComboBox = 1ul << 14,
        LabelsComboBox = 1ul << 15,
        FontComboBox = 1ul << 16,
        SignalSinkDataTypeCombobox = 1ul << 17,
        FileSelector = 1ul << 18,
        FileSelector_ProjectJailed = 1ul << 19,
        FolderSelector = 1ul << 20,
        FolderSelector_ProjectJailed = 1ul << 21,
        SpinBox_Uint5_Hex = 1ul << 22,
        SpinBox_Uint6_Hex = 1ul << 23,
        TextAlignmentComboBox = 1ul << 24,
        AnimationComboBox = 1ul << 25,
        DynamicImageAnimationComboBox = 1ul << 26,
        AnimationCheckBox = 1ul << 27,
        AnimationTriggers = 1ul << 28,
        ColorPreview_Label = 1ul << 29,
        SpinBox_Uint8_Min1 = 1ul << 30
    } DataType_t;

    typedef enum
    {
        BPP_1,
        BPP_2,
        BPP_4,
        BPP_8,
        BPP_16_565,
        BPP_24,
        BPP_32,
        BPP_COUNT
    } BitplaneDefinition_t;

    typedef enum
    {
        LeftAligned = 1,
        CenterAligned = 2,
        RightAligned = 3
    } TextAlignment_t;

    typedef enum
    {
        UIWidget_Type_BlockBar,
        UIWidget_Type_ColumnLevel,
        UIWidget_Type_DynamicIcon,
        UIWidget_Type_DynamicImage,
        UIWidget_Type_DynamicText,
        UIWidget_Type_DynamicValue,
        UIWidget_Type_Icon,
        UIWidget_Type_Image,
        UIWidget_Type_Label,
        UIWidget_Type_ListNavigation,
        UIWidget_Type_SelectableList,
        UIWidget_Type_SignalTrigger,
        UIWIdget_Type_DynamicLabel,
    } uiWidgetType_t;

    typedef enum
    {
        NameProperty,
        GCLNameProperty,
        XProperty,
        YProperty,
        WidthProperty,
        HeightProperty,
        IsBoxedProperty,
        PrimaryFGColorRedProperty,
        PrimaryFGColorGreenProperty,
        PrimaryFGColorBlueProperty,
        PrimaryBGIsTransparent,
        PrimaryBGColorRedProperty,
        PrimaryBGColorGreenProperty,
        PrimaryBGColorBlueProperty,
        SecondaryFGColorRedProperty,
        SecondaryFGColorGreenProperty,
        SecondaryFGColorBlueProperty,
        SecondaryBGIsTransparent,
        SecondaryBGColorRedProperty,
        SecondaryBGColorGreenProperty,
        SecondaryBGColorBlueProperty,
        WidgetBasePropertyCount
    } propertyIndex_t;

    typedef enum
    {
        DO_NOTHING = 0,
        RUN_ONE_CYCLE = 1,
        SHOW_DEFAULT = 2
    } AnimationOnLoadBehavior;

    typedef enum
    {
        START_FROM_BEGINNING,
        START_FROM_DEFAULT_FRAME,
        CONTINUE_FROM_CURRENT_FRAME
    } AnimationOnTriggerActiveBehavior;

    typedef enum
    {
        STOP = 0,
        STOP_AT_END_OF_CYCLE = 1,
        STOP_AT_DEFAULT_FRAME = 2,
        CONTINUE = 3
    } AnimationOnTriggerInactiveBehavior;

    explicit UIWidgetBase(uiWidgetType_t uiWidgetType, quint32 uiWidgetId = 0, ProjectObject* parent = 0);
    virtual ~UIWidgetBase(void);

    UIWidgetBase* Clone(void);

    quint32 GetUIWidgetID(void) const;

    static void ResetWidgetIDCounter(void);

    QString GetUserDefinedName(void);
    void SetUserDefinedName(QString userDefinedName);

    const QString GetDescription() const;
    void SetDescription(QString description);

    quint16 GetX(void);
    void SetX(quint16 x);
    quint16 GetY(void);
    void SetY(quint16 y);

    quint16 GetWidth(void) const;
    void SetWidth(const quint16 width);

    quint16 GetHeight(void) const;
    void SetHeight(const quint16 height);

    void SetPrimaryColorPair(ColorPair* colorPair);
    ColorPair* GetPrimaryColorPair(void);

    void SetUseSecondaryColorPair(bool useSecondaryColorPair);
    bool GetUseSecondaryColorPair(void);

    void SetSecondaryColorPair(ColorPair* colorPair);
    ColorPair* GetSecondaryColorPair(void);

    bool GetBoxed(void) const;
    void SetBoxed(const bool boxed);

    bool GetCanHandleFocus(void) const;
    void SetCanHandleFocus(bool canHandleFocus);

    quint8 GetNbrOfLayersNeeded(void) const;
    void SetNbrOfLayersNeeded(quint8 nbrOfLayers);

    BitplaneDefinition_t GetBPPMode() const;
    void SetBPPMode(BitplaneDefinition_t bppMode);

    uiWidgetType_t GetUIWidgetType(void) const;
    QString GetUIWidgetUniqueName(void) const;

    int GetTotalPropertyCount() const;
    QVariant GetPropertyData(int index);
    DataType_t GetPropertyDataType(int index) const;
    QString GetPropertyHeader(int index) const;
    void SetProperty(int index, QVariant value);

    void UpdateAllInternalSinks();
    void UpdateAllConnectedSinks();
    void AddGCLSignalSink(GCLSignalSink *sink);
    void AddGCLSignalSource(GCLSignalSource *source);

    static QString GetOnLoadBehaviorString(UIWidgetBase::AnimationOnLoadBehavior behavior);
    static QString GetOnTriggerActiveBehaviorString(UIWidgetBase::AnimationOnTriggerActiveBehavior behavior);
    static QString GetOnTriggerInactiveBehaviorString(UIWidgetBase::AnimationOnTriggerInactiveBehavior behavior);

    virtual GraphicsItemBase* GetGraphicsItem(bool drawChromaKeyColor = true);
    virtual QString ShortDescription(void);
    virtual QString LongDescription(void);

    virtual quint32 GetAnimationID(quint32 animationNumber);
    virtual bool GetAnimationUseTriggers(quint32 animationNumber);
    virtual void SetAnimationOnLoadBehavior(quint32 animationNumber, AnimationOnLoadBehavior behavior);
    virtual AnimationOnLoadBehavior GetAnimationOnLoadBehavior(quint32 animationNumber);
    virtual void SetAnimationOnTriggerActiveBehavior(quint32 animationNumber, AnimationOnTriggerActiveBehavior behavior);
    virtual AnimationOnTriggerActiveBehavior GetAnimationOnTriggerActiveBehavior(quint32 animationNumber);
    virtual void SetAnimationOnTriggerInactiveBehavior(quint32 animationNumber, AnimationOnTriggerInactiveBehavior behavior);
    virtual AnimationOnTriggerInactiveBehavior GetAnimationOnTriggerInactiveBehavior(quint32 animationNumber);
    virtual const QList<TransitionClause *> &GetAnimationTriggers(quint32 animationNumber);
    virtual void AddAnimationTrigger(quint32 animationNumber, TransitionClause *trigger);
    virtual void RemoveAnimationTrigger(quint32 animationNumber, TransitionClause *trigger);
    virtual void SetAnimationID(quint8 animationNumber, quint32 animationID);
    virtual void SetAnimationUseTriggers(quint8 animationNumber, bool useTriggers);

    virtual QString GetSecondaryColorPairForegroundHeader();
    virtual QString GetSecondaryColorPairBackgroundHeader();

    virtual QString GetUIWidgetTypeName(void) const = 0;
    virtual QVariant GetWidgetPropertyData(int index) const = 0;
    virtual QString GetWidgetPropertyHeader(int index) const = 0;
    virtual int GetWidgetPropertyCount() const = 0;
    virtual void SetWidgetProperty(int index, QVariant value) = 0;
    virtual DataType_t GetWidgetPropertyDataType(int index) const = 0;
    virtual QString WidgetPropertyToolTip(int index) const = 0;

    static const int UShort_Mask;
    static const int UInt_Mask;
    static const int Short_Mask;
    static const int Int_Mask;
    static const int String_Mask;
    static const int Bool_Mask;

protected:
    static quint32 uiWidgetCounter;
    virtual UIWidgetBase* CloneThis(quint32 widgetId) = 0;

    QList<GCLSignalSink*> m_sinks;
    QList<GCLSignalSource*> m_sources;

    void AddSignalSink(QString signalName, QString dataType, bool updateNotification, bool mandatory);
    void AddSignalSource(QString signalName, QString dataType);
    void RemoveSignalSource(QString signalName);

private:
    quint32 m_uiWidgetID;
    uiWidgetType_t m_uiWidgetType;

    QString m_userDefinedName;
    QString m_description;
    QString m_componentName;
    quint16 m_x;
    quint16 m_y;
    quint16 m_width;
    quint16 m_height;

    ColorPair* m_primaryColorPair;
    ColorPair* m_secondaryColorPair;

    bool m_isBoxed;
    quint8 m_nbrOfLayersNeeded;
    BitplaneDefinition_t m_bitsPerPixel;
    bool m_canHandleFocus;

    bool m_useSecondaryColorSet;
    ColorPair* secondaryColorSet;

    QList<TransitionClause*> m_emptyList;

public:
    // IGCLSignalSink interface
    const QList<GCLSignalSink*>& GetSignalSinks() const;
    GCLSignalSink* GetSignalSink(QString signalName) const;

    // IGCLSignalSource interface
    const QString GetComponentName() const;
    const QList<GCLSignalSource*>& GetSignalSources() const;
    GCLSignalSource* GetSignalSource(QString signalName) const;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);

    // ISelfValidation interface
public:
    bool objectSelfValidation(Project* project, QStringList *errorList);
};

#endif // UIWIDGETBASE_H
