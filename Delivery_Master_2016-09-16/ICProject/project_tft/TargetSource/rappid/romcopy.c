/*
 *######################################################################
 *                                RAppIDJDP
 *           Rapid Application Initialization and Documentation Tool
 *                         Freescale Semiconductor Inc.
 *######################################################################
 *
 * Project Name           : baseconfig
 *
 * Project File           : baseconfig.rsp
 *
 * Revision Number        : 1.0
 *
 * Tool Version           : 1.0.0.1	 
 * 
 * @file                  : romcopy.c
 *
 * Target Compiler        : Codewarrior
 *
 * Target Part            : MPC5604C
 *
 * Part Errata Fixes      : none
 *
 * Project Last Save Date : 03-Oct-2008 08:24:54
 *
 * Created on Date        : 05-Nov-2008 14:57:10
 * 
 * @Brief Description     : It copies data from ROM to RAM
 *
 *######################################################################
*/

#include "typedefs.h"

#define DCU_PRESENT    1       

extern int main();
extern int romcopy(void);
extern char __DATA_ROM[], __DATA_RAM[], __DATA_END[], __BSS_START[], __BSS_END[];

#if DCU_PRESENT
extern char __DATA_DCU_ROM[],__DATA_DCU_RAM[],__DATA_DCU_END[];
#endif   

int romcopy(void)
{
	int count;
	char *destinationLocation = (char*)__DATA_RAM;
	char *SourceLocation = (char*)__DATA_ROM;
	long int NoOfBytes = __DATA_END-__DATA_RAM;


	/* move initialized data from ROM to RAM if defined at	*/
	/* different locations					*/
	if (__DATA_ROM != __DATA_RAM)
	{
		for(count=0;count <NoOfBytes ; count++)
		{
			*destinationLocation = *SourceLocation;
			destinationLocation++;
			SourceLocation++;		
		}	
	}
	
#if DCU_PRESENT
	destinationLocation = (char*)__DATA_DCU_RAM;
	SourceLocation = (char*)__DATA_DCU_ROM;
	NoOfBytes = __DATA_DCU_END-__DATA_DCU_RAM;


	/* move initialized data from ROM to RAM if defined at	*/
	/* different locations					*/
	if (__DATA_DCU_ROM != __DATA_DCU_RAM)
	{
		for(count=0;count <NoOfBytes ; count++)
		{
			*destinationLocation = *SourceLocation;
			destinationLocation++;
			SourceLocation++;		
		}	
	}
#endif

	/* clear uninitialized data				*/
	if (__BSS_START != __BSS_END) 
	{
		destinationLocation = (char*) __BSS_START;
		NoOfBytes = __BSS_END-__BSS_START;
		for(count=0;count <NoOfBytes ; count++)
		{
			*destinationLocation = 0;
			destinationLocation++;	
		}
	}
	
	/*return to main program */
	return main() ;
}
