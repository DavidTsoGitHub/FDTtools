#ifndef BSPSCHEMARULES_H
#define BSPSCHEMARULES_H

#include <QDomDocument>
#include <QString>
#include <QStringList>

// Various functions for extracting info from the schema file.
class BspSchemaRules
{
public:
    static bool isAttributeRequired(QDomDocument schema, const QString& tagName, const QString &attrName);
    static QDomElement createElementUsingSchema(QDomDocument spec, QDomDocument schema, const QString &tagName);
    static int numElementsAllowedUsingSchema(QDomDocument schema, const QString &tagName);
    static QStringList enumerationValues(QDomDocument schema, const QString &tagName, const QString &attrName);

private:
    static QList<QDomElement> attributeElements(QDomDocument schema, const QString &tagName);
};

#endif // BSPSCHEMARULES_H
