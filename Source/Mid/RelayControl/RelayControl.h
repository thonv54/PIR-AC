/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: Thonv
 *
 * Last Changed By:  $Author: Thonv $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date: 2016-05-17 14:20:00 (Tue, 17 May 2016) $
 *
 ******************************************************************************/
#ifndef _RELAYCONTROL_H_
#define _RELAYCONTROL_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "app/framework/include/af.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef struct{
	boolean relayCurrentState;
	boolean relayLastState;
	int32u LastTimeFromGetState;
}RelayData_str;

typedef enum{
	boolRlOn = 1,
	boolRlOff = 0,
}boolRelayState_enum;


typedef enum{
    rlOffState = 0x00,
    rlOnState = 0x01,
}RelayState_enum;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
extern RelayData_str gRelay;
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void relayCallbackInit(boolCallbackFunc relayCallback);
void relayGetState(void);



/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */

#endif /* _MAIN_H_ */
