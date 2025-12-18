
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Stats.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <ZT/Result.h>

// ===== ZT_Lib =============================================================
#include "Stats.h"

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void Display_A(FILE * aOut, const char * aName, unsigned int aVal);
static void Display_B(FILE * aOut, const char * aName, unsigned int aVal0, unsigned int aVal1, const char * aUnit);
static void Display_C(FILE * aOut, const char * aName, unsigned int aVal, uint8_t aLast);
static void Display_D(FILE * aOut, const char * aName, unsigned int aVal, ZT::Result aLast);
static void Display_E(FILE * aOut, const char * aName, unsigned int aVal, unsigned int aSum);
static void Display_F(FILE * aOut, const char * aName, unsigned int aVal, uint32_t aLast);
static void Display_G(FILE * aOut, const char * aName, unsigned int aVal0, unsigned int aVal1, const char * aUnit0, const char * aUnit1);

// Public
/////////////////////////////////////////////////////////////////////////////

Stats::Stats()
{
    memset(this, 0, sizeof(Stats));
}

void Stats::Display(FILE * aOut)
{
    assert(NULL != aOut);

    fprintf(aOut, "    ===== Stats =====\n");

    Display_B(aOut, "Delay           ", mDelay        , mDelay_ms, "ms");
    Display_C(aOut, "Pos. Error      ", mPos_Error    , mPos_Error_Last);
    Display_A(aOut, "Pos. Get        ", mPos_Get);
    Display_D(aOut, "Pos. Get Error  ", mPos_Get_Error, mPos_Get_Error_Last);
    Display_A(aOut, "Pos. Get Unknown", mPos_Get_Unknown);
    Display_E(aOut, "Pos. Process    ", mPos_Process  , mPos_Request);
    Display_A(aOut, "Pos. Request    ", mPos_Request);
    Display_A(aOut, "Pos. Set        ", mPos_Set);
    Display_A(aOut, "Pos. Valid      ", mPos_Valid);
    Display_A(aOut, "Retry           ", mRetry);
    Display_G(aOut, "Rx              ", mRx_frame     , mRx_byte, "frames", "bytes");
    Display_C(aOut, "Rx Command Id   ", mRx_CmdId     , mRx_CmdId_Last);
    Display_C(aOut, "Rx Command Set  ", mRx_CmdSet    , mRx_CmdSet_Last);
    Display_C(aOut, "Rx Command Type ", mRx_CmdType   , mRx_CmdType_Last);
    Display_C(aOut, "Rx Encoded      ", mRx_Encoded   , mRx_Encoded_Last);
    Display_F(aOut, "Rx Id           ", mRx_Id        , mRx_Id_Last);
    Display_A(aOut, "Rx Overflow     ", mRx_Overflow);
    Display_C(aOut, "Rx Result       ", mRx_Result    , mRx_Result_Last);
    Display_C(aOut, "Rx SOF          ", mRx_SOF       , mRx_SOF_Last);
    Display_A(aOut, "Rx Too long     ", mRx_TooLong);
    Display_A(aOut, "Rx Too short    ", mRx_TooShort);
    Display_A(aOut, "Rx Unexpected   ", mRx_Unexpected);
    Display_A(aOut, "Rx Unordered    ", mRx_Unordered);
    Display_C(aOut, "Rx Version      ", mRx_Version   , mRx_Version_Last);
    Display_G(aOut, "Tx              ", mTx_frame     , mTx_byte, "frames", "bytes");
    Display_A(aOut, "Tx Error        ", mTx_Error);
    Display_A(aOut, "Wait Timeout    ", mWait_Timeout);
}

// Static functions
/////////////////////////////////////////////////////////////////////////////

void Display_A(FILE * aOut, const char * aName, unsigned int aVal)
{
    assert(NULL != aOut);
    assert(NULL != aName);

    if (0 < aVal)
    {
        fprintf(aOut, "    %s : %u\n", aName, aVal);
    }
}

void Display_B(FILE * aOut, const char * aName, unsigned int aVal0, unsigned int aVal1, const char * aUnit)
{
    assert(NULL != aOut);
    assert(NULL != aName);
    assert(NULL != aUnit);

    if (0 < aVal0)
    {
        fprintf(aOut, "    %s : %u, %u %s\n", aName, aVal0, aVal1, aUnit);
    }
}

void Display_C(FILE * aOut, const char * aName, unsigned int aVal, uint8_t aLast)
{
    assert(NULL != aOut);
    assert(NULL != aName);

    if (0 < aVal)
    {
        fprintf(aOut, "    %s : %u, 0x%02x\n", aName, aVal, aLast);
    }
}

void Display_D(FILE * aOut, const char * aName, unsigned int aVal, ZT::Result aLast)
{
    assert(NULL != aOut);
    assert(NULL != aName);

    if (0 < aVal)
    {
        fprintf(aOut, "    %s : %u, ", aName, aVal);
        Result_Display(aLast, aOut);
    }
}

void Display_E(FILE * aOut, const char * aName, unsigned int aVal, unsigned int aSum)
{
    assert(NULL != aOut);
    assert(NULL != aName);

    if (0 < aVal)
    {
        fprintf(aOut, "    %s : %u, %f %%\n", aName, aVal, static_cast<double>(aVal) * 100.0 / static_cast<double>(aSum));
    }
}

void Display_F(FILE * aOut, const char * aName, unsigned int aVal, uint32_t aLast)
{
    assert(NULL != aOut);
    assert(NULL != aName);

    if (0 < aVal)
    {
        fprintf(aOut, "    %s : %u, 0x%08x\n", aName, aVal, aLast);
    }
}

void Display_G(FILE * aOut, const char * aName, unsigned int aVal0, unsigned int aVal1, const char * aUnit0, const char * aUnit1)
{
    assert(NULL != aOut);
    assert(NULL != aName);
    assert(NULL != aUnit0);
    assert(NULL != aUnit1);

    if (0 < aVal0)
    {
        fprintf(aOut, "    %s : %u %s, %u %s\n", aName, aVal0, aUnit0, aVal1, aUnit1);
    }
}
