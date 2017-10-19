#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
/* Define resource clients. These are statically defined system wide.
*  Only platform services shall access the resource manager directly. Function blocks shall
*  use the respective manager to use resoruces (display / menu / sound ...)
*/
#define RM_CLIENT_MENUCONTROLLER            0U
#define RM_CLIENT_CHKMSGHANDLER             1U
#define RM_CLIENT_LAYOUTMANAGER             2U
#define RM_CLIENT_INVALID                   255U

//PRIO LIST
#define RM_PRIO_MENUCONTROLLER              0u
#define RM_PRIO_CHECKMESSAGEHANDLER         1u
#define RM_PRIO_STARTUP                     254u
#define RM_PRIO_INVALID                     255u

//Add new clients here. Remember to update MAX_NUMBER_OF_RESOURCE_CLIENTS
#define MAX_NUMBER_OF_RESOURCE_CLIENTS      3U

//Define resources
#define RM_RESOURCE_DISPLAY                 0U
#define RM_NUMBER_OF_RESOURCES              1U
#define RM_RESOURCE_INVALID               0xFF

#define RESOURCE_GRANTED                    1U
#define RESOURCE_DENIED                     0U


//Public typedefs
typedef void(*pResourceGranted_F)(void);
typedef void(*pResourceDenied_F )(void);

/***************************** Exported Functions ****************************/
//Scheduler functions
void ResourceManager_Init(void);
void ResourceManager_Run(void);

//Resource manager interfaces
void RM_RegisterResourceClient(uint8 u8ClientID, pResourceGranted_F grantedF, pResourceDenied_F deniedF );
void RM_RequestResource( uint8 u8ClientID, uint8 u8Prio );
void RM_ReleaseResource(uint8 u8ClientID);


#endif
