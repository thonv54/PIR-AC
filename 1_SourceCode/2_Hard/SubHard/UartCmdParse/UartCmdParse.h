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

#include "1_SourceCode/2_Hard/Hard/UartDriver/UartDriver.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

typedef void (*typeGetButtonCallback)(int8u *data);
typedef void (*typeGetSensorCallback)(int8u *data);
typedef void (*typeGetRelayCallback)(int8u *data);
typedef void (*typeGetLedCallback)(int8u *data);

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

void cmdParseButtonCallbackInit(typeGetButtonCallback getButtonCallback);
void cmdParseSensorCallbackInit(typeGetSensorCallback getSensorCallback);
void cmdParseRelayCallbackInit(typeGetRelayCallback  getRelayCallback);
void cmdParseLedCallbackInit(typeGetLedCallback  getLedCallback);


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
