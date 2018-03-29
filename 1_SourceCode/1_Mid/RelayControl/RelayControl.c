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
typedef struct{
	boolean relayCurrentState;
	boolean relayLastState;
	int32u LastTimeFromGetState;
}RelayData_str;

typedef void (*typeRelayCallback)(char data);
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typeRelayCallback pvRelayCallback;
RelayData_str pvRelay;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void relayHandler(int8u *data);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/



void RelayCallbackInit(typeRelayCallback relayCallback){
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
	pvRelay.relayCurrentState = data[3];
	int8u cmdType = data[1];

	switch (cmdType){
	case CMD_TYPE_UPDATE:
		break;
	default:
		break;
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
