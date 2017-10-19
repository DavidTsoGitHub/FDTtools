/*
 * CanInterrupts.h
 *
 *  Created on: Sep 16, 2015
 *      Author: Tomas
 */

#ifndef CANINTERRUPTS_H_
#define CANINTERRUPTS_H_

void canA_MB00_03int(void);
void canA_MB04_07int(void);
void canA_MB08_11int(void);
void canA_MB12_15int(void);
void canA_MB16_31int(void);
void canA_MB32_63int(void);
void canA_ERR_ISR(void);

#endif /* CANINTERRUPTS_H_ */
