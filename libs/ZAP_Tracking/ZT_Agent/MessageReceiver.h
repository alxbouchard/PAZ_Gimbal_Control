
// Author  KMS - Martin Dubois, P.Eng
// Client  ZAP
// Product Tracking
// File    ZT_Agent/MessageReceiver.h

#pragma once

// ===== Includes ===========================================================
#include <ZT/IMessageReceiver.h>

class MessageReceiver : public ZT::IMessageReceiver
{

public:

    static const unsigned int CODE;

    MessageReceiver();

    bool IsStopRequested();

    // ===== ZT::IMessageReceiver ===========================================
    virtual bool ProcessMessage(void * aSender, unsigned int aCode, const void * aData);

private:

    unsigned int mCounter;

};
