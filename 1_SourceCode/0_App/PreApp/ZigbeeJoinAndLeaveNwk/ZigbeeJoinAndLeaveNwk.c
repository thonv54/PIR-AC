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
#include "1_SourceCode/0_App/PreApp/ZigbeeJoinAndLeaveNwk/ZigbeeJoinAndLeaveNwk.h"
#include "1_SourceCode/1_Mid/Button/MidButton.h"
#include "1_SourceCode/1_Mid/LedControl/LedControl.h"
#include "include/ember-types.h"
#include "zigbee-framework/zigbee-device-common.h"
#include "1_SourceCode/CustomLib/macro.h"
#include "include/error-def.h"
#include "stdbool.h"
#include "1_SourceCode/0_App/PreApp/ZigbeeSend/ZigbeeSend.h"
#include "PIR_AC.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/



/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
EmberEventControl nwkLeaveEventControl;
EmberEventControl nwkJoinEventControl;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void nwkJoinEventFunction(void);
void nwkLeaveEventFunction(void);
void zigbeeLeaveNwkByButtonPress(int8u buttonState);
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
		ledGetState();     // lay trang thai led
		ledTurnOn(ledColorPink);
		break;
	case rlHold5s:
		int8u NetworkStatus = emberAfNetworkState();
		if ((NetworkStatus == EMBER_JOINED_NETWORK)
				|| (NetworkStatus == EMBER_JOINED_NETWORK_NO_PARENT)) {
			//send leave response
			int8u contents[ZDO_MESSAGE_OVERHEAD + 1];
			contents[0] = 0x00;
			(void) emberSendZigDevRequest(0x0000, LEAVE_RESPONSE,
					EMBER_AF_DEFAULT_APS_OPTIONS, contents, sizeof(contents));
			// refreshled
			switch(currentLedColor){
			case ledColorBlue:
			case ledColorPink:
			case ledColorRed:
				ledTurnOn(currentLedColor);
				break;
			case ledColorOff:
				ledTurnOff();
				break;
			}
			emberLeaveNetwork();
			emberClearBindingTable();
		}
		// neu thiet bi khong trong mang - reset luon
		else {

			ledBlink(ledColorPink,400,2,ledLastStateRefresh);
			emberEventControlSetInactive(nwkLeaveEventControl);
			emberEventControlSetDelayMS(nwkLeaveEventControl, 1000);
		}
		break;
	default:		// neu chuyen sang bat ki trang thai nao khac -- > led resfresh
		switch(currentLedColor){
		case ledColorBlue:
		case ledColorPink:
		case ledColorRed:
			ledTurnOn(currentLedColor);
			break;
		case ledColorOff:
			ledTurnOff();
			break;
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


			emberSetTxPowerMode(EMBER_AF_PLUGIN_NETWORK_FIND_RADIO_TX_POWER);
			emberAfStartSearchForJoinableNetwork();
			emberEventControlSetInactive(nwkJoinEventControl);
			emberEventControlSetDelayMS(nwkJoinEventControl,
							   5000 + ((int8u)halCommonGetRandom() << 5)); // 5s + random 8s

	}
	else if (NetworkStatus == EMBER_JOINED_NETWORK) {
		//if the first join to network, device must be read network infomation
//		GetHcActiveEndPoint();
//		ledBlink(ledColorPink,400,2,ledLastStateRefresh);
		emberEventControlSetInactive(nwkJoinEventControl);

//		// neu vao mang, gui trang thai theo chu ki 4 phut 1 lan
//		emberEventControlSetInactive(SendDeviceStatusEventControl);
//		emberEventControlSetDelayQS(SendDeviceStatusEventControl,
//								 ((int8u) halCommonGetRandom() << 2));
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
			ledBlink(ledColorPink,400,2,ledLastStateRefresh);
			emberEventControlSetInactive(nwkLeaveEventControl);
			emberEventControlSetDelayMS(nwkLeaveEventControl,1000);
		}
	}
	else if(status == EMBER_NETWORK_UP){
		ledBlink(ledColorPink,400,3,ledLastStateRefresh);
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
		getLuxValue();
	}
}
