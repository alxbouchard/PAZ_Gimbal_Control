
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    Includes/ZT/IControlLink.h

#pragma once

#include <ZT/IObject.h>
#include <ZT/Result.h>

namespace ZT
{

    class IGamepad;
    class IMessageReceiver;
    class ISystem;

    class IControlLink : public IObject
    {

    public:

        static IControlLink * Create();

        virtual Result ReadConfigFile(const char * aFileName) = 0;

        virtual Result Gamepad_Set(IGamepad * aGamepad) = 0;

        virtual Result Gimbals_Set(ISystem * aSystem) = 0;

        virtual Result Receiver_Set(ZT::IMessageReceiver * aReceiver, unsigned int aConfigured, unsigned int aUnknown) = 0;

        // If Start return an error, the IControlLink instance must be
        // deleted using the Release method.
        virtual Result Start() = 0;

        // After stopping a ControlLink instance, it cannot be restarted
        // using the Start methode. It must be deleted using the Release
        // method.
        virtual Result Stop() = 0;

    };

}
