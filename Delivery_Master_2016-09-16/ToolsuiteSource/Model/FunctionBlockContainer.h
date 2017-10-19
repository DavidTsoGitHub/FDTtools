#ifndef FUNCTIONBLOCKCONTAINER_H
#define FUNCTIONBLOCKCONTAINER_H

#include "ProjectObject.h"

class FunctionBlock;
template <typename T> class QList;
class QString;

class FunctionBlockContainer : public ProjectObject
{
public:

    explicit FunctionBlockContainer(ProjectObject* parent);
    ~FunctionBlockContainer(void);

    void AddFunctionBlock(FunctionBlock* blockToAdd);
    void RemoveFunctionBlock(FunctionBlock* blockToRemove);
    FunctionBlock* GetFunctionBlock(int functionBlockID);
    const QList<FunctionBlock*>& List(void) const;
    bool Initialize(QDir functionLibraryDir);
    const QString InitializationError(void) const;

private:
    QList<FunctionBlock*> m_functionBlocks;
    QString m_initializationError;
    int getNextAvailableId(void);

    // ProjectObject interface
public:
//    void Accept(ProjectObjectVisitor& visitor);
};
#endif // FUNCTIONBLOCKCONTAINER_H
