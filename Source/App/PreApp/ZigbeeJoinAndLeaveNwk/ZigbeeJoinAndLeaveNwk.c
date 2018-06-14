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


int8u g_checkHcConnectCnt;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void nwkJoinEventFunction(void);
void nwkLeaveEventFunction(void);
void zigbeeLeaveNwkByButtonPress(int8u buttonState);
void checkHcConnectEventFunction(void);
void checkHcConnectionIsOk(void);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void zigbeeLeaveByButtonInit(void);
/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void zigbeeLeaveByButtonInit(void) {
	buttonCallbackInit(zigbeeLeaveNwkByButtonPress, NULL);
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


void zigbeeLeaveNwkByButtonPress(int8u buttonState){
	switch(buttonState){
	case stHold5s:         // neu nhan giu den 5s --> led sang hong
		ledTurnOn(ledColorPink);
		break;
	case rlHold5s:
		EmberNetworkStatus NetworkStatus = emberAfNetworkState();
		if ((NetworkStatus == EMBER_JOINED_NETWORK)
				|| (NetworkStatus == EMBER_JOINED_NETWORK_NO_PARENT)) {
			//send leave response
			int8u contents[ZDO_MESSAGE_OVERHEAD + 1];
			contents[0] = 0x00;
			(void) emberSendZigDevRequest(0x0000, LEAVE_RESPONSE,
					EMBER_AF_DEFAULT_APS_OPTIONS, contents, sizeof(contents));
			// refreshled by relay state
			emberLeaveNetwork();
			emberClearBindingTable();
		}
		// neu thiet bi khong trong mang - reset luon
		else {
				ledBlink(ledColorPink,4,2,ledLastStateRefresh);
			emberEventControlSetInactive(nwkLeaveEventControl);
			emberEventControlSetDelayMS(nwkLeaveEventControl, 1000);
		}
		break;
	default:		// neu chuyen sang bat ki trang thai nao khac -- > led resfresh

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
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
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
							   2000 + ((int8u)halCommonGetRandom() << 5)); // 2s + random 8s

	}
	else if (NetworkStatus == EMBER_JOINED_NETWORK) {
		//if the first join to network, device must be read network infomation
//		GetHcActiveEndPoint();
//		ledBlink(ledColorPink,400,2,ledLastStateRefresh);
		emberEventControlSetInactive(nwkJoinEventControl);

		// neu vao mang, gui trang thai theo chu ki 4 phut 1 lan
		initActiveEndpointResponseCallback(checkHcConnectionIsOk);
		emberEventControlSetInactive(checkHcConnectEventControl);
		emberEventControlSetDelayMS(checkHcConnectEventControl,
								 halCommonGetRandom());
		// hoi Lux
		getLuxValue();
		relayGetState();
		getPirState();

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

void nwkLeaveEventFunction(void) {
	halReboot();
}


/** @brief Stack Status
 *
 * This function is called by the application framework from the stack status
 * handler.  This callbacks provides applications an opportunity to be notified
 * of changes to the stack status and take appropriate action.  The return code
 * from this callback is ignored by the framework.  The framework will always
 * process the stack status after the callback returns.
 *
 * @param status   Ver.: always
 */
boolean emberAfStackStatusCallback(EmberStatus status)
{
	if(status == EMBER_NETWORK_DOWN){
		int8u NetworkStatus = emberAfNetworkState();
		if (NetworkStatus == EMBER_NO_NETWORK){
				ledBlink(ledColorPink,4,2,ledLastStateRefresh);
			emberEventControlSetInactive(nwkLeaveEventControl);
			emberEventControlSetDelayMS(nwkLeaveEventControl,1000);
		}
	}
	else if(status == EMBER_NETWORK_UP){
			ledBlink(ledColorPink,4,3,ledLastStateRefresh);
	   // gui ngay thong tin model cua thiet bi, du sao thi HC cung hoi nen khong can gui cung duoc
		zbSendBasicModelAttributeResponse();
	}
	return false;
}

/**
 * @emberIncomingManyToOneRouteRequestHandler
 *
 * @brief  gui gia tri cam bien anh sang moi khi nhan duoc ban tin manytooneroute request
 * muc dich de duy tri ket noi
 *
 * @param  None
 *
 * @retval None
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
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void checkHcConnectionIsOk(void){
    g_checkHcConnectCnt = 0;
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
void checkHcConnectEventFunction(void){
	emberEventControlSetInactive(checkHcConnectEventControl);
	emberEventControlSetDelayQS(checkHcConnectEventControl,checkHcConnectTime);
	ZbSendZdoGetHcActiveEndpoint();
	g_checkHcConnectCnt++;
	if(g_checkHcConnectCnt >= 10){
		halReboot(); // neu 10 lan loi lien tiep // cho reset.
	}
}
