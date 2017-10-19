#include "bspschemarules.h"

bool BspSchemaRules::isAttributeRequired(QDomDocument schema, const QString &tagName, const QString &attrName)
{    
    auto schemaAttributes = attributeElements(schema, tagName);
    foreach(auto schemaAttribute, schemaAttributes)
    {
        if(schemaAttribute.attribute("name") == attrName)
            return schemaAttribute.attribute("use", "optional") == "required";
    }

    return false;
}

// Create a new element and add all attributes. The attributes are
// extracted from the schema file.
QDomElement BspSchemaRules::createElementUsingSchema(QDomDocument spec, QDomDocument schema, const QString &tagName)
{
    auto newElement = spec.createElement(tagName);

    auto schemaAttributes = attributeElements(schema, tagName);
    foreach(auto schemaAttribute, schemaAttributes)
    {
        // Add attribute to element
        if(schemaAttribute.hasAttribute("name"))
            newElement.setAttribute(schemaAttribute.attribute("name"), "");
    }

    return newElement;
}

int BspSchemaRules::numElementsAllowedUsingSchema(QDomDocument schema, const QString &tagName)
{
    auto schemaElements = schema.elementsByTagName("xs:element");
    for(int i=0; i<schemaElements.size(); ++i)
    {
        auto schemaElement = schemaElements.at(i).toElement();
        if(schemaElement.attribute("name") == tagName)
        {
            QString maxOccurs = schemaElement.attribute("maxOccurs", "1");
            if(maxOccurs == "unbounded")
                return std::numeric_limits<int>::max();

            return maxOccurs.toInt();
        }
    }

    return 0;
}

QStringList BspSchemaRules::enumerationValues(QDomDocument schema, const QString &tagName, const QString &attrName)
{
    QStringList values;
    auto attributes = attributeElements(schema, tagName);
    foreach(auto attribute, attributes)
    {
        if(attribute.attribute("name") == attrName)
        {
            auto enumerations = attribute.elementsByTagName("xs:enumeration");
            for(int i=0; i<enumerations.size(); ++i)
            {
                auto enumeration = enumerations.at(i).toElement();
                if(enumeration.hasAttribute("value"))
                    values.append(enumeration.attribute("value"));
            }
        }
    }

    return values;
}

QList<QDomElement> BspSchemaRules::attributeElements(QDomDocument schema, const QString &tagName)
{
    QList<QDomElement> attributes;

    auto schemaElements = schema.elementsByTagName("xs:element");
    for(int i=0; i<schemaElements.size(); ++i)
    {
        auto schemaElement = schemaElements.at(i).toElement();
        if(schemaElement.attribute("name") == tagName)
        {
            auto schemaAttributes = schemaElement.elementsByTagName("xs:attribute");
            for(int j=0; j<schemaAttributes.size(); j++)
            {
                auto schemaAttribute = schemaAttributes.at(j).toElement();

                // Exclude attributes that are not direct descendants to the element node
                // by traversing up the parent hierarchy and look for other xs:element
                bool excludeAttribute = false;
                auto parent = schemaAttribute.parentNode();
                while(parent != schemaElement)
                {
                    if(parent.nodeName() == "xs:element")
                        excludeAttribute = true;

                    parent = parent.parentNode();
                }
                if(excludeAttribute)
                    continue;

                attributes.append(schemaAttribute);
            }
        }
    }

    return attributes;
}
