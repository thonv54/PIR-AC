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

#ifndef _BUTTON_H_
#define _BUTTON_H_



/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include "app/framework/include/af.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef enum{
	stPress = 0,
	stHold3s = 1,
	stHold5s = 2,
	stHold10s = 3,
	rlPress  = 4,
	rlHold3s = 5,
	rlHold5s = 6,
	rlHold10s = 7,
	Press2 = 8,
	Press3 = 9,
	Press4 = 10,
	Press5 = 11,
	PressManyTime = 12,
}buttonState_enum;


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @function     :
 *
 * @brief        :
 *
 * @parameter    :
 *
 * @return value :
 */

void buttonCallbackInit(byteCallbackFunc buttonHandleCallback);


#endif /* 1_SOURCECODE_1_MID_BUTTON_BUTTON_H_ */
