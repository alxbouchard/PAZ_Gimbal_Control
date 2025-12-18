
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/OSX_Gamepad.cpp

// CODE REVIEW 2021-07-22 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== OSX ================================================================
#include <IOKit/IOCFPlugIn.h>

// ===== ZT_Lib =============================================================
#include "OSX_Gamepad.h"

// Data types
// //////////////////////////////////////////////////////////////////////////

typedef enum
{
    TYPE_CONST,
    TYPE_VALUE_16,
    TYPE_VALUE_8_HIGH,
    TYPE_VALUE_8_LOW,

    TYPE_END_OF_LIST
}
Type;

typedef struct
{
    Type mType;

    uint16_t mMask ;
    uint16_t mValue;

    ZT::IGamepad::Event mEvent;
}
TableEntry;

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_DUMMY (1)

#define MSG_THREAD_ITERATION (2)
#define MSG_THREAD_START     (3)

static const TableEntry TABLE_1[] =
{
    { TYPE_CONST, 0x0001, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::PAD_TOP        , 0.0 } },
    { TYPE_CONST, 0x0001, 0x0001, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::PAD_TOP        , 0.0 } },
    { TYPE_CONST, 0x0002, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::PAD_BOTTOM     , 0.0 } },
    { TYPE_CONST, 0x0002, 0x0002, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::PAD_BOTTOM     , 0.0 } },
    { TYPE_CONST, 0x0004, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::PAD_LEFT       , 0.0 } },
    { TYPE_CONST, 0x0004, 0x0004, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::PAD_LEFT       , 0.0 } },
    { TYPE_CONST, 0x0008, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::PAD_RIGHT      , 0.0 } },
    { TYPE_CONST, 0x0008, 0x0008, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::PAD_RIGHT      , 0.0 } },
    { TYPE_CONST, 0x0010, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::BUTTON_START   , 0.0 } },
    { TYPE_CONST, 0x0010, 0x0010, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::BUTTON_START   , 0.0 } },
    { TYPE_CONST, 0x0020, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::BUTTON_BACK    , 0.0 } },
    { TYPE_CONST, 0x0020, 0x0020, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::BUTTON_BACK    , 0.0 } },
    { TYPE_CONST, 0x0040, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::BUTTON_ANALOG_0, 0.0 } },
    { TYPE_CONST, 0x0040, 0x0040, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::BUTTON_ANALOG_0, 0.0 } },
    { TYPE_CONST, 0x0080, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::BUTTON_ANALOG_1, 0.0 } },
    { TYPE_CONST, 0x0080, 0x0080, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::BUTTON_ANALOG_1, 0.0 } },
    { TYPE_CONST, 0x0100, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::BUTTON_LEFT    , 0.0 } },
    { TYPE_CONST, 0x0100, 0x0100, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::BUTTON_LEFT    , 0.0 } },
    { TYPE_CONST, 0x0200, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::BUTTON_RIGHT   , 0.0 } },
    { TYPE_CONST, 0x0200, 0x0200, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::BUTTON_RIGHT   , 0.0 } },
    { TYPE_CONST, 0x1000, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::BUTTON_A       , 0.0 } },
    { TYPE_CONST, 0x1000, 0x1000, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::BUTTON_A       , 0.0 } },
    { TYPE_CONST, 0x2000, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::BUTTON_B       , 0.0 } },
    { TYPE_CONST, 0x2000, 0x2000, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::BUTTON_B       , 0.0 } },
    { TYPE_CONST, 0x4000, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::BUTTON_X       , 0.0 } },
    { TYPE_CONST, 0x4000, 0x4000, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::BUTTON_X       , 0.0 } },
    { TYPE_CONST, 0x8000, 0x0000, { ZT::IGamepad::ACTION_RELEASED, ZT::IGamepad::BUTTON_Y       , 0.0 } },
    { TYPE_CONST, 0x8000, 0x8000, { ZT::IGamepad::ACTION_PRESSED , ZT::IGamepad::BUTTON_Y       , 0.0 } },

    { TYPE_END_OF_LIST }
};

static const TableEntry TABLE_2[] =
{
    { TYPE_VALUE_8_HIGH, 0xff00, 0, { ZT::IGamepad::ACTION_CHANGED, ZT::IGamepad::TRIGGER_RIGHT, 0.0 } },
    { TYPE_VALUE_8_LOW , 0x00ff, 0, { ZT::IGamepad::ACTION_CHANGED, ZT::IGamepad::TRIGGER_LEFT , 0.0 } },

    { TYPE_END_OF_LIST }
};

static const TableEntry TABLE_3[] =
{
    { TYPE_VALUE_16, 0xffff, 0, { ZT::IGamepad::ACTION_CHANGED, ZT::IGamepad::ANALOG_0_X, 0.0 } },

    { TYPE_END_OF_LIST }
};

static const TableEntry TABLE_4[] =
{
    { TYPE_VALUE_16, 0xffff, 0, { ZT::IGamepad::ACTION_CHANGED, ZT::IGamepad::ANALOG_0_Y, 0.0 } },

    { TYPE_END_OF_LIST }
};

static const TableEntry TABLE_5[] =
{
    { TYPE_VALUE_16, 0xffff, 0, { ZT::IGamepad::ACTION_CHANGED, ZT::IGamepad::ANALOG_1_X, 0.0 } },

    { TYPE_END_OF_LIST }
};

static const TableEntry TABLE_6[] =
{
    { TYPE_VALUE_16, 0xffff, 0, { ZT::IGamepad::ACTION_CHANGED, ZT::IGamepad::ANALOG_1_Y, 0.0 } },

    { TYPE_END_OF_LIST }
};

static const TableEntry * TABLE[10] =
{
    NULL, TABLE_1, TABLE_2, TABLE_3, TABLE_4, TABLE_5, TABLE_6, NULL, NULL, NULL,
};

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static ZT::Result QueryInterface(io_service_t aService, CFUUIDRef aType, REFIID aInterfaceId, LPVOID * aInterface);

// Public
// //////////////////////////////////////////////////////////////////////////

OSX_Gamepad::OSX_Gamepad(io_service_t aService)
    : mDeviceInterface(NULL)
    , mService(aService)
    , mState(STATE_INIT)
    , mUSBInterface(NULL)
{
}

// ===== Gamepad ============================================================

OSX_Gamepad::~OSX_Gamepad()
{
    IOReturn lRetIO;

    switch (mState)
    {
    case STATE_INTERFACE_OPEN:
        Interface_Close();
        // no break

    case STATE_DEVICE_OPEN:
        if (NULL != mUSBInterface)
        {
            (*mUSBInterface)->Release(mUSBInterface);   
        }

        lRetIO = (* mDeviceInterface)->USBDeviceClose(mDeviceInterface);
        assert(kIOReturnSuccess == lRetIO);
        // no break

    case STATE_INIT:
        if (NULL != mDeviceInterface)
        {
            (*mDeviceInterface)->Release(mDeviceInterface);
        }

        kern_return_t lRetK;

        lRetK = IOObjectRelease(mService);
        assert(KERN_SUCCESS == lRetK);
        break;

    default: assert(false);
    }
}

ZT::Result OSX_Gamepad::Connect()
{
    ZT::Result lRetZ = QueryInterface(mService, kIOUSBDeviceUserClientTypeID, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID300), reinterpret_cast<LPVOID *>(&mDeviceInterface));
    if (ZT::ZT_OK != lRetZ)
    {
        return lRetZ;
    }

    IOReturn lRetIO = (*mDeviceInterface)->USBDeviceOpen(mDeviceInterface);
    if (kIOReturnSuccess != lRetIO)
    {
        TRACE_ERROR(stderr, "USBDeviceOpen( , ,  ) failed");
        return ZT::ZT_ERROR_NOT_A_GAMEPAD;
    }

    mState = STATE_DEVICE_OPEN;

    lRetIO = (*mDeviceInterface)->SetConfiguration(mDeviceInterface, 1);
    if (kIOReturnSuccess != lRetIO)
    {
        TRACE_ERROR(stderr, "SetConfiguration( ,  ) failed");
        return ZT::ZT_ERROR_NOT_A_GAMEPAD;
    }

    IOUSBFindInterfaceRequest lRequest;
        
    memset(&lRequest, 0, sizeof(lRequest));
        
    lRequest.bInterfaceClass    = 0xff;
    lRequest.bInterfaceProtocol = 0x01;
    lRequest.bInterfaceSubClass = 0x5d;
    
    io_iterator_t lIterator;
    
    lRetIO = (*mDeviceInterface)->CreateInterfaceIterator(mDeviceInterface, &lRequest, &lIterator);
    assert( kIOReturnSuccess == lRetIO );
    
    io_service_t lService;
    
    while ((lService = IOIteratorNext(lIterator)))
    {
        lRetZ = QueryInterface(lService, kIOUSBInterfaceUserClientTypeID, CFUUIDGetUUIDBytes(kIOUSBInterfaceInterfaceID700), reinterpret_cast<LPVOID *>(&mUSBInterface));

        kern_return_t lRetK = IOObjectRelease(lService);
        assert(KERN_SUCCESS == lRetK);

        if (ZT::ZT_OK != lRetZ)
        {
            continue;
        }

        lRetIO = (*mUSBInterface)->USBInterfaceOpen(mUSBInterface);
        if (kIOReturnSuccess != lRetIO)
        {
            TRACE_ERROR(stderr, "USBInterfaceOpen(  ) failed");
            (*mUSBInterface)->Release(mUSBInterface);
            mUSBInterface = NULL;
            continue;
        }

        mState = STATE_INTERFACE_OPEN;
        break;
    }
    
    if (NULL == mUSBInterface)
    {
        TRACE_ERROR(stderr, "No interface");
        return ZT::ZT_ERROR_NOT_A_GAMEPAD;
    }
    
    return ZT::ZT_OK;
}

// ===== ZT::IGamepad =======================================================

void OSX_Gamepad::Debug(void * aOut)
{
    FILE * lOut = (NULL == aOut) ? stdout : reinterpret_cast<FILE *>(aOut);

    fprintf(lOut, "OSX_Gamepad\n");
    fprintf(lOut, "    Report :");
    for (unsigned int i = 0; i < 10; i ++)
    {
        fprintf(lOut, " %04x", mReport[i]);
    }
    fprintf(lOut, "\n");
    fprintf(lOut, "    State  : %u\n", mState);

    Gamepad::Debug(lOut);
}

ZT::Result OSX_Gamepad::Receiver_Start(IMessageReceiver * aReceiver, unsigned int aCode)
{
    ZT::Result lResult = Gamepad::Receiver_Start(aReceiver, aCode);
    if (ZT::ZT_OK == lResult)
    {
        memset(&mReport, 0, sizeof(mReport));

        mReport[0] = 0x1400;

        lResult = mThread.Start(this, MSG_THREAD_START, MSG_THREAD_ITERATION, MSG_DUMMY);
    }

    return lResult;
}

ZT::Result OSX_Gamepad::Receiver_Stop()
{
    assert(STATE_INTERFACE_OPEN == mState);

    Interface_Close();

    ZT::Result lResult = mThread.Stop();
    if (ZT::ZT_OK == lResult)
    {
        lResult = Gamepad::Receiver_Stop();
    }

    return lResult;
}

// ===== ZT::IMessageReceiver ===============================================

bool OSX_Gamepad::ProcessMessage(void * aSender, unsigned int aCode, const void * aData)
{
    bool lResult = false;

    switch (aCode)
    {
    case MSG_DUMMY: lResult = true; break;

    case MSG_THREAD_ITERATION: lResult = Thread_Iteration(); break;
    case MSG_THREAD_START    : lResult = Thread_Start    (); break;

    default: assert(false);
    }

    return lResult;
}

// Private
// //////////////////////////////////////////////////////////////////////////

void OSX_Gamepad::Interface_Close()
{
    IOReturn lRetIO = (*mUSBInterface)->USBInterfaceClose(mUSBInterface);
    assert(kIOReturnSuccess == lRetIO);
    mState = STATE_DEVICE_OPEN;
}

bool OSX_Gamepad::Report_Process(const uint16_t * aIn)
{
    bool lResult = true;

    for (unsigned int i = 0; i < sizeof(mReport) / sizeof(uint16_t); i ++)
    {
        if (mReport[i] != aIn[i])
        {
            // printf("Report word %u changed from %04x to %04x\n", i, mReport[i], aIn[i]);

            const TableEntry * lEntry = TABLE[i];
            if (NULL != lEntry)
            {
                while (TYPE_END_OF_LIST != lEntry->mType)
                {
                    uint16_t lNew = aIn    [i] & lEntry->mMask;
                    uint16_t lOld = mReport[i] & lEntry->mMask;

                    if (lNew != lOld)
                    {
                        bool  lCall  = true;
                        Event lEvent = lEntry->mEvent;

                        switch (lEntry->mType)
                        {
                        case TYPE_CONST:
                            lCall = (lEntry->mValue == lNew);
                            break;

                        case TYPE_VALUE_16:
                            lEvent.mValue_pc  = static_cast<int16_t>(lNew);
                            lEvent.mValue_pc /= 0x7ffff;
                            lEvent.mValue_pc *= 100.0;
                            break;

                        case TYPE_VALUE_8_HIGH:
                            lEvent.mValue_pc  = lNew >> 8;
                            lEvent.mValue_pc /= 0xff;
                            lEvent.mValue_pc *= 100.0;
                            break;

                        case TYPE_VALUE_8_LOW:
                            lEvent.mValue_pc  = lNew;
                            lEvent.mValue_pc /= 0xff;
                            lEvent.mValue_pc *= 100.0;
                            break;

                        default: assert(false);
                        }

                        if (lCall)
                        {
                            lResult = Call(&lEvent);
                        }
                    }

                    lEntry++;
                }
            }

            mReport[i] = aIn[i];
        }
    }

    return lResult;
}

bool OSX_Gamepad::Thread_Iteration()
{
    uint8_t lBuffer[20];
    bool    lResult = true;
    UInt32  lSize_byte = sizeof(lBuffer);

    IOReturn lRetIO = (*mUSBInterface)->ReadPipe(mUSBInterface, 0x01, lBuffer, &lSize_byte);
    switch (lRetIO)
    {
    case kIOReturnSuccess:
        switch (lSize_byte)
        {
        case 3 : break;

        case sizeof(mReport): lResult = Report_Process(reinterpret_cast<uint16_t *>(lBuffer)); break;

        default: assert(false);
        }
        break;

    case kIOReturnAborted:
        Event lEvent;
        memset(&lEvent, 0, sizeof(lEvent));
        lEvent.mAction = ACTION_DISCONNECTED;
        Call(&lEvent);
        lResult = false;
        break;

    case kIOReturnOverrun:
        TRACE_ERROR(stderr, "ReadPipe( , , ,  ) failed - kIOReturnOverrun");
        lResult = false;
        break;

    default:
        TRACE_ERROR(stderr, "ReadPipe( , , ,  ) failed");
        fprintf(stderr, "    lRetIO = 0x%08x\n", lRetIO);
        fprintf(stderr, "    lSize  = %u bytes\n", lSize_byte);
        lResult = false;
    }

    return lResult;
}

bool OSX_Gamepad::Thread_Start()
{
    assert(NULL !=  mUSBInterface);
    assert(NULL != *mUSBInterface);

    IOReturn lRetIO = (*mUSBInterface)->ResetPipe(mUSBInterface, 0x01);
    assert(kIOReturnSuccess == lRetIO);

    return true;
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

ZT::Result QueryInterface(io_service_t aService, CFUUIDRef aType, REFIID aInterfaceId, LPVOID * aInterface)
{
    IOCFPlugInInterface ** lPlugInInterface;
    SInt32                 lScore          ;

    kern_return_t lRetK = IOCreatePlugInInterfaceForService(aService, aType, kIOCFPlugInInterfaceID, &lPlugInInterface, &lScore);
    if (KERN_SUCCESS != lRetK)
    {
        TRACE_ERROR(stderr, "IOCreatePlugInInterfaceForService( , , , ,  ) failed");
        return ZT::ZT_ERROR_NOT_A_GAMEPAD;
    }

    HRESULT lRetH = (*lPlugInInterface)->QueryInterface(lPlugInInterface, aInterfaceId, aInterface);

    (*lPlugInInterface)->Release(lPlugInInterface);

    if ((0 != lRetH) || (NULL == (*aInterface)))
    {
        TRACE_ERROR(stderr, "QueryInterface( , ,  ) failed");
        return ZT::ZT_ERROR_NOT_A_GAMEPAD;
    }

    return ZT::ZT_OK;
}
