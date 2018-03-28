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

#include "app/framework/util/config.h"
#include "1_SourceCode/2_Hard/Hard/UartDriver/UartDriver.h"
#include "1_SourceCode/3_CustomLib/macro.h"
#include "stack/include/event.h"


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

#define PacketSOF 0x01
#define PacketACK 0x06
#define PacketNACK 0x15


#define RX_BUFF	1024

#define MaximmumLengthPerUartPacket 100
#define DebugUartDriver


typedef struct{
	int8u DataReceiverStep;
	int8u DataPacketLength;
	int8u Data[RX_BUFF];
	int8u ReceiverByteCnt;
}uartRx_Str;





/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

uartRx_Str pvUartRx;
uartDriverInitData_str pvUartDriverInitData;

int8u txSeq = 0; // Seq cua ban tin gui di, tang dan sau moi 1 ban tin gui di
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/


// Event control struct declaration
EmberEventControl uartGetCmdEventControl;

// Event function forward declaration
void uartGetCmdEventFunction(void);




void uartSendAck(void);
void uartSendNack(void);


/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void uartDriverInit(uartDriverInitData_str uartDriverInitData);
void uartGetCommand(void);
void uartSendCommand(int8u txPacketLength,
					 int8u type,
					 int8u cmdId,
					 int8u cmdParam[10]);


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

	pvUartDriverInitData.port = uartDriverInitData.port;
	pvUartDriverInitData.parity = uartDriverInitData.parity;
	pvUartDriverInitData.rate = uartDriverInitData.rate;
	pvUartDriverInitData.stopBits = uartDriverInitData.stopBits;

	if(uartDriverInitData.GetDataCallback != NULL){
		pvUartDriverInitData.GetDataCallback = uartDriverInitData.GetDataCallback;
	}
	if(uartDriverInitData.UartAckCallback != NULL){
		pvUartDriverInitData.UartAckCallback = uartDriverInitData.UartAckCallback;
	}
	if(uartDriverInitData.UartNackCallback != NULL){
		pvUartDriverInitData.UartNackCallback = uartDriverInitData.UartNackCallback;
	}

	emberSerialInit(pvUartDriverInitData.port,
					pvUartDriverInitData.rate,
					pvUartDriverInitData.parity,
					pvUartDriverInitData.stopBits);
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
	int8u  byReadStatus;

	wNumOfByteReceiver = emberSerialReadAvailable(pvUartDriverInitData.port);

	for(int8u i = 0; i<wNumOfByteReceiver; i++){
		byReadStatus = emberSerialReadByte(pvUartDriverInitData.port,&byReadSerialData);
		wNumOfByteReceiver--;
		switch (pvUartRx.DataReceiverStep) {
		case 0:
			if (byReadSerialData == PacketSOF) {
				pvUartRx.DataReceiverStep ++;
			}
			else{
				pvUartRx.DataReceiverStep = 0;
				if(byReadSerialData == PacketACK){
					pvUartDriverInitData.UartAckCallback();
				}
				else if(byReadSerialData == PacketNACK){
					pvUartDriverInitData.UartNackCallback();
				}
			}
			break;
		case 1:
			pvUartRx.DataPacketLength = byReadSerialData;
			if (pvUartRx.DataPacketLength > MaximmumLengthPerUartPacket) {
				pvUartRx.DataReceiverStep = 0;
			}
			else {
				pvUartRx.DataReceiverStep++;
			}
			break;
		case 2:
			if (pvUartRx.ReceiverByteCnt < pvUartRx.DataPacketLength - 1) {
				pvUartRx.Data[pvUartRx.ReceiverByteCnt] = byReadSerialData;
				pvUartRx.ReceiverByteCnt++;
			}
			else {
				int8u packetCheckXor;
				int8u checkXor = 0xFF;
				packetCheckXor = byReadSerialData;
				{
					for (int8u j = 1; j < pvUartRx.DataPacketLength - 1; j++) {
						checkXor = checkXor ^ pvUartRx.Data[j];
					}
				}
				if (packetCheckXor == checkXor) {
					pvUartDriverInitData.GetDataCallback(pvUartRx.Data);
					wNumOfByteReceiver = 0; // end of For case
					uartSendAck();
				}
				else {
					uartSendNack();
#ifdef DebugUartDriver
				emberSerialPrintf(APP_SERIAL,"    Length ofPacket data :  %u \n\r",pvUartRx.DataPacketLength);
				emberSerialPrintf(APP_SERIAL,"		Packet data :");
				for(int8u i=0; i< pvUartRx.DataPacketLength - 1; i++){
				   emberSerialPrintf(APP_SERIAL,"%X ",pvUartRx.Data[i]);
				}
				emberSerialPrintf(APP_SERIAL,"		\n\r");
#endif // Debug
				}

				pvUartRx.DataPacketLength = 0;
				pvUartRx.DataReceiverStep = 0;
			}
			break;
		default:
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
					 int8u cmdParam[10]){

	int8u packetCheckXor;

	packetCheckXor = 0xFF ^ txSeq ^ type ^ cmdId ^ xorStr(cmdParam, (txPacketLength - 4));

	emberSerialWriteByte(pvUartDriverInitData.port,PacketSOF);  // 1byte SOF
	emberSerialWriteByte(pvUartDriverInitData.port,txPacketLength);  //1byte length
	emberSerialWriteByte(pvUartDriverInitData.port,txSeq);  //1byte txSeq
	emberSerialWriteByte(pvUartDriverInitData.port,type);  //1byte type
	emberSerialWriteByte(pvUartDriverInitData.port,cmdId);			//1byte cmd id
	emberSerialWriteData(pvUartDriverInitData.port,cmdParam,(txPacketLength - 4));  //dataLength = packetLength - 4
	emberSerialWriteByte(pvUartDriverInitData.port,packetCheckXor); // 1byte Xor
	txSeq++;

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
	emberSerialWriteByte(pvUartDriverInitData.port,PacketACK);
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
	emberSerialWriteByte(pvUartDriverInitData.port,PacketNACK);
}











