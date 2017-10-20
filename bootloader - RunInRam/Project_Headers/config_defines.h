/* -------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2009-2011 ArcCore AB <contact@arccore.com>
 * Licensed under ArcCore Embedded Software License Agreement.
 * -------------------------------- Arctic Core ------------------------------*/


#ifndef CONFIG_DEFINES_H_
#define CONFIG_DEFINES_H_


/* CAN_ID size */
#define CANID_11BIT                    0
#define CANID_29BIT                    1

/* Flags for CAN */
#define CAN_FLG_LOOPBACK        (1<<0)      /* Select loopback */
#define CAN_FLG_CLK_SRC_BUS     (1<<1)      /* Select bus clock instead of osc. */

/* MPC5xxx, CAN releated */
#define CAN_TX_BOX              0      /* box 0 */
#define CAN_RX_BOX_PHYSICAL     0      /* box 1 */
#define CAN_RX_BOX_FUNCTIONAL   1      /* box 2 */

/* default size */
#define CFG_SBL_SIZE                0x5000

#define APP_START_WITH_RESET	0
#define APP_START_WITH_CALL		1


#endif /* CONFIG_DEFINES_H_ */
