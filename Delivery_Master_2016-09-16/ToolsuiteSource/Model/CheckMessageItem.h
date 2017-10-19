#ifndef CHECKMESSAGEITEM
#define CHECKMESSAGEITEM

#include <QString>
#include "GCLSignalSink.h"

class CheckMessageItem
{
public:
    CheckMessageItem(){}
    CheckMessageItem(quint32 index, QString type, quint32 resourceID, quint32 widgetID) :
        Index(index),
        Type(type),
        ResourceID(resourceID),
        AssociatedWidgetID(widgetID)
    {}

    quint32 Index;
    //relates to a widget type in QString, e.g. "DynamicLabel"
    QString Type;
    //the resource id of the selected label
    //example use in CheckMessageModel::data()
    quint32 ResourceID;
    //the pointed at Widget's ID
    quint32 AssociatedWidgetID;
};

#endif // CHECKMESSAGEITEM

