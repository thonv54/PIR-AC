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


/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/


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
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
 */

void ledTurnOn(byte_t color){
	ledParam_str ledParam;
	ledParam.LedColor = color;
	ledParam.LedControlState = ledStateOn;
	uartSendCommand(leSetupLedCmd,CMD_TYPE_SETUP,CMD_ID_LED,&ledParam.LedColor,NULL);
}

/**
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
 */
void ledTurnOff(void){
	ledParam_str ledParam;
	ledParam.LedColor = ledColorPink;
	ledParam.LedControlState = ledStateOff;
	uartSendCommand(leSetupLedCmd,CMD_TYPE_SETUP,CMD_ID_LED,&ledParam.LedColor, NULL);
}

/**
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
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
 * @function      :
 *
 * @brief         :
 *
 * @parameter     :
 *
 * @return value  :
 */
