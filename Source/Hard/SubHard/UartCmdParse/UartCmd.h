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
    int8u cmdseq;
    int8u cmdtype;
    int8u cmdid;
} COMMON;

typedef struct _pir_state_ {
    int8u cmdseq;
    int8u cmdtype;
    int8u cmdId;
    int8u state;
} PIRSTATE;

typedef struct _button_state_ {
    int8u cmdseq;
    int8u cmdtype;
    int8u cmdId;
    int8u state;
} BUTTONSTATE;

typedef struct _led_state_ {
    int8u cmdseq;
    int8u cmdtype;
    int8u cmdId;
    int8u state;
} LEDSTATE;

typedef struct _relay_state_ {
    int8u cmdseq;
    int8u cmdtype;
    int8u cmdId;
    int8u state;
} RELAYSTATE;

typedef struct _lux_data_ {
    int8u cmdseq;
    int8u cmdtype;
    int8u cmdId;
    int8u high_byte_Lux;
    int8u low_byte_Lux;
} LUXDATA;

typedef struct _light_thress_data_ {
    int8u cmdseq;
    int8u cmdtype;
    int8u cmdId;
    int8u high_byte_LightThress;
    int8u low_byte_LightThress;
} LIGHTTHRESSDATA;

typedef struct _light_time_data_ {
    int8u cmdseq;
    int8u cmdtype;
    int8u cmdId;
    int8u highest_byte_TimeThress;
    int8u high_byte_TimeThress;
    int8u low_byte_TimeThress;
    int8u lowest_byte_TimeThress;
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
