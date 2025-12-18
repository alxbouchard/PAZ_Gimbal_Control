
// Author  KMS - Martin Dubois, P.Eng
// Client  ZAP
// Product Tracking
// File    ZT_Agent/Component.cpp

// CODE REVIEW 2021-07-22 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <ZT/IGamepad.h>

// ===== ZT_Agent ===========================================================
#include "MessageReceiver.h"

// Constants
// //////////////////////////////////////////////////////////////////////////

#define COUNTER_DISCONNECTED (0xffffffff)

// Public
// //////////////////////////////////////////////////////////////////////////

const unsigned int MessageReceiver::CODE = 1;

MessageReceiver::MessageReceiver() : mCounter(0)
{
}

bool MessageReceiver::IsStopRequested()
{
    bool lResult = false;

    switch (mCounter)
    {
    case COUNTER_DISCONNECTED: lResult = true; break;

    case 0: break;

    default:
        lResult = (2 <= mCounter);
        mCounter --;
        break;
    }

    return lResult;
}

// ===== ZT::IMessageReceiver ===============================================

bool MessageReceiver::ProcessMessage(void * aSender, unsigned int aCode, const void * aData)
{
    const ZT::IGamepad::Event * lEvent = reinterpret_cast<const ZT::IGamepad::Event *>(aData);

    bool lResult = true;

    switch (aCode)
    {
    case CODE:
        switch (lEvent->mAction)
        {
        case ZT::IGamepad::ACTION_DISCONNECTED:
            mCounter = COUNTER_DISCONNECTED;
            lResult = false;
            break;

        default : mCounter ++;
        }
        break;

    default: assert(false); lResult = false;
    }

    return lResult;
}
