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

#include "app/framework/include/af.h"
#include "Source/CustomLib/typedefs.h"
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

void uartParseDataInit(byte_t portInit, byte_pCallbackFunc InitHandleDataRx);
void cmdParseButtonCallbackInit(byte_pCallbackFunc getButtonCallback);
void cmdParseSensorCallbackInit(byte_pCallbackFunc getSensorCallback);
void cmdParseRelayCallbackInit(byte_pCallbackFunc  getRelayCallback);
void cmdParseLedCallbackInit(byte_pCallbackFunc getLedCallback);

void uartSendCommand(byte_t TxLength,
					 byte_t type,
					 byte_t cmdId,
					 byte_p data,
					 byteCallbackFunc TxCallbackFunc);


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
