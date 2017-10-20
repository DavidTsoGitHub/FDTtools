#ifndef FL_CFG_H
#define FL_CFG_H

/*=======[M A C R O S]========================================================*/
/** The physical memory location of boot request flag. LOCAL address*/
/* @type:uint32 range:0x00000000~0xFFFFFFFF note:NONE */
#define FL_BOOT_MODE    0x40017600uL

/** The physical memory location of application software update flag. LOCAL address*/
/* @type:uint32 range:0x00000000~0xFFFFFFFF note:NONE */
#define FL_APPL_UPDATE  0x40017601uL

/** Sleep time count:if configured 0,then no sleep function. */
/* @type:uint16 range:0~65535 note:unit ms */
#define FL_SLEEP_TIMER  0

#define FL_APPLICATION_BLOCK_TYPE	(uint8)0u
#define FL_CALIBRATION_BLOCK_TYPE	(uint8)1u

/** Total num of program blocks. */
/* @type:uint8 range:1~255 note:reference to num of FL_BlkInfo */
#define FL_NUM_LOGICAL_BLOCKS   (uint8)2u

/** Maximum of segment in one block. */
/* @type:uint8 range:1~255 note:NONE */
#define FL_MAX_SEGMENTS (uint8)3u

/** Value for fill gap,if configured 0xFF,then not execute. */
/* @type:uint8 range:0x00~0xFF note:NONE */
#define FL_GAP_FILL_VALUE   0xFFu

/* @type:define range:NONE note:auto generate */
#if (FL_GAP_FILL_VALUE == 0xFFu)
#define FL_USE_GAP_FILL FALSE
#else
#define  FL_USE_GAP_FILL TRUE
#endif

/** Start address of flash driver in RAM. LOCAL address */
/* @type:uint32 range:0x00000000~0xFFFFFFFF note:NONE */
#define FL_DEV_BASE_ADDRESS 0x4000A000uL

/** Length of flash driver in RAM. */
/* @type:uint32 range:0x00000000~0xFFFFFFFF note:NONE */
#define FL_DEV_SIZE 0x00001300uL

/*
** not standard config parameter
*/
/** Application RCHW address */
/* @type:uint32 range:0x0000C000~0xFFFFFFFF note:NONE */
#define FL_APP_RCHW_ADDR    0x1c000uL

/** Length of finger print information */
/* @type:uint16 range:1~65535 note:NONE */
#define FL_FINGER_PRINT_LENGTH  17u

/** bootloader information global address in NVM */
/* @type:uint32 range:NONE note:auto generate */
#define FL_NVM_INFO_ADDRESS  0x0000E000uL

/** maximum number of finger prints possible to store in the EEPROM (4096/24=170)*/
/* @type:uint8 range:1~255 note:NONE */
#define FL_MAX_NO_FINGER_PRINTS	170

/** code flash size that should program once time */
/* @type:uint32 range:NONE note:auto generate */
#define FL_FLASH_ALIGN_SIZE    (uint32)0x00000008uL

#define FL_MAX_PROGRAMMING_ATTEMPTS		1000u

/** Defines for DID positions in the PID area */
#define DID_BOOT_SW_ID					0x0000F50C	/* DID 0xF180 */
#define DID_BOOT_SW_ID_LEN				16	/* DID 0xF180 length */
#define DID_ECU_MANUFACTURING_DATE		0x0000F6C4	/* DID 0xF18B */
#define DID_ECU_MANUFACTURING_DATE_LEN	16	/* DID 0xF18B length*/
#define DID_ECU_SERIAL_NO				0x0000F670	/* DID 0xF18C */
#define DID_ECU_SERIAL_NO_LEN			20	/* DID 0xF18C length*/
#define DID_SYS_SUPL_ECU_HW_NO			0x0000F648	/* DID 0xF192 */
#define DID_SYS_SUPL_ECU_HW_NO_LEN		16	/* DID 0xF192 length */
#define DID_BOOT_ID						0x00000008	/* DID 0xF1FA */
#define DID_BOOT_ID_LEN					8	/* DID 0xF1FA length */

/* according to map file,determine the value of these macro */
#define  F_FLASHHEADER			0x4000a000
#define  F_FLASHHEADER_ROM	 	0x00009698
#define  F_FLASHHEADER_VOLUME	0x000000c8

#define  F_FLASHTEXT			0x4000a1a8
#define  F_FLASHTEXT_ROM		0x00009840
#define  F_FLASHTEXT_VOLUME		0x00001d08


extern uint32 _calibaration_start;
extern uint32 _application_start;
extern uint32 _calibaration_header_start;
extern uint32 _application_header_start;
extern uint32 _fingerprint_flash_start;


#endif/* endof FL_CFG_H */


