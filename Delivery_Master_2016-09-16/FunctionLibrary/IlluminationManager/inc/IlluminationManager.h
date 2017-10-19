#ifndef __ILLUMINATIONMANAGER_H
#define __ILLUMINATIONMANAGER_H

void IlluminationManager_Run(void);
void IlluminationManager_Init(void);

void IlluminationManager_IlluminationLevel_Indication(void);

#define NIGHT_TIME 0U
#define DAY_TIME   1U

#endif // __ILLUMINATIONMANAGER_H
