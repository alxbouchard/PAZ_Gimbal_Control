
// Author  KMS - Martin Dubois, P. Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Gimbal.cpp

// CODE REVIEW 2021-04-27 KMS - Martin Dubois, P. Eng.

// TEST COVERAGE 2021-04-27 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== ZT_Lib =============================================================
#include "Value.h"

#include "Gimbal.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define ANGLE_OFFSET_DEFAULT_deg (0.0)

#define SPEED_DEFAULT_deg_s (360.0)
#define SPEED_MAX_deg_s     (360.0)
#define SPEED_MIN_deg_s     (  0.1)

#define STIFFNESS_DEFAULT_pc (50.0)

static const ZT::IGimbal::Speed SPEED_STOPPED = { 0.0, 0.0, 0.0 };

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static bool Angle_Compare(double aA_deg, double aB_deg);

static void       Config_Axis_Init    (      ZT::IGimbal::Config_Axis * aOut);
static ZT::Result Config_Axis_Validate(const ZT::IGimbal::Config_Axis & aIn, const ZT::IGimbal::Info_Axis & aInfo);

static void       Position_Copy(ZT::IGimbal::Position * aOut, const ZT::IGimbal::Position & aIn, unsigned int aFlags);
static ZT::Result Position_Validate(double aIn, const ZT::IGimbal::Config_Axis & aConfig);

static void       Speed_Copy(ZT::IGimbal::Speed * aOut, const ZT::IGimbal::Speed & aIn, unsigned int aFlags);
static ZT::Result Speed_Validate(double aIn_deg_s, double aMax_deg_s);

// Public
/////////////////////////////////////////////////////////////////////////////

Gimbal::Gimbal()
    : mFocus_Position_pc(FOCUS_POSITION_MIN_pc)
    , mFocus_Speed_pc_s(FOCUS_SPEED_STOP_pc_s)
    , mPosition_Count(0)
    , mPosition_Flags(ZT_FLAG_IGNORE_ALL)
    , mPosition_State(STATE_UNKNOWN)
    , mRefCount(1)
{
    memset(&mConfig  , 0, sizeof(mConfig  ));
    memset(&mInfo    , 0, sizeof(mInfo    ));
    memset(&mPosition_Current, 0, sizeof(mPosition_Current));
    memset(&mPosition_Target , 0, sizeof(mPosition_Target ));
    memset(&mSpeed   , 0, sizeof(mSpeed   ));

    FOR_EACH_AXIS(a)
    {
        Config_Axis_Init(mConfig.mAxis + a);

        mInfo.mAxis[a].mSpeed_Max_deg_s = SPEED_MAX_deg_s;
    }
}

Gimbal::~Gimbal()
{
}

void Gimbal::IncRefCount()
{
    mRefCount++;
}

// ===== ZT::IGimbal ========================================================

ZT::Result Gimbal::Activate()
{
    return ZT::ZT_OK;
}

void Gimbal::Config_Get(ZT::IGimbal::Config * aOut) const
{
    assert(NULL != aOut);

    *aOut = mConfig;
}

ZT::Result Gimbal::Config_Set(const ZT::IGimbal::Config & aIn)
{
    ZT::Result lResult = Config_Validate(aIn);
    if (ZT::ZT_OK == lResult)
    {
        mConfig = aIn;
    }

    return lResult;
}

void Gimbal::Info_Get(ZT::IGimbal::Info * aOut) const
{
    assert(NULL != aOut);

    *aOut = mInfo;
}

ZT::Result Gimbal::Focus_Position_Set(double aPosition_pc)
{
    ZT::Result lResult = Value_Validate(aPosition_pc, FOCUS_POSITION_MIN_pc, FOCUS_POSITION_MAX_pc);
    if (ZT::ZT_OK == lResult)
    {
        mFocus_Position_pc = aPosition_pc;
    }

    return lResult;
}

ZT::Result Gimbal::Focus_Speed_Set(double aSpeed_pc_s)
{
    ZT::Result lResult = Value_Validate(aSpeed_pc_s, FOCUS_SPEED_MIN_pc_s, FOCUS_SPEED_MAX_pc_s);
    if (ZT::ZT_OK == lResult)
    {
        mFocus_Speed_pc_s = aSpeed_pc_s;
    }

    return lResult;
}

ZT::Result Gimbal::Position_Get(ZT::IGimbal::Position * aOut)
{
    assert(NULL != aOut);

    ZT::Result lResult = ZT::ZT_ERROR_NOT_READY;

    Position lPosition;

    if (Position_Current_Get(&lPosition))
    {
        FOR_EACH_AXIS(a)
        {
            aOut->mAxis_deg[a] = lPosition.mAxis_deg[a] - mConfig.mAxis[a].mOffset_deg;
        }

        lResult = ZT::ZT_OK;
    }

    return lResult;
}

ZT::Result Gimbal::Position_Set(const ZT::IGimbal::Position & aIn, unsigned int aFlags, unsigned int aDuration_ms)
{
    Position lPosition;

    FOR_EACH_AXIS(a)
    {
        if (0 == (aFlags & ZT_FLAG_IGNORE(a)))
        {
            lPosition.mAxis_deg[a] = aIn.mAxis_deg[a] + mConfig.mAxis[a].mOffset_deg;
        }
    }

    ZT::Result lResult = Position_Validate(lPosition, aFlags);
    if (ZT::ZT_OK == lResult)
    {
        // TRACE_DEBUG(stdout, "Gimbal::Position_Set - --> MOVING");
        mPosition_Flags &= aFlags;
        mPosition_State  = STATE_MOVING;

        Position_Copy(&mPosition_Target, lPosition, aFlags);
    }

    return lResult;
}

ZT::Result Gimbal::Speed_Get(ZT::IGimbal::Speed * aOut)
{
    assert(NULL != aOut);
    
    ZT::Result lResult = ZT::ZT_OK;

    switch (mPosition_State)
    {
    case STATE_KNOWN:
    case STATE_UNKNOWN: *aOut = SPEED_STOPPED; break;

    case STATE_MOVING: lResult = ZT::ZT_ERROR_STATE; break;

    case STATE_SPEED: *aOut = mSpeed; break;

    default: assert(false);
    }

    return ZT::ZT_OK;
}

ZT::Result Gimbal::Speed_Set(const ZT::IGimbal::Speed & aIn, unsigned int aFlags)
{
    ZT::Result lResult = Speed_Validate(aIn, aFlags);
    if (ZT::ZT_OK == lResult)
    {
        // TRACE_DEBUG(stdout, "Gimbal::Speed_Set - --> KNOWN");
        mPosition_State = STATE_KNOWN;

        Speed_Copy(&mSpeed, aIn, aFlags);

        FOR_EACH_AXIS(a)
        {
            if (0.0 != mSpeed.mAxis_deg_s[a])
            {
                // TRACE_DEBUG(stdout, "Gimbal::Speed_Set - UNKNOWN --> SPEED");
                mPosition_State = STATE_SPEED;
            }
        }
    }

    return lResult;
}

ZT::Result Gimbal::Speed_Stop()
{
    // TRACE_DEBUG(stdout, "Gimbal::Speed_Stop - --> KNOWN");
    mPosition_State = STATE_KNOWN;
    memset(&mSpeed, 0, sizeof(mSpeed));
    return ZT::ZT_OK;
}

// ===== ZT::IObject ========================================================

void Gimbal::Release()
{
    assert(0 < mRefCount);

    mRefCount --;
    if (0 == mRefCount)
    {
        delete this;
    }
}

// Protected
/////////////////////////////////////////////////////////////////////////////

void Gimbal::Display(FILE * aOut, const Position & aIn)
{
    assert(NULL != aOut);

    fprintf(aOut, "{");

    FOR_EACH_AXIS(a)
    {
        fprintf(aOut, " %f deg ", aIn.mAxis_deg[a]);
    }

    fprintf(aOut, "}\n");
}

ZT::Result Gimbal::Config_Validate(const Config & aIn) const
{
    ZT::Result lResult = ZT::ZT_OK;

    for (unsigned int a = 0; (a < AXIS_QTY) && (ZT::ZT_OK == lResult); a++)
    {
        lResult = Config_Axis_Validate(aIn.mAxis[a], mInfo.mAxis[a]);
    }

    return lResult;
}

bool Gimbal::IsFocusMoving() const
{
    return (FOCUS_SPEED_STOP_pc_s != mFocus_Speed_pc_s);
}

bool Gimbal::Position_Current_Get(Position * aOut) const
{
    assert(NULL != aOut);

    * aOut = mPosition_Current;

    bool lResult = false;

    switch (mPosition_State)
    {
    case STATE_KNOWN:
    case STATE_MOVING:
    case STATE_SPEED: lResult = 0 < mPosition_Count; break;

    case STATE_UNKNOWN: break;

    default: assert(false);
    }

    return lResult;
}

Gimbal::State Gimbal::Position_State_Get() const { return mPosition_State; }

void Gimbal::Position_Update(const Position & aIn)
{
    mPosition_Count   = 15;
    mPosition_Current = aIn;

    switch (mPosition_State)
    {
    case STATE_KNOWN:
    case STATE_SPEED: break;

    case STATE_MOVING:
        FOR_EACH_AXIS(a)
        {
            if (0 == (mPosition_Flags & ZT_FLAG_IGNORE(a)))
            {
                if (Angle_Compare(mPosition_Target.mAxis_deg[a], aIn.mAxis_deg[a]))
                {
                    mPosition_Flags |= ZT_FLAG_IGNORE(a);
                }
            }
        }

        if (ZT_FLAG_IGNORE_ALL == mPosition_Flags)
        {
            // TRACE_DEBUG(stdout, "Gimbal::Position_Update - MOVING --> KNOWN");
            mPosition_State = STATE_KNOWN;
        }
        break;

    case STATE_UNKNOWN:
        TRACE_DEBUG(stdout, "Gimbal::Position_Update - UNKNOWN --> KNOWN");
        mPosition_State = STATE_KNOWN;
        break;

    default: assert(false);
    }
}

ZT::Result Gimbal::Position_Validate(const Position & aIn, unsigned int aFlags) const
{
    ZT::Result lResult = ZT::ZT_OK;

    for (unsigned int a = 0; (a < AXIS_QTY) && (ZT::ZT_OK == lResult); a++)
    {
        if (0 == (aFlags & ZT_FLAG_IGNORE(a)))
        {
            lResult = ::Position_Validate(aIn.mAxis_deg[a], mConfig.mAxis[a]);
        }
    }

    return lResult;
}

void Gimbal::Tick()
{
    switch (mPosition_State)
    {
    case STATE_KNOWN:
        if (0 == mPosition_Count)
        {
            TRACE_DEBUG(stdout, "Gimbal::Tick - KNOWN --> UNKNOWN");
            mPosition_State = STATE_UNKNOWN;
        }
        break;

    case STATE_MOVING:
    case STATE_SPEED:
    case STATE_UNKNOWN: break;
    
    default: assert(false);
    }

    if (0 < mPosition_Count)
    {
        mPosition_Count--;
    }
}

// Private
/////////////////////////////////////////////////////////////////////////////

ZT::Result Gimbal::Speed_Validate(const Speed & aIn, unsigned int aFlags) const
{
    ZT::Result lResult = ZT::ZT_OK;

    for (unsigned int a = 0; (a < AXIS_QTY) && (ZT::ZT_OK == lResult); a++)
    {
        if (0 == (aFlags & ZT_FLAG_IGNORE(a)))
        {
            lResult = ::Speed_Validate(aIn.mAxis_deg_s[a], mInfo.mAxis[a].mSpeed_Max_deg_s);
        }
    }

    return lResult;
}

// Static functions
/////////////////////////////////////////////////////////////////////////////

bool Angle_Compare(double aA_deg, double aB_deg)
{
    double lDiff_deg = aA_deg - aB_deg;
    double lDiff2_deg2 = lDiff_deg * lDiff_deg;

    return (0.1 > lDiff2_deg2);
}

void Config_Axis_Init(ZT::IGimbal::Config_Axis * aOut)
{
    assert(NULL != aOut);

    aOut->mMax_deg = ZT::IGimbal::POSITION_MAX_deg;
    aOut->mMin_deg = ZT::IGimbal::POSITION_MIN_deg;
    aOut->mOffset_deg = ANGLE_OFFSET_DEFAULT_deg;
    aOut->mSpeed_deg_s = SPEED_DEFAULT_deg_s;
    aOut->mStiffness_pc = STIFFNESS_DEFAULT_pc;
}

ZT::Result Config_Axis_Validate(const ZT::IGimbal::Config_Axis & aIn, const ZT::IGimbal::Info_Axis & aInfo)
{
    if (ZT::IGimbal::POSITION_MAX_deg < aIn.mMax_deg)
    {
        return ZT::ZT_ERROR_ANGLE_MAX;
    }

    if ((ZT::IGimbal::POSITION_MIN_deg > aIn.mMin_deg) || (aIn.mMax_deg < aIn.mMin_deg))
    {
        return ZT::ZT_ERROR_ANGLE_MIN;
    }

    if ((SPEED_MIN_deg_s > aIn.mSpeed_deg_s) || (aInfo.mSpeed_Max_deg_s < aIn.mSpeed_deg_s))
    {
        return ZT::ZT_ERROR_SPEED;
    }

    return Value_Validate(aIn.mStiffness_pc, 0.0, 100.0);
}

void Position_Copy(ZT::IGimbal::Position * aOut, const ZT::IGimbal::Position & aIn, unsigned int aFlags)
{
    assert(NULL != aOut);

    FOR_EACH_AXIS(a)
    {
        if (0 == (aFlags & ZT_FLAG_IGNORE(a)))
        {
            aOut->mAxis_deg[a] = aIn.mAxis_deg[a];
        }
    }
}

ZT::Result Position_Validate(double aIn, const ZT::IGimbal::Config_Axis & aConfig)
{
    if (aConfig.mMax_deg < aIn)
    {
        return ZT::ZT_ERROR_ANGLE_MAX;
    }

    if (aConfig.mMin_deg > aIn)
    {
        return ZT::ZT_ERROR_ANGLE_MIN;
    }

    return ZT::ZT_OK;
}

void Speed_Copy(ZT::IGimbal::Speed * aOut, const ZT::IGimbal::Speed & aIn, unsigned int aFlags)
{
    assert(NULL != aOut);

    FOR_EACH_AXIS(a)
    {
        if (0 == (aFlags & ZT_FLAG_IGNORE(a)))
        {
            aOut->mAxis_deg_s[a] = aIn.mAxis_deg_s[a];
        }
    }
}

static ZT::Result Speed_Validate(double aIn_deg_s, double aMax_deg_s)
{
    if (aMax_deg_s < aIn_deg_s)
    {
        return ZT::ZT_ERROR_SPEED_MAX;
    }

    if ((-aMax_deg_s) > aIn_deg_s)
    {
        return ZT::ZT_ERROR_SPEED_MIN;
    }

    return ZT::ZT_OK;
}
