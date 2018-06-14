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
#include <Source/Mid/Button/MidButton.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

#ifdef DebugButton
#define DBG_BUTTON_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define DBG_BUTTON_PRINT(...)
#endif


typedef struct{
	int8u ButtonCurrentState;
	int8u ButtonLastState;
	int32u LastTimeFromGetState;
}ButtonData_str;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

typeButtonHold5sCallback pvButtonHold5sCallback;
typeButtonPressCallback pvButtonPressCallback;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/


/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void buttonStateFromUartHandle(int8u *data);

ButtonData_str pvButton;

void errorMidButtonCallbackPrint(void);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/


/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */

void buttonCallbackInit(typeButtonHold5sCallback buttonHold5sCallback,
				typeButtonPressCallback buttonPressCallback){
	cmdParseButtonCallbackInit(buttonStateFromUartHandle);
	if(buttonHold5sCallback != NULL){
		pvButtonHold5sCallback = buttonHold5sCallback;
	}
	if(buttonPressCallback != NULL){
		pvButtonPressCallback = buttonPressCallback;
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
void buttonStateFromUartHandle(int8u* data){

	pvButton.ButtonCurrentState = (int8u)*data;
	switch(pvButton.ButtonCurrentState){
	case BUTTON_HOLD10s:
		break;
	case BUTTON_HOLD5S:
		if(pvButtonHold5sCallback != NULL){
			pvButtonHold5sCallback(stHold5s);
		}
		else{
			// cb_error
			errorMidButtonCallbackPrint();
		}
		break;
	case BUTTON_HOLD3S:
		break;
	case BUTTON_PRESS:
		if(pvButtonPressCallback != NULL){
			pvButtonPressCallback(stPress);
		}
		else{
			// cb_error
			errorMidButtonCallbackPrint();
		}
		break;
	case BUTTON_RELEASE:
		switch(pvButton.ButtonLastState){
		case BUTTON_PRESS:
			if(pvButtonPressCallback != NULL){
				pvButtonPressCallback(rlPress);
			}
			else{
				// cb_error
				errorMidButtonCallbackPrint();
			}
			break;
		case BUTTON_HOLD5S:
			if(pvButtonHold5sCallback != NULL){
				pvButtonHold5sCallback(rlHold5s);
			}
			else{
				// cb_error
				errorMidButtonCallbackPrint();
			}
			break;
		default:
			if(pvButtonPressCallback != NULL){
				pvButtonPressCallback(rlPress);
			}
			else{
				// cb_error
				errorMidButtonCallbackPrint();
			}
			break;
		}
		break;
	default:
		break;
	}
	pvButton.ButtonLastState = pvButton.ButtonCurrentState;
	pvButton.LastTimeFromGetState = halCommonGetInt32uMillisecondTick();

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
void errorMidButtonCallbackPrint(void){
    DBG_BUTTON_PRINT("    CallbackInMidButtonError \n\r");
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

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
