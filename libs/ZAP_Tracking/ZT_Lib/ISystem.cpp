
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/ISystem.cpp

#include "Component.h"

// ===== ZT_Lib =============================================================
#include "System.h"

namespace ZT
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    ISystem * ISystem::Create()
    {
        return new System();
    }

}

