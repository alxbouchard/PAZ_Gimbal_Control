
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Stats.h

#pragma once

// Class
/////////////////////////////////////////////////////////////////////////////

class Stats
{

public:

    Stats();

    void Display(FILE * aOut);

    unsigned int mDelay;
    unsigned int mDelay_ms;
    unsigned int mPos_Error;
    unsigned int mPos_Error_Last;
    unsigned int mPos_Get;
    unsigned int mPos_Get_Error;
    ZT::Result   mPos_Get_Error_Last;
    unsigned int mPos_Get_Unknown;
    unsigned int mPos_Process;
    unsigned int mPos_Request;
    unsigned int mPos_Set;
    unsigned int mPos_Valid;
    unsigned int mRetry;
    unsigned int mRx_byte;
    unsigned int mRx_frame;
    unsigned int mRx_CmdId;
    uint8_t      mRx_CmdId_Last;
    unsigned int mRx_CmdSet;
    uint8_t      mRx_CmdSet_Last;
    unsigned int mRx_CmdType;
    uint8_t      mRx_CmdType_Last;
    unsigned int mRx_Encoded;
    uint8_t      mRx_Encoded_Last;
    unsigned int mRx_Id;
    uint32_t     mRx_Id_Last;
    unsigned int mRx_Overflow;
    unsigned int mRx_Result;
    uint8_t      mRx_Result_Last;
    unsigned int mRx_SOF;
    unsigned int mRx_SOF_Last;
    unsigned int mRx_TooLong;
    unsigned int mRx_TooShort;
    unsigned int mRx_Unexpected;
    unsigned int mRx_Unordered;
    unsigned int mRx_Version;
    uint8_t      mRx_Version_Last;
    unsigned int mTx_byte;
    unsigned int mTx_frame;
    unsigned int mTx_Error;
    unsigned int mWait_Timeout;

};
