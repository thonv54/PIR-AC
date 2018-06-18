/*
 * UartCmdParseSend.c
 *
 *  Created on: Jun 18, 2018
 *      Author: Long
 */

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "app/framework/include/af.h"
#include <string.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmd.h>
#include <Source/Hard/Hard/UartDriver/UartDriver.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmdParseSend.h>


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
//#define  DebugCmdParse

#define lengthMaxTxBuff 20

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

static int8u uartTxBuff[lengthMaxTxBuff];
static int8u lengthTxBuff;
static int8u g_byTxSeq = 0;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

void uartParseSendHandle (int8u response);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void uartSendCommand (int8u txPacketLength,
					 int8u type,
					 int8u cmdId,
					 int8u *cmdParam);
void uartParseSendInit (pCallbackHandle pUartParseSend);

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
void uartSendCommand(int8u txPacketLength,
					 int8u type,
					 int8u cmdId,
					 int8u *cmdParam){
	int8u i;
	int8u packetCheckXor = 0xFF;
	lengthTxBuff = txPacketLength + 2;

	uartTxBuff[0] = PacketSOF_1;         // 1byte SOF
	uartTxBuff[1] = PacketSOF_2;        // 1byte SOF
	uartTxBuff[2] = txPacketLength;    //1byte length
	uartTxBuff[3] = g_byTxSeq;        //1byte g_byTxSeq
	uartTxBuff[4] = type;            //1byte type
	uartTxBuff[5] = cmdId;			//1byte cmd id
	memcpy(&uartTxBuff[6],cmdParam,(txPacketLength - 4));

	for (i = 2; i < lengthTxBuff -1; i++){
		packetCheckXor ^= uartTxBuff[i];
	}
	uartTxBuff[lengthTxBuff - 1] = packetCheckXor;   // 1byte checkXor
	uartSendData (&uartTxBuff[0], lengthTxBuff);
	g_byTxSeq++;

}
void uartParseSendInit (pCallbackHandle pUartParseSend){
	if (pUartParseSend != NULL){
		uartDriverHandleAckInit(pUartParseSend);
	}
	else uartDriverHandleAckInit(&uartParseSendHandle);
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
void uartParseSendHandle (int8u response){
	if (response == resultRxACK){
		// do something
	}
	else if (response == resultRxNACK){
		uartSendData (&uartTxBuff[0], lengthTxBuff);
	}
	else ;
}
