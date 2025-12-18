
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Gamepad.h

// CODE REVIEW 2021-07-22 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <ZT/IMessageReceiver.h>

// ===== ZT_Lib =============================================================
#include "Gamepad.h"

// Public
// //////////////////////////////////////////////////////////////////////////

Gamepad::~Gamepad()
{
}

// ===== ZT::IGimbal ========================================================

void Gamepad::Debug(void * aOut)
{
    FILE * lOut = (NULL == aOut) ? stdout : reinterpret_cast<FILE *>(aOut);

    fprintf(lOut, "    Gimbal\n");
    fprintf(lOut, "        Receiver   : %s\n", (NULL == mReceiver) ? "NULL" : "Not NULL");
    fprintf(lOut, "        Recv. Code : %u\n", mReceiver_Code);
    fprintf(lOut, "        Ref. Count : %u\n", mRefCount);
}

ZT::Result Gamepad::Receiver_Start(ZT::IMessageReceiver * aReceiver, unsigned int aCode)
{
    if (NULL == aReceiver)
    {
        return ZT::ZT_ERROR_RECEIVER;
    }

    if (NULL != mReceiver)
    {
        return ZT::ZT_ERROR_ALREADY_STARTED;
    }

    mReceiver      = aReceiver;
    mReceiver_Code = aCode;

    return ZT::ZT_OK;
}

ZT::Result Gamepad::Receiver_Stop()
{
    if (NULL == mReceiver)
    {
        return ZT::ZT_ERROR_ALREADY_STOPPED;
    }

    mReceiver = NULL;

    return ZT::ZT_OK;
}

// ===== ZT::IObject ========================================================

void Gamepad::Release()
{
    if (1 == mRefCount)
    {
        delete this;
    }
    else
    {
        mRefCount --;
    }
}

// Protected
// //////////////////////////////////////////////////////////////////////////

Gamepad::Gamepad() : mReceiver(NULL), mReceiver_Code(0), mRefCount(1)
{
}

bool Gamepad::Call(const ZT::IGamepad::Event * aEvent)
{
    assert(NULL != mReceiver);
    assert(   0 != mReceiver_Code);

    return mReceiver->ProcessMessage(this, mReceiver_Code, aEvent);
}
