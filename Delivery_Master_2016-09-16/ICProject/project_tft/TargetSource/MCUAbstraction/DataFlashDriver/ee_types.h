/**HEADER********************************************************************
*
* Copyright (c) 2015 Freescale Semiconductor
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: ee_types.h$
* $Version : 1.2.0$
* $Date    : Jun-10-2015$
*
* Comments:
*
*   This file type defines the data types.
*
*END************************************************************************/

#ifndef _EE_TYPES_H_
#define _EE_TYPES_H_

#ifndef FALSE
#define FALSE 0U
#endif

#ifndef TRUE
#define TRUE 1U
#endif

typedef unsigned char BOOL;

typedef signed char INT8;
typedef unsigned char UINT8;
typedef volatile signed char VINT8;
typedef volatile unsigned char VUINT8;

typedef signed short INT16;
typedef unsigned short UINT16;
typedef volatile signed short VINT16;
typedef volatile unsigned short VUINT16;

typedef signed long INT32;
typedef unsigned long UINT32;
typedef volatile signed long VINT32;
typedef volatile unsigned long VUINT32;

/* Define discovery option for double bit fault event */
#define EER_MCR                         0U
#define IVOR_EXCEPTION                  1U

/* Define const macros*/
#define WORD_SIZE                       4U
#define DWORD_SIZE                      8U
#define ERASED_WORD                     (0xFFFFFFFFU)

/* Define scheme selection */
#define ECC4_FIXLENGTH                  0U
#define ECC4_VARLENGTH                  1U
#define ECC8_FIXLENGTH                  2U
#define ECC8_VARLENGTH                  3U
#define ECC16_FIXLENGTH                 4U
#define ECC16_VARLENGTH                 5U
#define ECC32_FIXLENGTH                 6U
#define ECC32_VARLENGTH                 7U

/* Define Flash module */
#define H7Fx                            0U
#define C90FL                           1U
#define C90LC_DFO                       2U
#define C90LC                           3U
#define C55                             4U

#endif  /*_EE_TYPES_H_*/
/* EOF */
