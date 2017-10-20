#ifndef BLFLASH_CFG_H_
#define BLFLASH_CFG_H_


/*=======[M A C R O S]========================================================*/

/* mcu type */
/* @type:define range:0~0xFF note:N/A */
#define BLFLASH_DRIVER_VERSION_MCUTYPE 0x12u

/* some mask number */
/* @type:define range:0~0xFF note:N/A */
#define BLFLASH_DRIVER_VERSION_MASKTYPE 0xabu 

/* interface version number */
/* @type:define range:0~0xFF note:N/A */
#define BLFLASH_DRIVER_VERSION_INTERFACE 0x01u 

/* major version number / interface */
/* @type:define range:0~0xFF note:N/A */
#define BLFLASH_DRIVER_VERSION_MAJOR 0x01u

/* minor version number / internal */
/* @type:define range:0~0xFF note:N/A */
#define BLFLASH_DRIVER_VERSION_MINOR 0x01u

/* bugfix / patchlevel */
/* @type:define range:0~0xFF note:N/A */
#define BLFLASH_DRIVER_VERSION_PATCH 0x00u 


#endif

