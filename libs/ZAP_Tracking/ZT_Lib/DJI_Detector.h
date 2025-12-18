
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/DJI_Detector.h

#pragma once

// ===== Import/Includes ====================================================
#include <EthCAN/System.h>

// ===== ZT_Lib =============================================================
#include "IDetector.h"

// Class
/////////////////////////////////////////////////////////////////////////////

class DJI_Detector : public IDetector
{

public:

    DJI_Detector();

    virtual ~DJI_Detector();

    // ===== IDetector ======================================================
    virtual void Gamepads_Detect(GamepadList *aList);
    virtual void Gimbals_Detect (GimbalList  *aList);

private:

    EthCAN::System * mSystem;

};
