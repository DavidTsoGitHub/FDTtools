#ifndef UIWIDGETSERIALIZER_H
#define UIWIDGETSERIALIZER_H

class UIWidgetBase;

class QXmlStreamWriter;

class UIWidgetSerializer
{
public:
    static void Serialize(QXmlStreamWriter& xmlWriter, UIWidgetBase* uiWidget);
};

#endif // UIWIDGETSERIALIZER_H
