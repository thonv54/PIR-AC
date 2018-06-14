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



#define PacketSOF_1 0xAA
#define PacketSOF_2 0x55
#define PacketACK 0x06
#define PacketNACK 0x15


#define RX_BUFF	1024

#define MaximmumLengthPerUartPacket 100
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
uartDriverInitData_str g_pUartDriverInitData;

int8u g_byTxSeq = 0; // Seq cua ban tin gui di, tang dan sau moi 1 ban tin gui di
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
void uartGetCommand(void);
void uartSendCommand(int8u txPacketLength,
					 int8u type,
					 int8u cmdId,
					 int8u *cmdParam);


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
	if(uartDriverInitData.UartAckCallback != NULL){
		g_pUartDriverInitData.UartAckCallback = uartDriverInitData.UartAckCallback;
	}
	if(uartDriverInitData.UartNackCallback != NULL){
		g_pUartDriverInitData.UartNackCallback = uartDriverInitData.UartNackCallback;
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
void uartGetCommand(void){
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
					if (g_pUartDriverInitData.UartAckCallback != NULL){
						g_pUartDriverInitData.UartAckCallback();
					}
					else{
						errorUartDriverCallbackPrint();
					}
				}
				else if(byReadSerialData == PacketNACK){
					if (g_pUartDriverInitData.UartNackCallback != NULL){
						g_pUartDriverInitData.UartNackCallback();
					}
					else{
						errorUartDriverCallbackPrint();
					}
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
			if (g_UartRx.DataPacketLength > MaximmumLengthPerUartPacket) {
				g_UartRx.DataReceiverStep = stateSOF1;
			}
			else {
				g_UartRx.DataReceiverStep = stateData;
			}
			break;
		case stateData:
			if (g_UartRx.ReceiverByteCnt < g_UartRx.DataPacketLength-1) {
				g_UartRx.Data[g_UartRx.ReceiverByteCnt] = byReadSerialData;
				g_UartRx.ReceiverByteCnt++;
			}
			else {
				int8u packetCheckXor;
				int8u checkXor = 0xFF;
				packetCheckXor = byReadSerialData;
				{
					for (int8u j = 0; j < g_UartRx.DataPacketLength -1; j++) {
						checkXor = checkXor ^ g_UartRx.Data[j];
					}
				}
				if (packetCheckXor == checkXor) {
					if(g_pUartDriverInitData.GetDataCallback != NULL){
						g_pUartDriverInitData.GetDataCallback(g_UartRx.Data);
					}
					else{
						errorUartDriverCallbackPrint();
					}

					wNumOfByteReceiver = 0; // end of For case
					uartSendAck();
				}
				else {
					uartSendNack();
					DBG_UART_DRIVER_PRINT("    Length of Packet data :  %u \n\r",g_UartRx.DataPacketLength);
					DBG_UART_DRIVER_PRINT("		Packet data :");
				for(int8u i=0; i< g_UartRx.DataPacketLength - 1; i++){
					DBG_UART_DRIVER_PRINT("%X ",g_UartRx.Data[i]);
				}
				DBG_UART_DRIVER_PRINT("		\n\r");

				}

				g_UartRx.DataPacketLength = 0;
				g_UartRx.DataReceiverStep = stateSOF1;
				g_UartRx.ReceiverByteCnt = 0;
			}
			break;
		default:
            g_UartRx.DataPacketLength = 0;
            g_UartRx.DataReceiverStep = stateSOF1;
            g_UartRx.ReceiverByteCnt = 0;
			break;
		}
	}
}


// Event function stub
void uartGetCmdEventFunction(void) {
	emberEventControlSetInactive(uartGetCmdEventControl);
	emberEventControlSetDelayMS(uartGetCmdEventControl,70); // 70ms 1 lan kiem tra buffer uart va xu ly
	uartGetCommand();
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
void uartSendCommand(int8u txPacketLength,
					 int8u type,
					 int8u cmdId,
					 int8u *cmdParam){

	int8u packetCheckXor;

	packetCheckXor = 0xFF ^ g_byTxSeq ^ type ^ cmdId ^ xorStr(cmdParam, (txPacketLength - 4));

	emberSerialWriteByte(g_pUartDriverInitData.port,PacketSOF_1);  // 1byte SOF
	emberSerialWriteByte(g_pUartDriverInitData.port,PacketSOF_2);  // 1byte SOF
	emberSerialWriteByte(g_pUartDriverInitData.port,txPacketLength);  //1byte length
	emberSerialWriteByte(g_pUartDriverInitData.port,g_byTxSeq);  //1byte g_byTxSeq
	emberSerialWriteByte(g_pUartDriverInitData.port,type);  //1byte type
	emberSerialWriteByte(g_pUartDriverInitData.port,cmdId);			//1byte cmd id
	emberSerialWriteData(g_pUartDriverInitData.port,cmdParam,(txPacketLength - 4));  //dataLength = packetLength - 4
	emberSerialWriteByte(g_pUartDriverInitData.port,packetCheckXor); // 1byte Xor
	g_byTxSeq++;

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
}/**
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

void errorUartDriverCallbackPrint(void){
	DBG_UART_DRIVER_PRINT("    CallbackInUartDriverError \n\r");
}









