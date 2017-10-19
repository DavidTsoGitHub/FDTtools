#ifndef TREENAVIGATORFILTER_H
#define TREENAVIGATORFILTER_H

#include <QDomNode>
#include <QString>

class TreeNavigatorFilter
{
public:
    virtual bool includeNode(QDomNode node) = 0;
    virtual QString nodeName(QDomNode node) = 0;
};

#endif // TREENAVIGATORFILTER_H
