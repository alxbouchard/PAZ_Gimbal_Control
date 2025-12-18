
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/IGamepad.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <ZT/IGamepad.h>

namespace ZT
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    const char * IGamepad::ACTION_NAMES[IGamepad::ACTION_QTY] =
    {
        "CHANGED",
        "DISCONNECTED",
        "PRESSED",
        "RELEASED",        
    };

    const char * IGamepad::CONTROL_NAMES[IGamepad::CONTROL_QTY] =
    {
        "CONTROL_NONE",
        "ANALOG_0_X",
        "ANALOG_0_Y",
        "ANALOG_1_X",
        "ANALOG_1_Y",
        "BUTTON_A",
        "BUTTON_ANALOG_0",
        "BUTTON_ANALOG_1",
        "BUTTON_B",
        "BUTTON_BACK",
        "BUTTON_LEFT",
        "BUTTON_RIGHT",
        "BUTTON_START",
        "BUTTON_X",
        "BUTTON_Y",
        "PAD_BOTTOM",
        "PAD_LEFT",
        "PAD_RIGHT",
        "PAD_TOP",
        "TRIGGER_LEFT",
        "TRIGGER_RIGHT",
    };

}
