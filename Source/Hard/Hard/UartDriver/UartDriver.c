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
#include "stack/include/event.h"
#include <Source/CustomLib/macro.h>
#include <Source/Hard/Hard/UartDriver/UartDriver.h>

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

#define DebugUartDriver

#ifdef DebugUartDriver
#define DBG_UART_DRIVER_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_UART_DRIVER_PRINT(...)
#endif


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

static bool_t bTxActive = FALSE;
static bool_t bRxActive = FALSE;
static byte_t timeCheckRes = 0;
static byte_t portUART;

word_t timeoutRx;
byte_t packetCheckXor;

byteCallbackFunc pvTxResultCallback = NULL;
byte_pCallbackFunc pvRxHandleCallback = NULL;

UNIT_BUFF TxBuffSend [MAX_BUFF];
SEQUENCE_BUFF TxBuffSeq;

static uartRx_Str uartRxStruct;


/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

EmberEventControl uartGetCmdEventControl;
EmberEventControl uartSendCmdEventControl;

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

byte_t uartGetCommand(void);
void uartGetCmdEventFunction(void);
void uartSendCmdEventFunction(void);
void ResultTxTimeout(byte_t pResponse);
void SendDataFromBottomBuffTx(void);
void InitBuffTx (void);
void uartTimeoutReset(void);
uint_t uartGetElapsedTime(uint_t timeStart);
void uartSendAck(void);
void uartSendNack(void);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

void uartDriverInit(byte_t portUartInit, byte_pCallbackFunc pRxCallbackFuncInit);
byte_t WriteOnTopBuffTx (byte_p data, byte_t lengthData, byteCallbackFunc TxCallbackFunc);


void uartDriverInit(byte_t portUartInit, byte_pCallbackFunc pRxCallbackFuncInit){
	portUART = portUartInit;
	if (pRxCallbackFuncInit != NULL){
		pvRxHandleCallback = pRxCallbackFuncInit;
	}

	uartRxStruct.DataReceiverStep = stateSOF1;
	InitBuffTx();
	emberEventControlSetActive(uartGetCmdEventControl);
}

/**
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
 */

byte_t uartGetCommand(void){
	byte_t wNumOfByteReceiver;
	byte_t  byReadSerialData;
	byte_t  byRetVal = resultRxIdle;

	wNumOfByteReceiver = emberSerialReadAvailable(portUART); //Number of byte in Buffer

	while ((wNumOfByteReceiver > 0)&&(byRetVal == resultRxIdle)){
		(void)emberSerialReadByte(portUART,&byReadSerialData);

		//DBG_UART_DRIVER_PRINT("%X",byReadSerialData);

		switch (uartRxStruct.DataReceiverStep){
		case stateSOF1:
			if (PacketSOF_1 == byReadSerialData){
				uartRxStruct.DataReceiverStep = stateSOF2;
				bRxActive = TRUE;                    // Receiver active
				uartTimeoutReset();       //  Reset time
			}
			else if (PacketACK == byReadSerialData){
				byRetVal = resultRxACK;
			}
			else if (PacketNACK == byReadSerialData){
				byRetVal = resultRxNACK;
			}
			else; // None
			break;
		case stateSOF2:
			if (PacketSOF_2 == byReadSerialData){
				uartRxStruct.DataReceiverStep = stateLength;
				uartTimeoutReset();
			}
			else {
				uartRxStruct.DataReceiverStep = stateSOF1;
				byRetVal = resultRxErr;
			}
			break;
		case stateLength:
			uartRxStruct.DataPacketLength = byReadSerialData;
			if ((uartRxStruct.DataPacketLength > MaxLengthUARTPacket)||
				(uartRxStruct.DataPacketLength < MinLengthUARTPacket)){

				uartRxStruct.DataReceiverStep = stateSOF1;
				byRetVal = resultRxErr;
			}
			else{
				uartRxStruct.DataReceiverStep = stateData;
				packetCheckXor = CXOR_INIT_VAL;
				uartRxStruct.ReceiverByteCount = 0;
				uartTimeoutReset();
			}
			break;
		case stateData:
			if (uartRxStruct.ReceiverByteCount < uartRxStruct.DataPacketLength - 1){
				uartRxStruct.Data[uartRxStruct.ReceiverByteCount] = byReadSerialData;
				packetCheckXor ^= byReadSerialData;
				uartRxStruct.ReceiverByteCount ++;
				uartTimeoutReset();
			}
			else{
				uartRxStruct.DataReceiverStep = stateSOF1;
				bRxActive = FALSE;                  // Receiver complete

				if (packetCheckXor == byReadSerialData){
					byRetVal = resultRxSUCCESS;
				}
				else byRetVal = resultRxErr;
			}
			break;
		default:
			uartRxStruct.DataReceiverStep = stateSOF1;
			break;
		}
		wNumOfByteReceiver--;
	}

	if(byRetVal == resultRxIdle){
		if ((bRxActive) && (uartGetElapsedTime(timeoutRx) >= RX_TIMEOUT)){
			bRxActive = FALSE;                     // Receiver complete
			uartRxStruct.DataReceiverStep = stateSOF1;
			byRetVal = resultRxTimeout;
		}
	}

	return byRetVal;
}

/**
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
 */

void uartGetCmdEventFunction(void){
	byte_t resultRx;
	resultRx = uartGetCommand();
	if (resultRx != resultRxIdle){
		switch (resultRx){
		case resultRxACK:
			ResultTxTimeout(resultTxSUCCESS); // resultTx Success
			DBG_UART_DRIVER_PRINT("ACK! \n");
			break;

		case resultRxNACK:
			ResultTxTimeout(resultTxFALSE);  // resultTx FALSE
			DBG_UART_DRIVER_PRINT("NACK! \n");
			break;

		case resultRxSUCCESS:                            // Send ACK
			if (pvRxHandleCallback != NULL){
				pvRxHandleCallback(uartRxStruct.Data);  // Callback function handle data
			}
			DBG_UART_DRIVER_PRINT("Received successfully! \n");
			uartSendAck();
			break;

		case resultRxErr:
			DBG_UART_DRIVER_PRINT("Received failed! \n");
			uartSendNack();                             // Send NACK
			break;

		case resultRxTimeout:
			DBG_UART_DRIVER_PRINT("Timeout Receiver! \n");
			uartSendNack();                             // Send NACK
			break;

		default:
			DBG_UART_DRIVER_PRINT("Result invalid!\n");
			break;
		}
	}

	if (bTxActive){
		timeCheckRes ++;
		if (TIME_WAIT_RES == timeCheckRes){
			ResultTxTimeout (resultTxTimeout);
		}
	}

	emberEventControlSetInactive(uartGetCmdEventControl);
	emberEventControlSetDelayMS(uartGetCmdEventControl,PERIOD_RX_MS); // 70ms check Buffer and Handle
}

/**
 * @function     :
 *
 * @brief        :
 *
 * @parameter    :
 *
 * @return value :
 */

void ResultTxTimeout(byte_t pResponse){
	byte_t result;
	if (pResponse == resultTxSUCCESS){
		result = resultTxSUCCESS;
	}
	else{
		result = resultTxFALSE;
	}
	if (bTxActive){
		bTxActive = FALSE;
		timeCheckRes = 0;
		if (pvTxResultCallback != NULL){
			byteCallbackFunc txResultFunc = pvTxResultCallback;
			pvTxResultCallback = NULL;
			txResultFunc(result);
		}

		TxBuffSeq.numberUnit --;
		if (TxBuffSeq.numberUnit >0) emberEventControlSetActive (uartSendCmdEventControl);
	}
}


/**
 * @function     :
 *
 * @brief        :
 *
 * @parameter    :
 *
 * @return value :
 */


byte_t WriteOnTopBuffTx (byte_p data, byte_t lengthData, byteCallbackFunc callbackFuntion){
	if (TxBuffSeq.numberUnit == MAX_BUFF){
		DBG_UART_DRIVER_PRINT("BuffFull.\n");
		return buffFull;
	}

	UNIT_BUFF* unitBuff = &TxBuffSend[TxBuffSeq.topBuff];
	unitBuff -> lengthTx = lengthData;
	memcpy ((byte_p) &(unitBuff->dataTx), data, lengthData);
	unitBuff -> callbackFuncTx = callbackFuntion;

	TxBuffSeq.topBuff = (TxBuffSeq.topBuff + 1) % MAX_BUFF;
	TxBuffSeq.numberUnit ++;

	if (!bTxActive) // First send request
		emberEventControlSetActive (uartSendCmdEventControl);
	return buffSuccess;
}

/**
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
 */

void SendDataFromBottomBuffTx(void){
	UNIT_BUFF* dataTx = &TxBuffSend[TxBuffSeq.bottomBuff];
	emberSerialWriteData(portUART, (byte_p) &(dataTx->dataTx), dataTx->lengthTx);

	pvTxResultCallback = dataTx->callbackFuncTx;
	TxBuffSeq.bottomBuff = (TxBuffSeq.bottomBuff + 1) % MAX_BUFF;
}

/**
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
 */

void uartSendCmdEventFunction(void){
	if (TxBuffSeq.numberUnit == 0){
		DBG_UART_DRIVER_PRINT("BuffEmpty.\n");
	}
	else{
		SendDataFromBottomBuffTx();
		bTxActive = TRUE;
	}
	emberEventControlSetInactive (uartSendCmdEventControl);
}

/**
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
 */

void InitBuffTx (void){
	byte_p initBuff = (byte_p) TxBuffSend;
	for ( word_t i = 0; i < sizeof (TxBuffSend); i++ ){
		*initBuff ++ = 0;
	}

	TxBuffSeq.bottomBuff = TxBuffSeq.topBuff = TxBuffSeq.numberUnit = 0;
}


/**
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
 */

void uartTimeoutReset(){
	timeoutRx = halCommonGetInt32uMillisecondTick();
}

/**
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
 */

uint_t uartGetElapsedTime(uint_t timeStart){
	return (halCommonGetInt32uMillisecondTick() - timeStart);
}

/**
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
 */

void uartSendAck(void){
	emberSerialWriteByte(portUART,PacketACK);
}
void uartSendNack(void){
	emberSerialWriteByte(portUART,PacketNACK);
}




