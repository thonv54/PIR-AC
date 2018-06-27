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
#include <Source/Mid/LedControl/LedControl.h>
#include "app/framework/include/af.h"
#include "stack/include/event.h"
#include "hal/micro/serial.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

#define DebugCmdParse

#ifdef DebugCmdParse
#define DBG_CMD_PARSE_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_CMD_PARSE_PRINT(...)
#endif

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
//void testTxCallback (byte_t result);

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
	/*byte_t color[10] = {50, 51, 52, 53, 54, 55, 56, 57, 58, 59};

	uartSendCommand (9, 2, 2, color, testTxCallback);
	uartSendCommand (9, 2, 2, &color[3], testTxCallback);

	emberEventControlSetInactive(TestEventControl);
	emberEventControlSetDelayMS(TestEventControl, 10000);
	*/
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

/*void testTxCallback (byte_t result){
	switch (result){
	case resultTxSUCCESS:
		DBG_CMD_PARSE_PRINT ("SUCCESS!");
		break;
	case resultTxFALSE:
		DBG_CMD_PARSE_PRINT ("FALSE!");
		break;
	default:
		break;
	}
}
*/

void emberAfMainInitCallback(void)
{
	uartParseDataInit(COM_USART0, NULL);
	zbSendInit();
	zigbeeLeaveByButtonInit();
	//emberEventControlSetDelayMS(TestEventControl, 1000);
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
