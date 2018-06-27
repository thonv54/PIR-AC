/*******************************************************************************
 *
 * Copyright (c) 2017
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: Thonv
 *
 * Last Changed By:  $Author: Thonv $
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date:  20/04/2018 $
 *
 ******************************************************************************/
#ifndef _UART_H_
#define _UART_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include <Source/CustomLib/typedefs.h>
#include "app/framework/include/af.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define CMD_TYPE_REQUEST    0
#define CMD_TYPE_RESPONSE   1
#define CMD_TYPE_SETUP      2
#define CMD_TYPE_UPDATE     3

#define CMD_ID_PIR          0
#define CMD_ID_BUTTON       1
#define CMD_ID_LED          2
#define CMD_ID_RELAY        3
#define CMD_ID_LUX          4
#define CMD_ID_LIGHT_THRES  5
#define CMD_ID_TIMEOUT      6
#define CMD_ID_ERROR        7


#define BUTTON_PRESS        0
#define BUTTON_HOLD3S       1
#define BUTTON_HOLD5S       2
#define BUTTON_HOLD10s      3
#define BUTTON_RELEASE      0xFF

#define RELAY_ON_STATE      0x00
#define RELAY_OFF_STATE     0x01

#define PIR_STATE_NO_MOTION       0x00
#define PIR_STATE_MOTION          0x01


//Length of send command
#define leUpdateButtonCmd            5
#define leUpdatePirStatecmd          5
#define leUpdateLuxCmd               6
#define leUpdateLigthThressCmd       6
#define leUpdateTimeoutCmd           6
#define leSetupLedCmd                9
#define leSetupRelayCmd              5
#define leSetupLightThressCmd        6
#define leSetupTimeThressCmd         6
#define leRequestCmd                 4


typedef struct _common_ {
    byte_t cmdseq;
    byte_t cmdtype;
    byte_t cmdid;
} COMMON;

typedef struct _pir_state_ {
    byte_t cmdseq;
    byte_t cmdtype;
    byte_t cmdId;
    byte_t state;
} PIRSTATE;

typedef struct _button_state_ {
    byte_t cmdseq;
    byte_t cmdtype;
    byte_t cmdId;
    byte_t state;
} BUTTONSTATE;

typedef struct _led_state_ {
    byte_t cmdseq;
    byte_t cmdtype;
    byte_t cmdId;
    byte_t state;
} LEDSTATE;

typedef struct _relay_state_ {
    byte_t cmdseq;
    byte_t cmdtype;
    byte_t cmdId;
    byte_t state;
} RELAYSTATE;

typedef struct _lux_data_ {
    byte_t cmdseq;
    byte_t cmdtype;
    byte_t cmdId;
    byte_t high_byte_Lux;
    byte_t low_byte_Lux;
} LUXDATA;

typedef struct _light_thress_data_ {
    byte_t cmdseq;
    byte_t cmdtype;
    byte_t cmdId;
    byte_t high_byte_LightThress;
    byte_t low_byte_LightThress;
} LIGHTTHRESSDATA;

typedef struct _light_time_data_ {
    byte_t cmdseq;
    byte_t cmdtype;
    byte_t cmdId;
    byte_t highest_byte_TimeThress;
    byte_t high_byte_TimeThress;
    byte_t low_byte_TimeThress;
    byte_t lowest_byte_TimeThress;
} TIMETHRESSDATA;

typedef union _CMD_BUFFER_ {
    COMMON uCommon;
    BUTTONSTATE uButtonState;
    PIRSTATE uPirState;
    LEDSTATE uLedState;
    RELAYSTATE uRelayState;
    LUXDATA uLuxData;
    LIGHTTHRESSDATA uLightThressData;
    TIMETHRESSDATA uTimeThressData;
} CMD_BUFFER, *CMD_BUFFER_P;

#endif /* END FILE */
