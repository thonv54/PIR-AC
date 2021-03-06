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
#include <Source/App/PreApp/ZigbeeJoinAndLeaveNwk/ZigbeeJoinAndLeaveNwk.h>
#include <Source/App/PreApp/ZigbeeSend/ZigbeeSend.h>
#include <Source/CustomLib/macro.h>
#include <Source/Hard/Hard/UartDriver/UartDriver.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmdParse.h>
#include <Source/Hard/SubHard/UartCmdParse/UartCmdParseSend.h>
#include <Source/Mid/LedControl/LedControl.h>
#include "app/framework/include/af.h"
#include "stack/include/event.h"
#include "hal/micro/serial.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
EmberEventControl TestEventControl;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void TestEventFunction(void);


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

void TestEventFunction(void) {
	emberEventControlSetInactive(TestEventControl);
	emberEventControlSetDelayMS(TestEventControl, 1000);
//	ledTurnOn(ledColorBlue);
}

/** @brief Main Init
 *
 * This function is called from the application's main function. It gives the
 * application a chance to do any initialization required at system startup. Any
 * code that you would normally put into the top of the application's main()
 * routine should be put into this function. This is called before the clusters,
 * plugins, and the network are initialized so some functionality is not yet
 * available.
        Note: No callback in the Application Framework is
 * associated with resource cleanup. If you are implementing your application on
 * a Unix host where resource cleanup is a consideration, we expect that you
 * will use the standard Posix system calls, including the use of atexit() and
 * handlers for signals such as SIGTERM, SIGINT, SIGCHLD, SIGPIPE and so on. If
 * you use the signal() function to register your signal handler, please mind
 * the returned value which may be an Application Framework function. If the
 * return value is non-null, please make sure that you call the returned
 * function from your handler to avoid negating the resource cleanup of the
 * Application Framework itself.
 *
 */
void emberAfMainInitCallback(void)
{
	uartDriverInitData_str uartInitData;
	uartInitData.GetDataCallback = NULL;
	uartInitData.port = COM_USART0;
	uartInitData.rate = BAUD_19200;
	uartInitData.parity = PARITY_NONE;
	uartInitData.stopBits = 1;
	pCallbackHandle uartParseSendInit = NULL;

	uartCmdParseInit(uartInitData);
	uartParseSendInit(uartParseSendInit);
	zbSendInit();
	zigbeeLeaveByButtonInit();

	emberEventControlSetDelayMS(uartGetCmdEventControl,1000); //

	emberEventControlSetDelayMS(nwkJoinEventControl,1000); //
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
