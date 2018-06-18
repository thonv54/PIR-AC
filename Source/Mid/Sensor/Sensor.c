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
#include <Source/CustomLib/macro.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmd.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmdParseSend.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmdParse.h>
#include <Source/Mid/Sensor/Sensor.h>
#include "app/framework/include/af.h"
#include "app/framework/util/config.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/



#ifdef DebugSensor
#define DBG_SENSOR_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define DBG_SENSOR_PRINT(...)
#endif
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

    CMD_BUFFER *cmd = (CMD_BUFFER*)data;

	if((cmd->uCommon.cmdtype == CMD_TYPE_UPDATE) || (cmd->uCommon.cmdtype == CMD_TYPE_RESPONSE)){
		switch (cmd->uCommon.cmdid){
		case CMD_ID_PIR:
			if(cmd->uPirState.state == PIR_STATE_MOTION){
				gSensor.pirCurrentState = boolPirMotion;
			}
			else if(cmd->uPirState.state == PIR_STATE_NO_MOTION){
				gSensor.pirCurrentState = boolPirNoMotion;
			}
			if(pvSensorPirCallback != NULL){
				pvSensorPirCallback(gSensor.pirCurrentState);
				DBG_SENSOR_PRINT("    pvSensorPirCallback  \n\r");
			}
			else{
				errorMidSensorCallbackPrint();
			}
			break;
		case CMD_ID_LUX:
			gSensor.luxValue = (((int16u)cmd->uLuxData.high_byte_Lux<< 8) | (cmd->uLuxData.low_byte_Lux)) ;
			if(pvSensorLuxValueCallback != NULL){
				pvSensorLuxValueCallback(gSensor.luxValue);

				DBG_SENSOR_PRINT("    pvSensorLuxValueCallback  \n\r");

			}
			else{
				errorMidSensorCallbackPrint();
			}
			break;
		case CMD_ID_LIGHT_THRES:
			gSensor.lightThress = (((int16u)cmd->uLightThressData.high_byte_LightThress<< 8)
			        | (cmd->uLightThressData.low_byte_LightThress));
			if(pvSensorLightThressCallback != NULL){
				pvSensorLightThressCallback(gSensor.lightThress);
				DBG_SENSOR_PRINT("    pvSensorLightThressCallback  \n\r");
			}
			else{
				errorMidSensorCallbackPrint();
			}
			break;;
		case CMD_ID_TIMEOUT:
			gSensor.pirTimeout = (((int32u)cmd->uTimeThressData.highest_byte_TimeThress<< 24) |
			        ((int32u)cmd->uTimeThressData.high_byte_TimeThress<<16) |
			        ((int32u)cmd->uTimeThressData.low_byte_TimeThress<< 8) |
			        cmd->uTimeThressData.lowest_byte_TimeThress);
			if(pvSensorPirTimeoutCallback != NULL){
				pvSensorPirTimeoutCallback(gSensor.pirTimeout);
				DBG_SENSOR_PRINT("    pvSensorPirTimeoutCallback  \n\r");
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
    DBG_SENSOR_PRINT("    CallbackInMidSensorError \n\r");
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

