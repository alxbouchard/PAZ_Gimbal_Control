
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Atem.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <map>
#include <string>

// ===== OS X ===============================================================
#include <CoreFoundation/CFString.h>

// ===== ZT_Lib =============================================================
#include "BM/BMDSwitcherAPI.h"

#include "Atem.h"

// Data types
// //////////////////////////////////////////////////////////////////////////

typedef std::map<std::string, Atem *> AtemMap;

// Static variables
// //////////////////////////////////////////////////////////////////////////

static AtemMap                 sAtems;
static IBMDSwitcherDiscovery * sDiscovery = NULL;

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static bool SDK_Init   ();
static void SDK_Release();

// Public
// //////////////////////////////////////////////////////////////////////////

Atem * Atem::FindOrCreate(const char * aId)
{
    assert(NULL != aId);

    Atem * lResult = NULL;

    if (SDK_Init())
    {
        AtemMap::iterator lIt = sAtems.find(aId);
        if (sAtems.end() != lIt)
        {
            assert(NULL != lIt->second);
            
            lResult = lIt->second;
        }
        else
        {
            lResult = new Atem();
            assert(NULL != lResult);

            if (lResult->Connect(aId))
            {
                sAtems.insert(AtemMap::value_type(aId, lResult));
            }
            else
            {
                fprintf(stderr, "ERROR  Atem::Connect( \"%s\" ) failed\n", aId);
                delete lResult;
                lResult = NULL;
            }
        }
    }

    return lResult;
}

Atem::Atem() : mCameraControl(NULL), mSwitcher(NULL)
{
    memset(&mFocus_Positions, 0, sizeof(mFocus_Positions));
}

#define CAMERA_CONTROL IBMDSwitcherCameraControl * lCameraControl = reinterpret_cast<IBMDSwitcherCameraControl *>(mCameraControl)
#define SWITCHER       IBMDSwitcher              * lSwitcher      = reinterpret_cast<IBMDSwitcher              *>(mSwitcher)

Atem::~Atem()
{
    if (NULL != mSwitcher)
    {
        if (NULL != mCameraControl)
        {
            CAMERA_CONTROL;

            lCameraControl->Release();
        }

        SWITCHER;

        lSwitcher->Release();
    }

    for (AtemMap::iterator lIt = sAtems.begin(); lIt != sAtems.end(); lIt++)
    {
        if (this == lIt->second)
        {
            sAtems.erase(lIt);
            break;
        }
    }

    SDK_Release();
}

bool Atem::Aperture_Absolute(unsigned int aPort, double aValue_pc)
{
    assert(aPort >= 1);
    assert(aValue_pc >= 0.0);
    assert(aValue_pc <= 100.0);

    CAMERA_CONTROL;

    double lValue = aValue_pc / 100.0;

    HRESULT lRet = lCameraControl->SetFloats(aPort, 0, 3, 1, &lValue);
    return (S_OK == lRet);
}

bool Atem::Focus_Absolute(unsigned int aPort, double aValue_pc, CameraType aCameraType)
{
    assert(aPort >= 1);
    assert(aPort <= PORT_QTY);
    assert(aValue_pc >= 0.0);
    assert(aValue_pc <= 100.0);

    CAMERA_CONTROL;

    double lValue;

    HRESULT lRet;

    switch (aCameraType)
    {
    case CAMERA_EF:
        lValue = aValue_pc;
        double lCurrent;
        double lOffset;
        lCurrent = mFocus_Positions[aPort - 1];
        lOffset = lValue - lCurrent;
        lRet = lCameraControl->OffsetFloats(aPort, 0, 0, 1, &lOffset);
        if (S_OK == lRet)
        {
            mFocus_Positions[aPort - 1] = lValue;
        }
        break;

    case CAMERA_MFT:
        lValue = aValue_pc / 100.0;
        lRet = lCameraControl->SetFloats(aPort, 0, 0, 1, &lValue);
        break;

    default: assert(false);
    }

    return (S_OK == lRet);
}

bool Atem::Gain_Absolute(unsigned int aPort, double aValue_pc)
{
    assert(aPort >= 1);
    assert(aPort <= PORT_QTY);
    assert(aValue_pc >= 0.0);
    assert(aValue_pc <= 100.0);

    CAMERA_CONTROL;

    double lValues[4];

    lValues[0] = aValue_pc / 100.0 * 16.0;

    for (unsigned int i = 1; i < 4; i++)
    {
        lValues[i] = lValues[0];
    }

    HRESULT lRet = lCameraControl->SetFloats(aPort, 8, 2, 4, lValues);
    return (S_OK == lRet);
}

bool Atem::Zoom(unsigned int aPort, double aValue_pc)
{
    assert(aPort >= 1);
    assert(aValue_pc >= 0.0);
    assert(aValue_pc <= 100.0);

    CAMERA_CONTROL;

    double lValue = aValue_pc / 100.0;

    HRESULT lRet = lCameraControl->SetFloats(aPort, 0, 9, 1, &lValue);
    return (S_OK == lRet);
}

bool Atem::Zoom_Absolute(unsigned int aPort, double aValue_pc)
{
    assert(aPort >= 1);
    assert(aValue_pc >= 0.0);
    assert(aValue_pc <= 100.0);

    CAMERA_CONTROL;

    double lValue = aValue_pc / 100.0;

    HRESULT lRet = lCameraControl->SetFloats(aPort, 0, 8, 1, &lValue);
    return (S_OK == lRet);
}

bool Atem::Aperture_Auto(unsigned int aPort)
{
    assert(aPort >= 1);

    CAMERA_CONTROL;

    HRESULT lRet = lCameraControl->SetFlags(aPort, 0, 5, 0, NULL);
    return (S_OK == lRet);
}

bool Atem::Focus_Auto(unsigned int aPort)
{
    assert(aPort >= 1);

    CAMERA_CONTROL;

    HRESULT lRet = lCameraControl->SetFlags(aPort, 0, 1, 0, NULL);
    return (S_OK == lRet);
}

// Internal
// //////////////////////////////////////////////////////////////////////////

bool Atem::Connect(const char * aId)
{
    assert(NULL != aId);

    bool lResult = false;

    char lId[64];

    if (1 == sscanf(aId, "IPv4 = %[0-9.]", lId))
    {
        lResult = Connect_IPv4(lId);
    }

    if (lResult)
    {
        SWITCHER;

        IBMDSwitcherCameraControl * lCameraControl;

        HRESULT lRet = lSwitcher->QueryInterface(IID_IBMDSwitcherCameraControl, reinterpret_cast<void **>(&lCameraControl));
        lResult = S_OK == lRet;
        if (lResult)
        {
            assert(NULL != lCameraControl);

            mCameraControl = lCameraControl;
        }
    }

    return lResult;
}

bool Atem::Connect_IPv4(const char * aIPv4)
{
    assert(NULL != aIPv4);

    assert(NULL != sDiscovery);

    CFStringRef lIPv4 = CFStringCreateWithCString(NULL, aIPv4, kCFStringEncodingASCII);
    assert(NULL != lIPv4);

    BMDSwitcherConnectToFailure lFailReason;
    IBMDSwitcher * lSwitcher;

    HRESULT lRet = sDiscovery->ConnectTo(lIPv4, &lSwitcher, &lFailReason);

    free((void *)lIPv4);

    bool lResult = S_OK == lRet;
    if (lResult)
    {
        assert(NULL != lSwitcher);

        mSwitcher = lSwitcher;
    }

    return lResult;
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

bool SDK_Init()
{
    if (0 == sAtems.size())
    {
        assert(NULL == sDiscovery);

        sDiscovery = CreateBMDSwitcherDiscoveryInstance();
        if (NULL == sDiscovery)
        {
            fprintf(stderr, "ERROR  CreateBMDSwitcherDiscoveryInstance() failed\n");
        }
    }

    return (NULL != sDiscovery);
}

void SDK_Release()
{
    if ((NULL != sDiscovery) && (0 == sAtems.size()))
    {
        sDiscovery->Release();
        sDiscovery = NULL;
    }
}
