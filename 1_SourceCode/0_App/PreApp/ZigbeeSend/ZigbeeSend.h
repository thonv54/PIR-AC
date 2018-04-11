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

#ifndef _ZIGBEESEND_H_
#define _ZIGBEESEND_H_


/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "app/framework/include/af.h"
#include "app/framework/util/config.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void zbSendInit(void);

void zbSendRelayState(boolean State);
void zbSendPirState(boolean State);
void zbSendLuxValue(int16u Value);
void zbSendLightThress(int16u Value);
void zbSendPirTimeout(int32u Value);
void zbSendBasicModelAttributeResponse(void);
void zbSendBasicManufacturerAttributeResponse(void);
void ZbSendZdoGetHcActiveEndpoint(void);

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

#endif /* 1_SOURCECODE_0_APP_PREAPP_ZIGBEESEND_ZIGBEESEND_H_ */
