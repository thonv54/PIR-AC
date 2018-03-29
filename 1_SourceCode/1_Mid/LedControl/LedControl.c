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
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/




/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void ledTurnOn(ledColor_enum color);
void ledTurnOff(void);
void ledBlink(ledColor_enum color,
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
void ledTurnOn(ledColor_enum color){
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
void ledBlink(ledColor_enum color,
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
