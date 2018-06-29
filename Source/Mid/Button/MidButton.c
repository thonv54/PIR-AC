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

#include <Source/Mid/LedControl/LedControl.h>
#include "app/framework/include/af.h"
#include "app/framework/util/config.h"
#include <Source/CustomLib/macro.h>
#include <Source/CustomLib/debugDef.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmd.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmdParse.h>
#include <Source/Mid/Button/MidButton.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/



typedef struct{
	byte_t ButtonCurrentState;
	byte_t ButtonLastState;
	uint_t LastTimeFromGetState;
}ButtonData_str;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

byteCallbackFunc pvButtonHandleCallback;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void buttonStateFromUartHandle(byte_t *data);

ButtonData_str pvButton;

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

void buttonCallbackInit(byteCallbackFunc buttonHandleCallback);

/**
 * @function     :
 *
 * @brief        :
 *
 * @parameter    :
 *
 * @return value :
 */

void buttonCallbackInit (byteCallbackFunc buttonHandleCallback){
	cmdParseButtonCallbackInit(buttonStateFromUartHandle);
	if (buttonHandleCallback != NULL){
		pvButtonHandleCallback = buttonHandleCallback;
	}
}


/**
 * @function     :
 *
 * @brief        :
 *
 * @parameter    :
 *
 * @return value :
 */

void buttonStateFromUartHandle(byte_t* data){

	pvButton.ButtonCurrentState = (byte_t)*data;
	switch(pvButton.ButtonCurrentState){
	case BUTTON_HOLD10s:
		break;
	case BUTTON_HOLD5S:
		DBG_BUTTON_PRINT("Button hold5s." );
		if(pvButtonHandleCallback != NULL){
			pvButtonHandleCallback(stHold5s);
		}
		break;
	case BUTTON_HOLD3S:
		break;
	case BUTTON_PRESS:
		if(pvButtonHandleCallback != NULL){
			pvButtonHandleCallback(stPress);
		}
		break;
	case BUTTON_RELEASE:
		switch(pvButton.ButtonLastState){
		case BUTTON_PRESS:
			if(pvButtonHandleCallback != NULL){
				pvButtonHandleCallback(rlPress);
			}
			break;
		case BUTTON_HOLD5S:
			DBG_BUTTON_PRINT("Button release hold5s." );
			if(pvButtonHandleCallback != NULL){
				pvButtonHandleCallback(rlHold5s);
			}
			break;
		default:
			if(pvButtonHandleCallback != NULL){
				pvButtonHandleCallback(rlPress);
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
 * @function     :
 *
 * @brief        :
 *
 * @parameter    :
 *
 * @return value :
 */
