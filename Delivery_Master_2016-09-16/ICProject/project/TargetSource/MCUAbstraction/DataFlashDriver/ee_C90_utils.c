/**HEADER********************************************************************
*
* Copyright (c) 2014 Freescale Semiconductor;
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
* $FileName: ee_CLC_DFO_utils.c$
* $Version : 1.2.0$
* $Date    : Jul-30-2015$
*
* Comments:
*
*   Contains disable/enable flash controller cache.
*
*END************************************************************************/
#include "ee_emulation.h"

#define FLASH_FMC           0xC3F88000
#define FLASH_PFB_CR        0x0000001CU /* PFBIU Configuration Register for port 0 */
#define FLASH_PFB_CR_BFEN   0x00000001U /* PFBIU Line Read Buffers Enable */

/**************************************************************
*                          Disable Flash Cache                  *
***************************************************************/
void DisableFlashControllerCache(UINT32 *origin_pflash_pfcr1,
                                 UINT32 *origin_pflash_pfcr2)
{
    /* Read the values of PFB_CR*/
    *origin_pflash_pfcr1 = READ32(FLASH_FMC + FLASH_PFB_CR);
    /* Disable Caches */
    CLEAR32(FLASH_FMC + FLASH_PFB_CR, FLASH_PFB_CR_BFEN);
}

/*****************************************************************
*               Restore configuration register of FCM              *
******************************************************************/
void RestoreFlashControllerCache(UINT32 pflash_pfcr1,
                                 UINT32 pflash_pfcr2)
{
    WRITE32(FLASH_FMC + FLASH_PFB_CR, pflash_pfcr1);
}
/* EOF */























