

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

typedef void (*typeGetDataCallback)(int8u *data);
typedef void (*typeUartAckCallback)(void);
typedef void (*typeUartNackCallback)(void);


typedef struct{
	int8u port;
	SerialBaudRate rate;
	SerialParity parity;
	int8u stopBits;
	typeGetDataCallback GetDataCallback;
	typeUartAckCallback UartAckCallback;
	typeUartNackCallback UartNackCallback;
}uartDriverInitData_str;




typedef enum{
	CMD_TYPE_REQUEST = 0,
	CMD_TYPE_RESPONSE = 1,
	CMD_TYPE_SETUP = 2,
	CMD_TYPE_UPDATE = 3,
}CmdType_enum;

typedef enum{
	CMD_ID_PIR 		   = 0,
	CMD_ID_BUTTON 	   = 1,
	CMD_ID_LED 		   = 2,
	CMD_ID_RELAY 	   = 3,
	CMD_ID_LUX 		   = 4,
	CMD_ID_LIGHT_THRES = 5,
	CMD_ID_TIMEOUT 	   = 6,
	CMD_ID_ERROR 	   = 7
}CmdID_enum;


typedef enum{
	BUTTON_PRESS = 0,
	BUTTON_HOLD3S = 1,
	BUTTON_HOLD5S = 2,
	BUTTON_HOLD10s = 3,
	BUTTON_RELEASE = 0xFF,
}ButtonStateID_enum;

typedef enum{
	rlOffState = 0x00,
	rlOnState = 0x01,
}RelayState_enum;

typedef enum{
	pirNoMotion = 0x00,
	pirMotion = 0x01,
}PirState_enum;

#define	leUpdateButtonCmd 		 	 5
#define	leUpdatePirStatecmd 		 5
#define	leUpdateLuxCmd 		 		 6
#define	leUpdateLigthThressCmd 		 6
#define	leUpdateTimeoutCmd 		 	 6
#define	leSetupLedCmd 		 		 9
#define	leSetupRelayCmd      		 5



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
