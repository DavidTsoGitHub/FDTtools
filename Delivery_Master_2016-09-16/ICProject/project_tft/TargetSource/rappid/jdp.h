/***************************************************************** 
 * PROJECT     : MPC560xS
 *               SPC560xS 
 * FILE        : jdp.h
 * 
 * DESCRIPTION : This is the header file describing the register
 *               set for the named microcontrollers
 * 
 * COPYRIGHT   :(c) 2009, Freescale & STMicroelectronics 
 * 
 * VERSION     : 0.1 - SPECTRUM 2
 * DATE        : March 31 2009 
 * AUTHOR      : ttz778 
 * HISTORY     : Based on jdp.h rev 1 for cut 1 MCUs
 ******************************************************************/

/*>>>>NOTE! this file is auto-generated please do not edit it!<<<<*/

/***************************************************************** 
* Example instantiation and use:            
*                                           
*  <MODULE>.<REGISTER>.B.<BIT> = 1;         
*  <MODULE>.<REGISTER>.R       = 0x10000000;
*                                           
******************************************************************/

#ifndef _JDP_H_
#define _JDP_H_

#include "typedefs.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef __MWERKS__
#pragma push
#pragma ANSI_strict off
#endif
/****************************************************************************/
/*                          MODULE : ADC                                   */
/****************************************************************************/
    struct ADC_tag {

        union {
            vuint32_t R;
            struct {
                vuint32_t OWREN:1;
                vuint32_t WLSIDE:1;
                vuint32_t MODE:1;
                vuint32_t EDGLEV:1;
                vuint32_t TRGEN:1;
                vuint32_t EDGE:1;
                vuint32_t XSTRTEN:1;
                vuint32_t NSTART:1;
                vuint32_t:1;
                vuint32_t JTRGEN:1;
                vuint32_t JEDGE:1;
                vuint32_t JSTART:1;
                vuint32_t:2;
                vuint32_t CTUEN:1;
                vuint32_t:8;
                vuint32_t ADCLKSEL:1;
                vuint32_t ABORTCHAIN:1;
                vuint32_t ABORT:1;
                vuint32_t ACK0:1;
                vuint32_t OFFREFRESH:1;
                vuint32_t OFFCANC:1;
                vuint32_t:2;
                vuint32_t PWDN:1;
            } B;
        } MCR;                 /* MAIN CONFIGURATION REGISTER */

        union {
            vuint32_t R;
            struct {
                vuint32_t:7;
                vuint32_t NSTART:1;
                vuint32_t JABORT:1;
                vuint32_t:2;
                vuint32_t JSTART:1;
                vuint32_t:3;
                vuint32_t CTUSTART:1;
                vuint32_t CHADDR:7;
                vuint32_t:3;
                vuint32_t ACK0:1;
                vuint32_t OFFREFRESH:1;
                vuint32_t OFFCANC:1;
                vuint32_t ADCSTATUS:3;
            } B;
        } MSR;                 /* MAIN STATUS REGISTER */

        int32_t ADC_reserved1[2];       /* (0x008 - 0x00F)/4 = 0x02 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:25;
                vuint32_t OFFCANCOVR:1;
                vuint32_t EOFFSET:1;
                vuint32_t EOCTU:1;
                vuint32_t JEOC:1;
                vuint32_t JECH:1;
                vuint32_t EOC:1;
                vuint32_t ECH:1;
            } B;
        } ISR;                 /* INTERRUPT STATUS REGISTER */

        union {
            vuint32_t R;
            struct {
                vuint32_t EOC31:1;
                vuint32_t EOC30:1;
                vuint32_t EOC29:1;
                vuint32_t EOC28:1;
                vuint32_t EOC27:1;
                vuint32_t EOC26:1;
                vuint32_t EOC25:1;
                vuint32_t EOC24:1;
                vuint32_t EOC23:1;
                vuint32_t EOC22:1;
                vuint32_t EOC21:1;
                vuint32_t EOC20:1;
                vuint32_t EOC19:1;
                vuint32_t EOC18:1;
                vuint32_t EOC17:1;
                vuint32_t EOC16:1;
                vuint32_t EOC15:1;
                vuint32_t EOC14:1;
                vuint32_t EOC13:1;
                vuint32_t EOC12:1;
                vuint32_t EOC11:1;
                vuint32_t EOC10:1;
                vuint32_t EOC9:1;
                vuint32_t EOC8:1;
                vuint32_t EOC7:1;
                vuint32_t EOC6:1;
                vuint32_t EOC5:1;
                vuint32_t EOC4:1;
                vuint32_t EOC3:1;
                vuint32_t EOC2:1;
                vuint32_t EOC1:1;
                vuint32_t EOC0:1;
            } B;
        } CEOCFR[3];                 /* Channel Pending Register 0 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:25;			//One bit added
                vuint32_t MSKOFFCANCOVR:1;	//Moved up
                vuint32_t MSKEOFFSET:1;		//Moved up
                vuint32_t MSKEOCTU:1;		//New for cut 2
                vuint32_t MSKJEOC:1;
                vuint32_t MSKJECH:1;
                vuint32_t MSKEOC:1;
                vuint32_t MSKECH:1;
            } B;
        } IMR;                  /* INTERRUPT MASK REGISTER */

        union {
            vuint32_t R;
            struct {
                vuint32_t CIM31:1;
                vuint32_t CIM30:1;
                vuint32_t CIM29:1;
                vuint32_t CIM28:1;
                vuint32_t CIM27:1;
                vuint32_t CIM26:1;
                vuint32_t CIM25:1;
                vuint32_t CIM24:1;
                vuint32_t CIM23:1;
                vuint32_t CIM22:1;
                vuint32_t CIM21:1;
                vuint32_t CIM20:1;
                vuint32_t CIM19:1;
                vuint32_t CIM18:1;
                vuint32_t CIM17:1;
                vuint32_t CIM16:1;
                vuint32_t CIM15:1;
                vuint32_t CIM14:1;
                vuint32_t CIM13:1;
                vuint32_t CIM12:1;
                vuint32_t CIM11:1;
                vuint32_t CIM10:1;
                vuint32_t CIM9:1;
                vuint32_t CIM8:1;
                vuint32_t CIM7:1;
                vuint32_t CIM6:1;
                vuint32_t CIM5:1;
                vuint32_t CIM4:1;
                vuint32_t CIM3:1;
                vuint32_t CIM2:1;
                vuint32_t CIM1:1;
                vuint32_t CIM0:1;
            } B;
        } CIMR[3];                 /* Channel Interrupt Mask Register 0 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:24;
                vuint32_t WDG3H:1;
                vuint32_t WDG2H:1;
                vuint32_t WDG1H:1;
                vuint32_t WDG0H:1;
                vuint32_t WDG3L:1;
                vuint32_t WDG2L:1;
                vuint32_t WDG1L:1;
                vuint32_t WDG0L:1;
            } B;
        } WTISR;               /* WATCHDOG INTERRUPT THRESHOLD REGISTER was WDGTHR */

        union {
            vuint32_t R;
            struct {
                vuint32_t:24;
                vuint32_t MSKWDG3H:1;
                vuint32_t MSKWDG2H:1;
                vuint32_t MSKWDG1H:1;
                vuint32_t MSKWDG0H:1;
                vuint32_t MSKWDG3L:1;
                vuint32_t MSKWDG2L:1;
                vuint32_t MSKWDG1L:1;
                vuint32_t MSKWDG0L:1;
            } B;
        } WTIMR;             /* WATCHDOG INTERRUPT MASK REGISTER was IMWDGTHR */

        int32_t ADC_reserved2[2];       /* (0x038 - 0x03F)/4 = 0x02 */

	union {
            vuint32_t R;
            struct {
                vuint32_t:30;			//was 16
                vuint32_t DCLR:1;		//moved
                vuint32_t DMAEN:1;		//moved
            } B;
        } DMAE;                 /* DMAE REGISTER */

        union {
            vuint32_t R;
            struct {
                vuint32_t DMA31:1;	//was unused [16]
                vuint32_t DMA30:1;
                vuint32_t DMA29:1;
                vuint32_t DMA28:1;
                vuint32_t DMA27:1;
                vuint32_t DMA26:1;
                vuint32_t DMA25:1;
                vuint32_t DMA24:1;
                vuint32_t DMA23:1;
                vuint32_t DMA22:1;
                vuint32_t DMA21:1;
                vuint32_t DMA20:1;
                vuint32_t DMA19:1;
                vuint32_t DMA18:1;
                vuint32_t DMA17:1;
                vuint32_t DMA16:1;
                vuint32_t DMA15:1;
                vuint32_t DMA14:1;
                vuint32_t DMA13:1;
                vuint32_t DMA12:1;
                vuint32_t DMA11:1;
                vuint32_t DMA10:1;
                vuint32_t DMA9:1;
                vuint32_t DMA8:1;
                vuint32_t DMA7:1;
                vuint32_t DMA6:1;
                vuint32_t DMA5:1;
                vuint32_t DMA4:1;
                vuint32_t DMA3:1;
                vuint32_t DMA2:1;
                vuint32_t DMA1:1;
                vuint32_t DMA0:1;
            } B;
        } DMAR[3];              /* DMA REGISTER  was [6] */

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t THREN:1;
                vuint32_t THRINV:1;
                vuint32_t THROP:1;
                  vuint32_t:6;
                vuint32_t THRCH:7;
            } B;
        } TRC[4];               /* ADC THRESHOLD REGISTER REGISTER */

        union {
            vuint32_t R;
            struct {		//were in TRA & TRB
                vuint32_t:4;
                vuint32_t THRH:12;
                vuint32_t:4;
                vuint32_t THRL:12;
            } B;
        } THRHLR[4];               /* THRESHOLD REGISTER */

        union {
            vuint32_t R;
            struct {		//were in TRAALT & TRBALT
                vuint32_t:4;
                vuint32_t THRH:12;
                vuint32_t:4;
                vuint32_t THRL:12;
            } B;
        } THRALT[4];            /* ADC THRESHOLD REGISTER REGISTER */

        union {
            vuint32_t R;
            struct {
                vuint32_t:25;		//was 26
                vuint32_t PREVAL2:2;	
                vuint32_t PREVAL1:2;
                vuint32_t PREVAL0:2;
                vuint32_t PREONCE:1;
            } B;
        } PSCR;               /* PRESAMPLING CONTROL REGISTER was PREREG */

        union {
            vuint32_t R;
            struct {
                vuint32_t PRES31:1;	//was reserved 16
                vuint32_t PRES30:1;
                vuint32_t PRES29:1;
                vuint32_t PRES28:1;
                vuint32_t PRES27:1;
                vuint32_t PRES26:1;
                vuint32_t PRES25:1;
                vuint32_t PRES24:1;
                vuint32_t PRES23:1;
                vuint32_t PRES22:1;
                vuint32_t PRES21:1;
                vuint32_t PRES20:1;
                vuint32_t PRES19:1;
                vuint32_t PRES18:1;
                vuint32_t PRES17:1;
                vuint32_t PRES16:1;
                vuint32_t PRES15:1;
                vuint32_t PRES14:1;
                vuint32_t PRES13:1;
                vuint32_t PRES12:1;
                vuint32_t PRES11:1;
                vuint32_t PRES10:1;
                vuint32_t PRES9:1;
                vuint32_t PRES8:1;
                vuint32_t PRES7:1;
                vuint32_t PRES6:1;
                vuint32_t PRES5:1;
                vuint32_t PRES4:1;
                vuint32_t PRES3:1;
                vuint32_t PRES2:1;
                vuint32_t PRES1:1;
                vuint32_t PRES0:1;
            } B;
        } PSR[3];              /* PRESAMPLING REGISTER was PRER[6]*/

        int32_t ADC_reserved3[1];       /* (0x090 - 0x093)/4 = 0x01 */

	union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t INPLATCH:1;
                  vuint32_t:1;
                vuint32_t OFFSHIFT:2;		//!!! This field only in CTR[0]
                  vuint32_t:1;
                vuint32_t INPCMP:2;
                  vuint32_t:1;
                vuint32_t INPSAMP:8;
            } B;
        } CTR[3];                /* CONVERSION TIMING REGISTER was CT[3] */

        int32_t ADC_reserved4[1];       /* (0x0A0 - 0x0A3)/4 = 0x01 */

	union {
            vuint32_t R;
            struct {
                vuint32_t CH31:1;		//was reserved 16
                vuint32_t CH30:1;
                vuint32_t CH29:1;
                vuint32_t CH28:1;
                vuint32_t CH27:1;
                vuint32_t CH26:1;
                vuint32_t CH25:1;
                vuint32_t CH24:1;
                vuint32_t CH23:1;
                vuint32_t CH22:1;
                vuint32_t CH21:1;
                vuint32_t CH20:1;
                vuint32_t CH19:1;
                vuint32_t CH18:1;
                vuint32_t CH17:1;
                vuint32_t CH16:1;
                vuint32_t CH15:1;
                vuint32_t CH14:1;
                vuint32_t CH13:1;
                vuint32_t CH12:1;
                vuint32_t CH11:1;
                vuint32_t CH10:1;
                vuint32_t CH9:1;
                vuint32_t CH8:1;
                vuint32_t CH7:1;
                vuint32_t CH6:1;
                vuint32_t CH5:1;
                vuint32_t CH4:1;
                vuint32_t CH3:1;
                vuint32_t CH2:1;
                vuint32_t CH1:1;
                vuint32_t CH0:1;
            } B;
        } NCMR[3];              /* NORMAL CONVERSION MASK REGISTER was [6] */

        int32_t ADC_reserved5[1];       /* (0x0B0 - 0x0B3)/4 = 0x01 */

        union {
            vuint32_t R;
            struct {
                vuint32_t CH31:1;		//was reserved 16
                vuint32_t CH30:1;
                vuint32_t CH29:1;
                vuint32_t CH28:1;
                vuint32_t CH27:1;
                vuint32_t CH26:1;
                vuint32_t CH25:1;
                vuint32_t CH24:1;
                vuint32_t CH23:1;
                vuint32_t CH22:1;
                vuint32_t CH21:1;
                vuint32_t CH20:1;
                vuint32_t CH19:1;
                vuint32_t CH18:1;
                vuint32_t CH17:1;
                vuint32_t CH16:1;
                vuint32_t CH15:1;
                vuint32_t CH14:1;
                vuint32_t CH13:1;
                vuint32_t CH12:1;
                vuint32_t CH11:1;
                vuint32_t CH10:1;
                vuint32_t CH9:1;
                vuint32_t CH8:1;
                vuint32_t CH7:1;
                vuint32_t CH6:1;
                vuint32_t CH5:1;
                vuint32_t CH4:1;
                vuint32_t CH3:1;
                vuint32_t CH2:1;
                vuint32_t CH1:1;
                vuint32_t CH0:1;
            } B;
        } JCMR[3];              /* Injected CONVERSION MASK REGISTER was ICMR[6] */

        union {
            vuint32_t R;
            struct {
                vuint32_t:15;
                vuint32_t OFFSETLOAD:1;		//new
                vuint32_t:8;
                vuint32_t OFFSETWORD:8;
            } B;
        } OFFWR;               /* OFFSET WORD REGISTER was OFFREG*/

        union {
            vuint32_t R;
            struct {
                vuint32_t:24;
                vuint32_t DSD:8;
            } B;
        } DSDR;                  /* DECODE SIGNALS DELAY REGISTER was DSD */

        union {
            vuint32_t R;
            struct {
                vuint32_t:24;
                vuint32_t PDED:8;	//was PDD
            } B;
        } PDEDR;                  /* POWER DOWN DELAY REGISTER was PDD */

        int32_t ADC_reserved6[9];       /* (0x0CC - 0x0EF)/4 = 0x09 */

        union {
            vuint32_t R;
            struct {
                vuint32_t TEST_CTL:32;
            } B;
        } TCTLR;                 /* Test control REGISTER */

        int32_t ADC_reserved7[3];       /* (0x0F4 - 0x0FF)/4 = 0x03 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:12;
                vuint32_t VALID:1;
                vuint32_t OVERW:1;
                vuint32_t RESULT:2;
                vuint32_t:4;
                vuint32_t CDATA:12;
            } B;
        } CDR[96];      /* Channel 0-95 Data REGISTER */

    };                          /* end of ADC_tag */
/****************************************************************************/
/*                          MODULE : CANSP                                   */
/****************************************************************************/
    struct CANSP_tag {
        union {
            vuint16_t R;
            struct {
                vuint16_t RX_COMPLETE:1;
                vuint16_t BUSY:1;
                vuint16_t ACTIVE_CK:1;
                  vuint16_t:3;
                vuint16_t MODE:1;
                vuint16_t CAN_RX_SEL:3;
                vuint16_t BRP:5;
                vuint16_t CAN_SMPLR_EN:1;
            } B;
        } CR;                   /* CANSP Control Register */

        int16_t CANSP_reserved;

        union {
            vuint32_t R;
        } SR[12];               /* CANSP Sample Register 0 to 11 */

    };                          /* end of CANSP_tag */
/****************************************************************************/
/*                          MODULE : ECSM                                   */
/****************************************************************************/
    struct ECSM_tag {

        union {
            vuint16_t R;
        } PCT;                  /* ECSM Processor Core Type Register */

        union {
            vuint16_t R;
        } REV;                  /* ECSM  Revision Register */

        int32_t MCM_reserved;

        union {
            vuint32_t R;
        } IMC;                   /* IPS Configuration Register */

        int8_t MCM_reserved1[3];

        union {
            vuint8_t R;
            struct {
                vuint8_t POR:1;
                vuint8_t DIR:1;
                  vuint8_t:6;
            } B;
        } MRSR;                 /* ECSM Miscellaneous Reset Status Register */

        int8_t MCM_reserved2[3];

        union {
            vuint8_t R;
            struct {
                vuint8_t ENBWCR:1;
                  vuint8_t:3;
                vuint8_t PRILVL:4;
            } B;
        } MWCR;                 /* ECSM Miscellaneous Wakeup Control Register */

        int32_t MCM_reserved3[2];
        int8_t MCM_reserved4[3];

        union {
            vuint8_t R;
            struct {
                vuint8_t FB0AI:1;
                vuint8_t FB0SI:1;
                vuint8_t FB1AI:1;
                vuint8_t FB1SI:1;
                  vuint8_t:4;
            } B;
        } MIR;                  /* ECSM Miscellaneous Interrupt Register */

        int32_t MCM_reserved5;

        union {
            vuint32_t R;
        } MUDCR;                /* ECSM Miscellaneous User-Defined Control Register */

        int32_t MCM_reserved6[6];       /* (0x040- 0x028)/4 = 0x06 */
        int8_t MCM_reserved7[3];

        union {
            vuint8_t R;
            struct {
                vuint8_t:2;
                vuint8_t ER1BR:1;
                vuint8_t EF1BR:1;
                  vuint8_t:2;
                vuint8_t ERNCR:1;
                vuint8_t EFNCR:1;
            } B;
        } ECR;                  /* ECSM ECC Configuration Register */

        int8_t MCM_reserved8[3];

        union {
            vuint8_t R;
            struct {
                vuint8_t:2;
                vuint8_t R1BC:1;
                vuint8_t F1BC:1;
                  vuint8_t:2;
                vuint8_t RNCE:1;
                vuint8_t FNCE:1;
            } B;
        } ESR;                  /* ECSM ECC Status Register */

        int16_t MCM_reserved9;

        union {
            vuint16_t R;
            struct {
                vuint16_t:2;
                vuint16_t FRC1BI:1;
                vuint16_t FR11BI:1;
                  vuint16_t:2;
                vuint16_t FRCNCI:1;
                vuint16_t FR1NCI:1;
                  vuint16_t:1;
                vuint16_t ERRBIT:7;
            } B;
        } EEGR;                 /* ECSM ECC Error Generation Register */

        int32_t MCM_reserved10;

        union {
            vuint32_t R;
        } FEAR;                 /* ECSM Flash ECC Address Register */

        int16_t MCM_reserved11;

        union {
            vuint8_t R;
            struct {
                vuint8_t:4;
                vuint8_t FEMR:4;
            } B;
        } FEMR;                 /* ECSM Flash ECC Master Number Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t WRITE:1;
                vuint8_t SIZE:3;
                vuint8_t PROTECTION:4;
            } B;
        } FEAT;                 /* ECSM Flash ECC Attributes Register */

        int32_t MCM_reserved12;

        union {
            vuint32_t R;
        } FEDR;                 /* ECSM Flash ECC Data Register */

        union {
            vuint32_t R;
        } REAR;                 /* ECSM RAM ECC Address Register */

        int8_t MCM_reserved13;

        union {
            vuint8_t R;
        } RESR;                 /* ECSM RAM ECC Address Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t:4;
                vuint8_t REMR:4;
            } B;
        } REMR;                 /* ECSM RAM ECC Master Number Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t WRITE:1;
                vuint8_t SIZE:3;
                vuint8_t PROTECTION:4;
            } B;
        } REAT;                 /* ECSM RAM ECC Attributes Register */

        int32_t MCM_reserved14;

        union {
            vuint32_t R;
        } REDR;                 /* ECSM RAM ECC Data Register */

    };                          /* end of ECSM_tag */
/****************************************************************************/
/*                          MODULE : RTC                                   */
/****************************************************************************/
    struct RTC_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t SUPV:1;
                  vuint32_t:31;
            } B;
        } RTCSUPV;              /* RTC Supervisor Control Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t CNTEN:1;
                vuint32_t RTCIE:1;
                vuint32_t FRZEN:1;
                vuint32_t ROVREN:1;
                vuint32_t RTCVAL:12;
                vuint32_t APIEN:1;
                vuint32_t APIE:1;
                vuint32_t CLKSEL:2;
                vuint32_t DIV512EN:1;
                vuint32_t DIV32EN:1;
                vuint32_t APIVAL:10;
            } B;
        } RTCC;                 /* RTC Control Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:2;
                vuint32_t RTCF:1;
                  vuint32_t:15;
                vuint32_t APIF:1;
                  vuint32_t:2;
                vuint32_t ROVRF:1;
                  vuint32_t:10;
            } B;
        } RTCS;                 /* RTC Status Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t RTCCNT:32;
            } B;
        } RTCCNT;               /* RTC Counter Register */

    };                          /* end of RTC_tag */
/****************************************************************************/
/*                     MODULE : SIU                                         */
/****************************************************************************/
    struct SIU_tag {

        int32_t SIU_reserved0;

        union {                 /* MCU ID Register 1 */
            vuint32_t R;
            struct {
                vuint32_t PARTNUM:16;
                vuint32_t PKG:4;
                vuint32_t MASKNUM:12;
            } B;
        } MIDR;

        union {                 /* MCU ID Register 2 */
            vuint32_t R;
            struct {
                vuint32_t SF:1;
                vuint32_t FLASH_SIZE_1:4;
                vuint32_t FLASH_SIZE_2:4;
                  vuint32_t:7;
                vuint32_t PARTNUM:8;
                  vuint32_t:3;
                vuint32_t EE:1;
                  vuint32_t:4;
            } B;
        } MIDR2;

        int32_t SIU_reserved1[2];

        union {                 /* Interrupt Status Flag Register */
            vuint32_t R;
            struct {
                vuint32_t EIF31:1;
                vuint32_t EIF30:1;
                vuint32_t EIF29:1;
                vuint32_t EIF28:1;
                vuint32_t EIF27:1;
                vuint32_t EIF26:1;
                vuint32_t EIF25:1;
                vuint32_t EIF24:1;
                vuint32_t EIF23:1;
                vuint32_t EIF22:1;
                vuint32_t EIF21:1;
                vuint32_t EIF20:1;
                vuint32_t EIF19:1;
                vuint32_t EIF18:1;
                vuint32_t EIF17:1;
                vuint32_t EIF16:1;
                vuint32_t EIF15:1;
                vuint32_t EIF14:1;
                vuint32_t EIF13:1;
                vuint32_t EIF12:1;
                vuint32_t EIF11:1;
                vuint32_t EIF10:1;
                vuint32_t EIF9:1;
                vuint32_t EIF8:1;
                vuint32_t EIF7:1;
                vuint32_t EIF6:1;
                vuint32_t EIF5:1;
                vuint32_t EIF4:1;
                vuint32_t EIF3:1;
                vuint32_t EIF2:1;
                vuint32_t EIF1:1;
                vuint32_t EIF0:1;
            } B;
        } ISR;

        union {                 /* Interrupt Request Enable Register */
            vuint32_t R;
            struct {
                vuint32_t EIRE31:1;
                vuint32_t EIRE30:1;
                vuint32_t EIRE29:1;
                vuint32_t EIRE28:1;
                vuint32_t EIRE27:1;
                vuint32_t EIRE26:1;
                vuint32_t EIRE25:1;
                vuint32_t EIRE24:1;
                vuint32_t EIRE23:1;
                vuint32_t EIRE22:1;
                vuint32_t EIRE21:1;
                vuint32_t EIRE20:1;
                vuint32_t EIRE19:1;
                vuint32_t EIRE18:1;
                vuint32_t EIRE17:1;
                vuint32_t EIRE16:1;
                vuint32_t EIRE15:1;
                vuint32_t EIRE14:1;
                vuint32_t EIRE13:1;
                vuint32_t EIRE12:1;
                vuint32_t EIRE11:1;
                vuint32_t EIRE10:1;
                vuint32_t EIRE9:1;
                vuint32_t EIRE8:1;
                vuint32_t EIRE7:1;
                vuint32_t EIRE6:1;
                vuint32_t EIRE5:1;
                vuint32_t EIRE4:1;
                vuint32_t EIRE3:1;
                vuint32_t EIRE2:1;
                vuint32_t EIRE1:1;
                vuint32_t EIRE0:1;
            } B;
        } IRER;

        int32_t SIU_reserved2[3];

        union {                 /* Interrupt Rising-Edge Event Enable Register */
            vuint32_t R;
            struct {
                vuint32_t IREE31:1;
                vuint32_t IREE30:1;
                vuint32_t IREE29:1;
                vuint32_t IREE28:1;
                vuint32_t IREE27:1;
                vuint32_t IREE26:1;
                vuint32_t IREE25:1;
                vuint32_t IREE24:1;
                vuint32_t IREE23:1;
                vuint32_t IREE22:1;
                vuint32_t IREE21:1;
                vuint32_t IREE20:1;
                vuint32_t IREE19:1;
                vuint32_t IREE18:1;
                vuint32_t IREE17:1;
                vuint32_t IREE16:1;
                vuint32_t IREE15:1;
                vuint32_t IREE14:1;
                vuint32_t IREE13:1;
                vuint32_t IREE12:1;
                vuint32_t IREE11:1;
                vuint32_t IREE10:1;
                vuint32_t IREE9:1;
                vuint32_t IREE8:1;
                vuint32_t IREE7:1;
                vuint32_t IREE6:1;
                vuint32_t IREE5:1;
                vuint32_t IREE4:1;
                vuint32_t IREE3:1;
                vuint32_t IREE2:1;
                vuint32_t IREE1:1;
                vuint32_t IREE0:1;
            } B;
        } IREER;

        union {                 /* Interrupt Falling-Edge Event Enable Register */
            vuint32_t R;
            struct {
                vuint32_t IFEE31:1;
                vuint32_t IFEE30:1;
                vuint32_t IFEE29:1;
                vuint32_t IFEE28:1;
                vuint32_t IFEE27:1;
                vuint32_t IFEE26:1;
                vuint32_t IFEE25:1;
                vuint32_t IFEE24:1;
                vuint32_t IFEE23:1;
                vuint32_t IFEE22:1;
                vuint32_t IFEE21:1;
                vuint32_t IFEE20:1;
                vuint32_t IFEE19:1;
                vuint32_t IFEE18:1;
                vuint32_t IFEE17:1;
                vuint32_t IFEE16:1;
                vuint32_t IFEE15:1;
                vuint32_t IFEE14:1;
                vuint32_t IFEE13:1;
                vuint32_t IFEE12:1;
                vuint32_t IFEE11:1;
                vuint32_t IFEE10:1;
                vuint32_t IFEE9:1;
                vuint32_t IFEE8:1;
                vuint32_t IFEE7:1;
                vuint32_t IFEE6:1;
                vuint32_t IFEE5:1;
                vuint32_t IFEE4:1;
                vuint32_t IFEE3:1;
                vuint32_t IFEE2:1;
                vuint32_t IFEE1:1;
                vuint32_t IFEE0:1;
            } B;
        } IFEER;

        union {                 /* Interrupt Filter Enable Register */
            vuint32_t R;
            struct {
                vuint32_t IFE31:1;
                vuint32_t IFE30:1;
                vuint32_t IFE29:1;
                vuint32_t IFE28:1;
                vuint32_t IFE27:1;
                vuint32_t IFE26:1;
                vuint32_t IFE25:1;
                vuint32_t IFE24:1;
                vuint32_t IFE23:1;
                vuint32_t IFE22:1;
                vuint32_t IFE21:1;
                vuint32_t IFE20:1;
                vuint32_t IFE19:1;
                vuint32_t IFE18:1;
                vuint32_t IFE17:1;
                vuint32_t IFE16:1;
                vuint32_t IFE15:1;
                vuint32_t IFE14:1;
                vuint32_t IFE13:1;
                vuint32_t IFE12:1;
                vuint32_t IFE11:1;
                vuint32_t IFE10:1;
                vuint32_t IFE9:1;
                vuint32_t IFE8:1;
                vuint32_t IFE7:1;
                vuint32_t IFE6:1;
                vuint32_t IFE5:1;
                vuint32_t IFE4:1;
                vuint32_t IFE3:1;
                vuint32_t IFE2:1;
                vuint32_t IFE1:1;
                vuint32_t IFE0:1;
            } B;
        } IFER;

        int32_t SIU_reserved3[3];

        union {                        /* Pad Configuration Registers */
            vuint16_t R;
            struct {
                  vuint16_t:1;
                vuint16_t SMC:1;
                vuint16_t APC:1;
                  vuint16_t:1;
                vuint16_t PA:2;
                vuint16_t OBE:1;
                vuint16_t IBE:1;
                 vuint16_t:2;
                vuint16_t ODE:1;
                 vuint16_t:2;
                vuint16_t SRC:1;
                vuint16_t WPE:1;
                vuint16_t WPS:1;
            } B;
        } PCR[133];

        int16_t SIU_reserved4;         /* {0x14A} */
        int32_t SIU_reserved5[237];    /* {0x500-0x14C}/0x4 */

        union {                 /* Pad Selection for Multiplexed Input Register */
            vuint8_t R;
            struct {
                vuint8_t:4;
                vuint8_t PADSEL:4;
            } B;
        } PSMI[256];

        union {                 /* GPIO Pin Data Output Registers */
            vuint8_t R;
            struct {
                vuint8_t:7;
                vuint8_t PDO:1;
            } B;
        } GPDO[133];

        int8_t SIU_reserved8[3];       /* {0x685-687} */
        int32_t SIU_reserved9[94];     /* {0x800-0x688}/0x4 */

        union {                 /* GPIO Pin Data Input Registers */
            vuint8_t R;
            struct {
                vuint8_t:7;
                vuint8_t PDI:1;
            } B;
        } GPDI[133];

        int8_t SIU_reserved10[3];      /* {0x885-887} */
        int32_t SIU_reserved11[222];   /* {0xC00-0x888}/0x4 */

        union {                 /* Parallel GPIO Pin Data Output Register */
            vuint32_t R;
            struct {
                vuint32_t PPD0:32;
            } B;
        } PGPDO[5];

        union {                 /* Parallel GPIO Pin Data Input Register */
            vuint32_t R;
            struct {
                vuint32_t PPDI:32;
            } B;
        } PGPDI[16];

        union {                 /* Masked Parallel GPIO Pin Data Out Register */
            vuint32_t R;
            struct {
                vuint32_t MASK:16;
                vuint32_t MPPDO:16;
            } B;
        } MPGPDO[32];

        int32_t SIU_reserved6[192];     /* {0x1000-0x0D00}/0x4 */

        union {                 /* Interrupt Filter Maximum Counter Register */
            vuint32_t R;
            struct {
                vuint32_t:28;
                vuint32_t MAXCNT:4;
            } B;
        } IFMC[32];

        union {                 /* Interrupt Filter Clock Prescaler Register */
            vuint32_t R;
            struct {
                vuint32_t:28;
                vuint32_t IFCP:4;
            } B;
        } IFCPR;

    };                          /* end of SIU_tag */
/****************************************************************************/
/*                          MODULE : SSCM                                   */
/****************************************************************************/
    struct SSCM_tag {
        union {
            vuint16_t R;
            struct {
                vuint16_t:4;
                vuint16_t NXEN:1;
                  vuint16_t:1;
                vuint16_t SEC:1;
                  vuint16_t:1;
                vuint16_t BMODE:3;
                vuint16_t DMID:1;
                vuint16_t ABD:1;
                  vuint16_t:3;
            } B;
        } STATUS;               /* Status Register */

        union {
            vuint16_t R;
            struct {
                vuint16_t SRAMSIZE:5;
                vuint16_t IFLASHSIZE:5;
                vuint16_t IVLD:1;
                vuint16_t DFLASHSIZE:4;
                vuint16_t DVLD:1;
            } B;
        } MEMCONFIG;            /* System Memory Configuration Register */

        int16_t SSCM_reserved;

        union {
            vuint16_t R;
            struct {
                vuint16_t:14;
                vuint16_t PAE:1;
                vuint16_t RAE:1;
            } B;
        } ERROR;                /* Error Configuration Register */

        union {
            vuint16_t R;
            struct {
                vuint16_t:13;
                vuint16_t DEBUG_MODE:3;
            } B;
        } DEBUGPORT;            /* Debug Status Port Register */

        int16_t SSCM_reserved1;

        union {
            vuint32_t R;
            struct {
                vuint32_t PWD_HI:32;
            } B;
        } PWCMPH;               /* Password Comparison Register High Word */

        union {
            vuint32_t R;
            struct {
                vuint32_t PWD_LO:32;
            } B;
        } PWCMPL;               /* Password Comparison Register Low Word */

    };                          /* end of SSCM_tag */
/****************************************************************************/
/*                          MODULE : STM                                   */
/****************************************************************************/
    struct STM_tag {

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t CPS:8;
                  vuint32_t:6;
                vuint32_t FRZ:1;
                vuint32_t TEN:1;
            } B;
        } CR0;                  /* STM Control Register */

        union {
            vuint32_t R;
        } CNT0;                 /* STM Count Register */

        int32_t STM_reserved[2];

        union {
            vuint32_t R;
            struct {
                vuint32_t:31;
                vuint32_t CEN:1;
            } B;
        } CCR0;                 /* STM Channel Control Register 0 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:31;
                vuint32_t CIF:1;
            } B;
        } CIR0;                 /* STM Channel Interrupt Register 0 */

        union {
            vuint32_t R;
        } CMP0;                 /* STM Channel Compare Register 0 */

        int32_t STM_reserved1;

        union {
            vuint32_t R;
            struct {
                vuint32_t:31;
                vuint32_t CEN:1;
            } B;
        } CCR1;                 /* STM Channel Control Register 1 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:31;
                vuint32_t CIF:1;
            } B;
        } CIR1;                 /* STM Channel Interrupt Register 1 */

        union {
            vuint32_t R;
        } CMP1;                 /* STM Channel Compare Register 1 */

        int32_t STM_reserved2;

        union {
            vuint32_t R;
            struct {
                vuint32_t:31;
                vuint32_t CEN:1;
            } B;
        } CCR2;                 /* STM Channel Control Register 2 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:31;
                vuint32_t CIF:1;
            } B;
        } CIR2;                 /* STM Channel Interrupt Register 2 */

        union {
            vuint32_t R;
        } CMP2;                 /* STM Channel Compare Register 2 */

        int32_t STM_reserved3;

        union {
            vuint32_t R;
            struct {
                vuint32_t:31;
                vuint32_t CEN:1;
            } B;
        } CCR3;                 /* STM Channel Control Register 3 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:31;
                vuint32_t CIF:1;
            } B;
        } CIR3;                 /* STM Channel Interrupt Register 3 */

        union {
            vuint32_t R;
        } CMP3;                 /* STM Channel Compare Register 3 */

    };                          /* end of STM_tag */
/****************************************************************************/
/*                          MODULE : SWT                                   */
/****************************************************************************/
    struct SWT_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t MAP0:1;
                vuint32_t MAP1:1;
                vuint32_t MAP2:1;
                vuint32_t MAP3:1;
                vuint32_t MAP4:1;
                vuint32_t MAP5:1;
                vuint32_t MAP6:1;
                vuint32_t MAP7:1;
                  vuint32_t:15;
                vuint32_t RIA:1;
                vuint32_t WND:1;
                vuint32_t ITR:1;
                vuint32_t HLK:1;
                vuint32_t SLK:1;
                vuint32_t CSL:1;
                vuint32_t STP:1;
                vuint32_t FRZ:1;
                vuint32_t WEN:1;
            } B;
        } CR;                   /* SWT Control Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:31;
                vuint32_t TIF:1;
            } B;
        } IR;                   /* SWT Interrupt Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t WTO:32;
            } B;
        } TO;                   /* SWT Time-Out Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t WST:32;
            } B;
        } WN;                   /* SWT Window Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t WSC:16;
            } B;
        } SR;                   /* SWT Service Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t CNT:32;
            } B;
        } CO;                   /* SWT Counter Output Register */

    };                          /* end of SWT_tag */
/****************************************************************************/
/*                          MODULE : WKUP                                   */
/****************************************************************************/
    struct WKUP_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t NIF0:1;
                vuint32_t NOVF0:1;
                  vuint32_t:6;
                vuint32_t NIF1:1;
                vuint32_t NOVF1:1;
                  vuint32_t:6;
                vuint32_t NIF2:1;
                vuint32_t NOVF2:1;
                  vuint32_t:6;
                vuint32_t NIF3:1;
                vuint32_t NOVF3:1;
                  vuint32_t:6;
            } B;
        } NSR;                  /* NMI Status Register */

        int32_t WKUP_reserved;

        union {
            vuint32_t R;
            struct {
                vuint32_t NLOCK0:1;
                vuint32_t NDSS0:2;
                vuint32_t NWRE0:1;
                  vuint32_t:1;
                vuint32_t NREE0:1;
                vuint32_t NFEE0:1;
                vuint32_t NFE0:1;
                vuint32_t NLOCK1:1;
                vuint32_t NDSS1:2;
                vuint32_t NWRE1:1;
                  vuint32_t:1;
                vuint32_t NREE1:1;
                vuint32_t NFEE1:1;
                vuint32_t NFE1:1;
                vuint32_t NLOCK2:1;
                vuint32_t NDSS2:2;
                vuint32_t NWRE2:1;
                  vuint32_t:1;
                vuint32_t NREE2:1;
                vuint32_t NFEE2:1;
                vuint32_t NFE2:1;
                vuint32_t NLOCK3:1;
                vuint32_t NDSS3:2;
                vuint32_t NWRE3:1;
                  vuint32_t:1;
                vuint32_t NREE3:1;
                vuint32_t NFEE3:1;
                vuint32_t NFE3:1;
            } B;
        } NCR;                  /* NMI Configuration Register */

        int32_t WKUP_reserved1[2];

        union {
            vuint32_t R;
            struct {
                vuint32_t EIF:32;
            } B;
        } WISR;                 /* Wakeup/Interrupt Status Flag Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t EIRE:32;
            } B;
        } IRER;                 /* Interrupt Request Enable Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t WRE:32;
            } B;
        } WRER;                 /* Wakeup Request Enable Register */

        int32_t WKUP_reserved2[2];

        union {
            vuint32_t R;
            struct {
                vuint32_t IREE:32;
            } B;
        } WIREER;               /* Wakeup/Interrupt Rising-Edge Event Enable Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t IFEE:32;
            } B;
        } WIFEER;               /* Wakeup/Interrupt Falling-Edge Event Enable Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t IFE:32;
            } B;
        } WIFER;                /* Wakeup/Interrupt Filter Enable Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t IPUE:32;
            } B;
        } WIPUER;               /* Wakeup/Interrupt Pullup Enable Register */

    };                          /* end of WKUP_tag */
/****************************************************************************/
/*                          MODULE : LINFLEX                                */
/****************************************************************************/

    struct LINFLEX_tag {

        int16_t LINFLEX_reserved1;

        union {
            vuint16_t R;
            struct {
                vuint16_t CCD:1;
                vuint16_t CFD:1;
                vuint16_t LASE:1;
                vuint16_t AWUM:1;       // LCH vuint16_t AUTOWU:1;
                vuint16_t MBL:4;
                vuint16_t BF:1;
                vuint16_t SLFM:1;
                vuint16_t LBKM:1;
                vuint16_t MME:1;
                vuint16_t SBDT:1;       // LCH vuint16_t SSBL:1;
                vuint16_t RBLM:1;
                vuint16_t SLEEP:1;
                vuint16_t INIT:1;
            } B;
        } LINCR1;               /* LINFLEX LIN Control Register 1 */

        int16_t LINFLEX_reserved2;

        union {
            vuint16_t R;
            struct {
                vuint16_t SZIE:1;
                vuint16_t OCIE:1;
                vuint16_t BEIE:1;
                vuint16_t CEIE:1;
                vuint16_t HEIE:1;
                  vuint16_t:2;
                vuint16_t FEIE:1;
                vuint16_t BOIE:1;
                vuint16_t LSIE:1;
                vuint16_t WUIE:1;
                vuint16_t DBFIE:1;
                vuint16_t DBEIE:1;
                vuint16_t DRIE:1;
                vuint16_t DTIE:1;
                vuint16_t HRIE:1;
            } B;
        } LINIER;               /* LINFLEX LIN Interrupt Enable Register */

        int16_t LINFLEX_reserved3;

        union {
            vuint16_t R;
            struct {
                vuint16_t LINS:4;
                  vuint16_t:2;
                vuint16_t RMB:1;
                  vuint16_t:1;
                vuint16_t RBSY:1;       // LCH vuint16_t RXBUSY:1;
                vuint16_t RPS:1;        // LCH vuint16_t RDI:1;
                vuint16_t WUF:1;
                vuint16_t DBFF:1;
                vuint16_t DBEF:1;
                vuint16_t DRF:1;
                vuint16_t DTF:1;
                vuint16_t HRF:1;
            } B;
        } LINSR;                /* LINFLEX LIN Status Register */

        int16_t LINFLEX_reserved4;

        union {
            vuint16_t R;
            struct {
                vuint16_t SZF:1;
                vuint16_t OCF:1;
                vuint16_t BEF:1;
                vuint16_t CEF:1;
                vuint16_t SFEF:1;
                vuint16_t SDEF:1;
                vuint16_t IDPEF:1;
                vuint16_t FEF:1;
                vuint16_t BOF:1;
                  vuint16_t:6;
                vuint16_t NF:1;
            } B;
        } LINESR;               /* LINFLEX LIN Error Status Register */

        int16_t LINFLEX_reserved5;

        union {
            vuint16_t R;
            struct {
                vuint16_t:1;
                vuint16_t TDFL:2;
                  vuint16_t:1;
                vuint16_t RDFL:2;
                  vuint16_t:4;
                vuint16_t RXEN:1;
                vuint16_t TXEN:1;
                vuint16_t OP:1; //LCH vuint16_t PARITYODD:1;
                vuint16_t PCE:1;
                vuint16_t WL:1;
                vuint16_t UART:1;
            } B;
        } UARTCR;               /* LINFLEX UART Mode Control Register */

        int16_t LINFLEX_reserved6;

        union {
            vuint16_t R;
            struct {
                vuint16_t SZF:1;
                vuint16_t OCF:1;
                vuint16_t PE:4;
                vuint16_t RMB:1;
                vuint16_t FEF:1;
                vuint16_t BOF:1;
                vuint16_t RPS:1;        // LCH vuint16_t RDI:1;
                vuint16_t WUF:1;
                  vuint16_t:2;
                vuint16_t DRF:1;
                vuint16_t DTF:1;
                vuint16_t NF:1;
            } B;
        } UARTSR;               /* LINFLEX UART Mode Status Register */

        int16_t LINFLEX_reserved7;

        union {
            vuint16_t R;
            struct {
                vuint16_t:5;
                vuint16_t LTOM:1;       //LCH vuint16_t MODE:1;
                vuint16_t IOT:1;
                vuint16_t TOCE:1;
                vuint16_t CNT:8;
            } B;
        } LINTCSR;              /* LINFLEX LIN Time-Out Control Status Register */

        int16_t LINFLEX_reserved8;

        union {
            vuint16_t R;
            struct {
                vuint16_t OC2:8;
                vuint16_t OC1:8;
            } B;
        } LINOCR;               /* LINFLEX LIN Output Compare Register */

        int16_t LINFLEX_reserved9;

        union {
            vuint16_t R;
            struct {
                vuint16_t:4;
                vuint16_t RTO:4;        // LCH vuint16_t RTC:4;
                  vuint16_t:1;
                vuint16_t HTO:7;        // LCH vuint16_t HTC:7;
            } B;
        } LINTOCR;              /* LINFLEX LIN Output Compare Register */

        int16_t LINFLEX_reserved10;

        union {
            vuint16_t R;
            struct {
                vuint16_t:12;
                vuint16_t DIV_F:4;      // LCH vuint16_t FBR:4;
            } B;
        } LINFBRR;              /* LINFLEX LIN Fractional Baud Rate Register */

        int16_t LINFLEX_reserved11;

        union {
            vuint16_t R;
            struct {
                vuint16_t:3;
                vuint16_t DIV_M:13;     // LCH vuint16_t IBR:13;
            } B;
        } LINIBRR;              /* LINFLEX LIN Integer Baud Rate Register */

        int16_t LINFLEX_reserved12;

        union {
            vuint16_t R;
            struct {
                vuint16_t:8;
                vuint16_t CF:8;
            } B;
        } LINCFR;               /* LINFLEX LIN Checksum Field Register */

        int16_t LINFLEX_reserved13;

        union {
            vuint16_t R;
            struct {
                vuint16_t:1;
                vuint16_t IOBE:1;
                vuint16_t IOPE:1;
                vuint16_t WURQ:1;
                vuint16_t DDRQ:1;
                vuint16_t DTRQ:1;
                vuint16_t ABRQ:1;
                vuint16_t HTRQ:1;
                  vuint16_t:8;
            } B;
        } LINCR2;               /* LINFLEX LIN Control Register 2 */

        int16_t LINFLEX_reserved14;

        union {
            vuint16_t R;
            struct {
                vuint16_t DFL:6;
                vuint16_t DIR:1;
                vuint16_t CCS:1;
                  vuint16_t:2;  // LCH vuint16_t:1;
                vuint16_t ID:6;
            } B;
        } BIDR;                 /* LINFLEX Buffer Identifier Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t DATA3:8;
                vuint32_t DATA2:8;
                vuint32_t DATA1:8;
                vuint32_t DATA0:8;
            } B;
        } BDRL;                 /* LINFLEX Buffer Data Register Least Significant */

        union {
            vuint32_t R;
            struct {
                vuint32_t DATA7:8;
                vuint32_t DATA6:8;
                vuint32_t DATA5:8;
                vuint32_t DATA4:8;
            } B;
        } BDRM;                 /* LINFLEX Buffer Data Register Most Significant */

        int16_t LINFLEX_reserved15;

        union {
            vuint16_t R;
            struct {
                vuint16_t:8;
                vuint16_t FACT:8;
            } B;
        } IFER;                 /* LINFLEX Identifier Filter Enable Register */

        int16_t LINFLEX_reserved16;

        union {
            vuint16_t R;
            struct {
                vuint16_t:12;
                vuint16_t IFMI:4;
            } B;
        } IFMI;                 /* LINFLEX Identifier Filter Match Index Register */

        int16_t LINFLEX_reserved17;

        union {
            vuint16_t R;
            struct {
                vuint16_t:12;
                vuint16_t IFM:4;
            } B;
        } IFMR;                 /* LINFLEX Identifier Filter Mode Register */

        int16_t LINFLEX_reserved18;

        union {
            vuint16_t R;
            struct {
                vuint16_t:3;
                vuint16_t DFL:3;
                vuint16_t DIR:1;
                vuint16_t CCS:1;
                  vuint16_t:2;
                vuint16_t ID:6;
            } B;
        } IFCR0;                /* LINFLEX Identifier Filter Control Register 0 */

        int16_t LINFLEX_reserved19;

        union {
            vuint16_t R;
            struct {
                vuint16_t:3;
                vuint16_t DFL:3;
                vuint16_t DIR:1;
                vuint16_t CCS:1;
                  vuint16_t:2;
                vuint16_t ID:6;
            } B;
        } IFCR1;                /* LINFLEX Identifier Filter Control Register 1 */

        int16_t LINFLEX_reserved20;

        union {
            vuint16_t R;
            struct {
                vuint16_t:3;
                vuint16_t DFL:3;
                vuint16_t DIR:1;
                vuint16_t CCS:1;
                  vuint16_t:2;
                vuint16_t ID:6;
            } B;
        } IFCR2;                /* LINFLEX Identifier Filter Control Register 2 */

        int16_t LINFLEX_reserved21;

        union {
            vuint16_t R;
            struct {
                vuint16_t:3;
                vuint16_t DFL:3;
                vuint16_t DIR:1;
                vuint16_t CCS:1;
                  vuint16_t:2;
                vuint16_t ID:6;
            } B;
        } IFCR3;                /* LINFLEX Identifier Filter Control Register 3 */

        int16_t LINFLEX_reserved22;

        union {
            vuint16_t R;
            struct {
                vuint16_t:3;
                vuint16_t DFL:3;
                vuint16_t DIR:1;
                vuint16_t CCS:1;
                  vuint16_t:2;
                vuint16_t ID:6;
            } B;
        } IFCR4;                /* LINFLEX Identifier Filter Control Register 4 */

        int16_t LINFLEX_reserved23;

        union {
            vuint16_t R;
            struct {
                vuint16_t:3;
                vuint16_t DFL:3;
                vuint16_t DIR:1;
                vuint16_t CCS:1;
                  vuint16_t:2;
                vuint16_t ID:6;
            } B;
        } IFCR5;                /* LINFLEX Identifier Filter Control Register 5 */

        int16_t LINFLEX_reserved24;

        union {
            vuint16_t R;
            struct {
                vuint16_t:3;
                vuint16_t DFL:3;
                vuint16_t DIR:1;
                vuint16_t CCS:1;
                  vuint16_t:2;
                vuint16_t ID:6;
            } B;
        } IFCR6;                /* LINFLEX Identifier Filter Control Register 6 */

        int16_t LINFLEX_reserved25;

        union {
            vuint16_t R;
            struct {
                vuint16_t:3;
                vuint16_t DFL:3;
                vuint16_t DIR:1;
                vuint16_t CCS:1;
                  vuint16_t:2;
                vuint16_t ID:6;
            } B;
        } IFCR7;                /* LINFLEX Identifier Filter Control Register 7 */

    };                          /* end of LINFLEX_tag */
/****************************************************************************/
/*                          MODULE : ME                                   */
/****************************************************************************/
    struct ME_tag {

        union {
            vuint32_t R;
            struct {
                vuint32_t S_CURRENTMODE:4;
                vuint32_t S_MTRANS:1;
                vuint32_t S_DC:1;
                  vuint32_t:2;
                vuint32_t S_PDO:1;
                  vuint32_t:2;
                vuint32_t S_MVR:1;
                vuint32_t S_DFLA:2;
                vuint32_t S_CFLA:2;
                  vuint32_t:8;
                vuint32_t S_PLL1:1;
                vuint32_t S_PLL0:1;
                vuint32_t S_OSC:1;
                vuint32_t S_RC:1;
                vuint32_t S_SYSCLK:4;
            } B;
        } GS;                   /* Global Status Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t TARGET_MODE:4;
                  vuint32_t:12;
                vuint32_t KEY:16;
            } B;
        } MCTL;                 /* Mode Control Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:18;
                vuint32_t STANDBY0:1;
                  vuint32_t:2;
                vuint32_t STOP0:1;
                  vuint32_t:1;
                vuint32_t HALT0:1;
                vuint32_t RUN3:1;
                vuint32_t RUN2:1;
                vuint32_t RUN1:1;
                vuint32_t RUN0:1;
                vuint32_t DRUN:1;
                vuint32_t SAFE:1;
                vuint32_t TEST:1;
                vuint32_t RESET:1;
            } B;
        } MER;                  /* Mode Enable Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:28;
                vuint32_t I_CONF:1;
                vuint32_t I_MODE:1;
                vuint32_t I_AFE:1;
                vuint32_t I_TC:1;
            } B;
        } IS;                   /* Interrupt Status Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:28;
                vuint32_t M_CONF:1;
                vuint32_t M_MODE:1;
                vuint32_t M_AFE:1;
                vuint32_t M_TC:1;
            } B;
        } IM;                   /* Interrupt Mask Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:27;
                vuint32_t S_MTI:1;
                vuint32_t S_MRI:1;
                vuint32_t S_DMA:1;
                vuint32_t S_NMA:1;
                vuint32_t S_SEA:1;
            } B;
        } IMTS;                 /* Invalid Mode Transition Status Register */

        int32_t ME_reserved0[2];

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t PDO:1;
                  vuint32_t:2;
                vuint32_t MVRON:1;
                vuint32_t DFLAON:2;
                vuint32_t CFLAON:2;
                  vuint32_t:8;
                vuint32_t PLL2ON:1;
                vuint32_t PLL1ON:1;
                vuint32_t XOSC0ON:1;
                vuint32_t IRCON:1;
                vuint32_t SYSCLK:4;
            } B;
        } RESET;                /* Reset Mode Configuration Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t PDO:1;
                  vuint32_t:2;
                vuint32_t MVRON:1;
                vuint32_t DFLAON:2;
                vuint32_t CFLAON:2;
                  vuint32_t:8;
                vuint32_t PLL2ON:1;
                vuint32_t PLL1ON:1;
                vuint32_t XOSC0ON:1;
                vuint32_t IRCON:1;
                vuint32_t SYSCLK:4;
            } B;
        } TEST;                 /* Test Mode Configuration Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t PDO:1;
                  vuint32_t:2;
                vuint32_t MVRON:1;
                vuint32_t DFLAON:2;
                vuint32_t CFLAON:2;
                  vuint32_t:8;
                vuint32_t PLL2ON:1;
                vuint32_t PLL1ON:1;
                vuint32_t XOSC0ON:1;
                vuint32_t IRCON:1;
                vuint32_t SYSCLK:4;
            } B;
        } SAFE;                 /* Safe Mode Configuration Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t PDO:1;
                  vuint32_t:2;
                vuint32_t MVRON:1;
                vuint32_t DFLAON:2;
                vuint32_t CFLAON:2;
                  vuint32_t:8;
                vuint32_t PLL2ON:1;
                vuint32_t PLL1ON:1;
                vuint32_t XOSC0ON:1;
                vuint32_t IRCON:1;
                vuint32_t SYSCLK:4;
            } B;
        } DRUN;                 /* DRUN Mode Configuration Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t PDO:1;
                  vuint32_t:2;
                vuint32_t MVRON:1;
                vuint32_t DFLAON:2;
                vuint32_t CFLAON:2;
                  vuint32_t:8;
                vuint32_t PLL2ON:1;
                vuint32_t PLL1ON:1;
                vuint32_t XOSC0ON:1;
                vuint32_t IRCON:1;
                vuint32_t SYSCLK:4;
            } B;
        } RUN[4];               /* RUN 0->4 Mode Configuration Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t PDO:1;
                  vuint32_t:2;
                vuint32_t MVRON:1;
                vuint32_t DFLAON:2;
                vuint32_t CFLAON:2;
                  vuint32_t:8;
                vuint32_t PLL2ON:1;
                vuint32_t PLL1ON:1;
                vuint32_t XOSC0ON:1;
                vuint32_t IRCON:1;
                vuint32_t SYSCLK:4;
            } B;
        } HALT0;                /* HALT0 Mode Configuration Register */

        int32_t ME_reserved1;

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t PDO:1;
                  vuint32_t:2;
                vuint32_t MVRON:1;
                vuint32_t DFLAON:2;
                vuint32_t CFLAON:2;
                  vuint32_t:8;
                vuint32_t PLL2ON:1;
                vuint32_t PLL1ON:1;
                vuint32_t XOSC0ON:1;
                vuint32_t IRCON:1;
                vuint32_t SYSCLK:4;
            } B;
        } STOP0;                /* STOP0 Mode Configuration Register */

        int32_t ME_reserved2[2];

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t PDO:1;
                  vuint32_t:2;
                vuint32_t MVRON:1;
                vuint32_t DFLAON:2;
                vuint32_t CFLAON:2;
                  vuint32_t:8;
                vuint32_t PLL2ON:1;
                vuint32_t PLL1ON:1;
                vuint32_t XOSC0ON:1;
                vuint32_t IRCON:1;
                vuint32_t SYSCLK:4;
            } B;
        } STANDBY0;             /* STANDBY0 Mode Configuration Register */

        int32_t ME_reserved3[2];

        union {
            vuint32_t R;
            struct {
                vuint32_t PERIPH:32;
            } B;
        } PS[4];                /* Peripheral Status 0->4 Register */

        int32_t ME_reserved4[4];

        union {
            vuint32_t R;
            struct {
                vuint32_t:24;
                vuint32_t RUN3:1;
                vuint32_t RUN2:1;
                vuint32_t RUN1:1;
                vuint32_t RUN0:1;
                vuint32_t DRUN:1;
                vuint32_t SAFE:1;
                vuint32_t TEST:1;
                vuint32_t RESET:1;
            } B;
        } RUNPC[8];             /* RUN Peripheral Configuration 0->7 Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:18;
                vuint32_t STANDBY0:1;
                  vuint32_t:2;
                vuint32_t STOP0:1;
                  vuint32_t:1;
                vuint32_t HALT0:1;
                  vuint32_t:8;
            } B;
        } LPPC[8];              /* Low Power Peripheral Configuration 0->7 Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t:1;
                vuint8_t DBG_F:1;
                vuint8_t LP_CFG:3;
                vuint8_t RUN_CFG:3;
            } B;
        } PCTL[144];            /* Peripheral Control 0->143 Register */

    };                          /* end of ME_tag */
/****************************************************************************/
/*                          MODULE : CGM                                   */
/****************************************************************************/
    struct CGM_tag {

        /* The CGM provides a unified register interface, enabling access to 
           all clock sources:
           
           Clock Type | Starting Address Map | Associated Clock Sources
           ------------------------------------------------------------
           System     | C3FE0000             | OSC_CTL
           "        | -                    | Reserved
           "        | C3FE0040             | LPOSC_CTL
           "        | C3FE0060             | RC_CTL
           "        | C3FE0080             | LPRC_CTL
           "        | C3FE00A0             | FMPLL_0
           "        | C3FE00C0             | FMPLL_1
           "        | -                    | Reserved
           MISC       | C3FE0100             | CMU_0 & CMU_1
           
         */

    /************************************/
        /* OSC_CTL @ CGM base address + 0x0000 */
    /************************************/
        union {
            vuint32_t R;
            struct {
                vuint32_t OSCBYP:1;
                  vuint32_t:7;
                vuint32_t EOCV:8;
                vuint32_t M_OSC:1;
                  vuint32_t:2;
                vuint32_t OSCDIV:5;
                vuint32_t I_OSC:1;
                  vuint32_t:5;
                vuint32_t S_OSC:1;
                vuint32_t OSCON:1;
            } B;
        } OSC_CTL;              /* Main OSC Control Register */

    /************************************/
        /* LPOSC_CTL @ CGM base address + 0x0040 */
    /************************************/
        int32_t CGM_reserved0[15];      /* (0x040 - 0x004)/4 = 0x0F */

        union {
            vuint32_t R;
            struct {
                vuint32_t OSCBYP:1;
                  vuint32_t:7;
                vuint32_t EOCV:8;
                vuint32_t M_OSC:1;
                  vuint32_t:2;
                vuint32_t OSCDIV:5;
                vuint32_t I_OSC:1;
                  vuint32_t:5;
                vuint32_t S_OSC:1;
                vuint32_t OSCON:1;
            } B;
        } LPOSC_CTL;            /* Low Power OSC Control Register */

    /************************************/
        /* RC_CTL @ CGM base address + 0x0060 */
    /************************************/
        int32_t CGM_reserved1[7];       /* (0x060 - 0x044)/4 = 0x07 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:10;
                vuint32_t RCTRIM:6;
                  vuint32_t:3;
                vuint32_t RCDIV:5;
                  vuint32_t:2;
                vuint32_t S_RC_STDBY:1;
                  vuint32_t:5;
            } B;
        } RC_CTL;               /* RC OSC Control Register */

    /*************************************/
        /* LPRC_CTL @ CGM base address + 0x0080 */
    /*************************************/
        int32_t CGM_reserved2[7];       /* (0x080 - 0x064)/4 = 0x07 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:11;
                vuint32_t LRCTRIM:5;
                  vuint32_t:3;
                vuint32_t LPRCDIV:5;
                  vuint32_t:3;
                vuint32_t S_LPRC:1;
                  vuint32_t:3;
                vuint32_t LPRCON_STDBY:1;
            } B;
        } LPRC_CTL;             /* Low Power RC OSC Control Register */

    /************************************/
        /* FMPLL_0 @ CGM base address + 0x00A0 */
        /* FMPLL_1 @ CGM base address + 0x0100 */
    /************************************/
        int32_t CGM_reserved3[7];       /* (0x0A0 - 0x084)/4 = 0x07 */

        struct {
            union {
                vuint32_t R;
                struct {
                    vuint32_t:2;
                    vuint32_t IDF:4;
                    vuint32_t ODF:2;
                      vuint32_t:1;
                    vuint32_t NDIV:7;
                      vuint32_t:7;
                    vuint32_t EN_PLL_SW:1;
                    vuint32_t MODE:1;
                    vuint32_t UNLOCK_ONCE:1;
                      vuint32_t:1;
                    vuint32_t I_LOCK:1;
                    vuint32_t S_LOCK:1;
                    vuint32_t PLL_FAIL_MASK:1;
                    vuint32_t PLL_FAIL_FLAG:1;
                      vuint32_t:1;
                } B;
            } CR;               /* FMPLL Control Register */

            union {
                vuint32_t R;
                struct {
                    vuint32_t STRB_BYPASS:1;
                      vuint32_t:1;
                    vuint32_t SPRD_SEL:1;
                    vuint32_t MOD_PERIOD:13;
                    vuint32_t FM_EN:1;
                    vuint32_t INC_STEP:15;
                } B;
            } MR;               /* FMPLL Modulation Register */

            int32_t CGM_reserved4[6];   /* (0x0C0 - 0x0A8)/4 = 0x06 */
            /* (0x0E0 - 0x0C8)/4 = 0x06 */

        } FMPLL[2];

    /************************************/
        /* CMU @ CGM base address + 0x0100  */
    /************************************/
        int32_t CGM_reserved5[8];       /* (0x100 - 0x0E0)/4 = 0x08 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t SFM:1;
                  vuint32_t:13;
                vuint32_t CLKSEL1:2;
                  vuint32_t:5;
                vuint32_t RCDIV:2;
                vuint32_t CME_A:1;
            } B;
        } CMU_0_CSR;            /* Control Status Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:12;
                vuint32_t FD:20;
            } B;
        } CMU_0_FDR;            /* Frequency Display Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:20;
                vuint32_t HFREF_A:12;
            } B;
        } CMU_0_HFREFR_A;       /* High Frequency Reference Register PLL_A Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:20;
                vuint32_t LFREF_A:12;
            } B;
        } CMU_0_LFREFR_A;       /* Low Frequency Reference Register PLL_A Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:29;
                vuint32_t FHHI_A:1;
                vuint32_t FLLI_A:1;
                vuint32_t OLRI:1;
            } B;
        } CMU_0_ISR;            /* Interrupt Status Register */

        int32_t CGM_reserved6;       /* 0x0014 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:12;
                vuint32_t MD:20;
            } B;
        } CMU_0_MDR;            /* Measurement Duration Register */


    /************************************/
        /* CGM General Registers @ CGM base address + 0x0370 */
    /************************************/
        int32_t CGM_reserved7[149];     /* (0x370 - 0x11C)/4 = 0x95 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:31;
                vuint32_t EN:1;
            } B;
        } OCEN;                 /* Output Clock Enable Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:2;
                vuint32_t SELDIV:2;
                vuint32_t SELCTL:4;
                  vuint32_t:24;
            } B;
        } OCDSSC;               /* Output Clock Division Select Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:4;
                vuint32_t SELSTAT:4;
                  vuint32_t:24;
            } B;
        } SCSS;                 /* System Clock Select Status */

        union {
            vuint32_t R;
            struct {
                vuint32_t DE0:1;
                  vuint32_t:3;
                vuint32_t DIV0:4;
                vuint32_t DE1:1;
                  vuint32_t:3;
                vuint32_t DIV1:4;
                vuint32_t DE2:1;
                  vuint32_t:3;
                vuint32_t DIV2:4;
                  vuint32_t:8;
            } B;
        } SCDC;                 /* System Clock Divider Configuration 0->2 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:4;
                vuint32_t SELCTL:4;
                  vuint32_t:24;
            } B;
        } AC0SC;                /* Aux Clock 0 Select Control */

        int32_t CGM_reserved8;  /* 0x0384 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:4;
                vuint32_t SELCTL:4;
                  vuint32_t:24;
            } B;
        } AC1SC;                /* Aux Clock 1 Select Control */

        union {
            vuint32_t R;
            struct {
                vuint32_t DE0:1;
                  vuint32_t:3;
                vuint32_t DIV0:4;
                  vuint32_t:24;
            } B;
        } AC1DC;                /* Aux Clock 1 Divider Configuration 0->3 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:4;
                vuint32_t SELCTL:4;
                  vuint32_t:24;
            } B;
        } AC2SC;                /* Aux Clock 2 Select Control */

        union {
            vuint32_t R;
            struct {
                vuint32_t DE0:1;
                  vuint32_t:3;
                vuint32_t DIV0:4;
                  vuint32_t:24;
            } B;
        } AC2DC;                /* Aux Clock 2 Divider Configuration 0->3 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:4;
                vuint32_t SELCTL:4;
                  vuint32_t:24;
            } B;
        } AC3SC;                /* Aux Clock 3 Select Control */

        union {
            vuint32_t R;
            struct {
                vuint32_t DE0:1;
                  vuint32_t:3;
                vuint32_t DIV0:4;
                  vuint32_t:24;
            } B;
        } AC3DC;                /* Aux Clock 3 Divider Configuration 0->3 */
        
    };                          /* end of CGM_tag */
/****************************************************************************/
/*                          MODULE : RGM                                   */
/****************************************************************************/
    struct RGM_tag {

        union {
            vuint16_t R;
            struct {
                vuint16_t F_EXR:1;
                  vuint16_t:3;
                vuint16_t F_CMU1_FHL:1;
                  vuint16_t:1;
                vuint16_t F_PLL1:1;
                vuint16_t F_FLASH:1;
                vuint16_t F_LVD45:1;
                vuint16_t F_CMU0_FHL:1;
                vuint16_t F_CMU0_OLR:1;
                vuint16_t F_PLL0:1;
                vuint16_t F_CHKSTOP:1;
                vuint16_t F_SOFT:1;
                vuint16_t F_CORE:1;
                vuint16_t F_JTAG:1;
            } B;
        } FES;                  /* Functional Event Status */

        union {
            vuint16_t R;
            struct {
                vuint16_t POR:1;
                  vuint16_t:7;
                vuint16_t F_COMP:1;
                vuint16_t F_LVD27_IO:1;
                vuint16_t F_LVD27_FLASH:1;
                vuint16_t F_LVD27_VREG:1;
                vuint16_t F_LVD27:1;
                vuint16_t F_SWT:1;
                vuint16_t F_LVD12_PD1:1;
                vuint16_t F_LVD12_PD0:1;
            } B;
        } DES;                  /* Destructive Event Status */

        union {
            vuint16_t R;
            struct {
                vuint16_t D_EXR:1;
                  vuint16_t:3;
                vuint16_t D_CMU1_FHL:1;
                  vuint16_t:1;
                vuint16_t D_PLL1:1;
                vuint16_t D_FLASH:1;
                vuint16_t D_LVD45:1;
                vuint16_t D_CMU0_FHL:1;
                vuint16_t D_CMU0_OLR:1;
                vuint16_t D_PLL0:1;
                vuint16_t D_CHKSTOP:1;
                vuint16_t D_SOFT:1;
                vuint16_t D_CORE:1;
                vuint16_t D_JTAG:1;
            } B;
        } FERD;                 /* Functional Event Reset Disable */

        union {
            vuint16_t R;
            struct {
                vuint16_t:8;
                vuint16_t D_COMP:1;
                vuint16_t D_LVD27_IO:1;
                vuint16_t D_LVD27_FLASH:1;
                vuint16_t D_LVD27_VREG:1;
                vuint16_t D_LVD27:1;
                vuint16_t D_SWT:1;
                vuint16_t D_LVD12_PD1:1;
                vuint16_t D_LVD12_PD0:1;
            } B;
        } DERD;                 /* Destructive Event Reset Disable */

        int16_t RGM_reserved0[4];

        union {
            vuint16_t R;
            struct {
                vuint16_t:4;
                vuint16_t AR_CMU1_FHL:1;
                  vuint16_t:1;
                vuint16_t AR_PLL1:1;
                vuint16_t AR_FLASH:1;
                vuint16_t AR_LVD45:1;
                vuint16_t AR_CMU0_FHL:1;
                vuint16_t AR_CMU0_OLR:1;
                vuint16_t AR_PLL0:1;
                vuint16_t AR_CHKSTOP:1;
                vuint16_t AR_SOFT:1;
                vuint16_t AR_CORE:1;
                vuint16_t AR_JTAG:1;
            } B;
        } FEAR;                 /* Functional Event Alternate Request */

        union {
            vuint16_t R;
            struct {
                vuint16_t:8;
                vuint16_t AR_COMP:1;
                vuint16_t AR_LVD27_IO:1;
                vuint16_t AR_LVD27_FLASH:1;
                vuint16_t AR_LVD27_VREG:1;
                vuint16_t AR_LVD27:1;
                vuint16_t AR_SWT:1;
                vuint16_t AR_LVD12_PD1:1;
                vuint16_t AR_LVD12_PD0:1;
            } B;
        } DEAR;                 /* Destructive Event Alternate Request */

        int16_t RGM_reserved1[2];

        union {
            vuint16_t R;
            struct {
                vuint16_t:7;
                vuint16_t SS_FLASH:1;
                vuint16_t SS_LVD45:1;
                vuint16_t SS_CMU0_FHL:1;
                vuint16_t SS_CMU0_OLR:1;
                vuint16_t SS_PLL0:1;
                vuint16_t SS_CHKSTOP:1;
                vuint16_t SS_SOFT:1;
                vuint16_t SS_CORE:1;
                vuint16_t SS_JTAG:1;
            } B;
        } FESS;                 /* Functional Event Short Sequence */

        union {
            vuint16_t R;
            struct {
                vuint16_t:8;
                vuint16_t BOOT:1;
                  vuint16_t:4;
                vuint16_t DRUND_FLA:1;
                  vuint16_t:1;
                vuint16_t DRUNC_FLA:1;
            } B;
        } STDBY;                /* STANDBY reset sequence */

        union {
            vuint16_t R;
            struct {
                vuint16_t:7;
                vuint16_t BE_FLASH:1;
                vuint16_t BE_LVD45:1;
                vuint16_t BE_CMU0_FHL:1;
                vuint16_t BE_CMU0_OLR:1;
                vuint16_t BE_PLL0:1;
                vuint16_t BE_CHKSTOP:1;
                vuint16_t BE_SOFT:1;
                vuint16_t BE_CORE:1;
                vuint16_t BE_JTAG:1;
            } B;
        } FBRE;                 /* Functional Bidirectional Reset Enable */

    };                          /* end of RGM_tag */
/****************************************************************************/
/*                          MODULE : PCU                                   */
/****************************************************************************/
    struct PCU_tag {

        union {
            vuint32_t R;
            struct {
                vuint32_t:18;
                vuint32_t STBY0:1;
                  vuint32_t:2;
                vuint32_t STOP0:1;
                  vuint32_t:1;
                vuint32_t HALT0:1;
                vuint32_t RUN3:1;
                vuint32_t RUN2:1;
                vuint32_t RUN1:1;
                vuint32_t RUN0:1;
                vuint32_t DRUN:1;
                vuint32_t SAFE:1;
                vuint32_t TEST:1;
                vuint32_t RST:1;
            } B;
        } PCONF[16];            /* Power domain 0-15 configuration register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t PD15:1;
                vuint32_t PD14:1;
                vuint32_t PD13:1;
                vuint32_t PD12:1;
                vuint32_t PD11:1;
                vuint32_t PD10:1;
                vuint32_t PD9:1;
                vuint32_t PD8:1;
                vuint32_t PD7:1;
                vuint32_t PD6:1;
                vuint32_t PD5:1;
                vuint32_t PD4:1;
                vuint32_t PD3:1;
                vuint32_t PD2:1;
                vuint32_t PD1:1;
                vuint32_t PD0:1;
            } B;
        } PSTAT;                /* Power Domain Status Register */

        int32_t PCU_reserved0[15];      /* {0x0080-0x0044}/0x4 = 0xF */

        union {
            vuint32_t R;
            struct {
                vuint32_t:15;
                vuint32_t MASK_LVDHV5:1;
            } B;
        } VCTL;                 /* Voltage Regulator Control Register */

    };                          /* end of PCU_tag */
/****************************************************************************/
/*                          MODULE : DCU                                    */
/****************************************************************************/
    struct DCU_FG_tag {

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t COLOR:24;
            } B;
        } F;

	union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t COLOR:24;
            } B;
        } B;
    };
	
    struct DCU_LAYER_tag {

        union {
            vuint32_t R;
            struct {
                vuint32_t:6;
                vuint32_t HEIGHT:10;
                  vuint32_t:5;
                vuint32_t WIDTH:11;
            } B;
        } CTRLDESCL1;           /* LAYER[X].CTRLDESCL1 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:6;
                vuint32_t POSY:10;
                  vuint32_t:4;
                vuint32_t POSX:12;
            } B;
        } CTRLDESCL2;           /* LAYER[X].CTRLDESCL2 */

        union {
            vuint32_t R;
            struct {
                vuint32_t ADDR:32;
            } B;
        } CTRLDESCL3;           /* LAYER[X].CTRLDESCL3 */

        union {
            vuint32_t R;
            struct {
                vuint32_t EN:1;
                vuint32_t TILEEN:1;
                vuint32_t DATASEL:1;
                vuint32_t SAFETYEN:1;
                vuint32_t TRANS:8;
                vuint32_t BPP:4;
                  vuint32_t:1;
                vuint32_t LUOFFS:11;
                  vuint32_t:1;
                vuint32_t BB:1;
                vuint32_t AB:2;
            } B;
        } CTRLDESCL4;           /* LAYER[X].CTRLDESCL4 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t CKMAXR:8;
                vuint32_t CKMAXG:8;
                vuint32_t CKMAXB:8;
            } B;
        } CTRLDESCL5;           /* LAYER[X].CTRLDESCL5 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t CKMINR:8;
                vuint32_t CKMING:8;
                vuint32_t CKMINB:8;
            } B;
        } CTRLDESCL6;           /* LAYER[X].CTRLDESCL6 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:6;
                vuint32_t TILEVERSIZE:10;
                  vuint32_t:8;
                vuint32_t TILEHORSIZE:8;
            } B;
        } CTRLDESCL7;           /* LAYER[X].CTRLDESCL7 */

    };                          /* end of DCU_LAYER_tag */

    struct DCU_tag {

        /* DCU.LAYER<[x]>.CTRLDESCL<y>.R  {x = 0-15, y = 1-7} */
        /* eg DCU.LAYER[0].CTRLDESCL1.R        = 0;           */
        /*    DCU.LAYER[0].CTRLDESCL1.B.HEIGHT = 0;           */
        struct DCU_LAYER_tag LAYER[16];

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t HEIGHT:8;
                  vuint32_t:8;
                vuint32_t WIDTH:8;
            } B;
        } CTRLDESCCURSOR1;      /* Control Descriptor Cursor_1 Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:6;
                vuint32_t POSY:10;
                  vuint32_t:6;
                vuint32_t POSX:10;
            } B;
        } CTRLDESCCURSOR2;      /* Control Descriptor Cursor_2 Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t CUREN:1;
                  vuint32_t:7;
                vuint32_t CURSORDEFAULTCOLOR:24;
            } B;
        } CTRLDESCCURSOR3;      /* Control Descriptor Cursor_3 Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t HWCBLINK_OFF:8;
                vuint32_t:7;
                vuint32_t ENBLINK:1;
                vuint32_t HWCBLINK_ON:8;
            } B;
        } CTRLDESCCURSOR4;      /* Control Descriptor Cursor_4 Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t DCUSWRST:1;
                vuint32_t:8;
                vuint32_t BLEND_ITER:3;
                vuint32_t PDISYNCLOCK:4;
                vuint32_t PDIINTERPOL_EN:1;		
                vuint32_t RASTER_EN:1;		
                vuint32_t PDIEN:1;
                vuint32_t PDIBYTEREV:1;
                vuint32_t PDIDEMODE:1;
                vuint32_t PDINARROWMODE:1;
                vuint32_t PDI_MODE:2;
                vuint32_t PDISLAVEMODE:1;
                vuint32_t TAGEN:1;
                vuint32_t SIGEN:1;
                vuint32_t PDISYNC:1;
                vuint32_t TEST:1;
                vuint32_t ENGAMMA:1;
                vuint32_t DCUMODE:2;
            } B;
        } DCUMODE;              /* DCU Mode Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t BGND_R:8;
                vuint32_t BGND_G:8;
                vuint32_t BGND_B:8;
            } B;
        } BGND;                 /* BGND Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:6;
                vuint32_t DELTAY:10;
                  vuint32_t:8;
                vuint32_t DELTAX:8;
            } B;
        } DSPSIZE;              /* DISP_SIZE Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:1;
                vuint32_t BPH:9;
                  vuint32_t:2;
                vuint32_t PWH:9;
                  vuint32_t:2;
                vuint32_t FPH:9;
            } B;
        } HSYNPARA;             /* HSYNPARA Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:1;
                vuint32_t BPV:9;
                  vuint32_t:2;
                vuint32_t PWV:9;
                  vuint32_t:2;
                vuint32_t FPV:9;
            } B;
        } VSYNPARA;             /* VSYNPARA Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:21;
                vuint32_t INVPDIDE:1;
                vuint32_t INVPDIHS:1;
                vuint32_t INVPDIVS:1;
                vuint32_t INVPDICLK:1;
                vuint32_t INVPXCK:1;
                vuint32_t NEG:1;
                vuint32_t BPVS:1;
                vuint32_t BPHS:1;
                vuint32_t INVCS:1;
                vuint32_t INVVS:1;
                vuint32_t INVHS:1;
            } B;
        } SYNPOL;               /* SYNPOL Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:6;
                vuint32_t LSBFVS:10;
                vuint32_t OUTBUFHIGH:8;
                vuint32_t OUTBUFLOW:8;
            } B;
        } THRESHOLD;            /* Threshold Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:12;
                vuint32_t P4FIFOHIFLAG:1;
                vuint32_t P4FIFOLOFLAG:1;
                vuint32_t P3FIFOHIFLAG:1;
                vuint32_t P3FIFOLOFLAG:1;
                vuint32_t:1;
                vuint32_t DMATRANSFINISH:1;
                vuint32_t IPSFIFOEMPTY:1;
                vuint32_t IPSFIFOFULL:1;
                vuint32_t IPMERROR:1;
                vuint32_t PROGEND:1;
                vuint32_t P2FIFOHIFLAG:1;
                vuint32_t P2FIFOLOFLAG:1;
                vuint32_t P1FIFOHIFLAG:1;
                vuint32_t P1FIFOLOFLAG:1;
                vuint32_t CRCOVERFLOW:1;
                vuint32_t CRCREADY:1;
                vuint32_t VSBLANK:1;
                vuint32_t LSBFVS:1;
                vuint32_t UNDRUN:1;
                vuint32_t VSYNC:1;
            } B;
        } INTSTATUS;            /* Interrupt Status Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:12;
                vuint32_t MP4FIFOHIFLAG:1;
                vuint32_t MP4FIFOLOFLAG:1;
                vuint32_t MP3FIFOHIFLAG:1;
                vuint32_t MP3FIFOLOFLAG:1;
                vuint32_t:1;
                vuint32_t MDMATRANSFINISH:1;
                vuint32_t MIPSFIFOEMPTY:1;
                vuint32_t MIPSFIFOFULL:1;
                vuint32_t MIPMERROR:1;
                vuint32_t MPROGEND:1;
                vuint32_t MP2FIFOHIFLAG:1;
                vuint32_t MP2FIFOLOFLAG:1;
                vuint32_t MP1FIFOHIFLAG:1;
                vuint32_t MP1FIFOLOFLAG:1;
                vuint32_t MCRCOVERFLOW:1;
                vuint32_t MCRCREADY:1;
                vuint32_t MVSBLANK:1;
                vuint32_t MLSBFVS:1;
                vuint32_t MUNDRUN:1;
                vuint32_t MVSYNC:1;
            } B;
        } INTMASK;              /* Interrupt Mask Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t COLBAR_R:8;
                vuint32_t COLBAR_G:8;
                vuint32_t COLBAR_B:8;
            } B;
        } COLBAR[8];            /* COLBAR 1-8 Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:26;
                vuint32_t DIVRATIO:6;
            } B;
        } DIVRATIO;             /* Clock Divider Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:6;
                vuint32_t SIGVERSIZE:10;
                  vuint32_t:6;
                vuint32_t SIGHORSIZE:10;
            } B;
        } SIGNCALC1;

        union {
            vuint32_t R;
            struct {
                vuint32_t:6;
                vuint32_t SIGVERPOS:10;
                  vuint32_t:6;
                vuint32_t SIGHORPOS:10;
            } B;
        } SIGNCALC2;

        union {
            vuint32_t R;
            struct {
                vuint32_t CRCVAL:32;
            } B;
        } CRCVAL;

        union {
            vuint32_t R;
            struct {
                vuint32_t:22;
                vuint32_t PDIBLANKINGERR:1;
                vuint32_t PDIECCERR2:1;
                vuint32_t PDIECCERR1:1;
                vuint32_t PDILOCKLOST:1;
                vuint32_t PDILOCKDET:1;
                vuint32_t PDIVSYNCDET:1;
                vuint32_t PDIHSYNCDET:1;
                vuint32_t PDIDEDET:1;
                vuint32_t PDICLKLOST:1;
                vuint32_t PDICLKDET:1;
            } B;
        } PDISTATUS;            /* PDI status Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:22;
                vuint32_t PDIBLANKINGERR:1;
                vuint32_t PDIECCERR2:1;
                vuint32_t PDIECCERR1:1;
                vuint32_t PDILOCKLOST:1;
                vuint32_t PDILOCKDET:1;
                vuint32_t PDIVSYNCDET:1;
                vuint32_t PDIHSYNCDET:1;
                vuint32_t PDIDEDET:1;
                vuint32_t PDICLKLOST:1;
                vuint32_t PDICLKDET:1;
            } B;
        } MASKPDISTATUS;        /* PDI Status Mask Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:13;
                vuint32_t HWCERR:1;
                vuint32_t SIGERR:1;
                vuint32_t DISPERR:1;
                vuint32_t LPARERR:16;
            } B;
        } PARRERRSTATUS;

        union {
            vuint32_t R;
            struct {
                vuint32_t:13;
                vuint32_t MHWCERR:1;
                vuint32_t MSIGERR:1;
                vuint32_t MDISPERR:1;
                vuint32_t MLPARERR:16;
            } B;
        } MASKPARRERRSTATUS;

        union {
            vuint32_t R;
            struct {
                vuint32_t INPBUFP2HI:8;
                vuint32_t INPBUFP2LO:8;
                vuint32_t INPBUFP1HI:8;
                vuint32_t INPBUFP1LO:8;
            } B;
        } THRESHOLDINP1;         /* Threshold Input Buffer Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t INPBUFP4HI:8;
                vuint32_t INPBUFP4LO:8;
                vuint32_t INPBUFP3HI:8;
                vuint32_t INPBUFP3LO:8;
            } B;
        } THRESHOLDINP2;         /* Threshold Input Buffer Register */

	union {
            vuint32_t R;
            struct {
                vuint32_t YRED:10;
		vuint32_t:1;
                vuint32_t YGREEN:10;
		vuint32_t:1;
                vuint32_t YBLUE:10;
            } B;
        } YCOMP;         /* Y_COMP Register */	

	union {
            vuint32_t R;
            struct {
		vuint32_t:5;
                vuint32_t CRRED:11;
		vuint32_t:4;
                vuint32_t CBRED:12;
            } B;
	} CHROMARED;	/* CHROMA_RED Register */

	union {
            vuint32_t R;
            struct {
		vuint32_t:5;
                vuint32_t CRGREEN:11;
		vuint32_t:4;
                vuint32_t CBGREEN:12;
            } B;
	} CHROMAGREEN;	/* CHROMA_GREEN Register */

	union {
            vuint32_t R;
            struct {
		vuint32_t:5;
                vuint32_t CRBLUE:11;
		vuint32_t:4;
                vuint32_t CBBLUE:12;
            } B;
	} CHROMABLUE;	/* CHROMA_BLUE Register */	

	union {
            vuint32_t R;
            struct {
                vuint32_t CRCPOS:32;
            } B;
        } CRCPOS;	/* CRC Position Register */
	
        struct DCU_FG_tag FG[16]; /* FGCOLOR 1-16 */

        vuint32_t DCU_reserved1[0x0C]; /* 12 registers */

	union {
            vuint32_t R;
            struct {
                vuint32_t HLB:1;
		vuint32_t:31;
            } B;
        } GPR;	/* Global Protection Register */
	
	union {
            vuint32_t R;
            struct {
                vuint32_t WEN_L0_1:1;
                vuint32_t WEN_L0_2:1;
                vuint32_t WEN_L0_3:1;
                vuint32_t WEN_L0_4:1;
                vuint32_t SLB_L0_1:1;
                vuint32_t SLB_L0_2:1;
                vuint32_t SLB_L0_3:1;
                vuint32_t SLB_L0_4:1;
                vuint32_t WEN_L0_5:1;
                vuint32_t WEN_L0_6:1;
                vuint32_t WEN_L0_7:1;
		vuint32_t:1;
                vuint32_t SLB_L0_5:1;
                vuint32_t SLB_L0_6:1;
                vuint32_t SLB_L0_7:1;
		vuint32_t:17;
            } B;
        } SLR_L0;	/* Soft Lock Register Layer 0 */
	
	union {
            vuint32_t R;
            struct {
                vuint32_t WEN_L1_1:1;
                vuint32_t WEN_L1_2:1;
                vuint32_t WEN_L1_3:1;
                vuint32_t WEN_L1_4:1;
                vuint32_t SLB_L1_1:1;
                vuint32_t SLB_L1_2:1;
                vuint32_t SLB_L1_3:1;
                vuint32_t SLB_L1_4:1;
                vuint32_t WEN_L1_5:1;
                vuint32_t WEN_L1_6:1;
                vuint32_t WEN_L1_7:1;
		vuint32_t:1;
                vuint32_t SLB_L1_5:1;
                vuint32_t SLB_L1_6:1;
                vuint32_t SLB_L1_7:1;
		vuint32_t:17;
            } B;
        } SLR_L1;	/* Soft Lock Register Layer 1 */
	
	union {
            vuint32_t R;
            struct {
                vuint32_t WEN_DISP:1;
		vuint32_t:3;
                vuint32_t SLB_DISP:1;
		vuint32_t:27;
            } B;
        } SLR_DISPSIZE;	/* Soft Lock Register DISP_SIZE */
	
	union {
            vuint32_t R;
            struct {
                vuint32_t WEN_HSYNC:1;
                vuint32_t WEN_VSYNC:1;
		vuint32_t:2;
                vuint32_t SLB_HSYNC:1;
                vuint32_t SLB_VSYNC:1;
		vuint32_t:26;
            } B;
        } SLR_HVSYNCPARA;	/* Soft Lock Register HSYNC VSYNC PARA */
	
	union {
            vuint32_t R;
            struct {
                vuint32_t WEN_POL:1;
		vuint32_t:3;
                vuint32_t SLB_POL:1;
		vuint32_t:27;
            } B;
        } SLR_POL;	/* Soft Lock Register POL */
	
	union {
            vuint32_t R;
            struct {
                vuint32_t WEN_L0_FCOLOR:1;
                vuint32_t WEN_L0_BCOLOR:1;
		vuint32_t:2;
                vuint32_t SLB_L0_FCOLOR:1;
                vuint32_t SLB_L0_BCOLOR:1;
		vuint32_t:26;
            } B;
        } SLR_L0TRANSP;	/* Soft Lock Register Layer 0 TRANSP */
	
	union {
            vuint32_t R;
            struct {
                vuint32_t WEN_L1_FCOLOR:1;
                vuint32_t WEN_L1_BCOLOR:1;
		vuint32_t:2;
                vuint32_t SLB_L1_FCOLOR:1;
                vuint32_t SLB_L1_BCOLOR:1;
		vuint32_t:26;
            } B;
        } SLR_L1TRANSP;	/* Soft Lock Register Layer 1 TRANSP */
	
        vuint32_t DCU_reserved2[0x038]; /* Unused from end of registers */

        vuint32_t CURSOR[0x0100];       /* Hardware Cursor = 256 * 32bit */

        vuint32_t GAMMARED[0x100];      /* Gamma red table = 256 * 32bit */

        vuint32_t GAMMAGREEN[0x100];    /* Gamma green table = 256 * 32bit */

        vuint32_t GAMMABLUE[0x100];     /* Gamma blue table = 256 * 32bit */

	vuint32_t DCU_reserved3[0x300]; /* Unused = 768 * 32bit */

        vuint32_t CLUT[0x0800]; /* CLUT-Pallete memory + Tile memory = 2k * 32bit */

    };                          /* end of DCU_tag */
/****************************************************************************/
/*                     MODULE : SGL                                         */
/****************************************************************************/
struct SGL_tag{
  union{
            vuint32_t R;
            struct{
            vuint32_t MP:1;
            vuint32_t SN_CTRL:3;
            vuint32_t SDCIE:1;
            vuint32_t :7;
            vuint32_t ch2_sel:4;
            vuint32_t pre:7;
            vuint32_t :5;
            vuint32_t ch1_sel:4;
            } B;
  }MODE_SEL;

  union{
           vuint32_t R;
  }SOUND_DUR;

 
  union{
           vuint32_t R;
  }HIGH_PER;

  union{
           vuint32_t R;
  }LOW_PER;
/* definition error!
  union{
          vuint8_t R;
          struct{
          vuint8_t :7;
          vuint8_t SDCIFC:1;
          } B;
  }SGL_STATUS;
*/
  union {
	  vuint32_t R;
	  struct{
		  vuint32_t:31;
		  vuint32_t SDCIFC:1;
	  }B;
  }SGL_STATUS;
};
/****************************************************************************/
/*                          MODULE : SMC - Stepper Motor Control            */
/****************************************************************************/
    struct SMC_tag {

        union {
            vuint8_t R;
            struct {
                vuint8_t:1;
                vuint8_t MCPRE:2;
                vuint8_t MCSWAI:1;
                  vuint8_t:1;
                vuint8_t DITH:1;
                  vuint8_t:1;
                vuint8_t MCTOIF:1;
            } B;
        } CTL0;                 /* Motor Controller Control Register 0 */

        union {
            vuint8_t R;
            struct {
                vuint8_t RECIRC:1;
                  vuint8_t:6;
                vuint8_t MCTOIE:1;
            } B;
        } CTL1;                 /* Motor Controller Control Register 1 */

        union {
            vuint16_t R;
            struct {
                vuint16_t:5;
                vuint16_t P:11;
            } B;
        } PER;                  /* Motor Controller Period Register */

        int32_t SMC_reserved0[3];       /* (0x010 - 0x004)/4 = 0x01 */

        union {
            vuint8_t R;
            struct {
                vuint8_t MCOM:2;
                vuint8_t MCAM:2;
                  vuint8_t:2;
                vuint8_t CD:2;
            } B;
        } CC[12];               /* Motor Controller Channel Control Register 0->11 */

        int32_t SMC_reserved1;  /* (0x020 - 0x01C)/4 = 0x01 */

        union {
            vuint16_t R;
            struct {
                vuint16_t S:5;
                vuint16_t D:11;
            } B;
        } DC[12];               /* Motor Controller Duty Cycle Register 0->11 */

        int8_t SMC_reserved2[8];        /* (0x040 - 0x038) = 0x08 */

        union {
            vuint8_t R;
            struct {
                vuint8_t TOUT:8;
            } B;
        } SDTO;                 /* Shortcut detector time-out register  */

        int8_t SMC_reserved3[3];        /* (0x044 - 0x041) = 0x03 */

        union {
            vuint8_t R;
            struct {
                vuint8_t EN:8;
            } B;
        } SDE[3];               /* Shortcut detector enable register 0->2 */

        int8_t SMC_reserved4;   /* (0x048 - 0x047) = 0x01 */

        union {
            vuint8_t R;
            struct {
                vuint8_t IRQ_EN:8;
            } B;
        } SDIEN[3];             /* Shortcut detector interrupt enable register 0->2 */

        int8_t SMC_reserved5;   /* (0x04C - 0x04B) = 0x01 */

        union {
            vuint8_t R;
            struct {
                vuint8_t IRQ:8;
            } B;
        } SDI[3];               /* Shortcut detector interrupt register 0->2 */

    };                          /* end of SMC_tag */
/****************************************************************************/
/*                          MODULE : SSD - Stepper Stall Detect             */
/****************************************************************************/
    struct SSD_tag {

        union {
            vuint16_t R;
            struct {
                vuint16_t TRIG:1;
                vuint16_t STEP:2;
                vuint16_t RCIR:1;
                vuint16_t ITGDIR:1;
                vuint16_t BLNDCL:1;
                vuint16_t ITGDCL:1;
                vuint16_t RTZE:1;
                  vuint16_t:1;
                vuint16_t BLNST:1;
                vuint16_t ITGST:1;
                  vuint16_t:3;
                vuint16_t SDCPU:1;
                vuint16_t DZDIS:1;
            } B;
        } CONTROL;              /* Control & Status Register */

        union {
            vuint16_t R;
            struct {
                vuint16_t BLNIF:1;
                vuint16_t ITGIF:1;
                  vuint16_t:5;
                vuint16_t ACOVIF:1;
                vuint16_t BLNIE:1;
                vuint16_t ITGIE:1;
                  vuint16_t:5;
                vuint16_t ACOVIE:1;
            } B;
        } IRQ;                  /* Interrupt Flag and Enable Register */

        union {
            vuint16_t R;
            struct {
                vuint16_t ITGACC:16;
            } B;
        } ITGACC;               /* Integrator Accumulator register */

        union {
            vuint16_t R;
            struct {
                vuint16_t DCNT:16;
            } B;
        } DCNT;                 /* Down Counter Count register */

        union {
            vuint16_t R;
            struct {
                vuint16_t BLNCNTLD:16;
            } B;
        } BLNCNTLD;             /* Blanking Counter Load register */

        union {
            vuint16_t R;
            struct {
                vuint16_t ITGCNTLD:16;
            } B;
        } ITGCNTLD;             /* Integration Counter Load register */

        union {
            vuint16_t R;
            struct {
                vuint16_t:1;
                vuint16_t BLNDIV:3;
                  vuint16_t:1;
                vuint16_t ITGDIV:3;
                  vuint16_t:2;
                vuint16_t OFFCNC:2;
                  vuint16_t:1;
                vuint16_t ACDIV:3;
            } B;
        } PRESCALE;             /* Prescaler register */

        union {
            vuint16_t R;
            struct {
                vuint16_t TMST:1;
                vuint16_t ANLOUT:1;
                vuint16_t ANLIN:1;
                vuint16_t SSDEN:1;
                vuint16_t STEP1:1;
                vuint16_t POL:1;
                vuint16_t ITG:1;
                vuint16_t DACHIZ:1;
                vuint16_t BUFHIZ:1;
                vuint16_t AMPHIZ:1;
                vuint16_t RESSHORT:1;
                vuint16_t ITSSDRV:1;
                vuint16_t ITSSDRVEN:1;
                vuint16_t REFDRV:1;
                vuint16_t REFDRVEN:1;
            } B;
        } FNTEST;               /* Functional Test Mode register */

    };                          /* end of SSD_tag */
/****************************************************************************/
/*                          MODULE : EMIOS                                  */
/****************************************************************************/
    struct EMIOS_CHANNEL_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t CADR:24;
            } B;
        } CADR;                 /* Channel A Data Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t CBDR:24;
            } B;
        } CBDR;                 /* Channel B Data Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t CCNTR:24;
            } B;
        } CCNTR;                /* Channel Counter Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t FREN:1;
                vuint32_t ODIS:1;
                vuint32_t ODISSL:2;
                vuint32_t UCPRE:2;
                vuint32_t UCPEN:1;
                vuint32_t DMA:1;
                  vuint32_t:1;
                vuint32_t IF:4;
                vuint32_t FCK:1;
                vuint32_t FEN:1;
                  vuint32_t:3;
                vuint32_t FORCMA:1;
                vuint32_t FORCMB:1;
                  vuint32_t:1;
                vuint32_t BSL:2;
                vuint32_t EDSEL:1;
                vuint32_t EDPOL:1;
                vuint32_t MODE:7;
            } B;
        } CCR;                  /* Channel Control Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t OVR:1;
                  vuint32_t:15;
                vuint32_t OVFL:1;
                  vuint32_t:12;
                vuint32_t UCIN:1;
                vuint32_t UCOUT:1;
                vuint32_t FLAG:1;
            } B;
        } CSR;                  /* Channel Status Register */

        union {
            vuint32_t R;        /* Alternate Channel A Data Register */
        } ALTCADR;

        uint32_t emios_channel_reserved[2];

    };                          /* end of EMIOS_CHANNEL_tag */

    struct EMIOS_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t:1;
                vuint32_t MDIS:1;
                vuint32_t FRZ:1;
                vuint32_t GTBE:1;
                vuint32_t ETB:1;
                vuint32_t GPREN:1;
                  vuint32_t:6;
                vuint32_t SRV:4;
                vuint32_t GPRE:8;
                  vuint32_t:8;
            } B;
        } MCR;                  /* Module Configuration Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t F23:1;
                vuint32_t F22:1;
                vuint32_t F21:1;
                vuint32_t F20:1;
                vuint32_t F19:1;
                vuint32_t F18:1;
                vuint32_t F17:1;
                vuint32_t F16:1;
                vuint32_t F15:1;
                vuint32_t F14:1;
                vuint32_t F13:1;
                vuint32_t F12:1;
                vuint32_t F11:1;
                vuint32_t F10:1;
                vuint32_t F9:1;
                vuint32_t F8:1;
                vuint32_t F7:1;
                vuint32_t F6:1;
                vuint32_t F5:1;
                vuint32_t F4:1;
                vuint32_t F3:1;
                vuint32_t F2:1;
                vuint32_t F1:1;
                vuint32_t F0:1;
            } B;
        } GFR;                  /* Global FLAG Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t OU23:1;
                vuint32_t OU22:1;
                vuint32_t OU21:1;
                vuint32_t OU20:1;
                vuint32_t OU19:1;
                vuint32_t OU18:1;
                vuint32_t OU17:1;
                vuint32_t OU16:1;
                vuint32_t OU15:1;
                vuint32_t OU14:1;
                vuint32_t OU13:1;
                vuint32_t OU12:1;
                vuint32_t OU11:1;
                vuint32_t OU10:1;
                vuint32_t OU9:1;
                vuint32_t OU8:1;
                vuint32_t OU7:1;
                vuint32_t OU6:1;
                vuint32_t OU5:1;
                vuint32_t OU4:1;
                vuint32_t OU3:1;
                vuint32_t OU2:1;
                vuint32_t OU1:1;
                vuint32_t OU0:1;
            } B;
        } OUDR;                 /* Output Update Disable Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:8;
                vuint32_t CHDIS23:1;
                vuint32_t CHDIS22:1;
                vuint32_t CHDIS21:1;
                vuint32_t CHDIS20:1;
                vuint32_t CHDIS19:1;
                vuint32_t CHDIS18:1;
                vuint32_t CHDIS17:1;
                vuint32_t CHDIS16:1;
                vuint32_t CHDIS15:1;
                vuint32_t CHDIS14:1;
                vuint32_t CHDIS13:1;
                vuint32_t CHDIS12:1;
                vuint32_t CHDIS11:1;
                vuint32_t CHDIS10:1;
                vuint32_t CHDIS9:1;
                vuint32_t CHDIS8:1;
                vuint32_t CHDIS7:1;
                vuint32_t CHDIS6:1;
                vuint32_t CHDIS5:1;
                vuint32_t CHDIS4:1;
                vuint32_t CHDIS3:1;
                vuint32_t CHDIS2:1;
                vuint32_t CHDIS1:1;
                vuint32_t CHDIS0:1;
            } B;
        } UCDIS;                /* Disable Channel Register */

        uint32_t emios_reserved1[4];

        struct EMIOS_CHANNEL_tag CH[28];

    };                          /* end of EMIOS_tag */
/****************************************************************************/
/*                          MODULE : pit                                    */
/****************************************************************************/
    struct PIT_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t:30;
                vuint32_t MDIS:1;
                vuint32_t FRZ:1;
            } B;
        } PITMCR;

        uint32_t pit_reserved1[63];     /* (0x0100 - 0x0004)/4 = 0x3F */

        struct {
            union {
                vuint32_t R;
                struct {
                    vuint32_t TSV:32;
                } B;
            } LDVAL;

            union {
                vuint32_t R;
                struct {
                    vuint32_t TVL:32;
                } B;
            } CVAL;

            union {
                vuint32_t R;
                struct {
                    vuint32_t:30;
                    vuint32_t TIE:1;
                    vuint32_t TEN:1;
                } B;
            } TCTRL;

            union {
                vuint32_t R;
                struct {
                    vuint32_t:31;
                    vuint32_t TIF:1;
                } B;
            } TFLG;
        } CH[6];

    };                          /* end of PIT_tag */
/****************************************************************************/
/*                          MODULE : i2c                                    */
/****************************************************************************/
    struct I2C_tag {
        union {
            vuint8_t R;
            struct {
                vuint8_t ADR:7;
                  vuint8_t:1;
            } B;
        } IBAD;                 /* Module Bus Address Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t IBC:8;
            } B;
        } IBFD;                 /* Module Bus Frequency Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t MDIS:1;
                vuint8_t IBIE:1;
                vuint8_t MS:1;
                vuint8_t TX:1;
                vuint8_t NOACK:1;
                vuint8_t RSTA:1;
                vuint8_t DMAEN:1;
                vuint8_t IBDOZE:1;
            } B;
        } IBCR;                 /* Module Bus Control Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t TCF:1;
                vuint8_t IAAS:1;
                vuint8_t IBB:1;
                vuint8_t IBAL:1;
                  vuint8_t:1;
                vuint8_t SRW:1;
                vuint8_t IBIF:1;
                vuint8_t RXAK:1;
            } B;
        } IBSR;                 /* Module Status Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t DATA:8;
            } B;
        } IBDR;                 /* Module Data Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t BIIE:1;
                  vuint8_t:7;
            } B;
        } IBIC;                 /* Module Interrupt Configuration Register */

    };                          /* end of I2C_tag */
/****************************************************************************/
/*                          MODULE : MPU                                    */
/****************************************************************************/
    struct MPU_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t SPERR:8;
                  vuint32_t:4;
                vuint32_t HRL:4;
                vuint32_t NSP:4;
                vuint32_t NGRD:4;
                  vuint32_t:7;
                vuint32_t VLD:1;
            } B;
        } CESR;                 /* Module Control/Error Status Register */

        uint32_t mpu_reserved1[3];      /* (0x010 - 0x004)/4 = 0x03 */

        union {
            vuint32_t R;
            struct {
                vuint32_t EADDR:32;
            } B;
        } EAR0;

        union {
            vuint32_t R;
            struct {
                vuint32_t EACD:16;
                vuint32_t EPID:8;
                vuint32_t EMN:4;
                vuint32_t EATTR:3;
                vuint32_t ERW:1;
            } B;
        } EDR0;

        union {
            vuint32_t R;
            struct {
                vuint32_t EADDR:32;
            } B;
        } EAR1;

        union {
            vuint32_t R;
            struct {
                vuint32_t EACD:16;
                vuint32_t EPID:8;
                vuint32_t EMN:4;
                vuint32_t EATTR:3;
                vuint32_t ERW:1;
            } B;
        } EDR1;

        union {
            vuint32_t R;
            struct {
                vuint32_t EADDR:32;
            } B;
        } EAR2;

        union {
            vuint32_t R;
            struct {
                vuint32_t EACD:16;
                vuint32_t EPID:8;
                vuint32_t EMN:4;
                vuint32_t EATTR:3;
                vuint32_t ERW:1;
            } B;
        } EDR2;

        union {
            vuint32_t R;
            struct {
                vuint32_t EADDR:32;
            } B;
        } EAR3;

        union {
            vuint32_t R;
            struct {
                vuint32_t EACD:16;
                vuint32_t EPID:8;
                vuint32_t EMN:4;
                vuint32_t EATTR:3;
                vuint32_t ERW:1;
            } B;
        } EDR3;

        uint32_t mpu_reserved2[244];    /* (0x0400 - 0x0030)/4 = 0x0F4 */

        struct {
            union {
                vuint32_t R;
                struct {
                    vuint32_t SRTADDR:27;
                      vuint32_t:5;
                } B;
            } WORD0;            /* Region Descriptor n Word 0 */

            union {
                vuint32_t R;
                struct {
                    vuint32_t ENDADDR:27;
                      vuint32_t:5;
                } B;
            } WORD1;            /* Region Descriptor n Word 1 */

            union {
                vuint32_t R;
                struct {
                    vuint32_t M7RE:1;
                    vuint32_t M7WE:1;
                    vuint32_t M6RE:1;
                    vuint32_t M6WE:1;
                    vuint32_t M5RE:1;
                    vuint32_t M5WE:1;
                    vuint32_t M4RE:1;
                    vuint32_t M4WE:1;
                    vuint32_t M3PE:1;
                    vuint32_t M3SM:2;
                    vuint32_t M3UM:3;
                    vuint32_t M2PE:1;
                    vuint32_t M2SM:2;
                    vuint32_t M2UM:3;
                    vuint32_t M1PE:1;
                    vuint32_t M1SM:2;
                    vuint32_t M1UM:3;
                    vuint32_t M0PE:1;
                    vuint32_t M0SM:2;
                    vuint32_t M0UM:3;
                } B;
            } WORD2;            /* Region Descriptor n Word 2 */

            union {
                vuint32_t R;
                struct {
                    vuint32_t PID:8;vuint32_t PIDMASK:8;vuint32_t:15;vuint32_t VLD:1;
                } B;
            } WORD3;            /* Region Descriptor n Word 3 */

        } RGD[16];

        uint32_t mpu_reserved3[192];    /* (0x0800 - 0x0500)/4 = 0x0C0 */

        union {
            vuint32_t R;
            struct {
                vuint32_t M7RE:1;
                vuint32_t M7WE:1;
                vuint32_t M6RE:1;
                vuint32_t M6WE:1;
                vuint32_t M5RE:1;
                vuint32_t M5WE:1;
                vuint32_t M4RE:1;
                vuint32_t M4WE:1;
                vuint32_t M3PE:1;
                vuint32_t M3SM:2;
                vuint32_t M3UM:3;
                vuint32_t M2PE:1;
                vuint32_t M2SM:2;
                vuint32_t M2UM:3;
                vuint32_t M1PE:1;
                vuint32_t M1SM:2;
                vuint32_t M1UM:3;
                vuint32_t M0PE:1;
                vuint32_t M0SM:2;
                vuint32_t M0UM:3;
            } B;
        } RGDAAC[16];           /* Region Descriptor Alternate Access Control n */

    };                          /* end of MPU_tag */
/****************************************************************************/
/*                          MODULE : eDMA                                   */
/****************************************************************************/

/*for "standard" format TCD (when EDMA.TCD[x].CITER.E_LINK==BITER.E_LINK=0) */
    struct EDMA_TCD_STD_tag {

        vuint32_t SADDR;        /* source address */

        vuint16_t SMOD:5;       /* source address modulo */
        vuint16_t SSIZE:3;      /* source transfer size */
        vuint16_t DMOD:5;       /* destination address modulo */
        vuint16_t DSIZE:3;      /* destination transfer size */
        vint16_t SOFF;          /* signed source address offset */

		union {
            vuint32_t R;
            struct {
				vuint32_t SMLOE:1;
				vuint32_t DMLOE:1;
				int32_t	  MLOFF:20;
				vuint32_t NBYTES:10;
            } B;
        } NBYTESu;           /* Region Descriptor Alternate Access Control n */
	
        vint32_t SLAST;         /* last destination address adjustment, or
                                   scatter/gather address (if e_sg = 1) */

        vuint32_t DADDR;        /* destination address */

        vuint16_t CITERE_LINK:1;
        vuint16_t CITER:15;

        vint16_t DOFF;          /* signed destination address offset */

        vint32_t DLAST_SGA;

        vuint16_t BITERE_LINK:1;        /* beginning ("major") iteration count */
        vuint16_t BITER:15;

        vuint16_t BWC:2;        /* bandwidth control */
        vuint16_t MAJORLINKCH:6;        /* enable channel-to-channel link */
        vuint16_t DONE:1;       /* channel done */
        vuint16_t ACTIVE:1;     /* channel active */
        vuint16_t MAJORE_LINK:1;        /* enable channel-to-channel link */
        vuint16_t E_SG:1;       /* enable scatter/gather descriptor */
        vuint16_t D_REQ:1;      /* disable ipd_req when done */
        vuint16_t INT_HALF:1;   /* interrupt on citer = (biter >> 1) */
        vuint16_t INT_MAJ:1;    /* interrupt on major loop completion */
        vuint16_t START:1;      /* explicit channel start */

    };                          /* end of EDMA_TCD_STD_tag */

/*for "channel link" format TCD (when EDMA.TCD[x].CITER.E_LINK==BITER.E_LINK=1)*/
    struct EDMA_TCD_CHLINK_tag {

        vuint32_t SADDR;        /* source address */

        vuint16_t SMOD:5;       /* source address modulo */
        vuint16_t SSIZE:3;      /* source transfer size */
        vuint16_t DMOD:5;       /* destination address modulo */
        vuint16_t DSIZE:3;      /* destination transfer size */
        vint16_t SOFF;          /* signed source address offset */

		union {
            vuint32_t R;
            struct {
				vuint32_t SMLOE:1;
				vuint32_t DMLOE:1;
				int32_t	  MLOFF:20;
				vuint32_t NBYTES:10;
            } B;
        } NBYTESu;           /* Region Descriptor Alternate Access Control n */
        vint32_t SLAST;         /* last destination address adjustment, or
                                   scatter/gather address (if e_sg = 1) */

        vuint32_t DADDR;        /* destination address */

        vuint16_t CITERE_LINK:1;
        vuint16_t CITERLINKCH:6;
        vuint16_t CITER:9;

        vint16_t DOFF;          /* signed destination address offset */

        vint32_t DLAST_SGA;

        vuint16_t BITERE_LINK:1;        /* beginning (major) iteration count */
        vuint16_t BITERLINKCH:6;
        vuint16_t BITER:9;

        vuint16_t BWC:2;        /* bandwidth control */
        vuint16_t MAJORLINKCH:6;        /* enable channel-to-channel link */
        vuint16_t DONE:1;       /* channel done */
        vuint16_t ACTIVE:1;     /* channel active */
        vuint16_t MAJORE_LINK:1;        /* enable channel-to-channel link */
        vuint16_t E_SG:1;       /* enable scatter/gather descriptor */
        vuint16_t D_REQ:1;      /* disable ipd_req when done */
        vuint16_t INT_HALF:1;   /* interrupt on citer = (biter >> 1) */
        vuint16_t INT_MAJ:1;    /* interrupt on major loop completion */
        vuint16_t START:1;      /* explicit channel start */

    };                          /* end of EDMA_TCD_CHLINK_tag */

    struct EDMA_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t:14;
                vuint32_t CX:1;
                vuint32_t ECX:1;
                vuint32_t GRP3PRI:2;
                vuint32_t GRP2PRI:2;
                vuint32_t GRP1PRI:2;
                vuint32_t GRP0PRI:2;
                vuint32_t EMLM:1;
                vuint32_t CLM:1;
                vuint32_t HALT:1;
                vuint32_t HOE:1;
                vuint32_t ERGA:1;
                vuint32_t ERCA:1;
                vuint32_t EDBG:1;
                vuint32_t EBW:1;
            } B;
        } CR;                   /* Control Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t VLD:1;
                  vuint32_t:14;
                vuint32_t ECX:1;
                vuint32_t GPE:1;
                vuint32_t CPE:1;
                vuint32_t ERRCHN:6;
                vuint32_t SAE:1;
                vuint32_t SOE:1;
                vuint32_t DAE:1;
                vuint32_t DOE:1;
                vuint32_t NCE:1;
                vuint32_t SGE:1;
                vuint32_t SBE:1;
                vuint32_t DBE:1;
            } B;
        } ESR;                  /* Error Status Register */

        int16_t EDMA_reserved1[3];      /* (0x0E - 0x08)/2 = 0x03 */

        union {
            vuint16_t R;
            struct {
                vuint16_t ERQ15:1;
                vuint16_t ERQ14:1;
                vuint16_t ERQ13:1;
                vuint16_t ERQ12:1;
                vuint16_t ERQ11:1;
                vuint16_t ERQ10:1;
                vuint16_t ERQ09:1;
                vuint16_t ERQ08:1;
                vuint16_t ERQ07:1;
                vuint16_t ERQ06:1;
                vuint16_t ERQ05:1;
                vuint16_t ERQ04:1;
                vuint16_t ERQ03:1;
                vuint16_t ERQ02:1;
                vuint16_t ERQ01:1;
                vuint16_t ERQ00:1;
            } B;
        } ERQRL;                /* DMA Enable Request Register Low */

        int16_t EDMA_reserved2[3];      /* (0x16 - 0x10)/2 = 0x03 */

        union {
            vuint16_t R;
            struct {
                vuint16_t EEI15:1;
                vuint16_t EEI14:1;
                vuint16_t EEI13:1;
                vuint16_t EEI12:1;
                vuint16_t EEI11:1;
                vuint16_t EEI10:1;
                vuint16_t EEI09:1;
                vuint16_t EEI08:1;
                vuint16_t EEI07:1;
                vuint16_t EEI06:1;
                vuint16_t EEI05:1;
                vuint16_t EEI04:1;
                vuint16_t EEI03:1;
                vuint16_t EEI02:1;
                vuint16_t EEI01:1;
                vuint16_t EEI00:1;
            } B;
        } EEIRL;                /* DMA Enable Error Interrupt Register Low */

        union {
            vuint8_t R;
            struct {
                vuint8_t NOP:1;
                vuint8_t SERQ:7;
            } B;
        } SERQR;                /* DMA Set Enable Request Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t NOP:1;
                vuint8_t CERQ:7;
            } B;
        } CERQR;                /* DMA Clear Enable Request Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t NOP:1;
                vuint8_t SEEI:7;
            } B;
        } SEEIR;                /* DMA Set Enable Error Interrupt Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t NOP:1;
                vuint8_t CEEI:7;
            } B;
        } CEEIR;                /* DMA Clear Enable Error Interrupt Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t NOP:1;
                vuint8_t CINT:7;
            } B;
        } CINTR;                /* DMA Clear Interrupt Request Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t NOP:1;
                vuint8_t CER:7;
            } B;
        } CERR;                 /* DMA Clear error Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t NOP:1;
                vuint8_t SSB:7;
            } B;
        } SSRT;                 /* Set Start Bit Register */

        union {
            vuint8_t R;
            struct {
                vuint8_t NOP:1;
                vuint8_t CDSB:7;
            } B;
        } CDNE;                /* Clear Done Status Bit Register */

        int16_t EDMA_reserved3[3];      /* (0x26 - 0x20)/2 = 0x03 */

        union {
            vuint16_t R;
            struct {
                vuint16_t INT15:1;
                vuint16_t INT14:1;
                vuint16_t INT13:1;
                vuint16_t INT12:1;
                vuint16_t INT11:1;
                vuint16_t INT10:1;
                vuint16_t INT09:1;
                vuint16_t INT08:1;
                vuint16_t INT07:1;
                vuint16_t INT06:1;
                vuint16_t INT05:1;
                vuint16_t INT04:1;
                vuint16_t INT03:1;
                vuint16_t INT02:1;
                vuint16_t INT01:1;
                vuint16_t INT00:1;
            } B;
        } IRQRL;                /* DMA Interrupt Request Low */

        int16_t EDMA_reserved4[3];      /* (0x2E - 0x28)/2 = 0x03 */

        union {
            vuint16_t R;
            struct {
                vuint16_t ERR15:1;
                vuint16_t ERR14:1;
                vuint16_t ERR13:1;
                vuint16_t ERR12:1;
                vuint16_t ERR11:1;
                vuint16_t ERR10:1;
                vuint16_t ERR09:1;
                vuint16_t ERR08:1;
                vuint16_t ERR07:1;
                vuint16_t ERR06:1;
                vuint16_t ERR05:1;
                vuint16_t ERR04:1;
                vuint16_t ERR03:1;
                vuint16_t ERR02:1;
                vuint16_t ERR01:1;
                vuint16_t ERR00:1;
            } B;
        } ERL;                  /* DMA Error Low */

        int16_t EDMA_reserved5[3];      /* (0x36 - 0x30)/2 = 0x03 */

        union {
            vuint16_t R;
            struct {
                vuint16_t HRS15:1;
                vuint16_t HRS14:1;
                vuint16_t HRS13:1;
                vuint16_t HRS12:1;
                vuint16_t HRS11:1;
                vuint16_t HRS10:1;
                vuint16_t HRS09:1;
                vuint16_t HRS08:1;
                vuint16_t HRS07:1;
                vuint16_t HRS06:1;
                vuint16_t HRS05:1;
                vuint16_t HRS04:1;
                vuint16_t HRS03:1;
                vuint16_t HRS02:1;
                vuint16_t HRS01:1;
                vuint16_t HRS00:1;
            } B;
        } HRSL;                 /* DMA Hardware Request Status Low */

        uint32_t edma_reserved1[50];    /* (0x100 - 0x038)/4 = 0x32 */

        union {
            vuint8_t R;
            struct {
                vuint8_t ECP:1;
                vuint8_t DPA:1;
                vuint8_t GRPPRI:2;
                vuint8_t CHPRI:4;
            } B;
        } CPR[16];              /* Channel n Priority */

        uint32_t edma_reserved2[956];   /* (0x1000 - 0x0110)/4 = 0x3BC */

	/* Select one of the following declarations depending on the DMA mode chosen */
        struct EDMA_TCD_STD_tag TCD[16];
        /* struct EDMA_TCD_CHLINK_tag TCD[16]; */

    };                          /* end of EDMA_tag */
/****************************************************************************/
/*                          MODULE : INTC                                   */
/****************************************************************************/
    struct INTC_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t:26;
                vuint32_t VTES:1;
                  vuint32_t:4;
                vuint32_t HVEN:1;
            } B;
        } MCR;                  /* Module Configuration Register */

        int32_t INTC_reserved1; /* (0x008 - 0x004)/4 = 0x01 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:28;
                vuint32_t PRI:4;
            } B;
        } CPR;                  /* Current Priority Register */

        int32_t INTC_reserved2; /* (0x010 - 0x00C)/4 = 0x01 */

        union {
            vuint32_t R;
            struct {
                vuint32_t VTBA:21;
                vuint32_t INTVEC:9;
                  vuint32_t:2;
            } B;
        } IACKR;                /* Interrupt Acknowledge Register */

        int32_t INTC_reserved3; /* (0x018 - 0x014)/4 = 0x01 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:32;
            } B;
        } EOIR;                 /* End of Interrupt Register */

        int32_t INTC_reserved4; /* (0x020 - 0x01C)/4 = 0x01 */

        union {
            vuint8_t R;
            struct {
                vuint8_t:6;
                vuint8_t SET:1;
                vuint8_t CLR:1;
            } B;
        } SSCIR[8];             /* Software Set/Clear Interruput Register */

        uint32_t intc_reserved5[6];     /* (0x040 - 0x028)/4 = 0x06 */

        union {
            vuint8_t R;
            struct {
                vuint8_t:4;
                vuint8_t PRI:4;
            } B;
        } PSR[512];             /* Software Set/Clear Interrupt Register */

    };                          /* end of INTC_tag */
/****************************************************************************/
/*                          MODULE : DSPI                                   */
/****************************************************************************/
    struct DSPI_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t MSTR:1;
                vuint32_t CONT_SCKE:1;
                vuint32_t DCONF:2;
                vuint32_t FRZ:1;
                vuint32_t MTFE:1;
                vuint32_t PCSSE:1;
                vuint32_t ROOE:1;
                  vuint32_t:2;
                vuint32_t PCSIS5:1;
                vuint32_t PCSIS4:1;
                vuint32_t PCSIS3:1;
                vuint32_t PCSIS2:1;
                vuint32_t PCSIS1:1;
                vuint32_t PCSIS0:1;
                vuint32_t DOZE:1;
                vuint32_t MDIS:1;
                vuint32_t DIS_TXF:1;
                vuint32_t DIS_RXF:1;
                vuint32_t CLR_TXF:1;
                vuint32_t CLR_RXF:1;
                vuint32_t SMPL_PT:2;
                  vuint32_t:7;
                vuint32_t HALT:1;
            } B;
        } MCR;                  /* Module Configuration Register */

        uint32_t dspi_reserved1;

        union {
            vuint32_t R;
            struct {
                vuint32_t TCNT:16;
                  vuint32_t:16;
            } B;
        } TCR;

        union {
            vuint32_t R;
            struct {
                vuint32_t DBR:1;
                vuint32_t FMSZ:4;
                vuint32_t CPOL:1;
                vuint32_t CPHA:1;
                vuint32_t LSBFE:1;
                vuint32_t PCSSCK:2;
                vuint32_t PASC:2;
                vuint32_t PDT:2;
                vuint32_t PBR:2;
                vuint32_t CSSCK:4;
                vuint32_t ASC:4;
                vuint32_t DT:4;
                vuint32_t BR:4;
            } B;
        } CTAR[8];              /* Clock and Transfer Attributes Registers */

        union {
            vuint32_t R;
            struct {
                vuint32_t TCF:1;
                vuint32_t TXRXS:1;
                  vuint32_t:1;
                vuint32_t EOQF:1;
                vuint32_t TFUF:1;
                  vuint32_t:1;
                vuint32_t TFFF:1;
                  vuint32_t:5;
                vuint32_t RFOF:1;
                  vuint32_t:1;
                vuint32_t RFDF:1;
                  vuint32_t:1;
                vuint32_t TXCTR:4;
                vuint32_t TXNXTPTR:4;
                vuint32_t RXCTR:4;
                vuint32_t POPNXTPTR:4;
            } B;
        } SR;                   /* Status Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t TCFRE:1;
                  vuint32_t:2;
                vuint32_t EOQFRE:1;
                vuint32_t TFUFRE:1;
                  vuint32_t:1;
                vuint32_t TFFFRE:1;
                vuint32_t TFFFDIRS:1;
                  vuint32_t:4;
                vuint32_t RFOFRE:1;
                  vuint32_t:1;
                vuint32_t RFDFRE:1;
                vuint32_t RFDFDIRS:1;
                  vuint32_t:16;
            } B;
        } RSER;                 /* DMA/Interrupt Request Select and Enable Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t CONT:1;
                vuint32_t CTAS:3;
                vuint32_t EOQ:1;
                vuint32_t CTCNT:1;
                  vuint32_t:4;
                vuint32_t PCS5:1;
                vuint32_t PCS4:1;
                vuint32_t PCS3:1;
                vuint32_t PCS2:1;
                vuint32_t PCS1:1;
                vuint32_t PCS0:1;
                vuint32_t TXDATA:16;
            } B;
        } PUSHR;                /* PUSH TX FIFO Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t RXDATA:16;
            } B;
        } POPR;                 /* POP RX FIFO Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t TXCMD:16;
                vuint32_t TXDATA:16;
            } B;
        } TXFR[4];              /* Transmit FIFO Registers */

        vuint32_t DSPI_reserved_txf[12];

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t RXDATA:16;
            } B;
        } RXFR[4];              /* Transmit FIFO Registers */

        vuint32_t DSPI_reserved_rxf[12];

        union {
            vuint32_t R;
            struct {
                vuint32_t MTOE:1;
                  vuint32_t:1;
                vuint32_t MTOCNT:6;
                  vuint32_t:4;
                vuint32_t TXSS:1;
                vuint32_t TPOL:1;
                vuint32_t TRRE:1;
                vuint32_t CID:1;
                vuint32_t DCONT:1;
                vuint32_t DSICTAS:3;
                  vuint32_t:6;
                vuint32_t DPCS5:1;
                vuint32_t DPCS4:1;
                vuint32_t DPCS3:1;
                vuint32_t DPCS2:1;
                vuint32_t DPCS1:1;
                vuint32_t DPCS0:1;
            } B;
        } DSICR;                /* DSI Configuration Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t SER_DATA:16;
            } B;
        } SDR;                  /* DSI Serialization Data Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t ASER_DATA:16;
            } B;
        } ASDR;                 /* DSI Alternate Serialization Data Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t COMP_DATA:16;
            } B;
        } COMPR;                /* DSI Transmit Comparison Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t DESER_DATA:16;
            } B;
        } DDR;                  /* DSI deserialization Data Register */

    };                          /* end of DSPI_tag */
/****************************************************************************/
/*                          MODULE : QUADSPI                                   */
/****************************************************************************/
    struct QUADSPI_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t MSTR:1;
                vuint32_t CONT_SCKE:1;
                  vuint32_t:2;
                vuint32_t FRZ:1;
                vuint32_t MTFE:1;
                vuint32_t PCSSE:1;
                vuint32_t ROOE:1;
                vuint32_t PCSIS7:1;
                vuint32_t PCSIS6:1;
                vuint32_t PCSIS5:1;
                vuint32_t PCSIS4:1;
                vuint32_t PCSIS3:1;
                vuint32_t PCSIS2:1;
                vuint32_t PCSIS1:1;
                vuint32_t PCSIS0:1;
                vuint32_t DOZE:1;
                vuint32_t MDIS:1;
                vuint32_t DIS_TXF:1;
                vuint32_t DIS_RXF:1;
                vuint32_t CLR_TXF:1;
                vuint32_t CLR_RXF:1;
                vuint32_t SMPL_PT:2;
                vuint32_t QMODE:1;
                vuint32_t VMID:4;
                  vuint32_t:2;
                vuint32_t HALT:1;
            } B;
        } MCR;

        uint32_t quadspi_reserved1;		/* BASE + 0x004 */

        vuint32_t TCR;

        union {
            vuint32_t R;
            struct {
                vuint32_t DBR:1;
                vuint32_t FMSZ:4;
                vuint32_t CPOL:1;
                vuint32_t CPHA:1;
                vuint32_t LSBFE:1;
                vuint32_t PCSSCK:2;
                vuint32_t PASC:2;
                vuint32_t PDT:2;
                vuint32_t PBR:2;
                vuint32_t CSSCK:4;
                vuint32_t ASC:4;
                vuint32_t DT:4;
                vuint32_t BR:4;
            } B;
        } CTAR[2];              /* Clock and Transfer Attributes Registers */

        uint32_t quadspi_reserved2[6];		/* {0x002C-0x0014}/0x4 = 0x06 */
        
        union {
            vuint32_t R;
            struct {
                vuint32_t TCF:1;
                vuint32_t TXRXS:1;
                  vuint32_t:1;
                vuint32_t EOQF:1;
                vuint32_t TFUF:1;
                  vuint32_t:1;
                vuint32_t TFFF:1;
                  vuint32_t:5;
                vuint32_t RFOF:1;
                  vuint32_t:1;
                vuint32_t RFDF:1;
                  vuint32_t:1;
                vuint32_t TXCTR:4;
                vuint32_t TXNXTPTR:4;
                vuint32_t RXCTR:4;
                vuint32_t POPNXTPTR:4;
            } B;
        } SPISR;

        union {
            vuint32_t R;
            struct {
                vuint32_t TCF_IE:1;
                  vuint32_t:2;
                vuint32_t EOQF_IE:1;
                vuint32_t TFUF_IE:1;
                  vuint32_t:1;
                vuint32_t TFFF_RE:1;
                vuint32_t TFFF_DIRS:1;
                  vuint32_t:4;
                vuint32_t RFOF_IE:1;
                  vuint32_t:1;
                vuint32_t RFDF_RE:1;
                vuint32_t RFDF_DIRS:1;
                  vuint32_t:16;
            } B;
        } SPIRSER;

        union {
            vuint32_t R;
            struct {
                vuint32_t CONT:1;
                vuint32_t CTAS:3;
                vuint32_t EOQ:1;
                vuint32_t CTCNT:1;
                  vuint32_t:2;
                vuint32_t PCS7:1;
                vuint32_t PCS6:1;
                vuint32_t PCS5:1;
                vuint32_t PCS4:1;
                vuint32_t PCS3:1;
                vuint32_t PCS2:1;
                vuint32_t PCS1:1;
                vuint32_t PCS0:1;
                vuint32_t TXDATA:16;
            } B;
        } PUSHR;

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t RXDATA:16;
            } B;
        } POPR;

        union {
            vuint32_t R;
            struct {
                vuint32_t TXCMD:16;
                vuint32_t TXDATA:16;
            } B;
        } TXFR[15];

        uint32_t quadspi_reserved3;			/* {0x0078-0x0078}/0x4 = 0x01 */

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t RXDATA:16;
            } B;
        } RXFR[15];

        uint32_t quadspi_reserved4[18];		/* {0x0100-0x00B8}/0x4 = 0x12 */

        vuint32_t SFAR;

        union {
            vuint32_t R;
            struct {
                vuint32_t ICO:24;
                vuint32_t IC:8;
            } B;
        } ICR;

        union {
            vuint32_t R;
            struct {
                  vuint32_t:25;
                vuint32_t FSDLY:1;
                vuint32_t FSPHS:1;
                  vuint32_t:2;
                vuint32_t HSDLY:1;
                vuint32_t HSPHS:1;
                vuint32_t HSENA:1;
            } B;
        } SMPR;

        union {
            vuint32_t R;
            struct {
                vuint32_t RDCTR:24;
                  vuint32_t:8;
            } B;
        } RBSR;

		vuint32_t RBDR[15];

        uint32_t quadspi_reserved5;			/* {0x0150-0x014C}/0x4 = 0x01 */

        union {
            vuint32_t R;
            struct {
                  vuint32_t:7;
                vuint32_t TRCTR:9;
                vuint32_t TRBFL:8;
                  vuint32_t:8;
            } B;
        } TBSR;

        vuint32_t TBDR;

        union {
            vuint32_t R;
            struct {
                  vuint32_t:8;
                vuint32_t ARMB:8;
                  vuint32_t:1;
                vuint32_t ARSZ:5;
                  vuint32_t:2;
                vuint32_t ARIC:8;
            } B;
        } ACR;

        union {
            vuint32_t R;
            struct {
                  vuint32_t:4;
                vuint32_t TXFULL:1;
                  vuint32_t:2;
                vuint32_t TXNE:1;
                  vuint32_t:4;
                vuint32_t RXFULL:1;
                  vuint32_t:2;
                vuint32_t RXNE:1;
                  vuint32_t:4;
                vuint32_t AHBFULL:1;
                  vuint32_t:2;
                vuint32_t AHBNE:1;
                  vuint32_t:4;
                vuint32_t CONTMODE:1;
                vuint32_t AHB_ACC:1;
                vuint32_t IP_ACC:1;
                vuint32_t BUSY:1;
            } B;
        } SFMSR;

        union {
            vuint32_t R;
            struct {
                  vuint32_t:4;
                vuint32_t TBFF:1;
                vuint32_t TBUF:1;
                  vuint32_t:8;
                vuint32_t RBOF:1;
                vuint32_t RBDF:1;
                  vuint32_t:6;
                vuint32_t ABOF:1;
                  vuint32_t:1;
                vuint32_t IPAEF:1;
                vuint32_t IPIEF:1;
                vuint32_t ICEF:1;
                  vuint32_t:4;
                vuint32_t TFF:1;
            } B;
        } SFMFR;

        union {
            vuint32_t R;
            struct {
                  vuint32_t:4;
                vuint32_t TBFIE:1;
                vuint32_t TBUIE:1;
                  vuint32_t:4;
                vuint32_t RBDDE:1;
                  vuint32_t:3;
                vuint32_t RBOIE:1;
                vuint32_t RBDIE:1;
                  vuint32_t:6;
                vuint32_t ABOIE:1;
                  vuint32_t:1;
                vuint32_t IPAEIE:1;
                vuint32_t IPIEIE:1;
                vuint32_t ICEIE:1;
                  vuint32_t:4;
                vuint32_t TFIE:1;
            } B;
        } SFMRSER;

        uint32_t quadspi_reserved6[38];		/* {0x0200-0x0168}/0x4 = 0x26 */

    };                          /* end of QUADSPI_tag */
/****************************************************************************/
/*                          MODULE : FlexCAN                                */
/****************************************************************************/
    struct FLEXCAN_BUF_t {
        union {
            vuint32_t R;
            struct {
                vuint32_t:4;
                vuint32_t CODE:4;
                  vuint32_t:1;
                vuint32_t SRR:1;
                vuint32_t IDE:1;
                vuint32_t RTR:1;
                vuint32_t LENGTH:4;
                vuint32_t TIMESTAMP:16;
            } B;
        } CS;

        union {
            vuint32_t R;
            struct {
                vuint32_t PRIO:3;
                vuint32_t STD_ID:11;
                vuint32_t EXT_ID:18;
            } B;
        } ID;

        union {
            vuint8_t  B[8]; /* Data buffer in Bytes (8 bits) */
            /*vuint16_t H[4]; *//* Data buffer in Half-words (16 bits) */
            vuint32_t W[2];     /* Data buffer in words (32 bits) */
            /*vuint32_t R[2]; *//* Data buffer in words (32 bits) */
        } DATA;

    };                          /* end of FLEXCAN_BUF_t */

    struct FLEXCAN_RXFIFO_t {
        union {
            vuint32_t R;
            struct {
                vuint32_t:9;
                vuint32_t SRR:1;
                vuint32_t IDE:1;
                vuint32_t RTR:1;
                vuint32_t LENGTH:4;
                vuint32_t TIMESTAMP:16;
            } B;
        } CS;

        union {
            vuint32_t R;
            struct {
                vuint32_t STD_ID:11;
                vuint32_t EXT_ID:18;
            } B;
        } ID;

        union {
            /*vuint8_t  B[8]; *//* Data buffer in Bytes (8 bits) */
            /*vuint16_t H[4]; *//* Data buffer in Half-words (16 bits) */
            vuint32_t W[2];     /* Data buffer in words (32 bits) */
            /*vuint32_t R[2]; *//* Data buffer in words (32 bits) */
        } DATA;

        uint32_t FLEXCAN_RXFIFO_reserved[20];   /* {0x00E0-0x0090}/0x4 = 0x14 */

        union {
            vuint32_t R;
        } IDTABLE[8];

    };                          /* end of FLEXCAN_RXFIFO_t */

    struct FLEXCAN_tag {
        union {
            vuint32_t R;
            struct {
                vuint32_t MDIS:1;
                vuint32_t FRZ:1;
                vuint32_t FEN:1;
                vuint32_t HALT:1;
                vuint32_t NOTRDY:1;
                vuint32_t WAKMSK:1;
                vuint32_t SOFTRST:1;
                vuint32_t FRZACK:1;
                vuint32_t SUPV:1;
                vuint32_t SLFWAK:1;
                vuint32_t WRNEN:1;
                vuint32_t LPMACK:1;
                vuint32_t WAKSRC:1;
                vuint32_t DOZE:1;
                vuint32_t SRXDIS:1;
                vuint32_t BCC:1;
                  vuint32_t:2;
                vuint32_t LPRIO_EN:1;
                vuint32_t AEN:1;
                  vuint32_t:2;
                vuint32_t IDAM:2;
                  vuint32_t:2;
                vuint32_t MAXMB:6;
            } B;
        } MCR;                  /* Module Configuration Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t PRESDIV:8;
                vuint32_t RJW:2;
                vuint32_t PSEG1:3;
                vuint32_t PSEG2:3;
                vuint32_t BOFFMSK:1;
                vuint32_t ERRMSK:1;
                vuint32_t CLKSRC:1;
                vuint32_t LPB:1;
                vuint32_t TWRNMSK:1;
                vuint32_t RWRNMSK:1;
                  vuint32_t:2;
                vuint32_t SMP:1;
                vuint32_t BOFFREC:1;
                vuint32_t TSYN:1;
                vuint32_t LBUF:1;
                vuint32_t LOM:1;
                vuint32_t PROPSEG:3;
            } B;
        } CR;                   /* Control Register */

        union {
            vuint32_t R;
        } TIMER;                /* Free Running Timer */

        uint32_t FLEXCAN_reserved1;

        union {
            vuint32_t R;
            struct {
                vuint32_t MI:32;
            } B;
        } RXGMASK;              /* RX Global Mask */

        union {
            vuint32_t R;
            struct {
                vuint32_t MI:32;
            } B;
        } RX14MASK;             /* RX 14 Mask */

        union {
            vuint32_t R;
            struct {
                vuint32_t MI:32;
            } B;
        } RX15MASK;             /* RX 15 Mask */

        union {
            vuint32_t R;
            struct {
                vuint32_t:16;
                vuint32_t RXECNT:8;
                vuint32_t TXECNT:8;
            } B;
        } ECR;                  /* Error Counter Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:14;
                vuint32_t TWRNINT:1;
                vuint32_t RWRNINT:1;
                vuint32_t BIT1ERR:1;
                vuint32_t BIT0ERR:1;
                vuint32_t ACKERR:1;
                vuint32_t CRCERR:1;
                vuint32_t FRMERR:1;
                vuint32_t STFERR:1;
                vuint32_t TXWRN:1;
                vuint32_t RXWRN:1;
                vuint32_t IDLE:1;
                vuint32_t TXRX:1;
                vuint32_t FLTCONF:2;
                  vuint32_t:1;
                vuint32_t BOFFINT:1;
                vuint32_t ERRINT:1;
                vuint32_t WAKINT:1;
            } B;
        } ESR;                  /* Error and Status Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t BUF63M:1;
                vuint32_t BUF62M:1;
                vuint32_t BUF61M:1;
                vuint32_t BUF60M:1;
                vuint32_t BUF59M:1;
                vuint32_t BUF58M:1;
                vuint32_t BUF57M:1;
                vuint32_t BUF56M:1;
                vuint32_t BUF55M:1;
                vuint32_t BUF54M:1;
                vuint32_t BUF53M:1;
                vuint32_t BUF52M:1;
                vuint32_t BUF51M:1;
                vuint32_t BUF50M:1;
                vuint32_t BUF49M:1;
                vuint32_t BUF48M:1;
                vuint32_t BUF47M:1;
                vuint32_t BUF46M:1;
                vuint32_t BUF45M:1;
                vuint32_t BUF44M:1;
                vuint32_t BUF43M:1;
                vuint32_t BUF42M:1;
                vuint32_t BUF41M:1;
                vuint32_t BUF40M:1;
                vuint32_t BUF39M:1;
                vuint32_t BUF38M:1;
                vuint32_t BUF37M:1;
                vuint32_t BUF36M:1;
                vuint32_t BUF35M:1;
                vuint32_t BUF34M:1;
                vuint32_t BUF33M:1;
                vuint32_t BUF32M:1;
            } B;
        } IMRH;                 /* Interruput Masks Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t BUF31M:1;
                vuint32_t BUF30M:1;
                vuint32_t BUF29M:1;
                vuint32_t BUF28M:1;
                vuint32_t BUF27M:1;
                vuint32_t BUF26M:1;
                vuint32_t BUF25M:1;
                vuint32_t BUF24M:1;
                vuint32_t BUF23M:1;
                vuint32_t BUF22M:1;
                vuint32_t BUF21M:1;
                vuint32_t BUF20M:1;
                vuint32_t BUF19M:1;
                vuint32_t BUF18M:1;
                vuint32_t BUF17M:1;
                vuint32_t BUF16M:1;
                vuint32_t BUF15M:1;
                vuint32_t BUF14M:1;
                vuint32_t BUF13M:1;
                vuint32_t BUF12M:1;
                vuint32_t BUF11M:1;
                vuint32_t BUF10M:1;
                vuint32_t BUF09M:1;
                vuint32_t BUF08M:1;
                vuint32_t BUF07M:1;
                vuint32_t BUF06M:1;
                vuint32_t BUF05M:1;
                vuint32_t BUF04M:1;
                vuint32_t BUF03M:1;
                vuint32_t BUF02M:1;
                vuint32_t BUF01M:1;
                vuint32_t BUF00M:1;
            } B;
        } IMRL;                 /* Interruput Masks Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t BUF63I:1;
                vuint32_t BUF62I:1;
                vuint32_t BUF61I:1;
                vuint32_t BUF60I:1;
                vuint32_t BUF59I:1;
                vuint32_t BUF58I:1;
                vuint32_t BUF57I:1;
                vuint32_t BUF56I:1;
                vuint32_t BUF55I:1;
                vuint32_t BUF54I:1;
                vuint32_t BUF53I:1;
                vuint32_t BUF52I:1;
                vuint32_t BUF51I:1;
                vuint32_t BUF50I:1;
                vuint32_t BUF49I:1;
                vuint32_t BUF48I:1;
                vuint32_t BUF47I:1;
                vuint32_t BUF46I:1;
                vuint32_t BUF45I:1;
                vuint32_t BUF44I:1;
                vuint32_t BUF43I:1;
                vuint32_t BUF42I:1;
                vuint32_t BUF41I:1;
                vuint32_t BUF40I:1;
                vuint32_t BUF39I:1;
                vuint32_t BUF38I:1;
                vuint32_t BUF37I:1;
                vuint32_t BUF36I:1;
                vuint32_t BUF35I:1;
                vuint32_t BUF34I:1;
                vuint32_t BUF33I:1;
                vuint32_t BUF32I:1;
            } B;
        } IFRH;                 /* Interruput Flag Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t BUF31I:1;
                vuint32_t BUF30I:1;
                vuint32_t BUF29I:1;
                vuint32_t BUF28I:1;
                vuint32_t BUF27I:1;
                vuint32_t BUF26I:1;
                vuint32_t BUF25I:1;
                vuint32_t BUF24I:1;
                vuint32_t BUF23I:1;
                vuint32_t BUF22I:1;
                vuint32_t BUF21I:1;
                vuint32_t BUF20I:1;
                vuint32_t BUF19I:1;
                vuint32_t BUF18I:1;
                vuint32_t BUF17I:1;
                vuint32_t BUF16I:1;
                vuint32_t BUF15I:1;
                vuint32_t BUF14I:1;
                vuint32_t BUF13I:1;
                vuint32_t BUF12I:1;
                vuint32_t BUF11I:1;
                vuint32_t BUF10I:1;
                vuint32_t BUF09I:1;
                vuint32_t BUF08I:1;
                vuint32_t BUF07I:1;
                vuint32_t BUF06I:1;
                vuint32_t BUF05I:1;
                vuint32_t BUF04I:1;
                vuint32_t BUF03I:1;
                vuint32_t BUF02I:1;
                vuint32_t BUF01I:1;
                vuint32_t BUF00I:1;
            } B;
        } IFRL;                 /* Interruput Flag Register */

        uint32_t FLEXCAN_reserved2[19]; /* {0x0080-0x0034}/0x4 = 0x13 */

/****************************************************************************/
/* Use either Standard Buffer Structure OR RX FIFO and Buffer Structure     */
/****************************************************************************/
        /* Standard Buffer Structure */
        struct FLEXCAN_BUF_t BUF[64];

        /* RX FIFO and Buffer Structure */
        /*struct FLEXCAN_RXFIFO_t RXFIFO; */
        /*struct FLEXCAN_BUF_t BUF[56];   */
/****************************************************************************/

        uint32_t FLEXCAN_reserved3[256];        /* {0x0880-0x0480}/0x4 = 0x100 */

        union {
            vuint32_t R;
            struct {
                vuint32_t MI:32;
            } B;
        } RXIMR[64];            /* RX Individual Mask Registers */

    };                          /* end of FLEXCAN_tag */
/****************************************************************************/
/*                          MODULE : DMAMUX                                 */
/****************************************************************************/
    struct DMAMUX_tag {
        union {
            vuint8_t R;
            struct {
                vuint8_t ENBL:1;
                vuint8_t TRIG:1;
                vuint8_t SOURCE:6;
            } B;
        } CHCONFIG[16];         /* DMA Channel Configuration Register */

    };                          /* end of DMAMUX_tag */
/****************************************************************************/
/*                          MODULE : LCD                                   */
/****************************************************************************/
    struct LCD_tag {

        union {
            vuint32_t R;
            struct {
                vuint32_t LCDEN:1;
                vuint32_t LCDRST:1;
                vuint32_t LCDRCS:1;
                vuint32_t DUTY:3;
                vuint32_t BIAS:1;
                vuint32_t VLCDS:1;
                vuint32_t PWR:2;
                vuint32_t BSTEN:1;
                vuint32_t BSTSEL:1;
                vuint32_t BSTAO:1;
                  vuint32_t:1;
                vuint32_t LCDINT:1;
                vuint32_t EOFF:1;
                vuint32_t NOF:8;
                  vuint32_t:2;
                vuint32_t LCDBPA:1;
                  vuint32_t:2;
                vuint32_t LCDBPS:3;
            } B;
        } CR;                   /* LCD Control Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t:4;
                vuint32_t LCLK:4;
                  vuint32_t:24;
            } B;
        } PCR;                  /* LCD Prescaler Control Register */

        union {
            vuint32_t R;
            struct {
                vuint32_t CCEN:1;
                  vuint32_t:4;
                vuint32_t LCC:11;
                  vuint32_t:16;
            } B;
        } CCR;                  /* LCD Contrast Control Register */

        int32_t LCD_reserved1;  /* (0x10 - 0x0C)/4 = 0x01 */

        union {
            vuint32_t R;
            struct {
                vuint32_t FP31EN:1;
                vuint32_t FP30EN:1;
                vuint32_t FP29EN:1;
                vuint32_t FP28EN:1;
                vuint32_t FP27EN:1;
                vuint32_t FP26EN:1;
                vuint32_t FP25EN:1;
                vuint32_t FP24EN:1;
                vuint32_t FP23EN:1;
                vuint32_t FP22EN:1;
                vuint32_t FP21EN:1;
                vuint32_t FP20EN:1;
                vuint32_t FP19EN:1;
                vuint32_t FP18EN:1;
                vuint32_t FP17EN:1;
                vuint32_t FP16EN:1;
                vuint32_t FP15EN:1;
                vuint32_t FP14EN:1;
                vuint32_t FP13EN:1;
                vuint32_t FP12EN:1;
                vuint32_t FP11EN:1;
                vuint32_t FP10EN:1;
                vuint32_t FP9EN:1;
                vuint32_t FP8EN:1;
                vuint32_t FP7EN:1;
                vuint32_t FP6EN:1;
                vuint32_t FP5EN:1;
                vuint32_t FP4EN:1;
                vuint32_t FP3EN:1;
                vuint32_t FP2EN:1;
                vuint32_t FP1EN:1;
                vuint32_t FP0EN:1;
            } B;
        } FPENR0;               /* LCD Frontplane Enable Register 0 */

        union {
            vuint32_t R;
            struct {
                vuint32_t FP63EN:1;
                vuint32_t FP62EN:1;
                vuint32_t FP61EN:1;
                vuint32_t FP60EN:1;
                vuint32_t FP59EN:1;
                vuint32_t FP58EN:1;
                vuint32_t FP57EN:1;
                vuint32_t FP56EN:1;
                vuint32_t FP55EN:1;
                vuint32_t FP54EN:1;
                vuint32_t FP53EN:1;
                vuint32_t FP52EN:1;
                vuint32_t FP51EN:1;
                vuint32_t FP50EN:1;
                vuint32_t FP49EN:1;
                vuint32_t FP48EN:1;
                vuint32_t FP47EN:1;
                vuint32_t FP46EN:1;
                vuint32_t FP45EN:1;
                vuint32_t FP44EN:1;
                vuint32_t FP43EN:1;
                vuint32_t FP42EN:1;
                vuint32_t FP41EN:1;
                vuint32_t FP40EN:1;
                vuint32_t FP39EN:1;
                vuint32_t FP38EN:1;
                vuint32_t FP37EN:1;
                vuint32_t FP36EN:1;
                vuint32_t FP35EN:1;
                vuint32_t FP34EN:1;
                vuint32_t FP33EN:1;
                vuint32_t FP32EN:1;
            } B;
        } FPENR1;               /* LCD Frontplane Enable Register 1 */

        int32_t LCD_reserved2[2];       /* (0x20 - 0x18)/4 = 0x02 */

        union {
            vuint32_t R;
            struct
            {
            	vuint8_t RByte_3;
            	vuint8_t RByte_2;
            	vuint8_t RByte_1;
            	vuint8_t RByte_0;
            }RByte;
        } RAM[16];              /* LCD RAM Register */

    };                          /* end of LCD_tag */
/****************************************************************************/
/*                     MODULE : DFLASH                                       */
/****************************************************************************/
    struct DFLASH_tag {
        union {                 /* Module Configuration Register */
            vuint32_t R;
            struct {
                vuint32_t EDC:1;
                  vuint32_t:4;
                vuint32_t SIZE:3;
                  vuint32_t:1;
                vuint32_t LAS:3;
                  vuint32_t:3;
                vuint32_t MAS:1;
                vuint32_t EER:1;
                vuint32_t RWE:1;
                  vuint32_t:1;
                  vuint32_t:1;
                vuint32_t PEAS:1;
                vuint32_t DONE:1;
                vuint32_t PEG:1;
                  vuint32_t:4;
                vuint32_t PGM:1;
                vuint32_t PSUS:1;
                vuint32_t ERS:1;
                vuint32_t ESUS:1;
                vuint32_t EHV:1;
            } B;
        } MCR;

        union {                 /* LML Register */
            vuint32_t R;
            struct {
                vuint32_t LME:1;
                  vuint32_t:10;
                vuint32_t TSLK:1;
                  vuint32_t:2;
                vuint32_t MLK:2;
                vuint32_t LLK:16;
            } B;
        } LML;

        union {                 /* HBL Register */
            vuint32_t R;
            struct {
                vuint32_t HBE:1;
                  vuint32_t:23;
                vuint32_t HBLOCK:8;
            } B;
        } HBL;

        union {                 /* SLML Register */
            vuint32_t R;
            struct {
                vuint32_t SLE:1;
                  vuint32_t:10;
                vuint32_t STSLK:1;
                  vuint32_t:2;
                vuint32_t SMK:2;
                vuint32_t SLK:16;
            } B;
        } SLL;

        union {                 /* LMS Register */
            vuint32_t R;
            struct {
                vuint32_t:14;
                vuint32_t MSL:2;
                vuint32_t LSL:16;
            } B;
        } LMS;

        union {                 /* High Address Space Block Select Register */
            vuint32_t R;
            struct {
                vuint32_t:26;
                vuint32_t HSL:6;
            } B;
        } HBS;

        union {                 /* Address Register */
            vuint32_t R;
            struct {
                vuint32_t:10;
                vuint32_t ADD:19;
                  vuint32_t:3;
            } B;
        } ADR;

        int32_t Dflash_reserved0[8];    /* {0x003C-0x001C}/0x4 = 0x08 */

        union {                 /* User Test Register 0 */
            vuint32_t R;
            struct {
                vuint32_t UTE:1;
                  vuint32_t:7;
                vuint32_t DSI:8;
                  vuint32_t:10;
                vuint32_t MRE:1;
                vuint32_t MRV:1;
                vuint32_t EIE:1;
                vuint32_t AIS:1;
                vuint32_t AIE:1;
                vuint32_t AID:1;
            } B;
        } UT0;

        union {                 /* User Test Register 1 */
            vuint32_t R;
            struct {
                vuint32_t DAI:32;
            } B;
        } UT1;

        union {                 /* User Test Register 2 */
            vuint32_t R;
            struct {
                vuint32_t DAI:32;
            } B;
        } UT2;

        union {                 /* User Multiple Input Signature Register 0-4 */
            vuint32_t R;
            struct {
                vuint32_t MS:32;
            } B;
        } UMISR[5];

    };                          /* end of Dflash_tag */
/****************************************************************************/
/*                     MODULE : CFLASH                                       */
/****************************************************************************/
    struct CFLASH_tag {
        union {                 /* Module Configuration Register */
            vuint32_t R;
            struct {
                vuint32_t EDC:1;
                  vuint32_t:4;
                vuint32_t SIZE:3;
                  vuint32_t:1;
                vuint32_t LAS:3;
                  vuint32_t:3;
                vuint32_t MAS:1;
                vuint32_t EER:1;
                vuint32_t RWE:1;
                  vuint32_t:1;
                  vuint32_t:1;
                vuint32_t PEAS:1;
                vuint32_t DONE:1;
                vuint32_t PEG:1;
                  vuint32_t:4;
                vuint32_t PGM:1;
                vuint32_t PSUS:1;
                vuint32_t ERS:1;
                vuint32_t ESUS:1;
                vuint32_t EHV:1;
            } B;
        } MCR;

        union {                 /* LML Register */
            vuint32_t R;
            struct {
                vuint32_t LME:1;
                  vuint32_t:10;
                vuint32_t TSLK:1;
                  vuint32_t:2;
                vuint32_t MLK:2;
                vuint32_t LLK:16;
            } B;
        } LML;

        union {                 /* HBL Register */
            vuint32_t R;
            struct {
                vuint32_t HBE:1;
                  vuint32_t:23;
                vuint32_t HBLOCK:8;
            } B;
        } HBL;

        union {                 /* SLML Register */
            vuint32_t R;
            struct {
                vuint32_t SLE:1;
                  vuint32_t:10;
                vuint32_t STSLK:1;
                  vuint32_t:2;
                vuint32_t SMK:2;
                vuint32_t SLK:16;
            } B;
        } SLL;

        union {                 /* LMS Register */
            vuint32_t R;
            struct {
                vuint32_t:14;
                vuint32_t MSL:2;
                vuint32_t LSL:16;
            } B;
        } LMS;

        union {                 /* High Address Space Block Select Register */
            vuint32_t R;
            struct {
                vuint32_t:26;
                vuint32_t HSL:6;
            } B;
        } HBS;

        union {                 /* Address Register */
            vuint32_t R;
            struct {
                vuint32_t:10;
                vuint32_t ADD:19;
                  vuint32_t:3;
            } B;
        } ADR;

        union {                 /* CFLASH Configuration Register 0 */
            vuint32_t R;
            struct {
                vuint32_t BK0_APC:5;
                vuint32_t BK0_WWSC:5;
                vuint32_t BK0_RWSC:5;
                vuint32_t BK0_RWWC2:1;
                vuint32_t BK0_RWWC1:1;
                vuint32_t B0_P1_BCFG:2;
                vuint32_t B0_P1_DPFE:1;
                vuint32_t B0_P1_IPFE:1;
                vuint32_t B0_P1_PFLM:2;
                vuint32_t B0_P1_BFE:1;
                vuint32_t BK0_RWWC0:1;
                vuint32_t B0_P0_BCFG:2;
                vuint32_t B0_P0_DPFE:1;
                vuint32_t B0_P0_IPFE:1;
                vuint32_t B0_P0_PFLM:2;
                vuint32_t B0_P0_BFE:1;
            } B;
        } PFCR0;

        union {                 /* CFLASH Configuration Register 1 */
            vuint32_t R;
            struct {
                vuint32_t BK1_APC:5;
                vuint32_t BK1_WWSC:5;
                vuint32_t BK1_RWSC:5;
                vuint32_t BK1_RWWC2:1;
                vuint32_t BK1_RWWC1:1;
                  vuint32_t:6;
                vuint32_t B0_P1_BFE:1;
                vuint32_t BK1_RWWC0:1;
                  vuint32_t:6;
                vuint32_t B1_P0_BFE:1;
            } B;
        } PFCR1;

        union {                 /* cflash Access Protection Register */
            vuint32_t R;
            struct {
                vuint32_t:6;
                vuint32_t ARBM:2;
                vuint32_t M7PFD:1;
                vuint32_t M6PFD:1;
                vuint32_t M5PFD:1;
                vuint32_t M4PFD:1;
                vuint32_t M3PFD:1;
                vuint32_t M2PFD:1;
                vuint32_t M1PFD:1;
                vuint32_t M0PFD:1;
                vuint32_t M7AP:2;
                vuint32_t M6AP:2;
                vuint32_t M5AP:2;
                vuint32_t M4AP:2;
                vuint32_t M3AP:2;
                vuint32_t M2AP:2;
                vuint32_t M1AP:2;
                vuint32_t M0AP:2;
            } B;
        } FAPR;

        int32_t CFLASH_reserved0[5];    /* {0x003C-0x0028}/0x4 = 0x05 */

        union {                 /* User Test Register 0 */
            vuint32_t R;
            struct {
                vuint32_t UTE:1;
                  vuint32_t:7;
                vuint32_t DSI:8;
                  vuint32_t:10;
                vuint32_t MRE:1;
                vuint32_t MRV:1;
                vuint32_t EIE:1;
                vuint32_t AIS:1;
                vuint32_t AIE:1;
                vuint32_t AID:1;
            } B;
        } UT0;

        union {                 /* User Test Register 1 */
            vuint32_t R;
            struct {
                vuint32_t DAI:32;
            } B;
        } UT1;

        union {                 /* User Test Register 2 */
            vuint32_t R;
            struct {
                vuint32_t DAI:32;
            } B;
        } UT2;

        union {                 /* User Multiple Input Signature Register 0-4 */
            vuint32_t R;
            struct {
                vuint32_t MS:32;
            } B;
        } UMISR[5];

    };                          /* end of CFLASH_tag */
/****************************************************************** 
| defines and macros (scope: module-local) 
|-----------------------------------------------------------------*/
/* Define instances of modules */
#define CFLASH0   (*(volatile struct CFLASH_tag *)    0xC3F88000UL)
#define DFLASH    (*(volatile struct DFLASH_tag *)    0xC3F8C000UL)
#define SIU       (*(volatile struct SIU_tag *)       0xC3F90000UL)
#define WKUP      (*(volatile struct WKUP_tag *)      0xC3F94000UL)
#define EMIOS_0   (*(volatile struct EMIOS_tag *)     0xC3FA0000UL)
#define EMIOS_1   (*(volatile struct EMIOS_tag *)     0xC3FA4000UL)
#define CFLASH1   (*(volatile struct CFLASH_tag *)    0xC3FB0000UL)
#define SSCM      (*(volatile struct SSCM_tag *)      0xC3FD8000UL)
#define ME        (*(volatile struct ME_tag *)        0xC3FDC000UL)
#define CGM       (*(volatile struct CGM_tag *)       0xC3FE0000UL)
#define RGM       (*(volatile struct RGM_tag *)       0xC3FE4000UL)
#define PCU       (*(volatile struct PCU_tag *)       0xC3FE8000UL)
#define RTC       (*(volatile struct RTC_tag *)       0xC3FEC000UL)
#define PIT       (*(volatile struct PIT_tag *)       0xC3FF0000UL)
#define ADC_0     (*(volatile struct ADC_tag *)       0xFFE00000UL)
#define I2C_0     (*(volatile struct I2C_tag *)       0xFFE30000UL)
#define I2C_1     (*(volatile struct I2C_tag *)       0xFFE34000UL)
#define I2C_2     (*(volatile struct I2C_tag *)       0xFFE38000UL)
#define I2C_3     (*(volatile struct I2C_tag *)       0xFFE3C000UL)
#define LINFLEX_0 (*(volatile struct LINFLEX_tag *)   0xFFE40000UL)
#define LINFLEX_1 (*(volatile struct LINFLEX_tag *)   0xFFE44000UL)
#define SMC       (*(volatile struct SMC_tag *)       0xFFE60000UL)
#define SSD_0     (*(volatile struct SSD_tag *)       0xFFE61000UL)
#define SSD_1     (*(volatile struct SSD_tag *)       0xFFE61800UL)
#define SSD_2     (*(volatile struct SSD_tag *)       0xFFE62000UL)
#define SSD_3     (*(volatile struct SSD_tag *)       0xFFE62800UL)
#define SSD_4     (*(volatile struct SSD_tag *)       0xFFE63000UL)
#define SSD_5     (*(volatile struct SSD_tag *)       0xFFE63800UL)
#define CANSP     (*(volatile struct CANSP_tag *)     0xFFE70000UL)
#define LCD       (*(volatile struct LCD_tag *)       0xFFE74000UL)
#define SGL       (*(volatile struct SGL_tag *)       0xFFE78000UL)
#define DCU       (*(volatile struct DCU_tag *)       0xFFE7C000UL)
#define MPU       (*(volatile struct MPU_tag *)       0xFFF10000UL)
#define SWT       (*(volatile struct SWT_tag *)       0xFFF38000UL)
#define STM       (*(volatile struct STM_tag *)       0xFFF3C000UL)
#define ECSM      (*(volatile struct ECSM_tag *)      0xFFF40000UL)
#define EDMA      (*(volatile struct EDMA_tag *)      0xFFF44000UL)
#define INTC      (*(volatile struct INTC_tag *)      0xFFF48000UL)
#define DSPI_0    (*(volatile struct DSPI_tag *)      0xFFF90000UL)
#define DSPI_1    (*(volatile struct DSPI_tag *)      0xFFF94000UL)
#define QUADSPI_0 (*(volatile struct QUADSPI_tag *)   0xFFFA8000UL)
#define CAN_0     (*(volatile struct FLEXCAN_tag *)   0xFFFC0000UL)
#define CAN_1     (*(volatile struct FLEXCAN_tag *)   0xFFFC4000UL)
#define DMAMUX    (*(volatile struct DMAMUX_tag *)    0xFFFDC000UL)

#ifdef __MWERKS__
#pragma pop
#endif

#ifdef  __cplusplus
}
#endif
#endif                          /* ifdef _JDP_H */
/* End of file */
