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
#include "app/framework/util/config.h"
#include "1_SourceCode/CustomLib/macro.h"
#include "serial/serial.h"

#include "1_SourceCode/2_Hard/SubHard/UartCmdParse/UartCmdParse.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/



/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typeGetButtonCallback pvGetButtonCallback;
typeGetSensorCallback pvGetSensorCallback;
typeGetRelayCallback  pvGetRelayCallback;


/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void GetDataHandler(int8u *data);

void errorUartCmdParseCallbackPrint(void);
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
void cmdParseButtonCallbackInit(typeGetButtonCallback getButtonCallback){
	if(getButtonCallback != NULL){
		pvGetButtonCallback = getButtonCallback;
	}
}
void cmdParseSensorCallbackInit(typeGetSensorCallback getSensorCallback){
	if(getSensorCallback !=  NULL){
		pvGetSensorCallback = getSensorCallback;
	}
}

void cmdParseRelayCallbackInit(typeGetRelayCallback  getRelayCallback){
	if(getRelayCallback != NULL){
		pvGetRelayCallback = getRelayCallback;
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
			if(pvGetButtonCallback != NULL){
				pvGetButtonCallback(data);
			}
			else{
				errorUartCmdParseCallbackPrint();
				// cb_error
			}
			break;
		case CMD_ID_LED:
			break;
		case CMD_ID_RELAY:
			if(pvGetRelayCallback!= NULL){
				pvGetRelayCallback(data);
			}
			else{
				errorUartCmdParseCallbackPrint();
				// cb_error
			}
			break;
		case CMD_ID_PIR:
		case CMD_ID_LUX:
		case CMD_ID_LIGHT_THRES:
		case CMD_ID_TIMEOUT:
			if(pvGetSensorCallback != NULL){
				pvGetSensorCallback(data);
			}
			else{
				errorUartCmdParseCallbackPrint();
				// cb_error
			}
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
void errorUartCmdParseCallbackPrint(void){
	emberSerialPrintf(APP_SERIAL,"    CallbackInUartCmdParseError \n\r");
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
