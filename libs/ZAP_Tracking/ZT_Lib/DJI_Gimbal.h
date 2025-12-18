
// Author  KMS - Martin Dubois, P. Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/DJI_Gimbal.h

#pragma once

// ===== C ==================================================================
#include <pthread.h>

// ===== Import/Includes ====================================================
#include <EthCAN/Device.h>

// ===== Includes ===========================================================
#include <ZT/IMessageReceiver.h>

// ===== ZT_Lib =============================================================
#include "DJI.h"
#include "DJI_Transaction.h"
#include "Gimbal.h"
#include "Stats.h"
#include "ZT_Lib/Thread.h"

// Class
/////////////////////////////////////////////////////////////////////////////

class DJI_Gimbal : public Gimbal, public ZT::IMessageReceiver
{

public:

    DJI_Gimbal(EthCAN::Device * aDevice);

    virtual ~DJI_Gimbal();

    ZT::Result Connect();

    // ===== ZT::IGimbal ====================================================

    virtual ZT::Result Activate();
    virtual ZT::Result Config_Set(const Config & aIn);
    virtual ZT::Result Focus_Cal(Operation aOperation);
    virtual ZT::Result Focus_Position_Set(double aFocus);
    virtual ZT::Result Position_Get(Position * aOut);
    virtual ZT::Result Position_Set(const Position & aIn, unsigned int aFlags, unsigned int aDuration_ms);
    virtual ZT::Result Speed_Set(const Speed & aIn, unsigned int aFlags);
    virtual ZT::Result Speed_Stop();
    virtual ZT::Result Track_Speed_Set(double aSpeed_pc);
    virtual ZT::Result Track_Switch();

    virtual void Debug(void * aOut);

    // ===== ZT::IMessageReceiver ===========================================
    virtual bool ProcessMessage(void * aSender, unsigned int aCode, const void * aData);

// Internal

    bool Receiver(const EthCAN_Frame & aFrame);

private:

    // --> INIT <--+     +---+==> ERROR_ETH <--+
    //      |      |     |   |        |        |
    //      +--> ACTIVATING  |  +-----|--> ERROR_CAN <--+
    //            |          |  |     |     |           |
    //            +--> TRANSACTION <==+     |           |
    //                  |             |     |           |
    //                  +----> ACTIVATED <--+           |
    //                                |                 |
    //                                +-----------------+
    typedef enum
    {
        STATE_ACTIVATED,
        STATE_ACTIVATING,
        STATE_ERROR_CAN,
        STATE_ERROR_ETH,
        STATE_INIT,
        STATE_TRANSACTION,

        STATE_QTY
    }
    State;

    unsigned int CalculateMoveDuration(const Position & aTo, unsigned int aFlags = 0) const;

    ZT::Result Config_Retrieve();

    void DumpRxBuffer(void * aOut, unsigned int aSize_byte);

    ZT::Result Frame_Send_Z0(DJI_Frame * aFrame);

    ZT::Result Info_Init();
    ZT::Result Info_Retrieve();

    // ===== On... ==========================================================

    bool OnConfig_Z0           (DJI_Transaction * aTr);
    bool OnConfigStiffness_Z0  (DJI_Transaction * aTr);
    bool OnInfo_Z0             (DJI_Transaction * aTr);
    bool OnPosition_Z0         (DJI_Transaction * aTr);
    bool OnPositionAndSignal_Z0(DJI_Transaction * aTr);
    bool OnRelease_Z0          (DJI_Transaction * aTr);
    bool OnSignal_Z0           (DJI_Transaction * aTr);

    bool OnTick();

    ZT::Result Position_Parse();

    void Receiver_CopyNewData(const EthCAN_Frame & aCF);
    void Receiver_Reset();
    ZT::Result Receiver_Validate_Z0(unsigned int aTo_byte);

    void ResetAndSleep_Z0(State aNextState);
    
    ZT::Result Retry(DJI_Transaction * aTr);

    // ===== State ==========================================================

    ZT::Result State_Change_Z0(State aFrom, State aTo, unsigned int aLine);
    ZT::Result State_Check();
    void       State_Set_Z0(State aTo, unsigned int aLine);

    void State_TRANSACTION_Z0();

    // ===== Tick ===========================================================

    void Tick_ACTIVATED_Z0();
    
    void Tick_Focus_Speed_Z0();
    void Tick_Position_Z0   ();
    void Tick_Speed_Z0      ();
    void Tick_Work_Z0       ();

    // ===== Tr =============================================================
    void       Tr_Complete_Z0 (DJI_Transaction * aTr);
    ZT::Result Tr_Queue       (DJI_Transaction * aTr);
    void       Tr_Queue_Z0    (DJI_Transaction * aTr);
    ZT::Result Tr_QueueAndWait(DJI_Transaction * aTr);
    void       Tr_Start_Z0    (DJI_Transaction * aTr);

    unsigned int mMoveDuration_ms;
    
    ZT_Lib::Thread mThread;

    // ===== Receiver =======================================================
    const DJI_Frame * mReply;
    uint8_t           mRxBuffer[128];

    // ===== Worker =========================================================
    unsigned int    mCounter;
    DJI_Transaction mTr_Position;

    // ===== Zone 0 =========================================================
    EthCAN::Device * mDevice;

    unsigned int mRxOffset_byte;
    unsigned int mRxSize_byte;

    State        mState;
    unsigned int mState_Counter;

    DJI_Transaction * mTr_Current;
    DJI_Transaction * mTr_Next;

};
