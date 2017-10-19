/********************************* Includes **********************************/
#include "ResourceManager.h"
#include "StandardTypes.h"
#ifdef _WIN32
#include <assert.h>
#endif

#include "ResourceManager.h"

/**************************** Constants and Types ****************************/
#define RESOURCEREQUEST_INACTIVE        0U
#define RESOURCEREQUEST_ACTIVE          1U
#define RESOURCE_UNALLOCATED            0xFFU

typedef struct
{
    pResourceGranted_F ResourceGranted;
    pResourceDenied_F ResourceDenied;
    uint8	u8ResourceRequested;
    uint8   u8RequestPrio;
} tResourceClient;

/********************************* Variables *********************************/
static uint8 u8resourceOwner;
static tResourceClient clientList[MAX_NUMBER_OF_RESOURCE_CLIENTS];

/************************* Local Function Prototypes *************************/
static void rm_DistributeResources(void);

/***************************** Exported Functions ****************************/
void ResourceManager_Init(void)
{
    //Clear resource list
    uint8 i;
#ifdef _WIN32
    assert( MAX_NUMBER_OF_RESOURCE_CLIENTS < 255 );
#endif

    for (i =0; i < MAX_NUMBER_OF_RESOURCE_CLIENTS; i++)
    {
        clientList[i].ResourceGranted = (pResourceGranted_F)0;
        clientList[i].ResourceDenied = (pResourceDenied_F)0;
        clientList[i].u8ResourceRequested = RESOURCEREQUEST_INACTIVE;
        clientList[i].u8RequestPrio = RM_PRIO_INVALID;
    }

    u8resourceOwner = RESOURCE_UNALLOCATED;
}

void ResourceManager_Run(void)
{
    //Resourcemanager does not require a periodic task at this moment
}

void RM_RegisterResourceClient(uint8 u8ClientID, pResourceGranted_F grantedF, pResourceDenied_F deniedF )
{
#ifdef _WIN32
    assert( grantedF );
    assert( deniedF );
    assert ( u8ClientID < MAX_NUMBER_OF_RESOURCE_CLIENTS );
#endif
    clientList[u8ClientID].ResourceGranted = grantedF;
    clientList[u8ClientID].ResourceDenied = deniedF;
 }

void RM_RequestResource( uint8 u8ClientID ,uint8 u8Prio)
{
#ifdef _WIN32
    assert ( u8ClientID < MAX_NUMBER_OF_RESOURCE_CLIENTS );
#endif
    clientList[u8ClientID].u8ResourceRequested = RESOURCEREQUEST_ACTIVE;
    clientList[u8ClientID].u8RequestPrio = u8Prio;

    // Redistribute resources.
    rm_DistributeResources();
}

void RM_ReleaseResource(uint8 u8ClientID)
{
#ifdef _WIN32
    assert ( u8ClientID < MAX_NUMBER_OF_RESOURCE_CLIENTS );
#endif
    clientList[u8ClientID].u8ResourceRequested = RESOURCEREQUEST_INACTIVE;
    clientList[u8ClientID].u8RequestPrio = RM_PRIO_INVALID;

    //Redistribute resources
    rm_DistributeResources();
}

/****************************** Local Functions ******************************/
static void rm_DistributeResources(void)
{
    uint8 i;
    uint8 u8ClientWithTopPrio = RM_CLIENT_INVALID;   //use to handle if a new request from same client has lower prio.

    //Loop thru the list and find the request with highest prio. u8ClientWithTopPrio has the client ID of highest prio
    for (i = 0; i < MAX_NUMBER_OF_RESOURCE_CLIENTS; i++)
    {
        if (clientList[i].u8ResourceRequested == RESOURCEREQUEST_ACTIVE)
        {
            if ((clientList[i].u8RequestPrio > u8ClientWithTopPrio) || (u8ClientWithTopPrio == RM_CLIENT_INVALID))  //we have a request with higher prio or the first prio
            {
                u8ClientWithTopPrio = i;
            }
        }
    }

    if (u8ClientWithTopPrio == RM_CLIENT_INVALID)
    {
        for (i = 0; i < MAX_NUMBER_OF_RESOURCE_CLIENTS; i++)
        {
            if (clientList[i].ResourceDenied != (pResourceDenied_F)0 )
            {
                clientList[i].ResourceDenied();
            }
        }
    }
    else
    {
        //Check if the resource owner shall be changed.
        if (u8ClientWithTopPrio != u8resourceOwner)
        {
            if (u8resourceOwner != RESOURCE_UNALLOCATED)
            {
               if (clientList[u8resourceOwner].ResourceDenied != (pResourceDenied_F)0 )
               {
                    clientList[u8resourceOwner].ResourceDenied();
               }
            }

            u8resourceOwner = u8ClientWithTopPrio;

            if (clientList[u8resourceOwner].ResourceGranted != (pResourceGranted_F)0)
            {
                clientList[u8resourceOwner].ResourceGranted();
            }
        }
    }
}
