/*
 * UartCmdParseSend.h
 *
 *  Created on: Jun 18, 2018
 *      Author: Long
 */

#ifndef SOURCE_HARD_SUBHARD_UARTCMDPARSE_UARTCMDPARSESEND_H_
#define SOURCE_HARD_SUBHARD_UARTCMDPARSE_UARTCMDPARSESEND_H_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <Source/Hard/Hard/UartDriver/UartDriver.h>

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/



/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void uartSendCommand(int8u txPacketLength,
					 int8u type,
					 int8u cmdId,
					 int8u *cmdParam);
void uartParseSendInit (pCallbackHandle pUartParseSend);


#endif /* SOURCE_HARD_SUBHARD_UARTCMDPARSE_UARTCMDPARSESEND_H_ */
