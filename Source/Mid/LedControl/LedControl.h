
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

#ifndef _LEDCONTROL_H_
#define _LEDCONTROL_H_


/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "app/framework/include/af.h"


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

typedef enum{
	ledColorOff = 0,
	ledColorBlue = 1,
	ledColorRed = 2,
	ledColorPink = 3,
}ledColor_enum;

typedef enum{
	ledStateOn = 0,
	ledStateOff = 1,
	ledStateBlink = 2,
}ledControlState_enum;

typedef struct{
	int8u LedColor;
	int8u LedControlState;
	int8u TimeDelayBlink;
	int8u BlinkTimes;
	int8u LastState;
}ledParam_str;

typedef enum{
	ledLastStateOff = 0,
	ledLastStateRed = 1,
	ledLastStateBlue = 2,
	ledLastStatePink = 3,
	ledLastStateRefresh = 4,
}ledLastState_enum;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/


/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void ledTurnOn(int8u color);
void ledTurnOff(void);
void ledBlink(int8u color,
				int8u timeDelay,
				int8u times,
				int8u lastState);



#endif /* 1_SOURCECODE_1_MID_LEDCONTROL_LEDCONTROL_H_ */
