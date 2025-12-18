
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/System.h

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <ZT/ISystem.h>

// ===== ZT_Lib =============================================================
#include "IDetector.h"

// Class
/////////////////////////////////////////////////////////////////////////////

class System : public ZT::ISystem
{

public:

    System();

    // ===== ZT::ISystem ====================================================

    virtual ZT::Result Gamepads_Detect();

    virtual ZT::IGamepad * Gamepad_Get(unsigned int aIndex);

    virtual ZT::Result Gimbals_Detect();

    virtual ZT::IGimbal * Gimbal_Find_IPv4(const char * aIPv4);
    virtual ZT::IGimbal * Gimbal_Find_IPv4(uint32_t aIPv4);
    virtual ZT::IGimbal * Gimbal_Get(unsigned int aIndex);

    // ===== ZT::IObject ====================================================
    virtual void Release();

private:

    virtual ~System();

    void Detectors_Release();

    void Gamepads_Release();

    void Gimbals_Release();

    typedef std::list<IDetector *> DetectorList;

    DetectorList mDetectors;

    IDetector::GamepadList mGamepads;
    
    IDetector::GimbalList mGimbals;

    unsigned int mRefCount;

};
