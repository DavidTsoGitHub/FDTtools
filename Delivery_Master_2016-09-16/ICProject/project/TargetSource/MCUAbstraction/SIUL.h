/*                                                               */
/* FILE        : SIUL.h                                          */
/* DATE        : 2015-04-28                                      */
/* AUTHOR      : Cao ChenGuang                                   */
/* DESCRIPTION : header file of SIUL module                      */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao ChenGuang on 2015-04-28    */
/*                                                               */
/*****************************************************************/
#ifndef SIUL_H
#define SIUL_H

#define PA0   0u
#define PA0_GPIO_0      0u
#define PA0_DCU_R0      1u
#define PA0_eMIOSA_22   2u
#define PA0_SOUND       3u

#define PA1   1u
#define PA1_GPIO_1      0u
#define PA1_DCU_R1      1u
#define PA1_eMIOSA_21   2u

#define PA2   2u
#define PA2_GPIO_2      0u
#define PA2_DCU_R2      1u
#define PA2_eMIOSA_20   2u

#define PA3   3u
#define PA3_GPIO_3      0u
#define PA3_DCU_R3      1u
#define PA3_eMIOSA_19   2u

#define PA4   4u
#define PA4_GPIO_4      0u
#define PA4_DCU_R4      1u
#define PA4_eMIOSA_18   2u

#define PA5   5u
#define PA5_GPIO_5      0u
#define PA5_DCU_R5      1u
#define PA5_eMIOSA_17   2u

#define PA6   6u
#define PA6_GPIO_6      0u
#define PA6_DCU_R6      1u
#define PA6_eMIOSA_15   2u

#define PA7   7u
#define PA6_GPIO_7      0u
#define PA6_DCU_R7      1u
#define PA6_eMIOSA_16   2u

#define PA8   8u
#define PA8_GPIO_8      0u
#define PA8_DCU_G0      1u
#define PA8_eMIOSB_23   2u
#define PA8_SCL_2       3u

#define PA9   9u
#define PA9_GPIO_9      0u
#define PA9_DCU_G1      1u
#define PA9_eMIOSB_18   2u
#define PA9_SDA_2       3u

#define PA10 10u
#define PA10_GPIO_10    0u
#define PA10_DCU_G2     1u
#define PA10_eMIOSB_20  2u

#define PA11 11u
#define PA11_GPIO_11    0u
#define PA11_DCU_G3     1u
#define PA11_eMIOSA_13  2u

#define PA12 12u
#define PA12_GPIO_12    0u
#define PA12_DCU_G4     1u
#define PA12_eMIOSA_12  2u

#define PA13 13u
#define PA13_GPIO_13    0u
#define PA13_DCU_G5     1u
#define PA13_eMIOSA_11  2u

#define PA14 14u
#define PA14_GPIO_14    0u
#define PA14_DCU_G6     1u
#define PA14_eMIOSA_10  2u

#define PA15 15u
#define PA15_GPIO_15    0u
#define PA15_DCU_G7     1u
#define PA15_eMIOSA_9   2u

#define PB0  16u
#define PB0_GPIO_16     0u
#define PB0_CANTX_0     1u
#define PB0_PDI1        2u

#define PB1  17u
#define PB1_GPIO_17     0u
#define PB1_CANRX_0     1u
#define PB1_PDI0        2u

#define PB2  18u
#define PB2_GPIO_18     0u
#define PB2_TXD_0       1u

#define PB3  19u
#define PB3_GPIO_19     0u
#define PB3_RXD_0       1u

#define PB4  20u
#define PB4_GPIO_20     0u
#define PB4_SCK_1       1u
#define PB4_MA0         2u

#define PB5  21u
#define PB5_GPIO_21     0u
#define PB5_SOUT_1      1u
#define PB5_MA1         2u
#define PB5_FABM        3u

#define PB6  22u
#define PB6_GPIO_22     0u
#define PB6_SIN_1       1u
#define PB6_MA2         2u
#define PB6_ABS_0       3u

#define PB7  23u
#define PB7_GPIO_23     0u
#define PB7_SIN_0       1u
#define eMIOSB_22       2u

#define PB8  24u
#define PB8_GPIO_24     0u
#define PB8_SOUT_0      1u
#define PB8_eMIOSB_21   2u

#define PB9  25u
#define PB9_GPIO_25     0u
#define PB9_SCK_0       1u
#define PB9_eMIOSB_20   2u

#define PB10 26u
#define PB10_GPIO_26    0u
#define PB10_CANRX_1    1u
#define PB10_PDI2       2u
#define PB10_eMIOSA_23  3u

#define PB11 27u
#define PB11_GPIO_27    0u
#define PB11_CANTX_1    1u
#define PB11_PDI3       2u
#define PB11_eMIOSA_16  3u

#define PB12 28u
#define PB12_GPIO_28    0u
#define PB12_RXD_1      1u
#define PB12_eMIOSB_19  2u
#define PB12_PCS2_0     3u

#define PB13 29u
#define PB13_GPIO_29    0u
#define PB13_TXD_1      1u
#define PB13_eMIOSB_18  2u
#define PB13_PCS1_0     3u

#define PC0  30u
#define PC0_GPIO_30     0u

#define PC1  31u
#define PC1_GPIO_31     0u

#define PC2  32u
#define PC2_GPIO_32     0u

#define PC3  33u
#define PC3_GPIO_33     0u

#define PC4  34u
#define PC4_GPIO_34     0u

#define PC5  35u
#define PC5_GPIO_35     0u

#define PC6  36u
#define PC6_GPIO_36     0u

#define PC7  37u
#define PC7_GPIO_37     0u

#define PC8  38u
#define PC8_GPIO_38     0u

#define PC9  39u
#define PC9_GPIO_39     0u

#define PC10 40u
#define PC10_GPIO_40    0u

#define PC11 41u
#define PC11_GPIO_41    0u

#define PC12 42u
#define PC12_GPIO_42    0u
#define PC12_MA1        2u
#define PC12_PCS1_1     3u

#define PC13 43u
#define PC13_GPIO_43    0u
#define PC13_MA2        2u
#define PC13_PCS0_1     3u

#define PC14 44u
#define PC14_GPIO_44    0u

#define PC15 45u
#define PC15_GPIO_45    0u

#define PD0  46u
#define PD0_GPIO_46     0u
#define PD0_M0C0M       1u
#define PD0_SSD0_0      2u
#define PD0_eMIOSB_23   3u

#define PD1  47u
#define PD1_GPIO_47     0u
#define PD1_M0C0P       1u
#define PD1_SSD0_1      2u
#define PD1_eMIOSB_22   3u

#define PD2  48u
#define PD2_GPIO_48     0u
#define PD2_M0C1M       1u
#define PD2_SSD0_2      2u
#define PD2_eMIOSB_21   3u

#define PD3  49u
#define PD3_GPIO_49     0u
#define PD3_M0C1P       1u
#define PD3_SSD0_3      2u
#define PD3_eMIOSB_20   3u

#define PD4  50u
#define PD4_GPIO_50     0u
#define PD4_M1C0M       1u
#define PD4_SSD1_0      2u
#define PD4_eMIOSB_19   3u

#define PD5  51u
#define PD5_GPIO_51     0u
#define PD5_M1C0P       1u
#define PD5_SSD1_1      2u
#define PD5_eMIOSB_18   3u

#define PD6  52u
#define PD6_GPIO_52     0u
#define PD6_M1C1M       1u
#define PD6_SSD1_2      2u
#define PD6_eMIOSB_17   3u

#define PD7  53u
#define PD7_GPIO_53     0u
#define PD7_M1C1P       1u
#define PD7_SSD1_3      2u
#define PD7_eMIOSB_16   3u

#define PD8  54u
#define PD8_GPIO_54     0u
#define PD8_M2C0M       1u
#define PD8_SSD2_0      2u

#define PD9  55u
#define PD9_GPIO_55     0u
#define PD9_M2C0P       1u
#define PD9_SSD2_1      2u

#define PD10 56u
#define PD10_GPIO_56    0u
#define PD10_M2C1M      1u
#define PD10_SSD2_2     2u

#define PD11 57u
#define PD11_GPIO_57    0u
#define PD11_M2C1P      1u
#define PD11_SSD2_3     2u

#define PD12 58u
#define PD12_GPIO_58    0u
#define PD12_M3C0M      1u
#define PD12_SSD3_0     2u

#define PD13 59u
#define PD13_GPIO_59    0u
#define PD13_M3C0P      1u
#define PD13_SSD3_1     2u

#define PD14 60u
#define PD14_GPIO_60    0u
#define PD14_M3C1M      1u
#define PD14_SSD3_2     2u

#define PD15 61u
#define PD15_GPIO_61    0u
#define PD15_M3C1P      1u
#define PD15_SSD3_3     2u

#define PE0 62u
#define PE0_GPIO_62     0u
#define PE0_M4C0M       1u
#define PE0_SSD4_0      2u
#define PE0_eMIOSA_15   3u

#define PE1 63u
#define PE1_GPIO_63     0u
#define PE1_M4C0P       1u
#define PE1_SSD4_1      2u
#define PE1_eMIOSA_14   3u

#define PE2 64u
#define PE2_GPIO_64     0u
#define PE2_M4C1M       1u
#define PE2_SSD4_2      2u
#define PE2_eMIOSA_13   3u

#define PE3 65u
#define PE3_GPIO_65     0u
#define PE3_M4C1P       1u
#define PE3_SSD4_3      2u
#define PE3_eMIOSA_12   3u

#define PE4 66u
#define PE4_GPIO_66     0u
#define PE4_M5C0M       1u
#define PE4_SSD5_0      2u
#define PE4_eMIOSA_11   3u

#define PE5 67u
#define PE5_GPIO_67     0u
#define PE5_M5C0P       1u
#define PE5_SSD5_1      2u
#define PE5_eMIOSA_10   3u

#define PE6 68u
#define PE6_GPIO_68     0u
#define PE6_M5C1M       1u
#define PE6_SSD5_2      2u
#define PE6_eMIOSA_9    3u

#define PE7 69u
#define PE7_GPIO_69     0u
#define PE7_M5C1P       1u
#define PE7_SSD5_3      2u
#define PE7_eMIOSA_8    3u

#define PF0  70u
#define PF0_GPIO_70     0u
#define PF0_eMIOSA_13   1u
#define PF0_PDI4        2u
#define PF0_eMIOSA_22   3u

#define PF1  71u
#define PF1_GPIO_71     0u
#define PF1_eMIOSA_12   1u
#define PF1_PDI5        2u
#define PF1_eMIOSA_21   3u

#define PF2  72u
#define PF2_GPIO_72     0u
#define PF2_NMI         1u

#define PF3  73u
#define PF3_GPIO_73     0u
#define PF3_eMIOSA_11   1u
#define PF3_PDI6        2u

#define PF4  74u
#define PF4_GPIO_74     0u
#define PF4_eMIOSA_10   1u
#define PF4_PDI7        2u

#define PF5  75u
#define PF5_GPIO_75     0u
#define PF5_eMIOSA_9    1u
#define PF5_DCU_TAG     2u

#define PF6  76u
#define PF6_GPIO_76     0u
#define PF6_SDA_0       1u

#define PF7  77u
#define PF7_GPIO_77     0u
#define PF7_SCL_0       1u
#define PF7_PCS2_1      2u

#define PF8  78u
#define PF8_GPIO_78     0u
#define PF8_SDA_1       1u
#define PF8_PCS1_1      2u
#define PF8_RXD_1       3u

#define PF9  79u
#define PF9_GPIO_79     0u
#define PF9_SCL_1       1u
#define PF9_PCS0_1      2u
#define PF9_TXD_1       3u

#define PF10 80u
#define PF10_GPIO_80    0u
#define PF10_eMIOSA_16  1u
#define PF10_PCS0_2     2u

#define PF11 81u
#define PF11_GPIO_81    0u
#define PF11_eMIOSB_23  1u
#define PF11_IO2_PCS1_2 2u

#define PF12 82u
#define PF12_GPIO_82    0u
#define PF12_eMIOSB_16  1u
#define PF12_IO3_PCS2_2 2u

#define PF13 83u
#define PF13_GPIO_83    0u
#define PF13_IO0_SIN_2  1u
#define PF13_CANRX_1    2u

#define PF14 84u
#define PF14_GPIO_84    0u
#define PF14_IO1_SOUT_2 1u
#define PF14_CANTX_1    2u

#define PF15 85u
#define PF15_GPIO_85    0u
#define PF15_SCK_2      1u

#define PG0  86u
#define PG0_GPIO_86     0u
#define PG0_DCU_B0      1u
#define PG0_SCL_3       2u
#define PG0_SOUND       3u

#define PG1  87u
#define PG1_GPIO_87     0u
#define PG1_DCU_B1      1u
#define PG1_SDA_3       2u

#define PG2  88u
#define PG2_GPIO_88     0u
#define PG2DCU_B2       1u
#define PG2_eMIOSB_19   2u

#define PG3  89u
#define PG3_GPIO_89     0u
#define PG3_DCU_B3      1u
#define PG3_eMIOSB_21   2u

#define PG4  90u
#define PG4_GPIO_90     0u
#define PG4_DCU_B4      1u
#define PG4_eMIOSB_17   2u

#define PG5  91u
#define PG5_GPIO_91     0u
#define PG5_DCU_B5      1u
#define PG5_eMIOSA_8    2u

#define PG6  92u
#define PG6_GPIO_92     0u
#define PG6_DCU_B6      1u

#define PG7  93u
#define PG7_GPIO_93     0u
#define PG7_DCU_B7      1u

#define PG8  94u
#define PG8_GPIO_94     0u
#define PG8_DCU_VSYNC   1u

#define PG9  95u
#define PG9_GPIO_95     0u
#define PG9_DCU_HSYNC   1u

#define PG10 96u
#define PG10_GPIO_96    0u
#define PG10_DCU_DE     1u

#define PG11 97u
#define PG11_GPIO_97    0u
#define PG11_DCU_PCLK   1u

#define PG12 98u
#define PG12_GPIO_98    0u
#define PG12_eMIOSA_23  1u
#define PG12_SOUND      2u
#define PG12_eMIOSA_8   3u

#define PH0  99u
#define PH0_GPIO_99     0u
#define PH0_TCK_99      1u

#define PH1 100u
#define PH1_GPIO_100    0u
#define PH1_TDI         1u

#define PH2 101u
#define PH2_GPIO_101    0u
#define PH2_TDO         1u

#define PH3 102u
#define PH3_GPIO_102    0u
#define PH3_TMS         1u

#define PH4 103u
#define PH4_GPIO_103    0u
#define PH4_PCS0_0      1u
#define PH4_eMIOSB_16   2u
#define PH4_CLKOUT      3u

#define PH5 104u
#define PH5_GPIO_104    0u
#define PH5_VLCD        1u

#define PJ0  105u
#define PJ0_GPIO_105    0u
#define PJ0_PDI_DE      1u

#define PJ1  106u
#define PJ1_GPIO_106    0u
#define PJ1_PDI_HSYNC   1u

#define PJ2  107u
#define PJ2_GPIO_107    0u
#define PJ2_PDI_VSYNC   1u

#define PJ3  108u
#define PJ3_GPIO_108    0u
#define PJ3_PDI_PCLK    1u

#define PJ4  109u
#define PJ4_GPIO_109    0u
#define PJ4_PDI_0       1u
#define PJ4_CANRX_0     2u

#define PJ5  110u
#define PJ5_GPIO_110    0u
#define PJ5_PDI_1       1u
#define PJ5_CANTX_0     2u

#define PJ6  111u
#define PJ6_GPIO_111    0u
#define PJ6_PDI_2       1u
#define PJ6_CANRX_1     2u
#define PJ6_eMIOSA_22   3u

#define PJ7  112u
#define PJ7_GPIO_112    0u
#define PJ7_PDI_3       1u
#define PJ7_CANTX_1     2u
#define PJ7_eMIOSA_21   3u

#define PJ8  113u
#define PJ8_GPIO_113    0u
#define PJ8_PDI_4       1u

#define PJ9  114u
#define PJ9_GPIO_114    0u
#define PJ9_PDI_5       1u

#define PJ10 115u
#define PJ10_GPIO_115   0u
#define PJ10_PDI_6      1u

#define PJ11 116u
#define PJ11_GPIO_116   0u
#define PJ11_PDI_7      1u

#define PJ12 117u
#define PJ12_GPIO_117   0u
#define PJ12_PDI_8      1u
#define PJ12_eMIOSB_17  2u

#define PJ13 118u
#define PJ13_GPIO_118   0u
#define PJ13_PDI_9      1u
#define PJ13_eMIOSB_20  2u

#define PJ14 119u
#define PJ14_GPIO_119   0u
#define PJ14_PDI_10     1u
#define PJ14_eMIOSA_20  2u

#define PJ15 120u
#define PJ15_GPIO_120   0u
#define PJ15_PDI_11     1u
#define PJ15_eMIOSA_19  2u

#define PK0  121u
#define PK0_GPIO_121    0u
#define PK0_PDI_12      1u
#define PK0_eMIOSA_18   2u
#define PK0_DCU_TAG     2u

#define PK1  122u
#define PK1_GPIO_122    0u
#define PK1_PDI_13      1u
#define PK1_eMIOSA_17   2u

#define PK2  123u
#define PK2_GPIO_123    0u
#define PK2_MCKO        1u
#define PK2_PDI_10      2u

#define PK3  124u
#define PK3_GPIO_124    0u
#define PK3_MSEO        1u
#define PK3_PDI_11      2u

#define PK4  125u
#define PK4_GPIO_125    0u
#define PK4_EVTO        1u
#define PK4_PDI_12      2u

#define PK5  126u
#define PK5_GPIO_126    0u
#define PK5_EVTI        1u
#define PK5_PDI_13      2u

#define PK6  127u
#define PK6_GPIO_127    0u
#define PK6_MDO0        1u
#define PK6_PDI_14      2u

#define PK7  128u
#define PK7_GPIO_128    0u
#define PK7_MDO1        1u
#define PK7_PDI_15      2u

#define PK8  129u
#define PK8_GPIO_129    0u
#define PK8_MDO2        1u
#define PK8_PDI_16      2u

#define PK9  130u
#define PK9_GPIO_130    0u
#define PK9_MDO3        1u
#define PK9_PDI_17      2u

#define PK10 131u
#define PK10_GPIO_131   0u
#define PK10_SDA_1      2u
#define PK10_eMIOSA_15  1u

#define PK11 132u
#define PK11_GPIO   0u
#define PK11_SCL_1  1u
#define PK11_eMIOSA 2u

#define SIUL_SetOutput(num)      SIU.PCR[num].R=0x0200u
#define SIUL_SetDIInput(num)     SIU.PCR[num].R=0x0100u
#define SIUL_SetADCInput(num)    SIU.PCR[num].R=0x2100u

#define SIUL_SetMode1AsEmiosOutput(num)    SIU.PCR[num].R=0x0600u
#define SIUL_SetMode2AsEmiosOutput(num)    SIU.PCR[num].R=0x0A00u
#define SIUL_SetMode3AsEmiosOutput(num)    SIU.PCR[num].R=0x0E00u

#define SIUL_Output(num, value)  SIU.GPDO[num].B.PDO=value
#define SIUL_GetInput(num)       SIU.GPDI[num].B.PDI

#define SIUL_POutput0(num, value)  SIU.PGPDO[num].R = ((SIU.PGPDO[num].R & 0x00FFFFFF) | ((uint32)value << 24u))

#endif
