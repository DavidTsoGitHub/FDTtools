#ifndef IRUNNABLE_H
#define IRUNNABLE_H

#include "Task.h"
#include <QList>

class IRunnable
{
public:
    virtual const QList<const Task*>& Tasks() const = 0;
};

#endif // IRUNNABLE_H
