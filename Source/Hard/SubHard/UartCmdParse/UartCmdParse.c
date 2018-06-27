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
#include <Source/CustomLib/macro.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmd.h>
#include <Source/Hard/Hard/UartDriver/UartDriver.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmdParse.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define  DebugCmdParse

#ifdef DebugCmdParse
#define DBG_CMD_PARSE_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_CMD_PARSE_PRINT(...)
#endif


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
byte_pCallbackFunc pvButtonCallback;
byte_pCallbackFunc pvLedCallback;
byte_pCallbackFunc pvRelayCallback;
byte_pCallbackFunc pvSensorCallback;
byte_pCallbackFunc pvUartInitCallback;

byte_t portUartInit;

static byte_t uartTxBuff[MAX_LENGTH_CMD_TX];
static byte_t lengthTxBuff;
static byte_t sequenceTx = 0;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

void handleDataRx(byte_p data);

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
 * @function     : uartParseDataInit
 *
 * @brief        : Initialize callback function
 *
 * @parameter    : Callback function
 *
 * @return value : None
 */

void uartParseDataInit(byte_t portInit, byte_pCallbackFunc InitHandleDataRx){
	portUartInit = portInit;
	if (InitHandleDataRx == NULL){
		pvUartInitCallback = handleDataRx;
	}
	else pvUartInitCallback = InitHandleDataRx;
	uartDriverInit(portUartInit, pvUartInitCallback);
}

/**
 * @function     : cmdParse CallbackInit
 *
 * @brief        : Initialize callback function
 *
 * @parameter    : Callback function
 *
 * @return value : None
 */

void cmdParseButtonCallbackInit(byte_pCallbackFunc getButtonCallback){
	if(getButtonCallback != NULL){
		pvButtonCallback = getButtonCallback;
	}
}
void cmdParseSensorCallbackInit(byte_pCallbackFunc getSensorCallback){
	if(getSensorCallback !=  NULL){
		pvSensorCallback = getSensorCallback;
	}
}

void cmdParseRelayCallbackInit(byte_pCallbackFunc  getRelayCallback){
	if(getRelayCallback != NULL){
		pvRelayCallback = getRelayCallback;
	}
}

void cmdParseLedCallbackInit(byte_pCallbackFunc  getLedCallback){
	if(getLedCallback != NULL){
		pvLedCallback = getLedCallback;
	}
}

/**
 * @function     : handleDataRx
 *
 * @brief        : Handle data received
 *
 * @parameter    : data
 *
 * @return value : None
 */

void handleDataRx(byte_p data){
	CMD_BUFFER *cmd = (CMD_BUFFER*) data;

	switch (cmd -> uCommon.cmdtype){
	case CMD_TYPE_UPDATE:
	case CMD_TYPE_RESPONSE:
		switch (cmd -> uCommon.cmdid){
		case CMD_ID_BUTTON:
			DBG_CMD_PARSE_PRINT ("Button!");
			if (pvButtonCallback != NULL){
				pvButtonCallback ((byte_p) &cmd -> uButtonState.state);
			}
			break;
		case CMD_ID_LED:
			DBG_CMD_PARSE_PRINT("Led!");
			if (pvLedCallback != NULL){
				pvLedCallback ((byte_p) &cmd -> uLedState.state);
			}
			break;
		case CMD_ID_RELAY:
			DBG_CMD_PARSE_PRINT("Relay");
			if (pvRelayCallback != NULL){
				pvRelayCallback ((byte_p) &cmd -> uRelayState.state);
			}
			break;
		case CMD_ID_PIR:
		case CMD_ID_LUX:
		case CMD_ID_LIGHT_THRES:
		case CMD_ID_TIMEOUT:
			if (pvSensorCallback != NULL){
				pvSensorCallback (data);
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
 * @function     : uartSendCommand
 *
 * @brief        : Send command
 *
 * @parameter    : length, type, cmdId, data
 *
 * @return value : None
 */

void uartSendCommand(byte_t txLength,
					 byte_t type,
					 byte_t cmdId,
					 byte_p data,
					 byteCallbackFunc TxCallbackFunc){
	byte_t i;
	byte_t txCheckXor = CXOR_INIT_VAL;
	lengthTxBuff = txLength + 3;

	uartTxBuff[0] = PacketSOF_1;         // 1byte SOF
	uartTxBuff[1] = PacketSOF_2;        // 1byte SOF
	uartTxBuff[2] = txLength;          //1byte length
	uartTxBuff[3] = sequenceTx;       //1byte sequenceTx
	uartTxBuff[4] = type;            //1byte type
	uartTxBuff[5] = cmdId;			//1byte cmdId
	memcpy(&uartTxBuff[6],data,(lengthTxBuff - 4));

	for (i = 2; i < lengthTxBuff -1; i++){
		txCheckXor ^= uartTxBuff[i];
	}
	uartTxBuff[lengthTxBuff - 1] = txCheckXor;   // 1byte checkXor

	WriteOnTopBuffTx ((byte_p) &uartTxBuff, lengthTxBuff, TxCallbackFunc);

	sequenceTx ++;
}
