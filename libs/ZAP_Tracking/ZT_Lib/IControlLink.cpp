
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/IControlLink.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <ZT/IControlLink.h>

// ===== ZT_Lib =============================================================
#include "ControlLink.h"

namespace ZT
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    IControlLink * IControlLink::Create()
    {
        return new ControlLink();
    }

}
