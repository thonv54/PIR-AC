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
#include "attribute-id.h"
#include "cluster-id.h"
#include "attribute-type.h"
#include "include/ember-types.h"
#include <Source/Hard/SubHard/UartCmdParse/UartCmd.h>
#include <Source/Mid/RelayControl/RelayControl.h>
#include <Source/App/PreApp/ZigbeeSend/ZigbeeSend.h>
#include <Source/App/PreApp/ZigbeeUtility/ZigbeeDefine.h>
#include <Source/CustomLib/typedefs.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmdParseSend.h>
#include <Source/App/PreApp/ZigbeeReceiver/ZigbeeReceiver.h>


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typeZdoActiveEndpointResponseCallback ZdoActiveEndpointResponseCallback;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/


/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/**
 *
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

void initActiveEndpointResponseCallback(typeZdoActiveEndpointResponseCallback callback){
    if(callback!= NULL){
        ZdoActiveEndpointResponseCallback = callback;
    }
}

/** @brief Pre ZDO Message Received
 *
 * This function passes the application an incoming ZDO message and gives the
 * appictation the opportunity to handle it. By default, this callback returns
 * false indicating that the incoming ZDO message has not been handled and
 * should be handled by the Application Framework.
 *
 * @param emberNodeId   Ver.: always
 * @param apsFrame   Ver.: always
 * @param message   Ver.: always
 * @param length   Ver.: always
 */
boolean emberAfPreZDOMessageReceivedCallback(EmberNodeId emberNodeId,
		EmberApsFrame* apsFrame, int8u* message, int16u length) {
	if (apsFrame->clusterId == ACTIVE_ENDPOINTS_RESPONSE) {

		int8u Status;
		int8u FirstEndpointID;
		Status = message[1];
		if (Status == EMBER_SUCCESS) {
			FirstEndpointID = message[5];
			gHcEndpoint = FirstEndpointID;
		}
		ZdoActiveEndpointResponseCallback();

	} else if (apsFrame->clusterId == SIMPLE_DESCRIPTOR_RESPONSE) {

	} else if (apsFrame->clusterId == LEAVE_REQUEST) {

	} else if (apsFrame->clusterId == END_DEVICE_BIND_RESPONSE) {

	}
	return FALSE;
}



/** @brief Illuminance Measurement Cluster Server Attribute Changed
 *
 * Server Attribute Changed
 *
 * @param endpoint Endpoint that is being initialized  Ver.: always
 * @param attributeId Attribute that changed  Ver.: always
 */
void emberAfIllumMeasurementClusterServerAttributeChangedCallback(int8u endpoint,
                                                                  EmberAfAttributeId attributeId){
	int16u value;
	if(attributeId == ZCL_ILLUM_MAX_MEASURED_VALUE_ATTRIBUTE_ID){
		emberAfReadServerAttribute(endpoint,
				ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,
				ZCL_ILLUM_MAX_MEASURED_VALUE_ATTRIBUTE_ID,
				(int8u*)&value,
				sizeof(value));
		uartSendCommand(leSetupLightThressCmd,CMD_TYPE_SETUP,CMD_ID_LIGHT_THRES,(int8u*)&value);
	}
	else if(attributeId == ZCL_ILLUM_MIN_MEASURED_VALUE_ATTRIBUTE_ID){
		emberAfReadServerAttribute(endpoint,
				ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,
				ZCL_ILLUM_MIN_MEASURED_VALUE_ATTRIBUTE_ID,
				(int8u*)&value,
				sizeof(value));
		uartSendCommand(leSetupTimeThressCmd,CMD_TYPE_SETUP,CMD_ID_TIMEOUT,(int8u*)&value);

	}
}


/** @brief On/off Cluster Off
 *
 *
 *
 */
boolean emberAfOnOffClusterOffCallback(void)
{
	int8u relayState = rlOffState;
	boolean value = 0;
	uartSendCommand(leSetupRelayCmd,CMD_TYPE_SETUP,CMD_ID_RELAY,(int8u*)&relayState);
	zbSendRelayState(value);

	return TRUE;
}

/** @brief On/off Cluster Toggle
 *
 *
 *
 */
boolean emberAfOnOffClusterToggleCallback(void)
{
  return FALSE;
}

/** @brief On/off Cluster On
 *
 *
 *
 */
boolean emberAfOnOffClusterOnCallback(void)
{
	int8u relayState = rlOnState;
	boolean value = 1;
	uartSendCommand(leSetupRelayCmd,CMD_TYPE_SETUP,CMD_ID_RELAY,(int8u*)&relayState);
	zbSendRelayState(value);
    return TRUE;
}
