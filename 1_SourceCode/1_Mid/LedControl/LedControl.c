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
#include "1_SourceCode/1_Mid/LedControl/LedControl.h"
#include "1_SourceCode/2_Hard/Hard/UartDriver/UartDriver.h"
#include "1_SourceCode/2_Hard/SubHard/UartCmdParse/UartCmdParse.h"
#include "app/framework/util/config.h"
#include "1_SourceCode/CustomLib/macro.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/



/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
int8u currentLedColor;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void errorMidLedCallbackPrint(void);
void ledResponseHandle(int8u* data);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void ledTurnOn(int8u color);
void ledTurnOff(void);
void ledBlink(int8u color,
				int8u timeDelay,
				int8u times,
				int8u lastState);


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
void ledResponseHandle(int8u* data){
	currentLedColor = data[3];
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
void ledTurnOn(int8u color){
	ledParam_str ledParam;
	ledParam.LedColor = color;
	ledParam.LedControlState = ledStateOn;
	uartSendCommand(leSetupLedCmd,CMD_TYPE_SETUP,CMD_ID_LED,&ledParam.LedColor);
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
	uartSendCommand(leSetupLedCmd,CMD_TYPE_SETUP,CMD_ID_LED,&ledParam.LedColor);
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
void ledBlink(int8u color,
				int8u timeDelay,
				int8u times,
				int8u lastState){
	ledParam_str ledParam;
	ledParam.LedColor = color;
	ledParam.LedControlState = ledStateBlink;
	ledParam.BlinkTimes = times;
	ledParam.TimeDelayBlink = timeDelay;
	ledParam.LastState = lastState;
	uartSendCommand(leSetupLedCmd,CMD_TYPE_SETUP,CMD_ID_LED,&ledParam.LedColor);
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
	uartSendCommand(leRequestCmd,CMD_TYPE_REQUEST,CMD_ID_LED,NULL);
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
void errorMidLedCallbackPrint(void){
	emberSerialPrintf(APP_SERIAL,"    CallbackInMidLedError \n\r");
}
