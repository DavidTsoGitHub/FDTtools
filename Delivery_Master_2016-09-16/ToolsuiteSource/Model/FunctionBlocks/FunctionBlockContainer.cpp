#include "FunctionBlockContainer.h"

#include "Exceptions/ExceptionBase.h"
#include "Exceptions/FunctionBlockInitializationException.h"
#include "FunctionBlock.h"
#include "Logger.h"
#include "ProjectObjectVisitor.h"

#include <QStringList>

FunctionBlockContainer::FunctionBlockContainer(ProjectObject* parent) : ProjectObject(QString("Function Blocks"), parent)
{  
}

FunctionBlockContainer::~FunctionBlockContainer()
{
}

void FunctionBlockContainer::AddFunctionBlock(FunctionBlock* blockToAdd)
{
    if (blockToAdd->GetID() == -1)
    {
        qint32 id = getNextAvailableId();
        blockToAdd->SetID(id);
    }

    blockToAdd->SetParent(this);
    m_functionBlocks.append(blockToAdd);
    NotifyPropertyUpdated();
}

void FunctionBlockContainer::RemoveFunctionBlock(FunctionBlock* blockToRemove)
{
    if (blockToRemove == NULL) return;
    blockToRemove->SetParent(NULL);
    m_functionBlocks.removeAll(blockToRemove);
    delete blockToRemove;
    NotifyPropertyUpdated();
}

FunctionBlock* FunctionBlockContainer::GetFunctionBlock(int functionBlockID)
{
    foreach (FunctionBlock* functionBlock, m_functionBlocks)
    {
        if (functionBlock->GetID() == functionBlockID)
        {
            return functionBlock;
        }
    }

    return NULL;
}

qint32 FunctionBlockContainer::getNextAvailableId()
{
    int maxId = 0;
    foreach (FunctionBlock* block, m_functionBlocks)
    {
        qint32 blockId = block->GetID();
        if (blockId > maxId)
        {
            maxId = blockId;
        }
    }
    return maxId+1;
}

const QList<FunctionBlock*>& FunctionBlockContainer::List() const
{
    return m_functionBlocks;
}

bool FunctionBlockContainer::Initialize(QDir functionLibraryDir)
{
    foreach (FunctionBlock* functionBlock, m_functionBlocks)
    {
        try
        {
            functionBlock->Initialize(functionLibraryDir);
        }
        catch (ExceptionBase& e)
        {
            e.InnerExceptionStrings().insert(0, e.Message());
            throw FunctionBlockInitializationException(functionBlock->GetID(), e.InnerExceptionStrings());
        }
    }

    return true;
}

const QString FunctionBlockContainer::InitializationError() const
{
    return m_initializationError;
}

//void FunctionBlockContainer::Accept(ProjectObjectVisitor& visitor)
//{
//    visitor.VisitFunctionBlockContainer(this);
//}
