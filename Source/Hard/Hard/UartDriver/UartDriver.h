

#ifndef _UARTDRIVER_H_
#define _UARTDRIVER_H_
/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: Thonv
 *
 * Last Changed By:  $Author: Thonv $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date: 2016-05-17 14:20:00 (Tue, 17 May 2016) $
 *
 ******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include "app/framework/include/af.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

#define MAX_BUFF              10
#define MAX_LENGTH_CMD_TX     20
#define PERIOD_RX_MS          70    // check receiver per T =70 milliseconds
#define TIME_WAIT_RES         5    //  wait response after n = 50 times check receiver. n x T milliseconds

#define RX_TIMEOUT            500  // 500ms

#define CXOR_INIT_VAL         0xFF
#define RX_BUFF               1024
#define MaxLengthUARTPacket   15
#define MinLengthUARTPacket   4

#define PacketSOF_1           0xAA
#define PacketSOF_2           0x55
#define PacketACK             0x06
#define PacketNACK            0x15

typedef struct _UART_RX_STR_{
	byte_t DataReceiverStep;
	byte_t DataPacketLength;
	byte_t Data[RX_BUFF];
	byte_t ReceiverByteCount;
}uartRx_Str;

typedef struct _UNIT_BUFF_{
	byte_t dataTx[MAX_LENGTH_CMD_TX];
	byte_t lengthTx;
	byteCallbackFunc callbackFuncTx;
} UNIT_BUFF, *UNIT_BUFF_p;

typedef struct _SEQUENCE_BUFF_{
	byte_t topBuff;
	byte_t bottomBuff;
	byte_t numberUnit;
} SEQUENCE_BUFF, *SEQUENCE_BUFF_p;

typedef enum{
	buffFull,
	buffEmpty,
	buffSuccess,
} RESULT_BUFF;

typedef enum{
	stateSOF1,
	stateSOF2,
	stateLength,
	stateData,
	stateCheckXor,
} RECEIVER_STEP;

typedef enum{
	resultRxACK,
	resultRxNACK,
	resultRxSUCCESS,
	resultRxErr,
	resultRxTimeout,
	resultRxIdle,
}RESULT_RX;

typedef enum{
	resultTxSUCCESS,
	resultTxFALSE,
	resultTxTimeout,
}RESULT_TX;



/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

extern EmberEventControl uartGetCmdEventControl;
extern EmberEventControl uartSendCmdEventControl;

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

void uartDriverInit(byte_t portUartInit, byte_pCallbackFunc pRxCallbackFuncInit);
byte_t WriteOnTopBuffTx (byte_p data, byte_t lengthData, byteCallbackFunc TxCallbackFunc);


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


#endif /* 1_SOURCECODE_2_HARD_HARD_UARTDRIVER_UARTDRIVER_H_ */
