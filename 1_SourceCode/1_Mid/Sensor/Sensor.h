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
#include "app/framework/util/config.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef struct {
	boolean pirCurrentState;
	boolean pirLastState;
	int16u luxValue;
	int16u lightThress;
	int16u pirTimeout;
}sensorData_str;

typedef enum{
	boolPirMotion = 1,
	boolPirNoMotion = 0,
}boolPirState_enum;


typedef void (*typeSensorPirCallback)(boolean state);
typedef void (*typeSensorLuxValueCallback)(int16u value);
typedef void (*typeSensorLightThressCallback)(int16u value);
typedef void (*typeSensorPirTimeoutCallback)(int16u value);
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
void sensorPirCallbackInit(typeSensorPirCallback sensorPirCallback);
void sensorLuxvalueCallbackInit(typeSensorLuxValueCallback sensorLuxValueCallback);
void sensorLightThressCallbackInit(typeSensorLightThressCallback SensorLightThressCallback );
void sensorPirTimeoutCallbackInit(typeSensorPirTimeoutCallback sensorPirTimeoutCallback);
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




#endif /* 1_SOURCECODE_1_MID_SENSOR_SENSOR_H_ */
