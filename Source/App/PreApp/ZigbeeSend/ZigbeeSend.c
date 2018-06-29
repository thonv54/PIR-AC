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
#include <Source/App/PreApp/ZigbeeSend/ZigbeeSend.h>
#include <Source/App/PreApp/ZigbeeUtility/ZigbeeDefine.h>
#include <Source/CustomLib/macro.h>
#include <Source/Mid/Button/MidButton.h>
#include <Source/Mid/LedControl/LedControl.h>
#include <Source/Mid/RelayControl/RelayControl.h>
#include <Source/Mid/Sensor/Sensor.h>
#include "app/framework/include/af.h"
#include "app/framework/util/config.h"
#include "app/util/zigbee-framework/zigbee-device-common.h"
#include "cluster-id.h"
#include "attribute-id.h"
#include "attribute-type.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

typedef struct {
  int16u AttributeID;
  int8u Status;
  int8u DataType;
  int8u Value[32];
} EmberReadAttributeResponse_str;

#define EmberReadAttributeResport_str EmberReadAttributeResponse_str



/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

int8u gHcEndpoint = HcEndpointDefault;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void SendGlobalServerToClientReadAttributeReport(int8u endpoint,
        										EmberAfClusterId clusterId,
												EmberAfAttributeId attributeId,
												int8u* value,
												int8u dataType);
void SendGlobalServerToClientReadAttributeResponse(int8u endpoint,
        										   EmberAfClusterId clusterId,
												   EmberAfAttributeId attributeId,
												   int8u* value,
												   int8u dataType);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void zbSendRelayState(bool_t State);
void zbSendPirState(bool_t State);
void zbSendLuxValue(int16u Value);
void zbSendLightThress(int16u Value);
void zbSendPirTimeout(int32u Value);
void ZbSendZdoGetHcActiveEndpoint(void);
void zbSendInit(void);


/******************************************************************************/

// Check connect to HC
void ZbSendZdoGetHcActiveEndpoint(void) {
    emberActiveEndpointsRequest(0x000, EMBER_AF_DEFAULT_APS_OPTIONS | 0x0040);
}

/**
 * @function     : zbSendInit
 *
 * @brief        : initialize callback
 *
 * @parameter    : None
 *
 * @return value : None
 */

void zbSendInit(void){
	sensorPirCallbackInit(zbSendPirState);
	sensorPirTimeoutCallbackInit(zbSendPirTimeout);
	sensorLightThressCallbackInit(zbSendLightThress);
	sensorLuxvalueCallbackInit(zbSendLuxValue);
	relayCallbackInit(zbSendRelayState);
}

/**
 * @function     : zbSendRelayState
 *
 * @brief        : send relay state to HC
 *
 * @parameter    : state
 *
 * @return value : None
 */

void zbSendRelayState(bool_t State){
	SendGlobalServerToClientReadAttributeResponse(zbLightEp,
			ZCL_ON_OFF_CLUSTER_ID,
			ZCL_ON_OFF_ATTRIBUTE_ID,
			(int8u*)&State,
			ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}

/**
 * @function     : zbSendPirState
 *
 * @brief        : Send PIR State to HC
 *
 * @parameter    : state
 *
 * @return value : None
 */

void zbSendPirState(bool_t State){
	int16u zoneStatusBitmap;
	if(State == boolPirNoMotion){
		zoneStatusBitmap = PIR_NO_MOTION;
	}
	else if (State == boolPirMotion){
		zoneStatusBitmap = PIR_MOTION_DETECTED;
	}

	SendGlobalServerToClientReadAttributeResponse(zbLightEp,
			ZCL_IAS_ZONE_CLUSTER_ID,
			ZCL_ZONE_STATUS_ATTRIBUTE_ID,
			(int8u*)&zoneStatusBitmap,
			ZCL_BITMAP16_ATTRIBUTE_TYPE);

}

/**
 * @function     : zbSendLuxValue
 *
 * @brief        : Send LUX value to HC
 *
 * @parameter    : value
 *
 * @return value :
 */

void zbSendLuxValue(int16u Value){
	SendGlobalServerToClientReadAttributeResponse(zbLuxEp,
												ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,
												ZCL_ILLUM_MEASURED_VALUE_ATTRIBUTE_ID,
												(int8u*)&Value,
												ZCL_INT16U_ATTRIBUTE_TYPE);
}

/**
 * @function     : zbSendLightThress
 *
 * @brief        : Send light Threshold to HC
 *
 * @parameter    : value
 *
 * @return value : None
 */

void zbSendLightThress(int16u Value){
	SendGlobalServerToClientReadAttributeResponse(zbLuxEp,
												ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,
												ZCL_ILLUM_MIN_MEASURED_VALUE_ATTRIBUTE_ID,
												(int8u*)&Value,
												ZCL_INT16U_ATTRIBUTE_TYPE);
}

/**
 * @function     : zbSendPirTimeout
 *
 * @brief        : Send time value to HC
 *
 * @parameter    : value
 *
 * @return value : None
 */

void zbSendPirTimeout(int32u Value){

	SendGlobalServerToClientReadAttributeResponse(zbLuxEp,
												ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,
												ZCL_ILLUM_MAX_MEASURED_VALUE_ATTRIBUTE_ID,
												(int8u*)&Value,
												ZCL_INT32U_ATTRIBUTE_TYPE);
}

/**
 * @function     : zbSendBasicModelAttributeResponse
 *
 * @brief        : Send information to HC
 *
 * @parameter    : None
 *
 * @return value : None
 */

void zbSendBasicModelAttributeResponse(void){
	int8u data[64] = {9,'L','M','-','P','I','R','-','A','C',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	SendGlobalServerToClientReadAttributeResponse(
			zbLightEp,
			ZCL_BASIC_CLUSTER_ID,
			ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID,
			(int8u*)&data,
			ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
}

/**
 * @function     : zbSendBasicManufacturerAttributeResponse
 *
 * @brief        : Send manufacturer information to HC
 *
 * @parameter    : None
 *
 * @return value : None
 */

void zbSendBasicManufacturerAttributeResponse(void){
	int8u data[64] = {4,'L','u','m','i',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	SendGlobalServerToClientReadAttributeResponse(
			zbLightEp,
			ZCL_BASIC_CLUSTER_ID,
			ZCL_MANUFACTURER_NAME_ATTRIBUTE_ID,
			(int8u*)&data,
			ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
}

/**
 * @function     : SendGlobalServerToClientReadAttributeResponse
 *
 * @brief        : Send data to HC
 *
 * @parameter    :
 *
 * @return value : None
 */

void SendGlobalServerToClientReadAttributeResponse(int8u endpoint,
        										   EmberAfClusterId clusterId,
												   EmberAfAttributeId attributeId,
												   int8u* value,
												   int8u dataType) {
    EmberReadAttributeResponse_str attributeReadResponse;
    int16u AttributeReadResponseLength;


    attributeReadResponse.AttributeID = attributeId;  // on-off
    attributeReadResponse.Status = 0x00;   // success
    attributeReadResponse.DataType = dataType; //boolean
    memcpy(attributeReadResponse.Value, value, emberAfGetDataSize(dataType));
    AttributeReadResponseLength = sizeof(attributeReadResponse.AttributeID)
            + sizeof(attributeReadResponse.Status)
            + sizeof(attributeReadResponse.DataType) + emberAfGetDataSize(dataType);

    emberAfFillCommandGlobalServerToClientReadAttributesResponse(clusterId,
            &attributeReadResponse, AttributeReadResponseLength);
    emberAfSetCommandEndpoints(endpoint, gHcEndpoint);
    emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);
}

/**
 * @function     : SendGlobalServerToClientReadAttributeReport
 *
 * @brief        :
 *
 * @parameter    : None
 *
 * @return value : None
 */

void SendGlobalServerToClientReadAttributeReport(int8u endpoint,
        										EmberAfClusterId clusterId,
												EmberAfAttributeId attributeId,
												int8u* value,
												int8u dataType) {
	EmberReadAttributeResport_str attributeReadReport;
    int16u attributeReadReportLength;

    attributeReadReport.AttributeID = attributeId;  // on-off
    attributeReadReport.DataType = dataType; //boolean
    memcpy(attributeReadReport.Value, value, emberAfGetDataSize(dataType));
    attributeReadReportLength = sizeof(attributeReadReport.AttributeID)
            + sizeof(attributeReadReport.DataType) + emberAfGetDataSize(dataType);

    emberAfFillCommandGlobalServerToClientReportAttributes(clusterId,
            &attributeReadReport, attributeReadReportLength);
    emberAfSetCommandEndpoints(endpoint, gHcEndpoint);
    emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);
}



