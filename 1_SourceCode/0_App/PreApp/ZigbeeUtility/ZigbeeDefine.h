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
 * Last Changed By:  $Author: Thonv $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date: 2016-05-17 14:20:00 (Tue, 17 May 2016) $
 *
 ******************************************************************************/

#ifndef _ZIGBEEDEFINE_H_
#define _ZIGBEEDEFINE_H_



/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define HcEndpointDefault 1

#define zbLightEp 	1
#define zbLightSwEp 2
#define zbPirEp		3
#define zbLuxEp		4


/* Define state of Device, check ZoneStatus attribute bit map for more detail */
#define PIR_NO_MOTION                   (0x0000)
#ifdef FOR_ZIPATO_HC
#define PIR_MOTION_DETECTED             (0x0003) // alarmed
#else
#define PIR_MOTION_DETECTED             (0x0003) // alarmed
#endif
#define PIR_NOT_STABLE                  (0x0080) // AC fault
#define PIR_CHECKING                    (0xffff)


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
extern int8u gHcEndpoint;
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */


/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */


/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */

#endif /* 1_SOURCECODE_0_APP_PREAPP_ZIGBEEUTILITY_ZIGBEEDEFINE_H_ */
