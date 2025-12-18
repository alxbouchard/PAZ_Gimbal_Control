
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/OSX_Gamepad.h

#pragma once

// ===== OSX ================================================================
#include <IOKit/IOKitLib.h>
#include <IOKit/usb/IOUSBLib.h>

// ===== Includes ===========================================================
#include <ZT/Result.h>

// ===== ZT_Lib =============================================================
#include "Gamepad.h"
#include "ZT_Lib/Thread.h"

class OSX_Gamepad : public Gamepad, public ZT::IMessageReceiver
{

public:

    OSX_Gamepad(io_service_t aService);

    // ===== Gimbal =========================================================
    
    virtual ~OSX_Gamepad();

    ZT::Result Connect();

    // ===== ZT::IGamepad ====================================================

    virtual void Debug(void * aOut);

    virtual ZT::Result Receiver_Start(ZT::IMessageReceiver * aReceiver, unsigned int aCode);

    virtual ZT::Result Receiver_Stop();

    // ===== ZT::IMessageReceiver ===========================================

    virtual bool ProcessMessage(void * aSender, unsigned int aCode, const void * aData);

private:

    // --> INIT <--+
    //      |      |
    //      +--> DEVICE_OPEN <--+
    //            |             |
    //            +--> INTERFACE_OPEN
    typedef enum
    {
        STATE_DEVICE_OPEN,
        STATE_INIT,
        STATE_INTERFACE_OPEN,

        STATE_QTY
    }
    State;

    void Interface_Close();

    bool Report_Process(const uint16_t * aIn);

    bool Thread_Iteration();
    bool Thread_Start();

    IOUSBDeviceInterface300 **mDeviceInterface;

    uint16_t mReport[10];

    io_service_t mService;

    State mState;

    ZT_Lib::Thread mThread;

    IOUSBInterfaceInterface700 **mUSBInterface;

};
