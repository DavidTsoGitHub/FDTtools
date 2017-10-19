#include "ResourceManagerMock.h"

static pResourceGranted_F grantedFunction;
static pResourceDenied_F deniedFunction;

static int registerResourceClientCallCount = 0;
static clock_t registerResourceClientFirstCallTimeStamp = -1;
static clock_t registerResourceClientLastCallTimeStamp = -1;

static int requestResourceCallCount = 0;
static clock_t requestResourceFirstCallTimeStamp = -1;
static clock_t requestResourceLastCallTimeStamp = -1;

static int releaseResourceCallCount = 0;
static clock_t releaseResourceFirstCallTimeStamp = -1;
static clock_t releaseResourceLastCallTimeStamp = -1;

void ResourceManagerMock_Reset()
{
    registerResourceClientCallCount = 0;
    registerResourceClientFirstCallTimeStamp = -1;
    registerResourceClientLastCallTimeStamp = -1;

    requestResourceCallCount = 0;
    requestResourceFirstCallTimeStamp = -1;
    requestResourceLastCallTimeStamp = -1;

    releaseResourceCallCount = 0;
    releaseResourceFirstCallTimeStamp = -1;
    releaseResourceLastCallTimeStamp = -1;
}

/**** ResourceManager.h interface ****/
void RM_RegisterResourceClient(uint8 u8ClientID, pResourceGranted_F grantedF, pResourceDenied_F deniedF )
{
    if (registerResourceClientFirstCallTimeStamp < 0)
    {
        registerResourceClientFirstCallTimeStamp = clock();
    }
    registerResourceClientLastCallTimeStamp = clock();
    ++registerResourceClientCallCount;

    grantedFunction = grantedF;
    deniedFunction = deniedF;
}

void RM_RequestResource(uint8 u8ClientID, uint8 u8Prio)
{
	(void)u8Prio;
	
    if (requestResourceFirstCallTimeStamp < 0)
    {
        requestResourceFirstCallTimeStamp = clock();
    }
    requestResourceLastCallTimeStamp = clock();
    ++requestResourceCallCount;
}

void RM_ReleaseResource(uint8 u8ClientID)
{
    if (releaseResourceFirstCallTimeStamp < 0)
    {
        releaseResourceFirstCallTimeStamp = clock();
    }
    releaseResourceLastCallTimeStamp = clock();
    ++releaseResourceCallCount;
}

/**** ResourceManagerMock.h interface ****/

void ResourceManagerMock_GrantResource(void)
{
    grantedFunction();
}

void ResourceManagerMock_RevokeResource(void)
{
    deniedFunction();
}

int ResourceManagerMock_RegisterResourceClient_CallCount()
{
    return registerResourceClientCallCount;
}

clock_t ResourceManagerMock_RegisterResourceClient_FirstCallTimeStamp()
{
    return registerResourceClientFirstCallTimeStamp;
}

clock_t ResourceManagerMock_RegisterResourceClient_LastCallTimeStamp()
{
    return registerResourceClientLastCallTimeStamp;
}

int ResourceManagerMock_RequestResource_CallCount()
{
    return requestResourceCallCount;
}

clock_t ResourceManagerMock_RequestResource_FirstCallTimeStamp()
{
    return requestResourceFirstCallTimeStamp;
}

clock_t ResourceManagerMock_RequestResource_LastCallTimeStamp()
{
    return requestResourceLastCallTimeStamp;
}

int ResourceManagerMock_ReleaseResource_CallCount()
{
    return releaseResourceCallCount;
}

clock_t ResourceManagerMock_ReleaseResource_FirstCallTimeStamp()
{
    return releaseResourceFirstCallTimeStamp;
}

clock_t ResourceManagerMock_ReleaseResource_LastCallTimeStamp()
{
    return releaseResourceLastCallTimeStamp;
}