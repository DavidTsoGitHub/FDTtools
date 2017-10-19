#ifndef BSPTREEFILTER_H
#define BSPTREEFILTER_H

#include "treenavigatorfilter.h"

#include <QMap>
#include <QString>
#include <QStringList>

class BSPTreeFilter : public TreeNavigatorFilter
{
public:
    bool includeNode(QDomNode node);
    QString nodeName(QDomNode node);

private:
    static QStringList s_excludedFromTree;

};

#endif // BSPTREEFILTER_H
