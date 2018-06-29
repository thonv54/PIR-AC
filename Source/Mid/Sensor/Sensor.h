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

#ifndef _SENSOR_H_
#define _SENSOR_H_


/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include "app/framework/include/af.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef struct {
	boolean pirCurrentState;
	boolean pirLastState;
	int16u luxValue;
	int16u lightThress;
	int32u pirTimeout;
}sensorData_str;

typedef enum{
	boolPirMotion = 1,
	boolPirNoMotion = 0,
}boolPirState_enum;


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
extern sensorData_str gSensor;
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void sensorPirCallbackInit(boolCallbackFunc sensorPirCallback);
void sensorLuxvalueCallbackInit(wordCallbackFunc sensorLuxValueCallback);
void sensorLightThressCallbackInit(wordCallbackFunc SensorLightThressCallback );
void sensorPirTimeoutCallbackInit(uintCallbackFunc sensorPirTimeoutCallback);
void getLuxValue(void);
void getPirState(void);



/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */




#endif /* 1_SOURCECODE_1_MID_SENSOR_SENSOR_H_ */
