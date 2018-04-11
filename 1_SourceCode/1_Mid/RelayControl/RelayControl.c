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
#include "1_SourceCode/1_Mid/RelayControl/RelayControl.h"
#include "1_SourceCode/2_Hard/SubHard/UartCmdParse/UartCmdParse.h"
#include "1_SourceCode/2_Hard/Hard/UartDriver/UartDriver.h"
#include "1_SourceCode/CustomLib/macro.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

//#define  DebugRelayControl
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typeRelayCallback pvRelayCallback;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
RelayData_str gRelay;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void relayHandler(int8u *data);
void errorMidRelayCallbackPrint(void);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void relayCallbackInit(typeRelayCallback relayCallback);
void relayGetState(void);


void relayCallbackInit(typeRelayCallback relayCallback){
	cmdParseRelayCallbackInit(relayHandler);
	if(relayCallback != NULL){
		pvRelayCallback = relayCallback;
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
void relayHandler(int8u *data){
	int8u relayState = data[3];

	if(relayState == rlOnState){
		gRelay.relayCurrentState = boolRlOn;
	}
	else if(relayState == rlOffState){
		gRelay.relayCurrentState = boolRlOff;
	}
	if(pvRelayCallback != NULL){
		pvRelayCallback(gRelay.relayCurrentState);
#ifdef DebugRelayControl
	emberSerialPrintf(APP_SERIAL,"    pvRelayCallback \n\r");
#endif
	}
	else{
		errorMidRelayCallbackPrint();
		// error_cb
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

void errorMidRelayCallbackPrint(void){
	emberSerialPrintf(APP_SERIAL,"    CallbackInMidRelaynError \n\r");
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
	cmdParseRelayCallbackInit(relayHandler);
	uartSendCommand(leRequestCmd,CMD_TYPE_REQUEST,CMD_ID_RELAY,NULL);
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
