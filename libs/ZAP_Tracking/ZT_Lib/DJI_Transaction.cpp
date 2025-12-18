
// Author  KMS - Martin Dubois, P. Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/DJI_Transaction.cpp

#include "Component.h"

// ===== ZT_Lib =============================================================
#include "Gimbal.h"

#include "DJI_Transaction.h"

// Static variables
// //////////////////////////////////////////////////////////////////////////

static unsigned int sSerial = 0;

// Public
// //////////////////////////////////////////////////////////////////////////

DJI_Transaction::DJI_Transaction() : mCode(0), mReceiver(NULL), mRxExpected_byte(0)
{
    Reset();
}

void DJI_Transaction::Complete(ZT::Result aResult)
{
    assert(ZT::ZT_RESULT_INVALID != aResult);

    assert(ZT::ZT_RESULT_INVALID == mResult);

    mResult = aResult;

    if (NULL != mReceiver)
    {
        mReceiver->ProcessMessage(this, mCode, NULL);
    }
}

void DJI_Transaction::Frame_Angle_Set(unsigned int aOffset_byte, double aIn_deg)
{
    mTxFrame.Angle_Set(aOffset_byte, aIn_deg);
}

uint8_t DJI_Transaction::Frame_Data_Get(unsigned int aOffset_byte) const
{
    assert(sizeof(mTxFrame.mData) > aOffset_byte);

    return mTxFrame.mData[aOffset_byte];
}

DJI_Frame * DJI_Transaction::Frame_Get()
{
    return & mTxFrame;
}

void DJI_Transaction::Frame_Init_ANGLE_GET()
{
    Frame_Init(3, DJI_CMD_TYPE_DO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_ANGLE_GET);

    mTxFrame.mData[2] = 0x01;
}

void DJI_Transaction::Frame_Init_ANGLE_LIMIT_GET()
{
    Frame_Init(3, DJI_CMD_TYPE_DO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_ANGLE_LIMIT_GET);

    mTxFrame.mData[2] = 0x01;
}

void DJI_Transaction::Frame_Init_ANGLE_LIMIT_SET(const ZT::IGimbal::Config & aConfig)
{
    Frame_Init(8, DJI_CMD_TYPE_DO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_ANGLE_LIMIT_SET);

    FOR_EACH_AXIS(a)
    {
        static const unsigned int OFFSETS[ZT::IGimbal::AXIS_QTY] = { 2, 7, 5 };

        mTxFrame.mData[OFFSETS[a]    ] = (0.0 < aConfig.mAxis[a].mMax_deg) ?   aConfig.mAxis[a].mMax_deg : 0.0;
        mTxFrame.mData[OFFSETS[a] + 1] = (0.0 > aConfig.mAxis[a].mMin_deg) ? - aConfig.mAxis[a].mMin_deg : 0.0;
    }

    mTxFrame.mData[2] = 0x01;
}

void DJI_Transaction::Frame_Init_FOCUS_CAL(ZT::IGimbal::Operation aOperation)
{
    static const uint8_t OP_CODES[ZT::IGimbal::OPERATION_QTY] = { 0x01, 0x02, 0x05, 0x04, 0x06 };

    assert(ZT::IGimbal::OPERATION_QTY > aOperation);

    Frame_Init(5, DJI_CMD_TYPE_NO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_FOCUS);

    mTxFrame.mData[2] = DJI_CMD_FOCUS_CAL;
        
    mTxFrame.mData[4] = OP_CODES[aOperation];
}

void DJI_Transaction::Frame_Init_FOCUS_SET(double aValue_pc)
{
    assert(0.0 <= aValue_pc);
    assert(100.0 >= aValue_pc);

    double lValue = aValue_pc;

    lValue /= 100.0;
    lValue *= 4095;

    int16_t lValueInt16 = lValue;

    Frame_Init(7, DJI_CMD_TYPE_NO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_FOCUS);

    mTxFrame.mData[2] = DJI_CMD_FOCUS_SET;
    mTxFrame.mData[4] = 0x02;
    mTxFrame.mData[5] = lValueInt16 & 0xff;
    mTxFrame.mData[6] = lValueInt16 >> 8;
}

void DJI_Transaction::Frame_Init_MOTOR_STIFFNESS_GET()
{
    Frame_Init(3, DJI_CMD_TYPE_DO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_MOTOR_STIFFNESS_GET);

    mTxFrame.mData[2] = 0x01;
}

void DJI_Transaction::Frame_Init_MOTOR_STIFFNESS_SET(const ZT::IGimbal::Config & aConfig)
{
    Frame_Init(6, DJI_CMD_TYPE_DO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_MOTOR_STIFFNESS_SET);

    FOR_EACH_AXIS(a)
    {
        mTxFrame.mData[3 + a] = aConfig.mAxis[a].mStiffness_pc;
    }

    mTxFrame.mData[2] = 0x01;
}

void DJI_Transaction::Frame_Init_POSITION_SET(const ZT::IGimbal::Position & aIn, unsigned int aFlags, unsigned int aDuration_ms)
{
    Frame_Init(10, DJI_CMD_TYPE_NO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_POSITION_SET);

    mTxFrame.mData[8] = 0x01;

    FOR_EACH_AXIS(a)
    {
        static const uint8_t      FLAGS  [ZT::IGimbal::AXIS_QTY] = { 0x08, 0x04, 0x02 };
        static const unsigned int OFFSETS[ZT::IGimbal::AXIS_QTY] = { 6, 4, 2 };

        if (0 == (aFlags & ZT_FLAG_IGNORE(a)))
        {
            Frame_Angle_Set(OFFSETS[a], aIn.mAxis_deg[a]);
        }
        else
        {
            mTxFrame.mData[8] |= FLAGS[a];
        }
    }

    mTxFrame.mData[9] = aDuration_ms / 100;
}

void DJI_Transaction::Frame_Init_SPEED_SET(const ZT::IGimbal::Speed & aSpeed)
{
    Frame_Init(9, DJI_CMD_TYPE_NO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_SPEED_SET);

    FOR_EACH_AXIS(a)
    {
        unsigned int OFFSETS[ZT::IGimbal::AXIS_QTY] = { 6, 4, 2 };

        mTxFrame.Speed_Set(OFFSETS[a], aSpeed.mAxis_deg_s[a]);
    }

    mTxFrame.mData[8] = 0x88;
}

void DJI_Transaction::Frame_Init_TLV_SET(double aSpeed_pc)
{
    assert(0.0 <= aSpeed_pc);
    assert(100.0 >= aSpeed_pc);

    Frame_Init(5, DJI_CMD_TYPE_DO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_TLV_SET);

    mTxFrame.mData[2] = 0x75;
    mTxFrame.mData[3] = 1;
    mTxFrame.mData[4] = static_cast<uint8_t>(aSpeed_pc / 100.0 * 29.0 + 1);
}

void DJI_Transaction::Frame_Init_TRACK_SWITCH()
{
    Frame_Init(3, DJI_CMD_TYPE_NO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_TRACK_SWITCH);

    mTxFrame.mData[2] = 0x03;
}

void DJI_Transaction::Frame_Init_VERSION()
{
    Frame_Init(6, DJI_CMD_TYPE_DO_REPLY, DJI_CMD_SET_DEFAULT, DJI_CMD_VERSION);

    mTxFrame.mData[2] = 1;
}

bool DJI_Transaction::IsOK() const
{
    return ZT::ZT_OK == mResult;
}

void DJI_Transaction::Prepare(ZT::IMessageReceiver * aReceiver, unsigned int aCode)
{
    assert(NULL != aReceiver);

    assert(NULL == mReceiver);

    mCode     = aCode;
    mReceiver = aReceiver;
}

void DJI_Transaction::Prepare(ZT::IMessageReceiver * aReceiver, unsigned int aCode, unsigned int aRxExpected_byte)
{
    Prepare(aReceiver, aCode);
    Prepare(aRxExpected_byte);
}

void DJI_Transaction::Prepare(unsigned int aRxExpected_byte)
{
    assert(0 < aRxExpected_byte);

    assert(0 == mRxExpected_byte);

    mRxExpected_byte = DJI_FRAME_TOTAL_SIZE(aRxExpected_byte) - DJI_FOOTER_SIZE_byte;
}

void DJI_Transaction::Reset()
{
    mResult = ZT::ZT_RESULT_INVALID;
}

ZT::Result DJI_Transaction::Result_Get() const
{
    return mResult;
}

void DJI_Transaction::Result_Set(ZT::Result aResult)
{
    assert(ZT::ZT_RESULT_INVALID != aResult);

    mResult = aResult;
}

unsigned int DJI_Transaction::RxExpected_Get() const
{
    return mRxExpected_byte;
}

void DJI_Transaction::RxTimeout_Set(unsigned int aIn_tick)
{
    mRxTimeout_tick = aIn_tick;
}

void DJI_Transaction::Started(ZT::Result aResult)
{
    if ((0 >= mRxExpected_byte) || (ZT::ZT_OK != aResult))
    {
        Complete(aResult);
    }
}

void DJI_Transaction::Tick()
{
    switch (mRxTimeout_tick)
    {
    case 0: break;

    case 1: Complete(ZT::ZT_ERROR_TIMEOUT); break;

    default: mRxTimeout_tick --;
    }
}

ZT::Result DJI_Transaction::Wait(ZT_Lib::Thread * aThread)
{
    assert(NULL != aThread);

    ZT::Result lResult = ZT::ZT_OK;

    while ((ZT::ZT_RESULT_INVALID == mResult) && (ZT::ZT_OK == lResult))
    {
        lResult = aThread->Condition_Wait();
    }

    if (ZT::ZT_OK == lResult)
    {
        lResult = mResult;
    }

    return lResult;
}

// Private
// //////////////////////////////////////////////////////////////////////////

void DJI_Transaction::Frame_Init(uint8_t aDataSize_byte, uint8_t aCmdType, uint8_t aCmdSet, uint8_t aCmdId)
{
    mTxFrame.Init(aDataSize_byte, aCmdType, aCmdSet, aCmdId, ++ sSerial);
}
