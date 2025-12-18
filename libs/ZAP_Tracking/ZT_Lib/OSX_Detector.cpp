
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/OSX_Detector.cpp

#include "Component.h"

// ===== OSX ================================================================

// ===== ZT_Lib =============================================================
#include "OSX_Gamepad.h"

#include "OSX_Detector.h"

// Public
// //////////////////////////////////////////////////////////////////////////

OSX_Detector::OSX_Detector() : mMatchingDict(IOServiceMatching(kIOUSBDeviceClassName))
{
    SInt32 lValue;

    lValue = 0x045e;
    CFDictionarySetValue(mMatchingDict, CFSTR(kUSBVendorID ), CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &lValue));

    lValue = 0x28e;
    CFDictionarySetValue(mMatchingDict, CFSTR(kUSBProductID), CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &lValue));
}

OSX_Detector::~OSX_Detector()
{
}

// ===== IDetector ==========================================================

void OSX_Detector::Gamepads_Detect(GamepadList *aList)
{
    assert(NULL != aList);

    mach_port_t   lMasterPort ;
    kern_return_t lRetK;

    lRetK = IOMasterPort(MACH_PORT_NULL, & lMasterPort);
    assert(KERN_SUCCESS == lRetK);

    CFRetain(mMatchingDict);

    io_iterator_t lIterator;

    lRetK = IOServiceGetMatchingServices(lMasterPort, mMatchingDict, & lIterator);
    if (KERN_SUCCESS == lRetK)
    {
        bool         lRun    = true          ;
        io_service_t lService                ;

        while ((lService = IOIteratorNext(lIterator)))
        {
            OSX_Gamepad * lGamepad = new OSX_Gamepad(lService);
            assert(NULL != lGamepad);

            ZT::Result lRet = lGamepad->Connect();
            if (ZT::ZT_OK == lRet)
            {
                aList->push_back(lGamepad);
            }
            else
            {
                delete lGamepad;
            }
        }

        IOObjectRelease(lIterator);
    }

    IOObjectRelease(lMasterPort);
}

void OSX_Detector::Gimbals_Detect(GimbalList *)
{
}
