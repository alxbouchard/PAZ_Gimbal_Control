
// Author  KMS - Martin Dubois, P.Eng
// Client  ZAP
// Product Tracking
// File    ZT_Agent/Instance.h

#pragma once

// ===== Includes ===========================================================
#include <ZT/IControlLink.h>
#include <ZT/IGamepad.h>
#include <ZT/Result.h>

class MessageReceiver;

class Instance
{

public:

    Instance(ZT::IGamepad * aGamepad, unsigned int aIndex);

    ~Instance();

    ZT::Result Init(ZT::ISystem * aSystem);

    ZT::Result Start(MessageReceiver * aReceiver, unsigned int aCode);

    void Stop();

private:

    ZT::IControlLink * mControlLink;
    ZT::IGamepad     * mGamepad;
    unsigned int       mIndex;

};
