
// Author  KMS - Martin Dubois, P. Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/DJI_Transaction.h

#pragma once

// ===== Includes ===========================================================
#include <ZT/IGimbal.h>
#include <ZT/IMessageReceiver.h>

// ===== ZT_Lib =============================================================
#include "DJI.h"
#include "ZT_Lib/Thread.h"

class Thread;

class DJI_Transaction
{

public:

    DJI_Transaction();

    void Complete(ZT::Result aResult);

    void Frame_Angle_Set(unsigned int aOffset_byte, double aIn_deg);

    uint8_t Frame_Data_Get(unsigned int aOffset_byte) const;

    DJI_Frame * Frame_Get();

    void Frame_Init_ANGLE_GET();
    void Frame_Init_ANGLE_LIMIT_GET();
    void Frame_Init_ANGLE_LIMIT_SET(const ZT::IGimbal::Config & aConfig);
    void Frame_Init_FOCUS_CAL(ZT::IGimbal::Operation aOperation);
    void Frame_Init_FOCUS_SET(double aIn_pc);
    void Frame_Init_MOTOR_STIFFNESS_GET();
    void Frame_Init_MOTOR_STIFFNESS_SET(const ZT::IGimbal::Config & aConfig);
    void Frame_Init_POSITION_SET(const ZT::IGimbal::Position & aIn, unsigned int aFlags, unsigned int aDuration_ms);
    void Frame_Init_SPEED_SET(const ZT::IGimbal::Speed & aSpeed);
    void Frame_Init_TLV_SET(double aSpeed_pc);
    void Frame_Init_TRACK_SWITCH();
    void Frame_Init_VERSION();

    bool IsOK() const;

    void Prepare(ZT::IMessageReceiver * aReceiver, unsigned int aCode);
    void Prepare(ZT::IMessageReceiver * aReceiver, unsigned int aCode, unsigned int aRxExpected_byte);
    void Prepare(unsigned int aRxExpected_byte);

    void Reset();

    ZT::Result Result_Get() const;
    void       Result_Set(ZT::Result aIn);

    unsigned int RxExpected_Get() const;

    void RxTimeout_Set(unsigned int aIn_tick);

    void Started(ZT::Result aResult);

    void Tick();

    ZT::Result Wait(ZT_Lib::Thread * aThread);

private:

    void Frame_Init(uint8_t aDataSize_byte, uint8_t aCmdType, uint8_t aCmdSet, uint8_t aCmdId);

    unsigned int           mCode;
    ZT::IMessageReceiver * mReceiver;

    ZT::Result mResult;

    unsigned int mRxExpected_byte;
    unsigned int mRxTimeout_tick;

    DJI_Frame mTxFrame;

};
