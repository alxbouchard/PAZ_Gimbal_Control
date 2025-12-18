
// Author  KMS - Martin Dubois, P. Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/DJI_Gimbal.cpp

// CODE REVIEW 2021-04-27 KMS - Martin Dubois, P. Eng.

// TEST COVERAGE 2021-04-27 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== C ==================================================================
#include <unistd.h>

// ===== Import/Includes ====================================================
#include <EthCAN/Display.h>

// ===== ZT_Lib =============================================================
#include "DJI_CRC.h"
#include "Value.h"

#include "DJI_Gimbal.h"

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_CONFIG              (1)
#define MSG_CONFIG_STIFFNESS    (2)
#define MSG_DUMMY               (3)
#define MSG_INFO                (4)
#define MSG_POSITION            (5)
#define MSG_POSITION_AND_SIGNAL (6)
#define MSG_RELEASE             (7)
#define MSG_REPEAT              (8)
#define MSG_SIGNAL              (9)
#define MSG_TICK                (10)

#define PERIOD_ms (10)

const char * STATE_CHANGES[6][6] =
{
    { "ACTIVATED -> ACTIVATED (I)" , "ACTIVATED -> ACTIVATING (I)"  , "ACTIVATED -> ERROR_CAN (E)"  , "ACTIVATED -> ERROR_ETH (I)" , "ACTIVATED -> INIT (I)"  , NULL                             },
    { "ACTIVATING -> ACTIVATED (I)", "ACTIVATING -> ACTIVATING (I)" , "ACTIVATING -> ERROR_CAN (I)" , "ACTIVATING -> ERROR_ETH (E)", "ACTIVATING -> INIT (E)" , "ACTIVATING -> TRANSACTION"      },
    { "ERROR_CAN -> ACTIVATED (R)" , "ERROR_CAN -> ACTIVATING (I)"  , "ERROR_CAN -> ERROR_CAN (I)"  , "ERROR_CAN -> ERROR_ETH (E)" , "ERROR_CAN -> INIT (I)"  , "ERROR_CAN -> TRANSACTION (I)"   },
    { "ERROR_ETH -> ACTIVATED (I)" , "ERROR_ETH -> ACTIVATING (I)"  , "ERROR_ETH -> ERROR_CAN (I)"  , "ERROR_ETH -> ERROR_ETH (I)" , "ERROR_ETH -> INIT (I)"  , "ERROR_ETH -> TRANSACTION (R)"   },
    { "INIT -> ACTIVATED (I)"      , "INIT -> ACTIVATING"           , "INIT -> ERROR_CAN (I)"       , "INIT -> ERROR_ETH (I)"      , "INIT -> INIT (I)"       , "INIT -> TRANSACTION (I)"        },
    { NULL                         , "TRANSACTION -> ACTIVATING (I)", "TRANSACTION -> ERROR_CAN (E)", "TRANSACTION -> ERROR_ETH"   , "TRANSACTION -> INIT (I)", "TRANSACTION -> TRANSACTION (I)" },
};

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static unsigned int CalculateMoveDuration(double aFrom_deg, double aTo_deg, double aSpeed_deg_s);

static ZT::Result ReturnError(ZT::Result aResult, unsigned int aLine);

// ===== Entry point ========================================================

static bool Receiver_Link(EthCAN::Device * aDevice, void * aContext, const EthCAN_Frame & aFrame);

// Public
/////////////////////////////////////////////////////////////////////////////

DJI_Gimbal::DJI_Gimbal(EthCAN::Device * aDevice)
    : mCounter(0)
    , mDevice(aDevice)
    , mReply(reinterpret_cast<DJI_Frame *>(mRxBuffer))
    , mRxOffset_byte(0)
    , mRxSize_byte(0)
    , mState(STATE_INIT)
    , mTr_Current(NULL)
    , mTr_Next(NULL)
{
    assert(NULL != aDevice);

    mTr_Position.Prepare(this, MSG_POSITION, 10);
}

DJI_Gimbal::~DJI_Gimbal()
{
    assert(NULL != mDevice);

    mDevice->Release();
}

ZT::Result DJI_Gimbal::Connect()
{
    assert(NULL != mDevice);

    ZT::Result lResult;

    EthCAN_Result lRet = mDevice->Protocol_Set(EthCAN::Device::PROTOCOL_TCP);
    if (EthCAN_OK == lRet)
    {
        lResult = Info_Init();
        if (ZT::ZT_OK == lResult)
        {
            EthCAN_Config lConfig;

            EthCAN_Result lRet = mDevice->Config_Get(&lConfig);
            if (   (EthCAN_OK != lRet)
                || (DJI_CAN_ID_RX != lConfig.mCAN_Filters[0])
                || (0x7ff != lConfig.mCAN_Masks[0])
                || (EthCAN_RATE_1_Mb != lConfig.mCAN_Rate))
            {
                lResult = ZT::ZT_ERROR_GIMBAL;
            }
        }
    }
    else
    {
        lResult = ZT::ZT_ERROR_PROTOCOL;
    }

    TRACE_RESULT(stderr, lResult);
    return lResult;
}

// ===== ZT::Gimbal =========================================================

ZT::Result DJI_Gimbal::Activate()
{
    assert(STATE_INIT == mState);
    assert(NULL != mDevice);

    ZT::Result lResult;

    try
    {
        State_Set_Z0(STATE_ACTIVATING, __LINE__);

        EthCAN_Result lRet = mDevice->Receiver_Start(Receiver_Link, this);
        if (EthCAN_OK != lRet)
        {
            State_Set_Z0(STATE_INIT, __LINE__);
            lResult = ZT::ZT_ERROR_RECEIVE;
        }
        else
        {
            lResult = mThread.Start(this, MSG_DUMMY, MSG_TICK, MSG_DUMMY);
            if (ZT::ZT_OK == lResult)
            {
                for (unsigned int lRetry = 0; lRetry < 2; lRetry ++)
                {
                    if (0 < lRetry)
                    {
                        mThread.Zone0_Enter();
                        {
                            State_Set_Z0(STATE_ERROR_CAN, __LINE__);
                            ResetAndSleep_Z0(STATE_ACTIVATING);
                        }
                        mThread.Zone0_Leave();
                    }

                    lResult = Info_Retrieve();
                    if (ZT::ZT_OK == lResult)
                    {
                        break;
                    }
                }

                if (ZT::ZT_OK == lResult)
                {
                    lResult = Config_Retrieve();
                    if (ZT::ZT_OK == lResult)
                    {
                        lResult = Gimbal::Activate();
                    }
                }

                if (ZT::ZT_OK != lResult)
                {
                    State_Set_Z0(STATE_INIT, __LINE__);
                    ZT::Result lRet = mThread.Stop();
                    assert(ZT::ZT_OK == lRet);
                }
            }
            else
            {
                State_Set_Z0(STATE_INIT, __LINE__);
            }
        }
    }
    catch (...)
    {
        lResult = ZT::ZT_ERROR_EXCEPTION;
    }

    TRACE_RESULT(stderr, lResult);
    return lResult;
}

#define BEGIN                     \
    try                           \
    {                             \
        lResult = State_Check();  \
        if (ZT::ZT_OK == lResult) \
        {

#define END                                                       \
        }                                                         \
    }                                                             \
    catch (...)                                                   \
    {                                                             \
        lResult = ZT::ZT_ERROR_EXCEPTION;                         \
    }                                                             \
    TRACE_RESULT(stderr, lResult);

ZT::Result DJI_Gimbal::Config_Set(const Config & aIn)
{
    ZT::Result lResult = Gimbal::Config_Set(aIn);
    if (ZT::ZT_OK == lResult)
    {
        BEGIN
            DJI_Transaction lTr[2];

            lTr[0].Frame_Init_ANGLE_LIMIT_SET    (mConfig);
            lTr[1].Frame_Init_MOTOR_STIFFNESS_SET(mConfig);

            for (unsigned int i = 0; (i < 2) && (ZT::ZT_OK == lResult); i ++)
            {
                lTr[i].Prepare(this, MSG_SIGNAL, 3);

                lResult = Retry(lTr + i);
            }
        END
    }

    return lResult;
}

ZT::Result DJI_Gimbal::Focus_Cal(Operation aOperation)
{
    if (OPERATION_QTY <= aOperation) { return ZT::ZT_ERROR_OPERATION; }

    ZT::Result lResult = ZT::ZT_OK;
    BEGIN
        DJI_Transaction * lTr = new DJI_Transaction;

        lTr->Frame_Init_FOCUS_CAL(aOperation);

        if (ZT::ZT_OK == lResult)
        {
            lResult = Tr_Queue(lTr);
        }
        else
        {
            delete lTr;
        }
    END
    return lResult;
}

ZT::Result DJI_Gimbal::Focus_Position_Set(double aFocus_pc)
{
    ZT::Result lResult = Value_Validate(aFocus_pc, 0.0, 100.0);
    if (ZT::ZT_OK == lResult)
    {
        BEGIN
            DJI_Transaction * lTr = new DJI_Transaction;

            lTr->Frame_Init_FOCUS_SET(aFocus_pc);

            lResult = Tr_Queue(lTr);
        END
    }
    
    return lResult;
}

ZT::Result DJI_Gimbal::Position_Get(Position * aOut)
{
    ZT::Result lResult = ZT::ZT_OK;

    if (!Position_Current_Get(aOut))
    {
        BEGIN
            DJI_Transaction lTr;

            lTr.Prepare(this, MSG_POSITION_AND_SIGNAL, 10);

            lTr.Frame_Init_ANGLE_GET();

            lResult = Tr_QueueAndWait(&lTr);
        END

        if (ZT::ZT_OK == lResult)
        {
            lResult = Gimbal::Position_Get(aOut);
        }
    }

    return lResult;
}

ZT::Result DJI_Gimbal::Position_Set(const Position & aIn, unsigned int aFlags, unsigned int aDuration_ms)
{
    ZT::Result lResult = Gimbal::Position_Set(aIn, aFlags, aDuration_ms);
    if (ZT::ZT_OK == lResult)
    {
        BEGIN
            mMoveDuration_ms = CalculateMoveDuration(aIn, aFlags);
            if (aDuration_ms > mMoveDuration_ms)
            {
                mMoveDuration_ms = aDuration_ms;
            }

            DJI_Transaction * lTr = new DJI_Transaction;

            lTr->Frame_Init_POSITION_SET(aIn, aFlags, mMoveDuration_ms);

            lResult = Tr_Queue(lTr);
            if (ZT::ZT_ERROR_NOT_READY == lResult)
            {
                lResult = ZT::ZT_OK;
            }
        END
    }

    return lResult;
}

ZT::Result DJI_Gimbal::Speed_Set(const Speed & aIn, unsigned int aFlags)
{
    ZT::Result lResult = Gimbal::Speed_Set(aIn, aFlags);
    if (ZT::ZT_OK == lResult)
    {
        BEGIN
            DJI_Transaction * lTr = new DJI_Transaction;

            lTr->Frame_Init_SPEED_SET(mSpeed);

            lResult = Tr_Queue(lTr);
            if (ZT::ZT_ERROR_NOT_READY == lResult)
            {
                lResult = ZT::ZT_OK;
            }
        END
    }

    return lResult;
}

ZT::Result DJI_Gimbal::Speed_Stop()
{
    ZT::Result lResult = Gimbal::Speed_Stop();
    if (ZT::ZT_OK == lResult)
    {
        BEGIN
            DJI_Transaction * lTr = new DJI_Transaction;

            lTr->Frame_Init_SPEED_SET(mSpeed);

            lResult = Tr_Queue(lTr);
        END
    }

    return lResult;
}

ZT::Result DJI_Gimbal::Track_Speed_Set(double aSpeed_pc)
{
    ZT::Result lResult;
    BEGIN
        DJI_Transaction lTr;

        lTr.Prepare(this, MSG_SIGNAL, 1);

        lTr.Frame_Init_TLV_SET(aSpeed_pc);

        lResult = Retry(&lTr);
    END
    return lResult;
}

ZT::Result DJI_Gimbal::Track_Switch()
{
    ZT::Result lResult;
    BEGIN
        DJI_Transaction * lTr = new DJI_Transaction;

        lTr->Frame_Init_TRACK_SWITCH();

        lResult = Tr_Queue(lTr);
    END
    return lResult;
}

void DJI_Gimbal::Debug(void * aOut)
{
    FILE * lOut = (NULL == aOut) ? stdout : reinterpret_cast<FILE *>(aOut);

    assert(NULL != mDevice);

    try
    {
        fprintf(lOut, "===== Debug Information =====\n");
        fprintf(lOut, "Rx Buffer     :");
        DumpRxBuffer(lOut, mRxSize_byte);
        fprintf(lOut, "Counter       : %u\n"      , mCounter);
        fprintf(lOut, "Rx Offset     : %u bytes\n", mRxOffset_byte);
        fprintf(lOut, "Rx Size       : %u bytes\n", mRxSize_byte);

        EthCAN_Info lInfo;
        EthCAN_Result lResult;

        mThread.Zone0_Enter();
        {
            lResult = mDevice->GetInfo(&lInfo);
        }
        mThread.Zone0_Leave();

        if (EthCAN_OK == lResult)
        {
            EthCAN::Display(lOut, lInfo);
        }
        else
        {
            EthCAN::Display(lOut, lResult);
        }
    }
    catch (...)
    {
        TRACE_ERROR(stderr, "DJI_Gimbal::Debug - Exception");
    }
}

// ===== ZT::IMessageReceiver ===============================================

bool DJI_Gimbal::ProcessMessage(void * aSender, unsigned int aCode, const void * aData)
{
    assert(NULL != aSender);

    bool lResult = false;

    DJI_Transaction * lTr = reinterpret_cast<DJI_Transaction *>(aSender);

    switch (aCode)
    {
    case MSG_CONFIG             : lResult = OnConfig_Z0           (lTr); break;
    case MSG_CONFIG_STIFFNESS   : lResult = OnConfigStiffness_Z0  (lTr); break;
    case MSG_INFO               : lResult = OnInfo_Z0             (lTr); break;
    case MSG_POSITION           : lResult = OnPosition_Z0         (lTr); break;
    case MSG_POSITION_AND_SIGNAL: lResult = OnPositionAndSignal_Z0(lTr); break;
    case MSG_RELEASE            : lResult = OnRelease_Z0          (lTr); break;
    case MSG_SIGNAL             : lResult = OnSignal_Z0           (lTr); break;

    case MSG_DUMMY: lResult = true; break;

    case MSG_TICK: lResult = OnTick(); break;

    default: assert(false);
    }

    return lResult;
}

// Internal
/////////////////////////////////////////////////////////////////////////////

bool DJI_Gimbal::Receiver(const EthCAN_Frame & aCF)
{
    assert(sizeof(aCF.mData) >= aCF.mDataSize_byte);

    assert(sizeof(mRxBuffer) >= mRxSize_byte);

    mThread.Zone0_Enter();
    {
        if ((DJI_CAN_ID_RX != aCF.mId) || (NULL == mTr_Current))
        {
            // EthCAN::Display(stderr, aCF);
        }
        else
        {
            unsigned int lRxSize_byte = mRxSize_byte + aCF.mDataSize_byte;
            if (sizeof(mRxBuffer) < lRxSize_byte)
            {
                Receiver_Reset();
            }
            else
            {
                Receiver_CopyNewData(aCF);

                ZT::Result lRet = Receiver_Validate_Z0(lRxSize_byte);
                if (ZT::ZT_OK == lRet)
                {
                    if (mTr_Current->RxExpected_Get() <= mRxSize_byte)
                    {
                        switch (mState)
                        {
                        case STATE_ERROR_ETH:
                            State_Set_Z0(STATE_ACTIVATED, __LINE__);
                            // no break
                        case STATE_ACTIVATED:
                        case STATE_TRANSACTION:
                            mState_Counter = 30;
                            break;
                        default: assert(false);
                        }

                        mTr_Current->Complete(ZT::ZT_OK);
                    }
                }
                else
                {
                    mTr_Current->Complete(lRet);
                }
            }
        }
    }
    mThread.Zone0_Leave();

    return true;
}

// Private
/////////////////////////////////////////////////////////////////////////////

// Thread : Users
unsigned int DJI_Gimbal::CalculateMoveDuration(const Position & aTo, unsigned int aFlags) const
{
    unsigned int lResult_ms = 0;

    Position lPosition;

    if (Position_Current_Get(&lPosition))
    {
        FOR_EACH_AXIS(a)
        {
            if (0 == (aFlags & ZT_FLAG_IGNORE(a)))
            {
                unsigned int lDuration_ms = ::CalculateMoveDuration(lPosition.mAxis_deg[a], aTo.mAxis_deg[a], mConfig.mAxis[a].mSpeed_deg_s);
                if (lResult_ms < lDuration_ms)
                {
                    lResult_ms = lDuration_ms;
                }
            }
        }
    }
    else
    {
        lResult_ms = 200;
    }

    return lResult_ms;
}

// Thread : Users
ZT::Result DJI_Gimbal::Config_Retrieve()
{
    DJI_Transaction lTr[2];

    lTr[0].Frame_Init_ANGLE_LIMIT_GET();
    lTr[1].Frame_Init_MOTOR_STIFFNESS_GET();

    ZT::Result lResult = ZT::ZT_OK;

    for (unsigned int i = 0; (i < 2) && (ZT::ZT_OK == lResult); i++)
    {
        static const unsigned int EXPECTED_bytes[2] = { 9, 6 };
        static const unsigned int MESSAGE_CODES [2] = { MSG_CONFIG, MSG_CONFIG_STIFFNESS };

        lTr[i].Prepare(this, MESSAGE_CODES[i], EXPECTED_bytes[i]);

        lResult = Retry(lTr + i);
    }

    return lResult;
}

void DJI_Gimbal::DumpRxBuffer(void * aOut, unsigned int aSize_byte)
{
    assert(NULL != aOut);
    assert(0 < aSize_byte);

    FILE * lOut = reinterpret_cast<FILE *>(aOut);

    for (unsigned int i = 0; i < aSize_byte; i++)
    {
        fprintf(lOut, " %02x", mRxBuffer[i]);
    }

    fprintf(lOut, "\n");
}

// Thread : Worker
ZT::Result DJI_Gimbal::Frame_Send_Z0(DJI_Frame * aFrame)
{
    assert(NULL != aFrame);

    assert(NULL != mDevice);

    uint8_t    * lFrame          = &aFrame->mSOF;
    unsigned int lOffset_byte    = 0;
    ZT::Result   lResult         = ZT::ZT_OK;
    unsigned int lTotalSize_byte = aFrame->mSize_byte;

    aFrame->Seal();

    do
    {
        unsigned int lSize_byte = lTotalSize_byte - lOffset_byte;
        if (8 < lSize_byte)
        {
            lSize_byte = (12 < lSize_byte) ? 8 : lSize_byte - DJI_FOOTER_SIZE_byte;
        }

        EthCAN_Frame lCF;

        memset(&lCF, 0, sizeof(lCF));

        lCF.mDataSize_byte = lSize_byte;
        lCF.mId = DJI_CAN_ID_TX;

        memcpy(lCF.mData, lFrame + lOffset_byte, lSize_byte);

        lOffset_byte += lSize_byte;

        EthCAN_Result lRet = mDevice->Send(lCF, EthCAN_FLAG_NO_RESPONSE);
        if (EthCAN_OK != lRet)
        {
            State_Set_Z0(STATE_ERROR_ETH, __LINE__);
            lResult = ZT::ZT_ERROR_SEND;
            break;
        }
    }
    while (lOffset_byte < lTotalSize_byte);
    
    TRACE_RESULT(stderr, lResult);
    return lResult;
}

ZT::Result DJI_Gimbal::Info_Init()
{
    assert(NULL != mDevice);

    EthCAN_Info lInfo;

    EthCAN_Result lRet = mDevice->GetInfo(&lInfo);
    if (EthCAN_OK != lRet)
    {
        return ReturnError(ZT::ZT_ERROR_GIMBAL, __LINE__);
    }

    mInfo.mIPv4_Address = lInfo.mIPv4_Address;
    mInfo.mIPv4_Gateway = lInfo.mIPv4_Gateway;
    mInfo.mIPv4_NetMask = lInfo.mIPv4_NetMask;

    memcpy(mInfo.mName, lInfo.mName, sizeof(mInfo.mName));

    return ZT::ZT_OK;
}

ZT::Result DJI_Gimbal::Info_Retrieve()
{
    DJI_Transaction lTr;

    lTr.Prepare(this, MSG_INFO, 11);

    lTr.Frame_Init_VERSION();

    ZT::Result lResult = Retry(&lTr);

    TRACE_RESULT(stderr, lResult);

    return lResult;
}

// ===== On =================================================================
// Thread  Worker

bool DJI_Gimbal::OnConfig_Z0(DJI_Transaction * aTr)
{
    assert(NULL != aTr);

    assert(NULL != mReply);

    if (aTr->IsOK())
    {
        FOR_EACH_AXIS(a)
        {
            unsigned int OFFSETS[AXIS_QTY] = { 3, 7, 5 };

            mConfig.mAxis[a].mMax_deg =   mReply->mData[OFFSETS[a]    ];
            mConfig.mAxis[a].mMin_deg = - mReply->mData[OFFSETS[a] + 1];
        }
    }

    return OnSignal_Z0(aTr);
}

bool DJI_Gimbal::OnConfigStiffness_Z0(DJI_Transaction * aTr)
{
    assert(NULL != aTr);

    assert(NULL != mReply);

    if (aTr->IsOK())
    {
        FOR_EACH_AXIS(a)
        {
            unsigned int OFFSETS[AXIS_QTY] = { 3, 5, 4 };

            mConfig.mAxis[a].mStiffness_pc = mReply->mData[OFFSETS[a]];
        }

        aTr->Result_Set(Config_Validate(mConfig));
    }

    return OnSignal_Z0(aTr);
}

bool DJI_Gimbal::OnInfo_Z0(DJI_Transaction * aTr)
{
    assert(NULL != aTr);

    assert(NULL != mReply);

    if (aTr->IsOK())
    {
        mInfo.mVersion[0] = mReply->mData[5];
        mInfo.mVersion[1] = mReply->mData[4];
        mInfo.mVersion[2] = mReply->mData[3];
        mInfo.mVersion[3] = mReply->mData[2];
    }

    return OnSignal_Z0(aTr);
}

bool DJI_Gimbal::OnPosition_Z0(DJI_Transaction * aTr)
{
    assert(NULL != aTr);

    if (aTr->IsOK())
    {
        aTr->Result_Set(Position_Parse());
    }

    Tr_Complete_Z0(aTr);

    return true;
}

bool DJI_Gimbal::OnPositionAndSignal_Z0(DJI_Transaction * aTr)
{
    if (aTr->IsOK())
    {
        aTr->Result_Set(Position_Parse());
    }

    return OnSignal_Z0(aTr);
}

bool DJI_Gimbal::OnRelease_Z0(DJI_Transaction * aTr)
{
    assert(NULL != aTr);

    if (ZT::ZT_OK_REPLACED != aTr->Result_Get())
    {
        Tr_Complete_Z0(aTr);

        State_Change_Z0(STATE_TRANSACTION, STATE_ACTIVATED, __LINE__);
    }

    delete aTr;

    return true;
}

bool DJI_Gimbal::OnSignal_Z0(DJI_Transaction * aTr)
{
    assert(NULL != aTr);

    mThread.Condition_Signal();

    Tr_Complete_Z0(aTr);

    State_Change_Z0(STATE_TRANSACTION, STATE_ACTIVATED, __LINE__);

    return true;
}

bool DJI_Gimbal::OnTick()
{
    bool lResult = true;

    usleep(PERIOD_ms * 1000);

    mThread.Zone0_Enter();
    {
        try
        {
            switch (mState)
            {
            case STATE_ACTIVATED : Tick_ACTIVATED_Z0(); break;

            case STATE_ACTIVATING:
            case STATE_ERROR_ETH :
                if (NULL != mTr_Next)
                {
                    State_TRANSACTION_Z0();
                }
                break;

            case STATE_TRANSACTION: mTr_Current->Tick(); break;

            case STATE_INIT: break;

            default: assert(false);
            }
        }
        catch (...)
        {
            TRACE_ERROR(stderr, "DJI_Gimbal::OnTick - Exception");
            lResult = false;
        }

        Gimbal::Tick();
    }
    mThread.Zone0_Leave();

    return lResult;
}

ZT::Result DJI_Gimbal::Position_Parse()
{
    assert(NULL != mReply);

    ZT::Result lResult;

    if (0x00 == mReply->mData[3])
    {
        lResult = ZT::ZT_ERROR_NOT_READY;
    }
    else
    {
        Position lPosition;

        FOR_EACH_AXIS(a)
        {
            static const unsigned int OFFSETS[AXIS_QTY] = { 8, 6, 4 };

            lPosition.mAxis_deg[a] = mReply->Angle_Get(OFFSETS[a]);
        }

        lResult = Position_Validate(lPosition, 0);
        if (ZT::ZT_OK == lResult)
        {
            Position_Update(lPosition);
        }
    }

    return lResult;
}

void DJI_Gimbal::Receiver_CopyNewData(const EthCAN_Frame & aCF)
{
    assert(sizeof(mRxBuffer) > mRxOffset_byte);
    assert(sizeof(mRxBuffer) > mRxSize_byte);
    assert(mRxOffset_byte <= mRxSize_byte);

    unsigned int lSize_byte = aCF.mDataSize_byte;

    unsigned int lToMove_byte = mRxSize_byte - mRxOffset_byte;
    if (0 < lToMove_byte)
    {
        memmove(mRxBuffer + mRxOffset_byte + lSize_byte, mRxBuffer + mRxOffset_byte, lToMove_byte);
    }

    memcpy(mRxBuffer + mRxOffset_byte, aCF.mData, lSize_byte);

    if (8 <= lSize_byte)
    {
        mRxOffset_byte += lSize_byte;
    }
}

void DJI_Gimbal::Receiver_Reset()
{
    assert(sizeof(mRxBuffer) >= mRxOffset_byte);
    assert(sizeof(mRxBuffer) >= mRxSize_byte);

    mRxOffset_byte = 0;
    mRxSize_byte   = 0;
}

ZT::Result DJI_Gimbal::Receiver_Validate_Z0(unsigned int aTo_byte)
{
    assert(NULL != mTr_Current);
    assert(mRxSize_byte < aTo_byte);
    assert(sizeof(mRxBuffer) >= aTo_byte);

    ZT::Result lResult = ZT::ZT_OK;

    if ((1 > mRxSize_byte) && (1 <= aTo_byte))
    {
        if (DJI_SOF != mReply->mSOF)
        {
            goto Error;
        }
    }

    if ((2 > mRxSize_byte) && (2 <= aTo_byte))
    {
        if (sizeof(mRxBuffer) < mReply->mSize_byte)
        {
            lResult = ZT::ZT_ERROR_FRAME_TOO_LONG;
            goto Error;
        }
        if (DJI_FRAME_TOTAL_SIZE(1) > mReply->mSize_byte)
        {
            lResult = ZT::ZT_ERROR_FRAME_TOO_SHORT;
            goto Error;
        }
    }

    if ((3 > mRxSize_byte) && (3 <= aTo_byte))
    {
        if (0 != (mReply->mVersion & 0xfc))
        {
            lResult = ZT::ZT_ERROR_GIMBAL_VERSION;
            goto Error;
        }
    }

    if ((4 > mRxSize_byte) && (4 <= aTo_byte))
    {
        if (DJI_CMD_TYPE_REPLY != mReply->mCmdType)
        {
            lResult = ZT::ZT_ERROR_CMD_TYPE;
            goto Error;
        }
    }

    if ((5 > mRxSize_byte) && (5 <= aTo_byte))
    {
        if (0 != mReply->mEncoded)
        {
            lResult = ZT::ZT_ERROR_ENCODED;
            goto Error;
        }
    }

    if ((10 > mRxSize_byte) && (10 <= aTo_byte))
    {
        // TODO DJI_Gimbal
        //      Verify serial
    }

    if ((12 > mRxSize_byte) && (12 <= aTo_byte))
    {
        // TODO DJI_Gimbal
        //      Verify CRC-16
    }

    if ((13 > mRxSize_byte) && (13 <= aTo_byte))
    {
        if (DJI_CMD_SET_DEFAULT != mReply->mData[DJI_DATA_CMD_SET])
        {
            lResult = ZT::ZT_ERROR_CMD_SET;
            goto Error;
        }
    }

    if ((14 > mRxSize_byte) && (14 <= aTo_byte))
    {
        if (mTr_Current->Frame_Data_Get(DJI_DATA_CMD_ID) != mReply->mData[DJI_DATA_CMD_ID])
        {
            lResult = ZT::ZT_ERROR_CMD_ID;
            goto Error;
        }
    }

    if ((15 > mRxSize_byte) && (15 <= aTo_byte))
    {
        if (DJI_OK != mReply->mData[DJI_REPLY_RESULT])
        {
            lResult = ZT::ZT_ERROR_GIMBAL;
            goto Error;
        }
    }

    mRxSize_byte = aTo_byte;
    return lResult;

Error:
    Receiver_Reset();

    TRACE_RESULT(stderr, lResult);
    return lResult;
}

void DJI_Gimbal::ResetAndSleep_Z0(State aNextState)
{
    assert(NULL != mDevice);
    assert(STATE_ERROR_CAN == mState);

    EthCAN_Result lRet = mDevice->CAN_Reset();
    if (EthCAN_OK == lRet)
    {
        sleep(1);

        mState_Counter = 10;
        State_Set_Z0(aNextState, __LINE__);
    }
    else
    {
        State_Set_Z0(STATE_ERROR_ETH, __LINE__);
    }
}

// Thread  Users
ZT::Result DJI_Gimbal::Retry(DJI_Transaction * aTr)
{
    assert(NULL != aTr);

    ZT::Result lResult;

    for (unsigned int lRetry = 0; lRetry < 2; lRetry++)
    {
        lResult = Tr_QueueAndWait(aTr);
        if ((ZT::ZT_ERROR_TIMEOUT != lResult) && (ZT::ZT_ERROR_GIMBAL != lResult))
        {
            break;
        }

        TRACE_DEBUG(stdout, "DJI_Gimbal::Retry");
    }

    TRACE_RESULT(stderr, lResult);
    return lResult;
}

// ===== State ==============================================================

ZT::Result DJI_Gimbal::State_Change_Z0(State aFrom, State aTo, unsigned int aLine)
{
    assert(STATE_QTY > aFrom);
    assert(STATE_QTY > aTo);

    if (mState != aFrom)
    {
        fprintf(stderr, "WARNING  DJI_Gimbal::State_Change - mState = %u, aFrom = %u, aTo = %u, aLine = %u\n", mState, aFrom, aTo, aLine);
        return ZT::ZT_ERROR_STATE;
    }

    State_Set_Z0(aTo, aLine);
    return ZT::ZT_OK;
}

ZT::Result DJI_Gimbal::State_Check()
{
    ZT::Result lResult = ZT::ZT_ERROR_STATE;
    mThread.Zone0_Enter();
    {
        switch (mState)
        {
        case STATE_ACTIVATED:
        case STATE_TRANSACTION:
            lResult = ZT::ZT_OK;
            break;

        case STATE_ACTIVATING:
        case STATE_INIT:
            break;

        case STATE_ERROR_ETH:
            EthCAN_Result lRet;
            mThread.Zone0_Leave();
            {
                lRet = mDevice->Protocol_Reset();
                assert(EthCAN_OK == lRet);
                lRet = mDevice->Receiver_Config();
            }
            mThread.Zone0_Enter();
            if (EthCAN_OK == lRet)
            {
                lResult = State_Change_Z0(STATE_ERROR_ETH, STATE_ACTIVATED, __LINE__);
                mState_Counter = 10;
            }
            break;

        default: assert(false);
        }
    }
    mThread.Zone0_Leave();
    TRACE_RESULT(stderr, lResult);
    return lResult;
}

void DJI_Gimbal::State_Set_Z0(State aTo, unsigned int aLine)
{
    assert(STATE_QTY > aTo);

    assert(STATE_QTY > mState);

    const char * lMsg = STATE_CHANGES[mState][aTo];
    if (NULL != lMsg)
    {
        fprintf(stdout, "DEBUG  DJI_Gimbal::State_Set_Z0 - %s (Line %u)\n", lMsg, aLine);
    }

    mState = aTo;
}

void DJI_Gimbal::State_TRANSACTION_Z0()
{
    assert(NULL != mTr_Next);

    State_Set_Z0(STATE_TRANSACTION, __LINE__);
    Tr_Start_Z0(mTr_Next);
    mTr_Next = NULL;
}

// ===== Tick ===============================================================
// Thread  Worker

void DJI_Gimbal::Tick_ACTIVATED_Z0()
{
    if (NULL != mTr_Next)
    {
        State_TRANSACTION_Z0();
    }
    else
    {
        Tick_Work_Z0();

        switch (mState_Counter)
        {
        case 0: assert(false);

        case 1:
            State_Set_Z0(STATE_ERROR_CAN, __LINE__);
            ResetAndSleep_Z0(STATE_ACTIVATED);
            break;

        default: mState_Counter--;
        }
    }
}

void DJI_Gimbal::Tick_Focus_Speed_Z0()
{
    if (IsFocusMoving())
    {
        double lPosition_pc = mFocus_Position_pc + mFocus_Speed_pc_s * 2 * PERIOD_ms / 1000.0;

        mFocus_Position_pc = Value_Limit(lPosition_pc, ZT::IGimbal::FOCUS_POSITION_MIN_pc, ZT::IGimbal::FOCUS_POSITION_MAX_pc);

        DJI_Transaction lTr;

        lTr.Frame_Init_FOCUS_SET(mFocus_Position_pc);

        ZT::Result lRet = Frame_Send_Z0(lTr.Frame_Get());
        assert(ZT::ZT_OK == lRet);
    }
}

void DJI_Gimbal::Tick_Position_Z0()
{
    mTr_Position.Frame_Init_ANGLE_GET();
    mTr_Position.Reset();

    Tr_Start_Z0(&mTr_Position);
}

void DJI_Gimbal::Tick_Speed_Z0()
{
    DJI_Transaction lTr;

    switch (Position_State_Get())
    {
    case STATE_KNOWN:
    case STATE_UNKNOWN: break;

    case STATE_MOVING:
        lTr.Frame_Init_POSITION_SET(mPosition_Target, mPosition_Flags, mMoveDuration_ms);
        Frame_Send_Z0(lTr.Frame_Get());
        break;

    case STATE_SPEED:
        lTr.Frame_Init_SPEED_SET(mSpeed);
        Frame_Send_Z0(lTr.Frame_Get());
        // We do not test the return value because this operation fail when
        // we reset the device after a communication error.
        break;

    default: assert(false);
    }
}

void DJI_Gimbal::Tick_Work_Z0()
{
    mCounter ++;

    switch (mCounter % 4)
    {
    case 0:
    case 2:
        Tick_Focus_Speed_Z0(); break;

    case 1: Tick_Position_Z0(); break;
    case 3: Tick_Speed_Z0   (); break;
    }
}

// ===== Tr =================================================================

// Thread  Worker
void DJI_Gimbal::Tr_Complete_Z0(DJI_Transaction * aTr)
{
    assert(NULL != aTr);

    if (mTr_Current == aTr)
    {
        mTr_Current = NULL;
    }
}

// Thread  Users
ZT::Result DJI_Gimbal::Tr_Queue(DJI_Transaction * aTr)
{
    ZT::Result lResult = ZT::ZT_ERROR_NOT_READY;

    aTr->Prepare(this, MSG_RELEASE);

    mThread.Zone0_Enter();
    {
        if (NULL == mTr_Next)
        {
            Tr_Queue_Z0(aTr);
            lResult = ZT::ZT_OK;
        }
        else
        {
            delete aTr;
        }
    }
    mThread.Zone0_Leave();

    return lResult;
}

// Thread  Users
void DJI_Gimbal::Tr_Queue_Z0(DJI_Transaction * aTr)
{
    assert(NULL != aTr);

    assert(NULL == mTr_Next);

    aTr->Reset();
    aTr->RxTimeout_Set(1000 / PERIOD_ms);

    mTr_Next = aTr;
}

// Thread  Users
ZT::Result DJI_Gimbal::Tr_QueueAndWait(DJI_Transaction * aTr)
{
    ZT::Result lResult = ZT::ZT_ERROR_NOT_READY;

    mThread.Zone0_Enter();
    {
        if (NULL == mTr_Next)
        {
            Tr_Queue_Z0(aTr);
            lResult = aTr->Wait(&mThread);
        }
    }
    mThread.Zone0_Leave();

    return lResult;
}

// Thread  Worker
void DJI_Gimbal::Tr_Start_Z0(DJI_Transaction * aTr)
{
    assert(NULL != aTr);

    mTr_Current = aTr;

    Receiver_Reset();

    ZT::Result lRet = Frame_Send_Z0(aTr->Frame_Get());

    aTr->Started(lRet);
}

// Static functions
/////////////////////////////////////////////////////////////////////////////

unsigned int CalculateMoveDuration(double aFrom_deg, double aTo_deg, double aSpeed_deg_s)
{
    assert(DJI_Gimbal::POSITION_MAX_deg >= aFrom_deg);
    assert(DJI_Gimbal::POSITION_MIN_deg <= aFrom_deg);
    assert(DJI_Gimbal::POSITION_MAX_deg >= aTo_deg);
    assert(DJI_Gimbal::POSITION_MIN_deg <= aTo_deg);
    assert(DJI_Gimbal::SPEED_MAX_deg_s >= aSpeed_deg_s);
    assert(DJI_Gimbal::SPEED_MIN_deg_s <= aSpeed_deg_s);

    double lDelta_deg = (aFrom_deg < aTo_deg) ? aTo_deg - aFrom_deg : aFrom_deg - aTo_deg;
    assert(0.0 <= lDelta_deg);

    double lDuration_s = lDelta_deg / aSpeed_deg_s;
    assert(0.0 <= lDuration_s);

    return lDuration_s * 1000;
}

ZT::Result ReturnError(ZT::Result aResult, unsigned int aLine)
{
    assert(0 < aLine);

    fprintf(stderr, "ERROR  %s (Line %u)\n", Result_GetName(aResult), aLine);

    return aResult;
}

// ===== Entry point ========================================================

bool Receiver_Link(EthCAN::Device * aDevice, void * aContext, const EthCAN_Frame & aFrame)
{
    assert(NULL != aContext);

    DJI_Gimbal * lThis = reinterpret_cast<DJI_Gimbal *>(aContext);

    return lThis->Receiver(aFrame);
}
