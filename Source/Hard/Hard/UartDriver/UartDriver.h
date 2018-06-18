

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
#ifndef int8u
#define int8u unsigned char
#endif

#ifndef int16u
#define int16u unsigned short
#endif

typedef void (*typeGetDataCallback)(int8u *data);
typedef void (*pCallbackHandle)(int8u byte);


typedef struct{
	int8u port;
	SerialBaudRate rate;
	SerialParity parity;
	int8u stopBits;
	typeGetDataCallback GetDataCallback;
}uartDriverInitData_str;



typedef enum{
	pirNoMotion = 0x00,
	pirMotion = 0x01,
}PirState_enum;

typedef enum{
	resultRxACK,
	resultRxNACK,
	resultRxCMD,
	resultRxErr,
	resultRxIdle,
}RESULT_RX;


#define	leUpdateButtonCmd 		 	 5
#define	leUpdatePirStatecmd 		 5
#define	leUpdateLuxCmd 		 		 6
#define	leUpdateLigthThressCmd 		 6
#define	leUpdateTimeoutCmd 		 	 6
#define	leSetupLedCmd 		 		 9
#define	leSetupRelayCmd      		 5
#define leSetupLightThressCmd		 6
#define leSetupTimeThressCmd		 6
#define leRequestCmd				 4

#define PacketSOF_1 0xAA
#define PacketSOF_2 0x55
#define PacketACK 0x06
#define PacketNACK 0x15


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
// Event control struct declaration
extern EmberEventControl uartGetCmdEventControl;
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

void uartDriverInit(uartDriverInitData_str uartDriverInitData);
void uartDriverHandleAckInit(pCallbackHandle pCallbackHandleAck);
void uartSendData(int8u *cmdData, int8u lengthTxData);


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
