
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    Includes/ZT/IMessageReceiver.h

#pragma once

namespace ZT
{

    class IMessageReceiver
    {

    public:

        virtual bool ProcessMessage(void * aSender, unsigned int aCode, const void * aData) = 0;

    };

}
