/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: Thonv
 *
 * Last Changed By:  $Author: trungtq $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date: 2016-05-20 10:20:00 (Fri, 20 May 2016) $
 *
 ******************************************************************************/
#ifndef _MACRO_H_
#define _MACRO_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define st(x)                   do { x } while(__LINE__ == -1)

#ifndef BV
#define BV(n)                   (1 << (n))
#endif /* BV */

#ifndef max
#define max(a, b)               (((a) > (b)) ? (a) : (b))
#define min(a, b)               (((a) < (b)) ? (a) : (b))
#endif /* max */

#ifndef HI_UINT16
#define HI_UINT16(a)            (((a) >> 8) & 0xFF)
#define LO_UINT16(a)            ((a) & 0xFF)
#define HI_UINT8(a)             (((a) >> 4) & 0x0F)
#define LO_UINT8(a)             ((a) & 0x0F)
#endif /* HI_UINT16 */

#ifndef MERGE16
#define MERGE16(hi, lo)         (((u16_t)(hi) << 8) | (lo))
#endif /* MERGE16 */
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
     
     
unsigned char xorStr(unsigned char *Str, unsigned char Length);
unsigned short SwapEndiannessInt16u(unsigned short val);
void SwapEndiannessEUI64(unsigned char *val);
/******************************************************************************/

#endif /* _TYPEDEFS_H_ */