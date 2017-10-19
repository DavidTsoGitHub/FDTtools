#include "UIWidgetBase.h"
#include "Logger.h"
#include "ProjectObjectVisitor.h"
#include "ProjectValidator.h"
#include "ColorPair.h"

quint32 UIWidgetBase::uiWidgetCounter = 0;

UIWidgetBase::UIWidgetBase(uiWidgetType_t uiWidgetType, quint32 uiWidgetId, ProjectObject* parent) :
    ProjectObject(QString("UIWidget"), parent),
    m_uiWidgetID(uiWidgetId),
    m_uiWidgetType(uiWidgetType),
    m_userDefinedName(""),
    m_description(""),
    m_componentName(""),
    m_x(0),
    m_y(0),
    m_width(0),
    m_height(0),
    m_primaryColorPair(new ColorPair(Qt::white, Qt::black, true, this)),
    m_secondaryColorPair(new ColorPair(Qt::black, Qt::white, true, this)),
    m_isBoxed(false),
    m_nbrOfLayersNeeded(0),
    m_bitsPerPixel(BPP_8),
    m_canHandleFocus(false),
    m_useSecondaryColorSet(false)
{
    if (UIWidgetBase::uiWidgetCounter < uiWidgetId)
    {
        UIWidgetBase::uiWidgetCounter = uiWidgetId;
    }
}

UIWidgetBase::~UIWidgetBase()
{
    while (!m_sinks.isEmpty())
    {
        delete m_sinks.takeFirst();
    }

    while (!m_sources.isEmpty())
    {
        delete m_sources.takeFirst();
    }
}

UIWidgetBase* UIWidgetBase::Clone()
{
    quint32 widgetId = ++UIWidgetBase::uiWidgetCounter;
    return CloneThis(widgetId);
}

void UIWidgetBase::ResetWidgetIDCounter()
{
    UIWidgetBase::uiWidgetCounter = 0;
}

quint32 UIWidgetBase::GetUIWidgetID() const
{
    return m_uiWidgetID;
}

QString UIWidgetBase::GetUserDefinedName()
{
    return m_userDefinedName;
}

void UIWidgetBase::SetUserDefinedName(QString userDefinedName)
{
    setObjectName(userDefinedName);
    m_userDefinedName = userDefinedName;
    m_componentName = "Widgets_" + QString::number(m_uiWidgetID) + "_" + m_userDefinedName.replace(" ", "");
    UpdateAllInternalSinks();
    UpdateAllConnectedSinks();
    NotifyPropertyUpdated();
}

const QString UIWidgetBase::GetDescription() const
{
    return m_description;
}

void UIWidgetBase::SetDescription(QString description)
{
    m_description = description;
}

UIWidgetBase::uiWidgetType_t UIWidgetBase::GetUIWidgetType() const
{
    return m_uiWidgetType;
}

quint16 UIWidgetBase::GetX()
{
    return m_x;
}

void UIWidgetBase::SetX(quint16 x)
{
    m_x = x;
    NotifyPropertyUpdated();
}

quint16 UIWidgetBase::GetY()
{
    return m_y;
}

void UIWidgetBase::SetY(quint16 y)
{
    m_y = y;
    NotifyPropertyUpdated();
}

quint16 UIWidgetBase::GetWidth() const
{
    return m_width;
}

void UIWidgetBase::SetWidth(const quint16 width)
{
    m_width = width;
    NotifyPropertyUpdated();
}

quint16 UIWidgetBase::GetHeight() const
{
    return m_height;
}

void UIWidgetBase::SetHeight(const quint16 height)
{
    m_height = height;
    NotifyPropertyUpdated();
}

void UIWidgetBase::SetPrimaryColorPair(ColorPair* colorPair)
{
    m_primaryColorPair = colorPair;
}

ColorPair*UIWidgetBase::GetPrimaryColorPair()
{
    return m_primaryColorPair;
}

void UIWidgetBase::SetUseSecondaryColorPair(bool useSecondaryColorPair)
{
    m_useSecondaryColorSet = useSecondaryColorPair;
}

bool UIWidgetBase::GetUseSecondaryColorPair(void)
{
    return m_useSecondaryColorSet;
}

void UIWidgetBase::SetSecondaryColorPair(ColorPair* colorPair)
{
    m_secondaryColorPair = colorPair;
}

ColorPair* UIWidgetBase::GetSecondaryColorPair()
{
    return m_secondaryColorPair;
}

bool UIWidgetBase::GetBoxed() const
{
    return m_isBoxed;
}

void UIWidgetBase::SetBoxed(const bool boxed)
{
    m_isBoxed = boxed;
    NotifyPropertyUpdated();
}

bool UIWidgetBase::GetCanHandleFocus() const
{
    return m_canHandleFocus;
}

void UIWidgetBase::SetCanHandleFocus(bool canHandleFocus)
{
    m_canHandleFocus = canHandleFocus;
}

quint8 UIWidgetBase::GetNbrOfLayersNeeded() const
{
    return m_nbrOfLayersNeeded;
}

void UIWidgetBase::SetNbrOfLayersNeeded(quint8 nbrOfLayers)
{
    m_nbrOfLayersNeeded = nbrOfLayers;
}

UIWidgetBase::BitplaneDefinition_t UIWidgetBase::GetBPPMode() const
{
    return m_bitsPerPixel;
}

void UIWidgetBase::SetBPPMode(BitplaneDefinition_t bppMode)
{
    m_bitsPerPixel = bppMode;
}

void UIWidgetBase::AddSignalSink(QString signalName, QString dataType, bool updateNotification, bool mandatory)
{
    GCLSignalSink* sink = new GCLSignalSink(signalName, m_componentName, dataType, updateNotification, mandatory);
    m_sinks.append(sink);
    sink->SetParent(this);
    NotifyPropertyUpdated();
}

void UIWidgetBase::AddSignalSource(QString signalName, QString dataType)
{
    GCLSignalSource* source = new GCLSignalSource(signalName, m_componentName, dataType, this);
    m_sources.append(source);
    source->SetParent(this);
    NotifyPropertyUpdated();
}

void UIWidgetBase::RemoveSignalSource(QString signalName)
{
    int i = 0;
    foreach (const GCLSignalSource* source, m_sources)
    {
        if (0 == source->SignalName().compare(signalName))
        {
            m_sources.removeAt(i);
            NotifyPropertyUpdated();
            delete source;
        }
        ++i;
    }
}

QString UIWidgetBase::GetUIWidgetUniqueName() const
{
    return m_componentName;
}

int UIWidgetBase::GetTotalPropertyCount() const
{
    return WidgetBasePropertyCount + GetWidgetPropertyCount();
}

QVariant UIWidgetBase::GetPropertyData(int index)
{
    switch (index)
    {
        case NameProperty:
            return GetUserDefinedName();
        case GCLNameProperty:
            return GetUIWidgetUniqueName();
        case XProperty:
            return GetX();
        case YProperty:
            return GetY();
        case WidthProperty:
            return GetWidth();
        case HeightProperty:
            return GetHeight();
        case IsBoxedProperty:
            return GetBoxed();
        case PrimaryFGColorRedProperty:
            return GetPrimaryColorPair()->GetFGColor().red();
        case PrimaryFGColorGreenProperty:
            return GetPrimaryColorPair()->GetFGColor().green();
        case PrimaryFGColorBlueProperty:
            return GetPrimaryColorPair()->GetFGColor().blue();
        case PrimaryBGIsTransparent:
            return GetPrimaryColorPair()->GetBGIsTransparent();
        case PrimaryBGColorRedProperty:
            return GetPrimaryColorPair()->GetBGColor().red();
        case PrimaryBGColorGreenProperty:
            return GetPrimaryColorPair()->GetBGColor().green();
        case PrimaryBGColorBlueProperty:
            return GetPrimaryColorPair()->GetBGColor().blue();
        case SecondaryFGColorRedProperty:
            return GetSecondaryColorPair()->GetFGColor().red();
        case SecondaryFGColorGreenProperty:
            return GetSecondaryColorPair()->GetFGColor().green();
        case SecondaryFGColorBlueProperty:
            return GetSecondaryColorPair()->GetFGColor().blue();
        case SecondaryBGIsTransparent:
            return GetSecondaryColorPair()->GetBGIsTransparent();
        case SecondaryBGColorRedProperty:
            return GetSecondaryColorPair()->GetBGColor().red();
        case SecondaryBGColorGreenProperty:
            return GetSecondaryColorPair()->GetBGColor().green();
        case SecondaryBGColorBlueProperty:
            return GetSecondaryColorPair()->GetBGColor().blue();
    default:
            int transposedPropertyIndex = index - WidgetBasePropertyCount;
            return GetWidgetPropertyData(transposedPropertyIndex);
    }
}

UIWidgetBase::DataType_t UIWidgetBase::GetPropertyDataType(int index) const
{
    switch (index)
    {
        case NameProperty:
            return LineEdit_Editable;
        case GCLNameProperty:
            return LineEdit_NonEditable;
        case XProperty:
            return SpinBox_Uint16;
        case YProperty:
            return SpinBox_Uint16;
        case WidthProperty:
            return SpinBox_Uint16;
        case HeightProperty:
            return SpinBox_Uint16;
        case IsBoxedProperty:
            return CheckBox;
        case PrimaryFGColorRedProperty:
            return SpinBox_Uint5_Hex;
        case PrimaryFGColorGreenProperty:
            return SpinBox_Uint6_Hex;
        case PrimaryFGColorBlueProperty:
            return SpinBox_Uint5_Hex;
        case PrimaryBGIsTransparent:
            return CheckBox;
        case PrimaryBGColorRedProperty:
            return SpinBox_Uint5_Hex;
        case PrimaryBGColorGreenProperty:
            return SpinBox_Uint6_Hex;
        case PrimaryBGColorBlueProperty:
            return SpinBox_Uint5_Hex;
        case SecondaryFGColorRedProperty:
            return SpinBox_Uint5_Hex;
        case SecondaryFGColorGreenProperty:
            return SpinBox_Uint6_Hex;
        case SecondaryFGColorBlueProperty:
            return SpinBox_Uint5_Hex;
        case SecondaryBGIsTransparent:
            return CheckBox;
        case SecondaryBGColorRedProperty:
            return SpinBox_Uint5_Hex;
        case SecondaryBGColorGreenProperty:
            return SpinBox_Uint6_Hex;
        case SecondaryBGColorBlueProperty:
            return SpinBox_Uint5_Hex;
        default:
            int transposedPropertyIndex = index - WidgetBasePropertyCount;
            return GetWidgetPropertyDataType(transposedPropertyIndex);
    }
}

QString UIWidgetBase::GetPropertyHeader(int index) const
{
    switch (index)
    {
        case NameProperty:
            return "Name";
        case GCLNameProperty:
            return "GCL Name";
        case XProperty:
            return "X";
        case YProperty:
            return "Y";
        case WidthProperty:
            return "Width";
        case HeightProperty:
            return "Height";
        case IsBoxedProperty:
            return "Boxed";
        case PrimaryFGColorRedProperty:
            return "Primary Foreground Red";
        case PrimaryFGColorGreenProperty:
            return "Primary Foreground Green";
        case PrimaryFGColorBlueProperty:
            return "Primary Foreground Blue";
        case PrimaryBGIsTransparent:
            return "Primary Background Is Transparent";
        case PrimaryBGColorRedProperty:
            return "Primary Background Red";
        case PrimaryBGColorGreenProperty:
            return "Primary Background Green";
        case PrimaryBGColorBlueProperty:
            return "Primary Background Blue";
        case SecondaryFGColorRedProperty:
            return "Secondary Foreground Red";
        case SecondaryFGColorGreenProperty:
            return "Secondary Foreground Green";
        case SecondaryFGColorBlueProperty:
            return "Secondary Foreground Blue";
        case SecondaryBGIsTransparent:
            return "Secondary Background Is Transparent";
        case SecondaryBGColorRedProperty:
            return "Secondary Background Red";
        case SecondaryBGColorGreenProperty:
            return "Secondary Background Green";
        case SecondaryBGColorBlueProperty:
            return "Secondary Background Blue";
        default:
            int transposedPropertyIndex = index - WidgetBasePropertyCount;
            return GetWidgetPropertyHeader(transposedPropertyIndex);
    }
}

void UIWidgetBase::SetProperty(int index, QVariant value)
{
    switch (index)
    {
        case NameProperty:
            if (value.canConvert<QString>())
            {
                SetUserDefinedName(value.toString());
            }
            break;
        case GCLNameProperty:
            // Not editable
            break;
        case XProperty:
            if (value.canConvert<int>())
            {
                SetX(value.toInt());
            }
            break;
        case YProperty:
            if (value.canConvert<int>())
            {
                SetY(value.toInt());
            }
            break;
        case WidthProperty:
            if (value.canConvert<int>())
            {
                SetWidth(value.toInt());
            }
            break;
        case HeightProperty:
            if (value.canConvert<int>())
            {
                SetHeight(value.toInt());
            }
            break;
        case IsBoxedProperty:
            if (value.canConvert<bool>())
            {
                SetBoxed(value.toBool());
            }
            break;
        case PrimaryFGColorRedProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::RED;
                GetPrimaryColorPair()->SetFGColorComponent(color, value.toUInt());
            }
            break;
        case PrimaryFGColorGreenProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::GREEN;
                GetPrimaryColorPair()->SetFGColorComponent(color, value.toUInt());
            }
            break;
        case PrimaryFGColorBlueProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::BLUE;
                GetPrimaryColorPair()->SetFGColorComponent(color, value.toUInt());
            }
            break;
        case PrimaryBGIsTransparent:
        {
            if (value.canConvert<bool>())
            {
                GetPrimaryColorPair()->SetBGIsTransparent(value.toBool());
            }
        }
        case PrimaryBGColorRedProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::RED;
                GetPrimaryColorPair()->SetBGColorComponent(color, value.toUInt());
            }
            break;
        case PrimaryBGColorGreenProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::GREEN;
                GetPrimaryColorPair()->SetBGColorComponent(color, value.toUInt());
            }
            break;
        case PrimaryBGColorBlueProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::BLUE;
                GetPrimaryColorPair()->SetBGColorComponent(color, value.toUInt());
            }
            break;
        case SecondaryFGColorRedProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::RED;
                GetSecondaryColorPair()->SetFGColorComponent(color, value.toUInt());
            }
            break;
        case SecondaryFGColorGreenProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::GREEN;
                GetSecondaryColorPair()->SetFGColorComponent(color, value.toUInt());
            }
            break;
        case SecondaryFGColorBlueProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::BLUE;
                GetSecondaryColorPair()->SetFGColorComponent(color, value.toUInt());
            }
            break;
        case SecondaryBGIsTransparent:
        {
            if (value.canConvert<bool>())
            {
                GetSecondaryColorPair()->SetBGIsTransparent(value.toBool());
            }
        }
        case SecondaryBGColorRedProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::RED;
                GetSecondaryColorPair()->SetBGColorComponent(color, value.toUInt());
            }
            break;
        case SecondaryBGColorGreenProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::GREEN;
                GetSecondaryColorPair()->SetBGColorComponent(color, value.toUInt());
            }
            break;
        case SecondaryBGColorBlueProperty:
            if (value.canConvert<quint8>())
            {
                ColorComponent color = ColorComponent::BLUE;
                GetSecondaryColorPair()->SetBGColorComponent(color, value.toUInt());
            }
            break;
        default:
            int transposedPropertyIndex = index - WidgetBasePropertyCount;
            SetWidgetProperty(transposedPropertyIndex, value);
    }
    NotifyPropertyUpdated();
}

GraphicsItemBase* UIWidgetBase::GetGraphicsItem(bool drawChromaKeyColor)
{
    return new GraphicsItemBase(m_x, m_y, m_width, m_height, Qt::black, Qt::black, drawChromaKeyColor);
}

QString UIWidgetBase::ShortDescription()
{
    return QString();
}

QString UIWidgetBase::LongDescription()
{
    return QString();
}

quint32 UIWidgetBase::GetAnimationID(quint32 animationNumber)
{
    Q_UNUSED(animationNumber)
    return 0xFFFFFFFF;
}

void UIWidgetBase::SetAnimationID(quint8 animationNumber, quint32 animationID)
{
    Q_UNUSED(animationNumber);
    Q_UNUSED(animationID);
}

void UIWidgetBase::SetAnimationUseTriggers(quint8 animationNumber, bool useTriggers)
{
    Q_UNUSED(animationNumber);
    Q_UNUSED(useTriggers);
}

bool UIWidgetBase::GetAnimationUseTriggers(quint32 animationNumber)
{
    Q_UNUSED(animationNumber)
    return false;
}

void UIWidgetBase::SetAnimationOnLoadBehavior(quint32 animationNumber, UIWidgetBase::AnimationOnLoadBehavior behavior)
{
    Q_UNUSED(animationNumber)
    Q_UNUSED(behavior)
}

UIWidgetBase::AnimationOnLoadBehavior UIWidgetBase::GetAnimationOnLoadBehavior(quint32 animationNumber)
{
    Q_UNUSED(animationNumber)
    return DO_NOTHING;
}

void UIWidgetBase::SetAnimationOnTriggerActiveBehavior(quint32 animationNumber, UIWidgetBase::AnimationOnTriggerActiveBehavior behavior)
{
    Q_UNUSED(animationNumber)
    Q_UNUSED(behavior)
}

UIWidgetBase::AnimationOnTriggerActiveBehavior UIWidgetBase::GetAnimationOnTriggerActiveBehavior(quint32 animationNumber)
{
    Q_UNUSED(animationNumber)
    return START_FROM_BEGINNING;
}

void UIWidgetBase::SetAnimationOnTriggerInactiveBehavior(quint32 animationNumber, UIWidgetBase::AnimationOnTriggerInactiveBehavior behavior)
{
    Q_UNUSED(animationNumber)
    Q_UNUSED(behavior)
}

UIWidgetBase::AnimationOnTriggerInactiveBehavior UIWidgetBase::GetAnimationOnTriggerInactiveBehavior(quint32 animationNumber)
{
    Q_UNUSED(animationNumber)
    return STOP_AT_DEFAULT_FRAME;
}

const QList<TransitionClause *> &UIWidgetBase::GetAnimationTriggers(quint32 animationNumber)
{
    Q_UNUSED(animationNumber)
    return m_emptyList;
}

void UIWidgetBase::AddAnimationTrigger(quint32 animationNumber, TransitionClause *trigger)
{
    Q_UNUSED(animationNumber)
    Q_UNUSED(trigger)
}

void UIWidgetBase::RemoveAnimationTrigger(quint32 animationNumber, TransitionClause *trigger)
{
    Q_UNUSED(animationNumber)
    Q_UNUSED(trigger)
}

QString UIWidgetBase::GetSecondaryColorPairForegroundHeader()
{
    return "INVALID HEADER";
}

QString UIWidgetBase::GetSecondaryColorPairBackgroundHeader()
{
    return "INVALID HEADER";
}

const int UIWidgetBase::UShort_Mask = SpinBox_Uint16 | SpinBox_Uint8 | SpinBox_Uint8_Min1 | SpinBox_Uint5_Hex | SpinBox_Uint6_Hex | SpinBox_DynamicValueType;

const int UIWidgetBase::UInt_Mask = SpinBox_Uint32 | IconsComboBox | LabelsComboBox | FontComboBox | ImagesComboBox | TextAlignmentComboBox | AnimationComboBox | DynamicImageAnimationComboBox;

const int UIWidgetBase::Short_Mask = SpinBox_Int16 | SpinBox_Int8;

const int UIWidgetBase::Int_Mask = (int)SpinBox_Int32;

const int UIWidgetBase::String_Mask = LineEdit_Editable | LineEdit_NonEditable | CompilerComboBox | SignalSinkDataTypeCombobox | FileSelector | FileSelector_ProjectJailed | FolderSelector | FolderSelector_ProjectJailed;

const int UIWidgetBase::Bool_Mask = (int)CheckBox | (int)AnimationCheckBox;


/** IGCLSignalSink/Source **/

const QList<GCLSignalSink*>& UIWidgetBase::GetSignalSinks() const
{
    return m_sinks;
}

GCLSignalSink* UIWidgetBase::GetSignalSink(QString signalName) const
{
    foreach (GCLSignalSink* sink, m_sinks)
    {
        if (sink->SignalName().compare(signalName) == 0)
        {
            return sink;
        }
    }

    return NULL;
}

const QString UIWidgetBase::GetComponentName() const
{
    return GetUIWidgetUniqueName();
}

const QList<GCLSignalSource *> &UIWidgetBase::GetSignalSources() const
{
    return m_sources;
}

GCLSignalSource* UIWidgetBase::GetSignalSource(QString signalName) const
{
    foreach (GCLSignalSource* source, m_sources)
    {
        if (source->SignalName().compare(signalName) == 0)
        {
            return source;
        }
    }

    return NULL;
}

void UIWidgetBase::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitUIWidgetBase(this);
}

bool UIWidgetBase::objectSelfValidation(Project* project, QStringList *errorList)
{
    Q_UNUSED(project);

    bool validation = true;

    if (GetWidth() == 0 || GetHeight() == 0)
    {
        if (GetUIWidgetType() != UIWidget_Type_Icon)
        {
            errorList->append("Width or height is 0 in '" + GetUserDefinedName() + "' widget ID: " + QString::number(GetUIWidgetID()));
            validation &= false;
        }
    }

    if (ProjectValidator::ValidCIdentifierRegExp().exactMatch(GetUserDefinedName()) == false)
    {
        errorList->append(QString("User-defined widget name \"%1\" (widgetID: %2) failed validation (it probably contains special characters).").arg(GetUserDefinedName(), QString::number(GetUIWidgetID())));
        validation &= false;
    }

    return validation;
}

QString UIWidgetBase::GetOnLoadBehaviorString(UIWidgetBase::AnimationOnLoadBehavior behavior)
{
    QString behaviorString;

    switch (behavior)
    {
        case DO_NOTHING:
            behaviorString = "DO_NOTHING";
            break;
        case RUN_ONE_CYCLE:
            behaviorString = "RUN_ONE_CYCLE";
            break;
        case SHOW_DEFAULT:
            behaviorString = "SHOW_DEFAULT";
            break;
        default:
            behaviorString = "DO_NOTHING";
            break;
    }

    return behaviorString;
}

QString UIWidgetBase::GetOnTriggerActiveBehaviorString(UIWidgetBase::AnimationOnTriggerActiveBehavior behavior)
{
    QString behaviorString;

    switch (behavior)
    {
        case START_FROM_BEGINNING:
            behaviorString = "START_FROM_BEGINNING";
            break;
        case START_FROM_DEFAULT_FRAME:
            behaviorString = "START_FROM_DEFAULT_FRAME";
            break;
        case CONTINUE_FROM_CURRENT_FRAME:
            behaviorString = "CONTINUE_FROM_CURRENT_FRAME";
            break;
        default:
            behaviorString = "START_FROM_BEGINNING";
            break;
    }

    return behaviorString;
}

QString UIWidgetBase::GetOnTriggerInactiveBehaviorString(UIWidgetBase::AnimationOnTriggerInactiveBehavior behavior)
{
    QString behaviorString;

    switch (behavior)
    {
        case STOP:
            behaviorString = "STOP";
            break;
        case STOP_AT_END_OF_CYCLE:
            behaviorString = "STOP_AT_END_OF_CYCLE";
            break;
        case STOP_AT_DEFAULT_FRAME:
            behaviorString = "STOP_AT_DEFAULT_FRAME";
            break;
        case CONTINUE:
            behaviorString = "CONTINUE";
            break;
        default:
            behaviorString = "STOP";
            break;
    }

    return behaviorString;
}

void UIWidgetBase::UpdateAllInternalSinks()
{
    QList<GCLSignalSink*> newSignalSinks;

    foreach (GCLSignalSink* sink, m_sinks)
    {
        GCLSignalSource* source = sink->GetSignalSource();
        sink->DisconnectFromSource();
        GCLSignalSink* newSink = new GCLSignalSink(sink->SignalName(), m_componentName, sink->DataType(), sink->HasUpdateNotification(), sink->IsMandatory());

        newSink->ConnectTo(source);
        newSignalSinks.append(newSink);
        m_sinks.removeAll(sink);
    }

    foreach (GCLSignalSink* sink, newSignalSinks)
    {
        m_sinks.append(sink);
    }
}

void UIWidgetBase::UpdateAllConnectedSinks()
{
    QList<GCLSignalSource*> newSignalSources;

    foreach (GCLSignalSource* source, m_sources)
    {
        GCLSignalSource* newSource = new GCLSignalSource(source->SignalName(), m_componentName, source->DataType());
        newSignalSources.append(newSource);

        foreach (GCLSignalSink* sink, source->GetConnectedSinks())
        {
            sink->ConnectTo(newSource);
        }
        m_sources.removeAll(source);
    }

    foreach (GCLSignalSource* source, newSignalSources)
    {
        AddGCLSignalSource(source);
    }
}

void UIWidgetBase::AddGCLSignalSink(GCLSignalSink* sink)
{
    m_sinks.append(sink);
    sink->SetParent(this);
}

void UIWidgetBase::AddGCLSignalSource(GCLSignalSource* source)
{
    m_sources.append(source);
    source->SetParent(this);
}
