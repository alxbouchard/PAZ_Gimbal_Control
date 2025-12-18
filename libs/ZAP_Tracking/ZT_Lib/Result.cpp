
// Author  KMS - Martin Dubois, P. Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Stats.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <ZT/Result.h>

namespace ZT
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void Result_Display(Result aIn, void * aOut)
    {
        FILE * lOut = (NULL == aOut) ? stdout : static_cast<FILE *>(aOut);

        fprintf(lOut, "%s\n", Result_GetName(aIn));
    }

    const char * Result_GetName(Result aIn)
    {
        const char * lResult;

        switch(aIn)
        {
            case ZT::ZT_OK: lResult = "ZT_OK"; break;
            
            case ZT::ZT_OK_REPLACED: lResult = "ZT_OK_REPLACED"; break;

            case ZT::ZT_ERROR                 : lResult = "ZT_ERROR"                 ; break;
            case ZT::ZT_ERROR_ACTION          : lResult = "ZT_ERROR_ACTION"          ; break;
            case ZT::ZT_ERROR_ALREADY_STARTED : lResult = "ZT_ERROR_ALREADY_STARTED" ; break;
            case ZT::ZT_ERROR_ALREADY_STOPPED : lResult = "ZT_ERROR_ALREADY_STOPPED" ; break;
            case ZT::ZT_ERROR_ALREADY_STOPPING: lResult = "ZT_ERROR_ALREADY_STOPPING"; break;
            case ZT::ZT_ERROR_ANGLE_MAX       : lResult = "ZT_ERROR_ANGLE_MAX"       ; break;
            case ZT::ZT_ERROR_ANGLE_MIN       : lResult = "ZT_ERROR_ANGLE_MIN"       ; break;
            case ZT::ZT_ERROR_CMD_ID          : lResult = "ZT_ERROR_CMD_ID"          ; break;
            case ZT::ZT_ERROR_CMD_SET         : lResult = "ZT_ERROR_CMD_SET"         ; break;
            case ZT::ZT_ERROR_CMD_TYPE        : lResult = "ZT_ERROR_CMD_TYPE"        ; break;
            case ZT::ZT_ERROR_CODE            : lResult = "ZT_ERROR_CODE"            ; break;
            case ZT::ZT_ERROR_CONFIG          : lResult = "ZT_ERROR_CONFIG"          ; break;
            case ZT::ZT_ERROR_CONTROL         : lResult = "ZT_ERROR_CONTROL"         ; break;
            case ZT::ZT_ERROR_ENCODED         : lResult = "ZT_ERROR_ENCODED"         ; break;
            case ZT::ZT_ERROR_EXCEPTION       : lResult = "ZT_ERROR_EXCEPTION"       ; break;
            case ZT::ZT_ERROR_FILE_OPEN       : lResult = "ZT_ERROR_FILE_OPEN"       ; break;
            case ZT::ZT_ERROR_FRAME_TOO_LONG  : lResult = "ZT_ERROR_FRAME_TOO_LONG"  ; break;
            case ZT::ZT_ERROR_FRAME_TOO_SHORT : lResult = "ZT_ERROR_FRAME_TOO_SHORT" ; break;
            case ZT::ZT_ERROR_FUNCTION        : lResult = "ZT_ERROR_FUNCTION"        ; break;
            case ZT::ZT_ERROR_GIMBAL          : lResult = "ZT_ERROR_GIMBAL"          ; break;
            case ZT::ZT_ERROR_GIMBAL_OFF      : lResult = "ZT_ERROR_GIMBAL_OFF"      ; break;
            case ZT::ZT_ERROR_GIMBAL_VERSION  : lResult = "ZT_ERROR_GIMBAL_VERSION"  ; break;
            case ZT::ZT_ERROR_MAX             : lResult = "ZT_ERROR_MAX"             ; break;
            case ZT::ZT_ERROR_MIN             : lResult = "ZT_ERROR_MIN"             ; break;
            case ZT::ZT_ERROR_NOT_A_GAMEPAD   : lResult = "ZT_ERROR_NOT_A_GAMEPAD"   ; break;
            case ZT::ZT_ERROR_NOT_READY       : lResult = "ZT_ERROR_NOT_READY"       ; break;
            case ZT::ZT_ERROR_OPERATION       : lResult = "ZT_ERROR_OPERATION"       ; break;
            case ZT::ZT_ERROR_PROTOCOL        : lResult = "ZT_ERROR_PROTOCOL"        ; break;
            case ZT::ZT_ERROR_RECEIVE         : lResult = "ZT_ERROR_RECEIVE"         ; break;
            case ZT::ZT_ERROR_RECEIVER        : lResult = "ZT_ERROR_RECEIVE"         ; break;
            case ZT::ZT_ERROR_RESULT          : lResult = "ZT_ERROR_RESULT"          ; break;
            case ZT::ZT_ERROR_SEND            : lResult = "ZT_ERROR_SEND"            ; break;
            case ZT::ZT_ERROR_SPEED           : lResult = "ZT_ERROR_SPEED"           ; break;
            case ZT::ZT_ERROR_SPEED_MAX       : lResult = "ZT_ERROR_SPEED_MAX"       ; break;
            case ZT::ZT_ERROR_SPEED_MIN       : lResult = "ZT_ERROR_SPEED_MIN"       ; break;
            case ZT::ZT_ERROR_STATE           : lResult = "ZT_ERROR_STATE"           ; break;
            case ZT::ZT_ERROR_THREAD          : lResult = "ZT_ERROR_THREAD"          ; break;
            case ZT::ZT_ERROR_TIMEOUT         : lResult = "ZT_ERROR_TIMEOUT"         ; break;
            case ZT::ZT_ERROR_TODO            : lResult = "ZT_ERROR_TODO"            ; break;

            case ZT::ZT_RESULT_INVALID: lResult = "ZT_RESULT_INVALID"; break;

            default:
                static char sResult[64];
                sprintf(sResult, "Invalid ZT::Result value (%u, 0x%x)", aIn, aIn);
                lResult = sResult;
        }

        return lResult;
    }

}