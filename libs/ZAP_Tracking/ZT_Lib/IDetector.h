
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/IDetector.h

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== ZT_Lib =============================================================
#include "Gamepad.h"
#include "Gimbal.h"

// Interface
/////////////////////////////////////////////////////////////////////////////

class IDetector
{

public:

    typedef std::list<Gamepad *> GamepadList;
    typedef std::list<Gimbal  *> GimbalList;

    virtual ~IDetector();

    virtual void Gamepads_Detect(GamepadList *aList) = 0;
    virtual void Gimbals_Detect (GimbalList  *aList) = 0;

};
