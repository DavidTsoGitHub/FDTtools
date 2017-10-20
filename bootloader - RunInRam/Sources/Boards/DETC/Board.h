/*
 * Board.h
 *
 *  Created on: Sep 22, 2016
 *      Author: think
 */
#include "Platform_Types.h"
#include "SecM.h"

#ifndef BOARD_H_
#define BOARD_H_

void Board_Init( void );
void Board_SetBootRequest(uint32 ProgRequest, uint32 InverseProgRequest);
uint8 Board_StayInBoot(void);
uint8 Board_IsStayInBoot(void);
void Board_Reset( void );
void Appl_BootStayTimer(void);
SecM_StatusType Board_ComputeKey(SecM_SeedType seed,
                                SecM_WordType k,
                                SecM_KeyType *key);
void Board_Tsw(void);
#endif /* BOARD_H_ */
