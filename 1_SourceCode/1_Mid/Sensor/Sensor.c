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
#include "1_SourceCode/1_Mid/Sensor/Sensor.h"
#include "1_SourceCode/2_Hard/SubHard/UartCmdParse/UartCmdParse.h"
#include "1_SourceCode/2_Hard/Hard/UartDriver/UartDriver.h"
#include "1_SourceCode/CustomLib/macro.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
//#define  DebugSensor

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typeSensorPirCallback pvSensorPirCallback;
typeSensorLuxValueCallback  pvSensorLuxValueCallback;
typeSensorLightThressCallback pvSensorLightThressCallback;
typeSensorPirTimeoutCallback pvSensorPirTimeoutCallback;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
sensorData_str gSensor;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void pvSensorHandle(int8u *data);
void errorMidSensorCallbackPrint(void);
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
void sensorPirCallbackInit(typeSensorPirCallback sensorPirCallback){
	cmdParseSensorCallbackInit(pvSensorHandle);
	if(sensorPirCallback != NULL){
		pvSensorPirCallback = sensorPirCallback;
	}
}
void sensorLuxvalueCallbackInit(typeSensorLuxValueCallback sensorLuxValueCallback){
	cmdParseSensorCallbackInit(pvSensorHandle);
	if(sensorLuxValueCallback!= NULL){
		pvSensorLuxValueCallback = sensorLuxValueCallback;
	}
}
void sensorLightThressCallbackInit(typeSensorLightThressCallback SensorLightThressCallback ){
	cmdParseSensorCallbackInit(pvSensorHandle);
	if(SensorLightThressCallback != NULL){
		pvSensorLightThressCallback = SensorLightThressCallback;
	}
}
void sensorPirTimeoutCallbackInit(typeSensorPirTimeoutCallback sensorPirTimeoutCallback){
	cmdParseSensorCallbackInit(pvSensorHandle);
	if(sensorPirTimeoutCallback != NULL){
		pvSensorPirTimeoutCallback = sensorPirTimeoutCallback;
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
void pvSensorHandle(int8u *data){

	int8u cmdType = data[1];
	int8u cmdId = data[2];

	if((cmdType == CMD_TYPE_UPDATE) || (cmdType == CMD_TYPE_RESPONSE)){
		switch (cmdId){
		case CMD_ID_PIR:
			int8u pirState = data[3];
			if(pirState == pirMotion){
				gSensor.pirCurrentState = boolPirMotion;
			}
			else if(pirState == pirNoMotion){
				gSensor.pirCurrentState = boolPirNoMotion;
			}
			if(pvSensorPirCallback != NULL){
				pvSensorPirCallback(gSensor.pirCurrentState);
#ifdef DebugSensor
	emberSerialPrintf(APP_SERIAL,"    pvSensorPirCallback  \n\r");
#endif
			}
			else{
				errorMidSensorCallbackPrint();
			}
			break;
		case CMD_ID_LUX:
			gSensor.luxValue = (((int16u)data[3]<< 8) | (data[4])) ;
			if(pvSensorLuxValueCallback != NULL){
				pvSensorLuxValueCallback(gSensor.luxValue);
#ifdef DebugSensor
	emberSerialPrintf(APP_SERIAL,"    pvSensorLuxValueCallback  \n\r");
#endif
			}
			else{
				errorMidSensorCallbackPrint();
			}
			break;
		case CMD_ID_LIGHT_THRES:
			gSensor.lightThress = (((int16u)data[3]<< 8) | (data[4]));
			if(pvSensorLightThressCallback != NULL){
				pvSensorLightThressCallback(gSensor.lightThress);
#ifdef DebugSensor
	emberSerialPrintf(APP_SERIAL,"    pvSensorLightThressCallback  \n\r");
#endif
			}
			else{
				errorMidSensorCallbackPrint();
			}
			break;;
		case CMD_ID_TIMEOUT:
			gSensor.pirTimeout = (((int32u)data[3]<< 24) | ((int32u)data[4]<<16) | ((int32u)data[5]<< 8) | data[6]);
			if(pvSensorPirTimeoutCallback != NULL){
				pvSensorPirTimeoutCallback(gSensor.pirTimeout);
#ifdef DebugSensor
	emberSerialPrintf(APP_SERIAL,"    pvSensorPirTimeoutCallback  \n\r");
#endif
			}
			else{
				errorMidSensorCallbackPrint();
			}
			break;
		default:
			break;
		}
	}
	gSensor.pirLastState = gSensor.pirCurrentState;
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
void errorMidSensorCallbackPrint(void){
	emberSerialPrintf(APP_SERIAL,"    CallbackInMidSensorError \n\r");
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
void getLuxValue(void){
	uartSendCommand(leRequestCmd,CMD_TYPE_REQUEST,CMD_ID_LUX,NULL);
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
void getPirState(void){
	uartSendCommand(leRequestCmd,CMD_TYPE_REQUEST,CMD_ID_PIR,NULL);
}

