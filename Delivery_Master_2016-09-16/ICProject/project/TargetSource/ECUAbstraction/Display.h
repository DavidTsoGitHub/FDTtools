/*                                                               */
/* FILE        : Display.h                                       */
/* DATE        : 2015-06-09                                      */
/* AUTHOR      : Cao Wei                                         */
/* DESCRIPTION : source file of display module                   */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Cao Chenguang on 2015-06-20    */
/*                                                               */
/*****************************************************************/
#ifndef	DISPALY_H
#define	DISPALY_H

#define WIDTH_PICTURE    240
#define HEIGHT_PICTURE   320

#define ON               1
#define OFF              0

/*
 * Name        : ECU_MatrixLcd_Init
 * Description : Initialize matrix display
 * Arguments   : none
 * Return      : none
 */
extern void ECU_MatrixLcd_Init(void);

/*
 * Name        : ECU_MatrixLcd_Open
 * Description : open matrix display
 * Arguments   : none
 * Return      : none
 */
extern void ECU_MatrixLcd_Open(void);

/*
 * Name        : ECU_MatrixLcd_Close
 * Description : close matrix display
 * Arguments   : none
 * Return      : none
 */
extern void ECU_MatrixLcd_Close(void);

/*
 * Name        : ECU_MatrixLcd_Clear
 * Description : erase all the matrix display, the whole display is white
 * Arguments   : none
 * Return      : none
 */
extern void ECU_MatrixLcd_Clear(void);

/*
 * Name        : ECU_MatrixLcd_Fill
 * Description : set all the matrix display, the whole display is black
 * Arguments   : none
 * Return      : none
 */
extern void ECU_MatrixLcd_Fill(void);

/*
 * Name         : ECU_SegmentLcd_Open
 * Description  : Open segment display
 * Arguments    : void
 * Return       : void                  
 */
extern void ECU_SegmentLcd_Open(void);

/*
 * Name         : ECU_SegmentLcd_Close
 * Description  : close segment display
 * Arguments    : void
 * Return       : void         
 */
extern void ECU_SegmentLcd_Close(void);

/*
 * Name         : ECU_SegmentLcd_Clear
 * Description  : clear segment display
 * Arguments    : void
 * Return       : void                  
 */
extern void ECU_SegmentLcd_Clear(void);

/*
 * Name         : ECU_SegmentLcd_Fill 
 * Description  : display all segment characters
 * Arguments    : void
 * Return       : void                  
 */
extern void ECU_SegmentLcd_Fill(void);               

#endif
