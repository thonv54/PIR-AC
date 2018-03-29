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
#include <1_SourceCode/CustomLib/macro.h>
#include "1_SourceCode/1_Mid/Button/Button.h"
#include "1_SourceCode/2_Hard/SubHard/UartCmdParse/UartCmdParse.h"
#include "app/framework/include/af.h"
#include "app/framework/util/config.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/


typedef enum{
	stPress = 0,    // bat dau nhan nut
	stHold3s = 1,	// thoi diem giu toi 3s
	stHold5s = 2,
	stHold10s = 3,
	rlPress  = 4,
	rlHold3s = 5,
	rlHold5s = 6,
	rlHold10s = 7,
	Press2 = 8,
	Press3 = 9,
	Press4 = 10,
	Press5 = 11,
	PressManyTime = 12,
}buttonState_enum;

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
	if(buttonPressCallback != buttonPressCallback){
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
void buttonStateFromUartHandle(int8u *data){

	pvButton.ButtonCurrentState = data[3];
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
			break;
		}
		break;
	default:
		break;
	}
	pvButton.ButtonLastState = pvButton.ButtonCurrentState;

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
	emberSerialPrintf(APP_SERIAL,"    CallbackInMidButtonError \n\r");
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
