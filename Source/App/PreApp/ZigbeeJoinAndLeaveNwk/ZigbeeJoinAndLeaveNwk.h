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

#ifndef _ZIGBEEJOINANDLEAVENWK_H_
#define _ZIGBEEJOINANDLEAVENWK_H_



/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include "app/framework/include/af.h"


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define checkHcConnectTime 960  // 960QS = 240S = 4p
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/


/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
extern EmberEventControl nwkJoinEventControl;
extern EmberEventControl nwkLeaveEventControl;
extern EmberEventControl checkHcConnectEventControl;
extern int8u checkHcConectionErrorCnt;
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


#endif /* 1_SOURCECODE_0_APP_PREAPP_ZIGBEEJOINANDLEAVENWK_ZIGBEEJOINANDLEAVENWK_H_ */
