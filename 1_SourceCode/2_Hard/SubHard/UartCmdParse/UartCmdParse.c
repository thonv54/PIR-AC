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

#include "app/framework/include/af.h"

#include "1_SourceCode/2_Hard/SubHard/UartCmdParse/UartCmdParse.h"
#include "1_SourceCode/2_Hard/Hard/UartDriver/UartDriver.h"

#include "app/framework/util/config.h"
#include "1_SourceCode/3_CustomLib/macro.h"


#include "serial/serial.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

typedef void (*typeButtonCallback)(int8u *data);
typedef void (*typeSensorCallback)(int8u *data);
typedef void (*typeRelayCallback)(int8u *data);

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typeButtonCallback pvButtonCallback;
typeSensorCallback pvSensorCallback;
typeRelayCallback  pvRelayCallback;

void GetDataHandler(int8u *data);
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

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
void cmdParseInitButtonCallback(typeButtonCallback buttonCallback){
	if(buttonCallback != NULL){
		pvButtonCallback = buttonCallback;
	}
}
void cmdParseInitSensorCallback(typeSensorCallback sensorCallback){
	if(sensorCallback !=  NULL){
		pvSensorCallback = sensorCallback;
	}
}

void cmdParseInitRelayCallback(typeRelayCallback  relayCallback){
	if(relayCallback != NULL){
		pvRelayCallback = relayCallback;
	}
}
void uartCmdParseInit(uartDriverInitData_str uartDriverInitData){
	if(uartDriverInitData.GetDataCallback == NULL){
		uartDriverInitData.GetDataCallback = GetDataHandler;      // neu tang tren khong Handler, thu se Handler o tang nay
	}
	uartDriverInit(uartDriverInitData);
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
void GetDataHandler(int8u *data){
	int8u byPacketRxSeq = data[0];
	int8u byPacketType  = data[1];
	int8u byPacketCmdId = data[2];


	switch(byPacketType){
	case CMD_TYPE_UPDATE:
	case CMD_TYPE_RESPONSE:
		switch(byPacketCmdId){
		case CMD_ID_BUTTON:
			pvButtonCallback(data);
			break;
		case CMD_ID_LED:
			break;
		case CMD_ID_RELAY:
			pvRelayCallback(data);
			break;
		case CMD_ID_PIR:
		case CMD_ID_LUX:
		case CMD_ID_LIGHT_THRES:
		case CMD_ID_TIMEOUT:
			pvSensorCallback(data);
			break;
		case CMD_ID_ERROR:
			break;
		default:
			break;
		}
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
