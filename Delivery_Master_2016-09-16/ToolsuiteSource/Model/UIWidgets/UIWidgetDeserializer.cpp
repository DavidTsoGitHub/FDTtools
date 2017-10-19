#include "UIWidgetDeserializer.h"

#include "Project.h"
#include "UIWidgetBase.h"
#include "RegEx/RegExPatterns.h"
#include "IconWidget.h"
#include "ImageWidget.h"
#include "LabelWidget.h"
#include "DynamicValueLabelWidget.h"
#include "DynamicTextLabelWidget.h"
#include "BlockBarWidget.h"
#include "SelectableListWidget.h"
#include "DynamicIconWidget.h"
#include "DynamicImageWidget.h"
#include "SignalTriggerWidget.h"
#include "ColumnLevelWidget.h"
#include "ListNavigationWidget.h"
#include "DynamicLabelWidget.h"

#include <QXmlStreamReader>

UIWidgetBase* UIWidgetDeserializer::Deserialize(QXmlStreamReader& xmlReader, Project& project)
{
    QString widgetType = xmlReader.attributes().value("Type").toString();
    quint32 widgetID = xmlReader.attributes().value("WidgetID").toUInt();
    UIWidgetBase* uiWidget = UIWidgetDeserializer::getNewUIWidgetByTypeName(widgetType, widgetID, project);

    quint32 numberOfProperties = uiWidget->GetTotalPropertyCount();

    for (quint32 i = 0; i < numberOfProperties; ++i)
    {
        UIWidgetBase::DataType_t dataType = uiWidget->GetPropertyDataType(i);
        QString fieldName = uiWidget->GetPropertyHeader(i).replace(RegExPatterns::NotAcceptedCodeCharacters(), "");

        if (((int)dataType & UIWidgetBase::UShort_Mask) == (int)dataType)
        {
            quint16 value = xmlReader.attributes().value(fieldName).toUShort();
            uiWidget->SetProperty(i, value);
        }
        else if (((int)dataType & UIWidgetBase::UInt_Mask) == (int)dataType)
        {
            quint32 value = xmlReader.attributes().value(fieldName).toUInt();
            uiWidget->SetProperty(i, value);
        }
        else if (((int)dataType & UIWidgetBase::Short_Mask) == (int)dataType)
        {
            qint16 value = xmlReader.attributes().value(fieldName).toShort();
            uiWidget->SetProperty(i, value);
        }
        else if (((int)dataType & UIWidgetBase::Int_Mask) == (int)dataType)
        {
            qint32 value = xmlReader.attributes().value(fieldName).toInt();
            uiWidget->SetProperty(i, value);
        }
        else if (((int)dataType & UIWidgetBase::String_Mask) == (int)dataType)
        {
            QString value = xmlReader.attributes().value(fieldName).toString();
            uiWidget->SetProperty(i, value);
        }
        else if (((int)dataType & UIWidgetBase::Bool_Mask) == (int)dataType)
        {
            uchar value = xmlReader.attributes().value(fieldName).toString().compare("true", Qt::CaseInsensitive) == 0 ? 1 : 0;
            uiWidget->SetProperty(i, value);
        }
    }

    return uiWidget;
}

UIWidgetBase* UIWidgetDeserializer::getNewUIWidgetByTypeName(QString typeName, quint32 widgetID, Project& project)
{
    UIWidgetBase* uiWidget = NULL;

    if (typeName.compare(IconWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new IconWidget(project.GetIconDefinition(), widgetID);
    }
    else if (typeName.compare(ImageWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new ImageWidget(project.GetLayoutDefinition(), project.GetAnimationDefinition(), project.GetImageDefinition(), widgetID);
    }
    else if (typeName.compare(LabelWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new LabelWidget(project.GetLabelDefinition(), widgetID, project.GetLayoutDefinition());
    }
    else if (typeName.compare(DynamicTextLabelWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new DynamicTextLabelWidget(project.GetLabelDefinition(), widgetID, project.GetLayoutDefinition());
    }
    else if (typeName.compare(DynamicValueLabelWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new DynamicValueLabelWidget(project.GetLabelDefinition(), widgetID, project.GetLayoutDefinition());
    }
    else if (typeName.compare(BlockBarWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new BlockBarWidget(widgetID, project.GetLayoutDefinition());
    }
    else if (typeName.compare(ColumnLevelWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new ColumnLevelWidget(widgetID, project.GetLayoutDefinition());
    }
    else if (typeName.compare(SelectableListWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new SelectableListWidget(widgetID, project.GetLayoutDefinition());
    }
    else if (typeName.compare(DynamicIconWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new DynamicIconWidget(widgetID);
    }
    else if (typeName.compare(DynamicImageWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new DynamicImageWidget(project.GetAnimationDefinition(), widgetID);
    }
    else if (typeName.compare(SignalTriggerWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new SignalTriggerWidget(widgetID);
    }
    else if (typeName.compare(ListNavigationWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new ListNavigationWidget(widgetID, project.GetLayoutDefinition());
    }
    else if (typeName.compare(DynamicLabelWidget::TypeName, Qt::CaseInsensitive) == 0)
    {
        uiWidget = new DynamicLabelWidget(widgetID, project.GetLayoutDefinition());
    }


    return uiWidget;
}
