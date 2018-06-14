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


#ifndef _UARTCMDPARSE_H_
#define _UARTCMDPARSE_H_





/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <Source/Hard/Hard/UartDriver/UartDriver.h>
#include "app/framework/include/af.h"


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

typedef void (*typeGetCallback)(int8u *data);

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void uartCmdParseInit(uartDriverInitData_str uartDriverInitData);

void cmdParseButtonCallbackInit(typeGetCallback getButtonCallback);
void cmdParseSensorCallbackInit(typeGetCallback getSensorCallback);
void cmdParseRelayCallbackInit(typeGetCallback  getRelayCallback);
void cmdParseLedCallbackInit(typeGetCallback  getLedCallback);


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



#endif /* 1_SOURCECODE_2_HARD_SUBHARD_UARTCMDPARSE_UARTCMDPARSE_H_ */
