#ifndef BLFLASH_H_
#define BLFLASH_H_

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "Nvm_Cfg.h"



/*=======[M A C R O S]========================================================*/

/*result values of flash driver routines*/
#define kFlashOk	0x0000u
#define kFlashFailed	0x0001u

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* flash driver major version number */
typedef uint8 tMajorVersion;
/* flash driver minor version number */
typedef uint8 tMinorVersion;
/* bugfix / patchlevel number */
typedef uint8 tBugfixVersion;
/* result of flash driver routines */
typedef uint16 tFlashResult; 

/* logical address */
typedef uint32 tFlashAddress; 
/* length (in bytes) */
typedef uint32 tFlashLength; 
/* ambiguous data */
typedef uint8 tFlashData; 
/* watchdog trigger routine */
typedef void (*tWDTriggerFct) (void); 

typedef struct 
{	
	/* initialization: input parameters */
	/* flash driver patch level version */
	tBugfixVersion patchLevel;
	/* flash driver minor version number */
	tMajorVersion minorNumber;
	/* flash driver major version number */
	tMinorVersion majorNumber;
	/* reserved for future use, set to 0x00 for now */
	uint8 reserved1; 
	/* retrun value / error code: output parameters */
	tFlashResult errorCode;
	/* reserved for future use, set to 0x0000 for now */
	uint16 reserved2; 
	/* erase / write: input parameters */
	 /* logical target address */
	tFlashAddress address;
	/* lenght information (in bytes) */
	tFlashLength length; 
	/* pointer to data buffer */
	const tFlashData *data;
	/* pointer to watchdog trigger routine */
	tWDTriggerFct wdTriggerFct; 
} tFlashParam;

/* prototype of flash driver routine */
typedef void (*tFlashFct) ( tFlashParam* flashParam ); 

/** flash driver header type */
typedef struct 
{	
	const uint8 mcuType;
	const uint8 maskType;
	const uint8 reserve;
	const uint8 interface;
	const tFlashFct flashInitFct;
	const tFlashFct flashDeInitFct;
	const tFlashFct flashEraseFct;
	const tFlashFct flashWriteFct;
    const uint32 reserved;
} tFlashInfo;

/*=======[E X T E R N A L   D A T A]==========================================*/
extern const tFlashInfo BLFlash_Info;


/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
void BlFlash_Init( tFlashParam* flashParam );
void BlFlash_Deinit ( tFlashParam* flashParam );
void BlFlash_Erase( tFlashParam* flashParam );
void BlFlash_Write ( tFlashParam* flashParam );

#endif


