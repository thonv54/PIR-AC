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
#include "1_SourceCode/1_Mid/Button/MidButton.h"
#include "1_SourceCode/0_App/PreApp/ZigbeeSend/ZigbeeSend.h"
#include "1_SourceCode/0_App/PreApp/ZigbeeUtility/ZigbeeDefine.h"
#include "1_SourceCode/1_Mid/RelayControl/RelayControl.h"
#include "1_SourceCode/1_Mid/Sensor/Sensor.h"
#include "1_SourceCode/CustomLib/macro.h"
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
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void zbSendRelayState(boolean State);
void zbSendPirState(boolean State);
void zbSendLuxValue(int16u Value);
void zbSendLightThress(int16u Value);
void zbSendPirTimeout(int16u Value);
void zbSendInit(void);


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
void zbSendInit(void){
	sensorPirCallbackInit(zbSendPirState);
	sensorPirTimeoutCallbackInit(zbSendPirTimeout);
	sensorLightThressCallbackInit(zbSendLightThress);
	sensorLuxvalueCallbackInit(zbSendLuxValue);
	relayCallbackInit(zbSendRelayState);
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
void zbSendRelayState(boolean State){
	SendGlobalServerToClientReadAttributeReport(zbLightEp,
			ZCL_ON_OFF_CLUSTER_ID,
			ZCL_ON_OFF_ATTRIBUTE_ID,
			(int8u*)&State,
			ZCL_BOOLEAN_ATTRIBUTE_TYPE);
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

void zbSendPirState(boolean State){
	int16u zoneStatusBitmap;
	if(State == boolPirNoMotion){
		zoneStatusBitmap = PIR_NO_MOTION;
	}
	else if (State == boolPirMotion){
		zoneStatusBitmap = PIR_MOTION_DETECTED;
	}

	SendGlobalServerToClientReadAttributeReport(zbLightEp,
			ZCL_IAS_ZONE_CLUSTER_ID,
			ZCL_ZONE_STATUS_ATTRIBUTE_ID,
			(int8u*)&zoneStatusBitmap,
			ZCL_BITMAP16_ATTRIBUTE_TYPE);

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

void zbSendLuxValue(int16u Value){
	SendGlobalServerToClientReadAttributeReport(zbLuxEp,
												ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,
												ZCL_ILLUM_MEASURED_VALUE_ATTRIBUTE_ID,
												(int8u*)&Value,
												ZCL_INT16U_ATTRIBUTE_TYPE);
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

void zbSendLightThress(int16u Value){
	SendGlobalServerToClientReadAttributeReport(zbLuxEp,
												ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,
												ZCL_ILLUM_MIN_MEASURED_VALUE_ATTRIBUTE_ID,
												(int8u*)&Value,
												ZCL_INT16U_ATTRIBUTE_TYPE);
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

void zbSendPirTimeout(int16u Value){
	SendGlobalServerToClientReadAttributeReport(zbLuxEp,
												ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,
												ZCL_ILLUM_MAX_MEASURED_VALUE_ATTRIBUTE_ID,
												(int8u*)&Value,
												ZCL_INT16U_ATTRIBUTE_TYPE);
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

void SendGlobalServerToClientReadAttributeResponse(int8u endpoint,
        										   EmberAfClusterId clusterId,
												   EmberAfAttributeId attributeId,
												   int8u* value,
												   int8u dataType) {
    EmberReadAttributeResponse_str attributeReadResponse;
    int16u AttributeReadResponseLength;


    attributeReadResponse.AttributeID = attributeId;  // on-off
    attributeReadResponse.Status = 0x00;   // success
    attributeReadResponse.DataType = dataType; //bool
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
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */

void SendGlobalServerToClientReadAttributeReport(int8u endpoint,
        										EmberAfClusterId clusterId,
												EmberAfAttributeId attributeId,
												int8u* value,
												int8u dataType) {
	EmberReadAttributeResport_str attributeReadReport;
    int16u attributeReadReportLength;

    attributeReadReport.AttributeID = attributeId;  // on-off
    attributeReadReport.DataType = dataType; //bool
    memcpy(attributeReadReport.Value, value, emberAfGetDataSize(dataType));
    attributeReadReportLength = sizeof(attributeReadReport.AttributeID)
            + sizeof(attributeReadReport.DataType) + emberAfGetDataSize(dataType);

    emberAfFillCommandGlobalServerToClientReportAttributes(clusterId,
            &attributeReadReport, attributeReadReportLength);
    emberAfSetCommandEndpoints(endpoint, gHcEndpoint);
    emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);
}



