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
#ifndef int8u
#define int8u unsigned char
#endif

#ifndef int16u
#define int16u unsigned short
#endif

#define CXOR_INIT_VAL 0xFF;

#define stateSOF1   0
#define stateSOF2   1
#define stateLen    2
#define stateData   3
#define stateCheckXor   4


#ifdef DebugUartDriver
#define DBG_UART_DRIVER_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_UART_DRIVER_PRINT(...)
#endif


#define RX_BUFF	1024

#define MaximumLengthPerUartPacket 100
#define MinimumLengthPerUartPacket  5
//#define DebugUartDriver


typedef struct{
	int8u DataReceiverStep;
	int8u DataPacketLength;
	int8u Data[RX_BUFF];
	int8u ReceiverByteCnt;
}uartRx_Str;





/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

uartRx_Str g_UartRx;
pCallbackHandle pvCallbackHandleAck;
uartDriverInitData_str g_pUartDriverInitData;
static int8u packetCheckXor;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
// Event control struct declaration
EmberEventControl uartGetCmdEventControl;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/




// Event function forward declaration
void uartGetCmdEventFunction(void);

void uartSendAck(void);
void uartSendNack(void);

void errorUartDriverCallbackPrint(void);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void uartDriverInit(uartDriverInitData_str uartDriverInitData);
int8u uartGetCommand(void);
void uartSendData(int8u *cmdData, int8u lengthTxData);
void uartDriverHandleAckInit(pCallbackHandle pCallbackHandleAck);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void uartDriverInit(uartDriverInitData_str uartDriverInitData){

	g_UartRx.DataReceiverStep = 0;
	g_UartRx.DataPacketLength = 0;
	g_UartRx.ReceiverByteCnt = 0;

	g_pUartDriverInitData.port = uartDriverInitData.port;
	g_pUartDriverInitData.parity = uartDriverInitData.parity;
	g_pUartDriverInitData.rate = uartDriverInitData.rate;
	g_pUartDriverInitData.stopBits = uartDriverInitData.stopBits;

	if(uartDriverInitData.GetDataCallback != NULL){
		g_pUartDriverInitData.GetDataCallback = uartDriverInitData.GetDataCallback;
	}
}

void uartDriverHandleAckInit(pCallbackHandle pCallbackHandleAck){
	if (pCallbackHandleAck != NULL){
		pvCallbackHandleAck = pCallbackHandleAck;
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
int8u uartGetCommand(void){
	int16u wNumOfByteReceiver;
	int8u  byReadSerialData;

	wNumOfByteReceiver = emberSerialReadAvailable(g_pUartDriverInitData.port);

	for(int8u i = 0; i<wNumOfByteReceiver; wNumOfByteReceiver--){
		(void)emberSerialReadByte(g_pUartDriverInitData.port,&byReadSerialData);
		DBG_UART_DRIVER_PRINT("%X",byReadSerialData);
		switch (g_UartRx.DataReceiverStep) {
		case stateSOF1:
			if (byReadSerialData == PacketSOF_1) {
				g_UartRx.DataReceiverStep = stateSOF2;
			}
			else{
				g_UartRx.DataReceiverStep = stateSOF1;
				if(byReadSerialData == PacketACK){
					return resultRxACK;
				}
				else if(byReadSerialData == PacketNACK){
					return resultRxNACK;
				}
			}
			break;
		case stateSOF2:
			if (byReadSerialData == PacketSOF_2) {
				g_UartRx.DataReceiverStep =  stateLen;
			}
			else{
				g_UartRx.DataReceiverStep = stateSOF1;
			}
			break;
		case stateLen:
			g_UartRx.DataPacketLength = byReadSerialData;
			if ((g_UartRx.DataPacketLength > MaximumLengthPerUartPacket)||
					(g_UartRx.DataPacketLength < MinimumLengthPerUartPacket)) {
				g_UartRx.DataReceiverStep = stateSOF1;
			}
			else {
				g_UartRx.DataReceiverStep = stateData;
				packetCheckXor = CXOR_INIT_VAL;
	            g_UartRx.ReceiverByteCnt = 0;
			}
			break;
		case stateData:
			if (g_UartRx.ReceiverByteCnt < g_UartRx.DataPacketLength-1) {
				g_UartRx.Data[g_UartRx.ReceiverByteCnt] = byReadSerialData;
				packetCheckXor ^= byReadSerialData;
				g_UartRx.ReceiverByteCnt++;
			}
			else {
				if (packetCheckXor == byReadSerialData) {
					if(g_pUartDriverInitData.GetDataCallback != NULL){
						g_pUartDriverInitData.GetDataCallback(g_UartRx.Data);
					}
					return resultRxCMD;
				}
				return resultRxErr;
			}
			break;
		default:
            g_UartRx.DataReceiverStep = stateSOF1;
			break;
		}
	} return resultRxIdle;
}

// Event function stub
void uartGetCmdEventFunction(void) {
	int8u resultRx;
	emberEventControlSetInactive(uartGetCmdEventControl);
	emberEventControlSetDelayMS(uartGetCmdEventControl,70); // 70ms 1 lan kiem tra buffer uart va xu ly
	resultRx = uartGetCommand();
	if (resultRx != resultRxIdle){
		switch (resultRx){
		case resultRxACK:
			if (pvCallbackHandleAck != NULL){
				pvCallbackHandleAck (resultRxACK);
			} break;
		case resultRxNACK:
			if (pvCallbackHandleAck != NULL){
				pvCallbackHandleAck (resultRxNACK);
			} break;
		case resultRxCMD:
			uartSendAck();
			DBG_UART_DRIVER_PRINT("Received successfully!")
			break;
		case resultRxErr:
			uartSendNack();
			DBG_UART_DRIVER_PRINT("Received failed!");
			DBG_UART_DRIVER_PRINT(" Length of Packet data :  %u \n\r",g_UartRx.DataPacketLength);
			DBG_UART_DRIVER_PRINT(" Packet data :");
			for(int8u i=0; i< g_UartRx.DataPacketLength - 1; i++){
				DBG_UART_DRIVER_PRINT("%X ",g_UartRx.Data[i]);
			}
			DBG_UART_DRIVER_PRINT(" \n\r");
			break;
		default:
			DBG_UART_DRIVER_PRINT("Result invalid!\n");
			break;
		}
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
void uartSendData(int8u *cmdData, int8u lengthTxData){
	emberSerialWriteData(g_pUartDriverInitData.port,cmdData,lengthTxData);

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
void uartSendAck(void){
	emberSerialWriteByte(g_pUartDriverInitData.port,PacketACK);

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
void uartSendNack(void){
	emberSerialWriteByte(g_pUartDriverInitData.port,PacketNACK);
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
void errorUartDriverCallbackPrint(void){
	DBG_UART_DRIVER_PRINT("    CallbackInUartDriverError \n\r");
}









