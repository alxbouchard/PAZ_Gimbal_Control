
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Gamepad.h

#pragma once

// ===== Includes ===========================================================
#include <ZT/IGamepad.h>

class Gamepad : public ZT::IGamepad
{

public:

    virtual ~Gamepad();

    void IncRefCount();

    // ===== ZT::IGamepad ===================================================

    virtual void Debug(void * aOut = NULL);

    virtual ZT::Result Receiver_Start(ZT::IMessageReceiver * aReceiver, unsigned int aCode);
    virtual ZT::Result Receiver_Stop();

    // ===== ZT::IObject ====================================================
    virtual void Release();

protected:

    Gamepad();

    bool Call(const ZT::IGamepad::Event * aEvent);

private:

    ZT::IMessageReceiver * mReceiver;
    unsigned int           mReceiver_Code;

    unsigned int mRefCount;

};
