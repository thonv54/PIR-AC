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
#include "serial/serial.h"
#include "1_SourceCode/CustomLib/macro.h"
#include "1_SourceCode/2_Hard/Hard/UartDriver/UartCmd.h"
#include "1_SourceCode/2_Hard/SubHard/UartCmdParse/UartCmdParse.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
//#define  DebugCmdParse

#ifdef DebugCmdParse
#define DBG_CMD_PARSE_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_CMD_PARSE_PRINT(...)
#endif


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typeGetCallback g_pGetButtonCallback;
typeGetCallback g_pGetSensorCallback;
typeGetCallback g_pGetRelayCallback;
typeGetCallback	g_pGetLedCallback;


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
void cmdParseButtonCallbackInit(typeGetCallback getButtonCallback){
	if(getButtonCallback != NULL){
		g_pGetButtonCallback = getButtonCallback;
	}
}
void cmdParseSensorCallbackInit(typeGetCallback getSensorCallback){
	if(getSensorCallback !=  NULL){
		g_pGetSensorCallback = getSensorCallback;
	}
}

void cmdParseRelayCallbackInit(typeGetCallback  getRelayCallback){
	if(getRelayCallback != NULL){
		g_pGetRelayCallback = getRelayCallback;
	}
}


void cmdParseLedCallbackInit(typeGetCallback  getLedCallback){
	if(getLedCallback != NULL){
		g_pGetLedCallback = getLedCallback;
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

    CMD_BUFFER *cmd = (CMD_BUFFER*)data;

	switch(cmd->uCommon.cmdid){
	case CMD_TYPE_UPDATE:
	case CMD_TYPE_RESPONSE:
		switch(cmd->uCommon.cmdtype){
		case CMD_ID_BUTTON:
			if(g_pGetButtonCallback != NULL){
				g_pGetButtonCallback((int8u*)&cmd->uButtonState.state);

				DBG_CMD_PARSE_PRINT("    g_pGetButtonCallback \n\r");

			}
			else{
				errorUartCmdParseCallbackPrint();
				// cb_error
			}
			break;
		case CMD_ID_LED:
			if(g_pGetLedCallback!= NULL){
				g_pGetLedCallback((int8u*)&cmd->uLedState.state);

				DBG_CMD_PARSE_PRINT("    g_pGetLedCallback \n\r");

			}
			else{
				errorUartCmdParseCallbackPrint();
				// cb_error
			}
			break;
		case CMD_ID_RELAY:
			if(g_pGetRelayCallback!= NULL){
				g_pGetRelayCallback((int8u*)&cmd->uRelayState.state);
				DBG_CMD_PARSE_PRINT("    g_pGetRelayCallback \n\r");

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
			if(g_pGetSensorCallback != NULL){
				g_pGetSensorCallback(data);
				DBG_CMD_PARSE_PRINT("    g_pGetSensorCallback \n\r");
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
	DBG_CMD_PARSE_PRINT("    CallbackInUartCmdParseError \n\r");
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
