#ifndef CHECKMESSAGEMANAGER_H
#define CHECKMESSAGEMANAGER_H

#include "StandardTypes.h"

typedef enum
{
    CM_STATE_OFF,
    CM_STATE_RUN
} cm_state_t;

#define CHECKMESSAGEMANAGER_CYCLETIME 100
#define CHECKMESSAGEMANAGER_LOG_NAME "CheckMessageManager"

void CheckMessageManager_Init(void);
void CheckMessageManager_Run(void);

#endif //CHECKMESSAGEMANAGER_H
