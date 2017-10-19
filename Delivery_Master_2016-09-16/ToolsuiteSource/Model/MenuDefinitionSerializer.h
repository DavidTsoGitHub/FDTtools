#ifndef MENUDEFINITIONSERIALIZER_H
#define MENUDEFINITIONSERIALIZER_H

#include "SerializerBase.h"

class MenuDefinition;
class MenuItem;

class QXmlStreamWriter;

class MenuDefinitionSerializer : public SerializerBase
{
public:
    explicit MenuDefinitionSerializer(MenuDefinition& menuDefinition);
    ~MenuDefinitionSerializer(void);

    QString Serialize();

private:
    MenuDefinition& m_menuDefinition;

    static void writeMenuItem(QXmlStreamWriter& xmlWriter, const MenuItem* menuItem, qint16 index);
};

#endif // MENUDEFINITIONSERIALIZER_H
