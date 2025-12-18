
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/System.cpp

// CODE REVIEW 2021-04-06 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== ZT_Lib =============================================================
#include "DJI_Detector.h"
#include "OSX_Detector.h"

#include "System.h"

// Public
/////////////////////////////////////////////////////////////////////////////

System::System() : mRefCount(1)
{
    mDetectors.push_back(new DJI_Detector());
    mDetectors.push_back(new OSX_Detector());
}

// ===== ZT::ISystem ========================================================

ZT::Result System::Gamepads_Detect()
{
    Gamepads_Release();

    for (DetectorList::iterator lIt = mDetectors.begin(); lIt != mDetectors.end(); lIt++)
    {
        (*lIt)->Gamepads_Detect(&mGamepads);
    }

    return ZT::ZT_OK;
}

ZT::IGamepad * System::Gamepad_Get(unsigned int aIndex)
{
    unsigned int lIndex = 0;

    for (IDetector::GamepadList::iterator lIt = mGamepads.begin(); lIt != mGamepads.end(); lIt ++)
    {
        if (aIndex == lIndex)
        {
            Gamepad * lGamepad = (*lIt);
            assert(NULL != lGamepad);

            mGamepads.erase(lIt);

            return lGamepad;
        }

        lIndex ++;
    }

    return NULL;
}

ZT::Result System::Gimbals_Detect()
{
    Gimbals_Release();

    for (DetectorList::iterator lIt = mDetectors.begin(); lIt != mDetectors.end(); lIt++)
    {
        (*lIt)->Gimbals_Detect(&mGimbals);
    }

    return ZT::ZT_OK;
}

ZT::IGimbal * System::Gimbal_Find_IPv4(const char * aIPv4)
{
    if (NULL == aIPv4)
    {
        TRACE_ERROR(stderr, "System::Gimbal_Find_IPv4 - Invalid address");
        return NULL;
    }

    unsigned int lIP[4];

    if (4 != sscanf(aIPv4, "%u.%u.%u.%u", lIP + 0, lIP + 1, lIP + 2, lIP + 3))
    {
        TRACE_ERROR(stderr, "System::Gimbal_Find_IPv4 - Invalid address format");
        return NULL;
    }

    uint32_t lIPv4 = 0;

    for (unsigned int i = 0; i < 4; i++)
    {
        if (255 < lIP[i])
        {
            TRACE_ERROR(stderr, "System::Gimbal_Find_IPv4 - Invalid address part");
            return NULL;
        }

        lIPv4 |= lIP[i] << (i * 8);
    }

    return Gimbal_Find_IPv4(lIPv4);
}

ZT::IGimbal * System::Gimbal_Find_IPv4(uint32_t aIPv4)
{
    for (IDetector::GimbalList::iterator lIt = mGimbals.begin(); lIt != mGimbals.end(); lIt ++)
    {
        Gimbal * lGimbal = (*lIt);
        assert(NULL != lGimbal);

        ZT::IGimbal::Info lInfo;

        lGimbal->Info_Get(&lInfo);

        if (aIPv4 == lInfo.mIPv4_Address)
        {
            mGimbals.erase(lIt);

            return lGimbal;
        }
    }

    return NULL;
}

ZT::IGimbal * System::Gimbal_Get(unsigned int aIndex)
{
    unsigned int lIndex = 0;

    for (IDetector::GimbalList::iterator lIt = mGimbals.begin(); lIt != mGimbals.end(); lIt ++)
    {
        if (aIndex == lIndex)
        {
            Gimbal * lGimbal = (*lIt);
            assert(NULL != lGimbal);

            mGimbals.erase(lIt);

            return lGimbal;
        }

        lIndex ++;
    }

    return NULL;
}

// ===== ZT::IObject ========================================================

void System::Release()
{
    mRefCount --;
    if (0 >= mRefCount)
    {
        delete this;
    }
}

// Private
/////////////////////////////////////////////////////////////////////////////

System::~System()
{
    Gamepads_Release();
    Gimbals_Release();

    Detectors_Release();
}

void System::Detectors_Release()
{
    for (DetectorList::iterator lIt = mDetectors.begin(); lIt != mDetectors.end(); lIt++)
    {
        delete (*lIt);
    }

    mDetectors.clear();
}

void System::Gamepads_Release()
{
    for (IDetector::GamepadList::iterator lIt = mGamepads.begin(); lIt != mGamepads.end(); lIt++)
    {
        (*lIt)->Release();
    }

    mGamepads.clear();
}

void System::Gimbals_Release()
{
    for (IDetector::GimbalList::iterator lIt = mGimbals.begin(); lIt != mGimbals.end(); lIt++)
    {
        (*lIt)->Release();
    }

    mGimbals.clear();
}
