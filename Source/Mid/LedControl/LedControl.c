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
#include <Source/Hard/SubHard/UartCmdParse/UartCmdParse.h>
#include <Source/Mid/LedControl/LedControl.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
byte_t currentLedColor;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void ledResponseHandle(byte_t* data);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void ledTurnOn(byte_t color);
void ledTurnOff(void);
void ledBlink(byte_t color,
				byte_t timeDelay,
				byte_t times,
				byte_t lastState);


/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void ledResponseCallbackInit(void){
	cmdParseLedCallbackInit(ledResponseHandle);
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
void ledResponseHandle(byte_t* data){
	currentLedColor = (byte_t)*data;
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
void ledTurnOn(byte_t color){
	ledParam_str ledParam;
	ledParam.LedColor = color;
	ledParam.LedControlState = ledStateOn;
	uartSendCommand(leSetupLedCmd,CMD_TYPE_SETUP,CMD_ID_LED,&ledParam.LedColor,NULL);
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
void ledTurnOff(void){
	ledParam_str ledParam;
	ledParam.LedColor = ledColorPink;
	ledParam.LedControlState = ledStateOff;
	uartSendCommand(leSetupLedCmd,CMD_TYPE_SETUP,CMD_ID_LED,&ledParam.LedColor, NULL);
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
void ledBlink(byte_t color,
				byte_t timeDelay,
				byte_t times,
				byte_t lastState){
	ledParam_str ledParam;
	ledParam.LedColor = color;
	ledParam.LedControlState = ledStateBlink;
	ledParam.BlinkTimes = times;
	ledParam.TimeDelayBlink = timeDelay;
	ledParam.LastState = lastState;
	uartSendCommand(leSetupLedCmd,CMD_TYPE_SETUP,CMD_ID_LED,&ledParam.LedColor,NULL);
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
void ledGetState(void){
	ledResponseCallbackInit();
	uartSendCommand(leRequestCmd,CMD_TYPE_REQUEST,CMD_ID_LED,NULL,NULL);
}
