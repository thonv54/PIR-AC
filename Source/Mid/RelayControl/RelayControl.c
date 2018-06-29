/******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: Thonv
 *
 * Last Changed By:  $Author: Thonv $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date: 2016-05-16 11:45:00 (Tue, 16 May 2016) $
 *
 ******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include <Source/CustomLib/macro.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmd.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmdParse.h>
#include <Source/Mid/RelayControl/RelayControl.h>
#include "app/framework/include/af.h"
#include "app/framework/util/config.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

boolCallbackFunc pvRelayCallbackHandle;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

RelayData_str gRelay;

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

void relayHandler(byte_t *data);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void relayCallbackInit(boolCallbackFunc relayCallback);
void relayGetState(void);


void relayCallbackInit(boolCallbackFunc relayCallback){
	cmdParseRelayCallbackInit(relayHandler);
	if(relayCallback != NULL){
		pvRelayCallbackHandle = relayCallback;
	}
}
/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void relayHandler(byte_t *data){

	byte_t relayState = (byte_t)*data;

	if(relayState == RELAY_ON_STATE){
		gRelay.relayCurrentState = boolRlOn;
	}
	else if(relayState == RELAY_OFF_STATE){
		gRelay.relayCurrentState = boolRlOff;
	}
	if(pvRelayCallbackHandle != NULL){
		pvRelayCallbackHandle(gRelay.relayCurrentState);
	}
	gRelay.relayLastState = gRelay.relayCurrentState;
	gRelay.LastTimeFromGetState = halCommonGetInt32uMillisecondTick();
}

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void relayGetState(void){
	uartSendCommand(leRequestCmd,CMD_TYPE_REQUEST,CMD_ID_RELAY,NULL, NULL);
}
/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
