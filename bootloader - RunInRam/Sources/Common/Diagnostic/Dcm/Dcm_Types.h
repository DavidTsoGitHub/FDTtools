#ifndef DCM_TYPES_H
#define DCM_TYPES_H

/*=======[I N C L U D E S]====================================================*/
#include "Dcm.h"
#include "FlashLoader.h"
#include "SecM.h"
#include "Dcm_Cfg.h"

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/** service function type */
typedef void (*Dcm_ServiceFct) (const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

/** service pending function type */
typedef void (*Dcm_PendingFct) (const FL_ResultType errorCode,\
                                const Dcm_BuffType * rxBuff,\
                                Dcm_BuffType * txBuff);

/** service table parameter config */
typedef struct
{
	/* service Id */
    const uint8 SID;
    
    /* if function address is supportted */
    const boolean funcAddrSupportted;
    
    /* supportted session mode */
    const Dcm_SessionType sessionSupp[DCM_SESSION_SUPPORT_NUM];
    
    /* service function */
    const Dcm_ServiceFct serviceFct;
    
    /* service pending function */
    const Dcm_PendingFct pendingFct;
}Dcm_ServiceTableType;

/** 0x10 service parameter config */
typedef struct
{
	/* session mode type */
    const Dcm_SessionType sessionType;
    
    /* supportted session mode */
    const Dcm_SessionType sessionSupp[DCM_SESSION_SUPPORT_NUM];
    
    /* supportted security level */
    const Dcm_SecurityType securitySupp[DCM_SECURITY_SUPPORT_NUM]; 
}Dcm_SessionRowType;

/** 0x11 service parameter config */
typedef struct
{
	/* reset mode type */
    const Dcm_ResetType resetType;
    
    /* supportted session mode */
    const Dcm_SessionType sessionSupp[DCM_SESSION_SUPPORT_NUM];
    
    /* supportted security level */
    const Dcm_SecurityType securitySupp[DCM_SECURITY_SUPPORT_NUM]; 
}Dcm_ResetRowType;

#if(DCM_READDID_NUM > 0)
/** read data function type */
typedef uint16 (*Dcm_ReadDataFct) (uint8 *readData);

/** 0x22 service parameter config */
typedef struct
{
	/* read data Id */
    const uint16 DID;
    
    /* supportted security level */
    const Dcm_SecurityType securitySupp[DCM_SECURITY_SUPPORT_NUM]; 
    
    /* read data funtion */ 
    const Dcm_ReadDataFct readDataFct;
}Dcm_ReadDidRowType;
#endif

/** generate seed function type */
typedef SecM_StatusType (*Dcm_GenerateSeedFct)(SecM_SeedType *seed);

/** compare key function type */
typedef SecM_StatusType (*Dcm_CompareKeyFct)(SecM_KeyType key,SecM_SeedType seed);

/** 0x27 service parameter config */
typedef struct
{
	/* request seed subfunction Id */
    const uint8 reqSeedId;
    
    /* send key subfunction Id */
    const uint8 sendKeyId;
    
    /* security access level */
    const Dcm_SecurityType secAccessLevel;
    
    /* supportted session mode */
    const Dcm_SessionType sessionSupp[DCM_SESSION_SUPPORT_NUM];
    
    /* supportted security level */
    const Dcm_SecurityType securitySupp[DCM_SECURITY_SUPPORT_NUM];
    
    /* generate seed function */
    const Dcm_GenerateSeedFct generateSeed;
    
    /* compare key function */
    const Dcm_CompareKeyFct compareKey;
}Dcm_SecurityRowType;

#if (DCM_COM_CONTROL_NUM > 0)
/** 0x28 service parameter config */
typedef struct
{
	/* communication control type */
    const Dcm_ComControlType controlType;
    
    /* supportted session mode */
    const Dcm_SessionType sessionSupp[DCM_SESSION_SUPPORT_NUM];
    
    /* supportted security level */
    const Dcm_SecurityType securitySupp[DCM_SECURITY_SUPPORT_NUM];
}Dcm_ComControlRowType;
#endif

/** write data function type */
typedef FL_ResultType (*Dcm_WriteDataFct) (uint8 *data,const uint16 length);

/* 0x2E service parameter config */
typedef struct
{
	/* write data Id */
    const uint16 DID;
    
    /* write length */
    const uint16 dataLength;
    
    /* supportted security level */
    const Dcm_SecurityType securitySupp[DCM_SECURITY_SUPPORT_NUM];  
    
    /* write data funtion */
    const Dcm_WriteDataFct writeDataFct;
}Dcm_WriteDidRowType;

/** routine control function type */
typedef void (*Dcm_RoutineControlFct) (const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

/** 0x31 service parameter config */
typedef struct
{
	/* routine Id */
    const uint16 routineId;
    
    /* routine option length */
    const uint8 optionLength;
    
    /* if function address supportted */
    const boolean funcAddrSupp;
    
    /* supportted session mode */
    const Dcm_SessionType sessionSupp[DCM_SESSION_SUPPORT_NUM];
    
    /* supportted security level */
    const Dcm_SecurityType securitySupp[DCM_SECURITY_SUPPORT_NUM];
    
    /* routine control funtion */      
    const Dcm_RoutineControlFct routineControl;
}Dcm_RoutineControlRowType;

/** 0x34 0x36 0x37 service parameter config */
typedef struct
{
	/* supportted security level */
    const Dcm_SecurityType securitySupp[DCM_SECURITY_SUPPORT_NUM];  
}Dcm_DownloadRowType;

#if (DCM_DTC_SET_NUM > 0)
/** 0x85 service parameter config */
typedef struct
{
	/* DTC setting type */
    const Dcm_DTCSettingType DTCSet;
    
    /* supportted session mode */
    const Dcm_SessionType sessionSupp[DCM_SESSION_SUPPORT_NUM];
    
    /* supportted security level */
    const Dcm_SecurityType securitySupp[DCM_SECURITY_SUPPORT_NUM];
}Dcm_DTCSettingRowType;
#endif

/*=======[E X T E R N A L   D A T A]==========================================*/
extern const Dcm_ServiceTableType Dcm_ServiceTable[DCM_SERVICE_NUM];

extern const Dcm_SessionRowType Dcm_SessionRow[DCM_SESSION_NUM];

extern const Dcm_ResetRowType Dcm_ResetRow[DCM_RESET_NUM];

#if(DCM_READDID_NUM > 0)
extern const Dcm_ReadDidRowType Dcm_ReadDidRow[DCM_READDID_NUM];
#endif

extern const Dcm_SecurityRowType Dcm_SecurityRow[DCM_SECURITY_NUM];

#if (DCM_COM_CONTROL_NUM > 0)
extern const Dcm_ComControlRowType Dcm_ComControlRow[DCM_COM_CONTROL_NUM];
#endif

extern const Dcm_WriteDidRowType Dcm_WriteDidRow[DCM_WRITEDID_NUM];

extern const Dcm_RoutineControlRowType Dcm_RoutineControlRow[DCM_ROUTINE_CONTROL_NUM];

extern const Dcm_DownloadRowType Dcm_DownloadRow;

#if (DCM_DTC_SET_NUM > 0)
extern const Dcm_DTCSettingRowType Dcm_DTCSettingRow[DCM_DTC_SET_NUM];
#endif

#endif/* endof DCM_TYPES_H */

