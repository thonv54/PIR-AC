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
#include "include/ember-types.h"
#include "zigbee-framework/zigbee-device-common.h"
#include "include/error-def.h"
#include "stdbool.h"
#include "PIR_AC.h"
#include <Source/App/PreApp/ZigbeeJoinAndLeaveNwk/ZigbeeJoinAndLeaveNwk.h>
#include <Source/App/PreApp/ZigbeeReceiver/ZigbeeReceiver.h>
#include <Source/App/PreApp/ZigbeeSend/ZigbeeSend.h>
#include <Source/CustomLib/macro.h>
#include <Source/Mid/Button/MidButton.h>
#include <Source/Mid/LedControl/LedControl.h>
#include <Source/Mid/RelayControl/RelayControl.h>
#include <Source/Mid/Sensor/Sensor.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

typedef void (*checkHcConnectCallback)(void);

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/



/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
EmberEventControl checkHcConnectEventControl;
EmberEventControl nwkLeaveEventControl;
EmberEventControl nwkJoinEventControl;


byte_t g_checkHcConnectCnt;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void nwkJoinEventFunction(void);
void nwkLeaveEventFunction(void);
void zigbeeLeaveNwkByButtonHandle(byte_t buttonState);
void checkHcConnectEventFunction(void);
void checkHcConnectionIsOk(void);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void zigbeeLeaveByButtonInit(void);

/**
 * @function     :
 *
 * @brief        :
 *
 * @parameter    :
 *
 * @return value :
 */

void zigbeeLeaveByButtonInit(void) {
	buttonCallbackInit(zigbeeLeaveNwkByButtonHandle);
	emberEventControlSetDelayMS(nwkJoinEventControl,1000);
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

void zigbeeLeaveNwkByButtonHandle(byte_t buttonState){
	switch(buttonState){
	case stHold5s:         // hold 5s --> led on pink
		ledTurnOn(ledColorPink);
		break;
	case rlHold5s:
		EmberNetworkStatus NetworkStatus = emberAfNetworkState();
		if ((NetworkStatus == EMBER_JOINED_NETWORK)
				|| (NetworkStatus == EMBER_JOINED_NETWORK_NO_PARENT)) {
			//send leave response
			byte_t contents[ZDO_MESSAGE_OVERHEAD + 1];
			contents[0] = 0x00;
			(void) emberSendZigDevRequest(0x0000, LEAVE_RESPONSE,
					EMBER_AF_DEFAULT_APS_OPTIONS, contents, sizeof(contents));
			// refresh led by relay state
			emberLeaveNetwork();
			emberClearBindingTable();
		}
		// reset if not connect
		else {
				ledBlink(ledColorPink,4,2,ledLastStateRefresh);
			emberEventControlSetInactive(nwkLeaveEventControl);
			emberEventControlSetDelayMS(nwkLeaveEventControl, 1000);
		}
		break;
	default:		// led refresh
		if(gRelay.relayCurrentState == boolRlOn){
			ledTurnOn(ledColorBlue);
		}
		else{
			ledTurnOn(ledColorRed);
		}
		break;
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

void nwkJoinEventFunction(void) {
	EmberNetworkStatus NetworkStatus;

	NetworkStatus = emberAfNetworkState();
	if (NetworkStatus == EMBER_NO_NETWORK) {
			ledBlink(ledColorRed,4,1,ledLastStateRefresh);
			emberSetTxPowerMode(EMBER_AF_PLUGIN_NETWORK_FIND_RADIO_TX_POWER);
			emberAfStartSearchForJoinableNetwork();
			emberEventControlSetInactive(nwkJoinEventControl);
			emberEventControlSetDelayMS(nwkJoinEventControl,
							   2000 + ((byte_t)halCommonGetRandom() << 5)); // 2s + random 8s

	}
	else if (NetworkStatus == EMBER_JOINED_NETWORK) {
		//if the first join to network, device must be read network information
//		GetHcActiveEndPoint();
//		ledBlink(ledColorPink,400,2,ledLastStateRefresh);
		emberEventControlSetInactive(nwkJoinEventControl);

		// if Joined in network, check connect per 4 minutes
		initActiveEndpointResponseCallback(checkHcConnectionIsOk);
		emberEventControlSetInactive(checkHcConnectEventControl);
		emberEventControlSetDelayMS(checkHcConnectEventControl,
								 halCommonGetRandom());
		// get sensor data
		getLuxValue();
		relayGetState();
		getPirState();

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

void nwkLeaveEventFunction(void) {
	halReboot();
}


/** @brief Stack Status
 *
 * This function is called by the application framework from the stack status
 * handler.  This callback provides applications an opportunity to be notified
 * of changes to the stack status and take appropriate action.  The return code
 * from this callback is ignored by the framework.  The framework will always
 * process the stack status after the callback returns.
 *
 * @function     :
 *
 * @brief        :
 *
 * @parameter    :
 *
 * @return value :
 */

boolean emberAfStackStatusCallback(EmberStatus status)
{
	if(status == EMBER_NETWORK_DOWN){
		byte_t NetworkStatus = emberAfNetworkState();
		if (NetworkStatus == EMBER_NO_NETWORK){
				ledBlink(ledColorPink,4,2,ledLastStateRefresh);
			emberEventControlSetInactive(nwkLeaveEventControl);
			emberEventControlSetDelayMS(nwkLeaveEventControl,1000);
		}
	}
	else if(status == EMBER_NETWORK_UP){
			ledBlink(ledColorPink,4,3,ledLastStateRefresh);
	   // send model of device
		zbSendBasicModelAttributeResponse();
	}
	return false;
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

void emberIncomingManyToOneRouteRequestHandler(EmberNodeId source,
                                               EmberEUI64 longId,
                                               uint8_t cost)
{
	if(source == 0x0000){
		emberEventControlSetInactive(checkHcConnectEventControl);
		emberEventControlSetDelayMS(checkHcConnectEventControl,halCommonGetRandom()>>4); // random 4s
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

void checkHcConnectionIsOk(void){
    g_checkHcConnectCnt = 0;
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

void checkHcConnectEventFunction(void){
	emberEventControlSetInactive(checkHcConnectEventControl);
	emberEventControlSetDelayQS(checkHcConnectEventControl,checkHcConnectTime);
	ZbSendZdoGetHcActiveEndpoint();
	g_checkHcConnectCnt++;
	if(g_checkHcConnectCnt >= 10){
		halReboot(); // if g_checkHcConnectCount = 10, reset device
	}
}
