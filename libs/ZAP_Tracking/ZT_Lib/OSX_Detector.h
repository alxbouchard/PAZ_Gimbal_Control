
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/OSX_Detector.h

#pragma once

// ===== OSX ================================================================
#include <CoreFoundation/CoreFoundation.h>

// ===== ZT_Lib =============================================================
#include "IDetector.h"

// Class
/////////////////////////////////////////////////////////////////////////////

class OSX_Detector : public IDetector
{

public:

    OSX_Detector();

    virtual ~OSX_Detector();

    // ===== IDetector ======================================================
    virtual void Gamepads_Detect(GamepadList *aList);
    virtual void Gimbals_Detect (GimbalList  *aList);

private:

    CFMutableDictionaryRef mMatchingDict;

};
