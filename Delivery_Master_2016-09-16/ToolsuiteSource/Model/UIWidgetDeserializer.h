#ifndef UIWIDGETDESERIALIZER_H
#define UIWIDGETDESERIALIZER_H

class Project;
class UIWidgetBase;

class QString;
class QXmlStreamReader;

class UIWidgetDeserializer
{
public:
    static UIWidgetBase* Deserialize(QXmlStreamReader& xmlReader, Project& project);

private:
    static UIWidgetBase* getNewUIWidgetByTypeName(QString typeName, quint32 widgetID, Project& project);
};

#endif // UIWIDGETDESERIALIZER_H
