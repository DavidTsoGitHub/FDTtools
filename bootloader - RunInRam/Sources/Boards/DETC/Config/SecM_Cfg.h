#ifndef SECM_CFG_H
#define SECM_CFG_H

/*=======[M A C R O S]========================================================*/

/** value k for security access */
/* @type:uint32 range:0x00000000~0xFFFFFFFF note:NONE */
#define SEC_ECU_KEY (uint32)0xFFFFFFFFuL 

#define ORIGIN_ALGORITHM          1

#define ENABLE_DFLZ_AlGORITHM     0  /*1:enable the algorithm of securiy access about Dongfeng Liu Zhou EMS. 0:disable*/

#define ENABLE_DFMC_AlGORITHM     0  /*1:enable the algorithm of securiy access about Conti EMS. 0:disable*/

#if((ORIGIN_ALGORITHM+ENABLE_DFLZ_AlGORITHM+ENABLE_DFMC_AlGORITHM)>1)
#error "Please check the algorithm setting , more algorithm are enabled than one !"
#endif

#endif/* endof SECM_CFG_H */
