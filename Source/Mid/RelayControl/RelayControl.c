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
#include <Source/Hard/Hard/UartDriver/UartDriver.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmdParse.h>
#include <Source/Mid/RelayControl/RelayControl.h>
#include "app/framework/include/af.h"
#include "app/framework/util/config.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

//#define  DebugRelayControl

#ifdef DebugRelayControl
#define DBG_RELAY_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define DBG_RELAY_PRINT(...)
#endif
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

	int8u relayState = (int8u)*data;

	if(relayState == RELAY_ON_STATE){
		gRelay.relayCurrentState = boolRlOn;
	}
	else if(relayState == RELAY_OFF_STATE){
		gRelay.relayCurrentState = boolRlOff;
	}
	if(pvRelayCallback != NULL){
		pvRelayCallback(gRelay.relayCurrentState);
		DBG_RELAY_PRINT("    pvRelayCallback \n\r");
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
    DBG_RELAY_PRINT("    CallbackInMidRelaynError \n\r");
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
