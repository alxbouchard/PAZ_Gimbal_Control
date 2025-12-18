
// Author  KMS - Martin Dubois, P. Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Gimbal.h

#pragma once

// ===== Includes ===========================================================
#include <ZT/IGimbal.h>

// Class
/////////////////////////////////////////////////////////////////////////////

class Gimbal : public ZT::IGimbal
{

public:

    Gimbal();

    virtual ~Gimbal();

    void IncRefCount();

    // ===== ZT::IGimbal ====================================================

    virtual ZT::Result Activate();

    virtual void       Config_Get(      Config * aOut) const;
    virtual ZT::Result Config_Set(const Config & aIn);

    virtual ZT::Result Focus_Position_Set(double aFocus);
    virtual ZT::Result Focus_Speed_Set(double aFocus);

    virtual void Info_Get(Info * aOut) const;

    virtual ZT::Result Position_Get(Position * aOut);
    virtual ZT::Result Position_Set(const Position & aIn, unsigned int aFlags, unsigned int aDuration_ms);

    virtual ZT::Result Speed_Get(Speed * aOut);
    virtual ZT::Result Speed_Set(const Speed & aIn, unsigned int aFlags);
    virtual ZT::Result Speed_Stop();

    // ===== ZT::IObject ====================================================
    virtual void Release();

protected:

    // Internaly, all position include offset.

    //             +----+================> POSITION
    //             |    |                   | | |
    // --+==> UNKNOWN --|----+==> MOVING <--+ | |
    //   |     |   |    |    |                | |
    //   |     |   +----|--> KNOWN <----------+ |
    //   |     |        |    |                  |
    //   |     +--> SPEED <==+------------------+
    //   |           |
    //   +-----------+
    typedef enum
    {
        STATE_KNOWN,
        STATE_MOVING,
        STATE_SPEED,
        STATE_UNKNOWN,

        STATE_QTY
    }
    State;

    static void Display(FILE * aOut, const Position & aIn);

    ZT::Result Config_Validate(const Config & aIn) const;

    bool IsFocusMoving  () const;

    bool       Position_Current_Get(Position * aOut) const;
    State      Position_State_Get() const;
    void       Position_Update(const Position & aIn);
    ZT::Result Position_Validate(const Position & aIn, unsigned int aFlags = 0) const;

    void Tick();

    Config   mConfig;
    double   mFocus_Position_pc;
    double   mFocus_Speed_pc_s;
    Info     mInfo;
    unsigned int mPosition_Flags;
    Position     mPosition_Target;
    Speed    mSpeed;

private:

    ZT::Result Speed_Validate(const Speed & aIn, unsigned int aFlags = 0) const;

    unsigned int mPosition_Count;
    Position     mPosition_Current;
    State        mPosition_State;

    unsigned int mRefCount;

};

// Macros
/////////////////////////////////////////////////////////////////////////////

#define FOR_EACH_AXIS(A) for (unsigned int A = 0; A < ZT::IGimbal::AXIS_QTY; A ++)
