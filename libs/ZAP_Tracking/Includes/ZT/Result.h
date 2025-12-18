
// Author  KMS - Martin Dubois, P. Eng.
// Client  ZAP
// Product Tacking
// File    Includes/ZT/Result.h

#pragma once

namespace ZT
{

    // Data types
    /////////////////////////////////////////////////////////////////////////

    typedef enum
    {
        ZT_OK,
        ZT_OK_REPLACED,

        ZT_OK_QTY,

        ZT_ERROR = 0x100,
        ZT_ERROR_ACTION,
        ZT_ERROR_ALREADY_STARTED,
        ZT_ERROR_ALREADY_STOPPED,
        ZT_ERROR_ALREADY_STOPPING,
        ZT_ERROR_ANGLE_MAX,
        ZT_ERROR_ANGLE_MIN,
        ZT_ERROR_CMD_ID,
        ZT_ERROR_CMD_SET,
        ZT_ERROR_CMD_TYPE,
        ZT_ERROR_CODE,
        ZT_ERROR_CONFIG,
        ZT_ERROR_CONTROL,
        ZT_ERROR_ENCODED,
        ZT_ERROR_EXCEPTION,
        ZT_ERROR_FILE_OPEN,
        ZT_ERROR_FRAME_TOO_LONG,
        ZT_ERROR_FRAME_TOO_SHORT,
        ZT_ERROR_FUNCTION,
        ZT_ERROR_GIMBAL,
        ZT_ERROR_GIMBAL_OFF,
        ZT_ERROR_GIMBAL_VERSION,
        ZT_ERROR_MAX,
        ZT_ERROR_MIN,
        ZT_ERROR_NOT_A_GAMEPAD,
        ZT_ERROR_NOT_READY,
        ZT_ERROR_OPERATION,
        ZT_ERROR_PROTOCOL,
        ZT_ERROR_RECEIVE,
        ZT_ERROR_RECEIVER,
        ZT_ERROR_RESULT,
        ZT_ERROR_SEND,
        ZT_ERROR_SPEED,
        ZT_ERROR_SPEED_MAX,
        ZT_ERROR_SPEED_MIN,
        ZT_ERROR_STATE,
        ZT_ERROR_THREAD,
        ZT_ERROR_TIMEOUT,
        ZT_ERROR_TODO,

        ZT_ERROR_QTY,

        ZT_RESULT_INVALID = 0x7fff
    }
    Result;

    // Functions
    /////////////////////////////////////////////////////////////////////////

    extern void Result_Display(Result aIn, void * aOut = NULL);

    extern const char * Result_GetName(Result aIn);

}
