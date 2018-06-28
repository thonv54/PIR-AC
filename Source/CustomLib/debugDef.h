/*
 * debugDef.h
 *
 *  Created on: Jun 28, 2018
 *      Author: Long
 */

#ifndef SOURCE_CUSTOMLIB_DEBUGDEF_H_
#define SOURCE_CUSTOMLIB_DEBUGDEF_H_


/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

//#define DebugUartDriver
#ifdef DebugUartDriver
#define DBG_UART_DRIVER_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_UART_DRIVER_PRINT(...)
#endif

//#define DebugCmdParse
#ifdef DebugCmdParse
#define DBG_CMD_PARSE_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_CMD_PARSE_PRINT(...)
#endif

//#define DebugButton
#ifdef DebugButton
#define DBG_BUTTON_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_BUTTON_PRINT(...)
#endif

//#define DebugLed
#ifdef DebugLed
#define DBG_LED_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_LED_PRINT(...)
#endif

//#define DebugUartRelay
#ifdef DebugRelay
#define DBG_RELAY_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_RELAY_PRINT(...)
#endif

//#define DebugSensor
#ifdef DebugSensor
#define DBG_SENSOR_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_SENSOR_PRINT(...)
#endif

//#define DebugZigBeeNwk
#ifdef DebugZigBeeNwk
#define DBG_ZB_NWK_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_ZB_NWK_PRINT(...)
#endif

//#define DebugZigBeeRx
#ifdef DebugZigBeeRx
#define DBG_ZB_RECEIVER_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_ZB_RECEIVER_PRINT(...)
#endif

//#define DebugZigBeeTx
#ifdef DebugZigBeeTx
#define DBG_ZB_SEND_PRINT(...) emberSerialPrintf(APP_SERIAL, __VA_ARGS__)
#else
#define	DBG_ZB_SEND_PRINT(...)
#endif

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/




#endif /* SOURCE_CUSTOMLIB_DEBUGDEF_H_ */
