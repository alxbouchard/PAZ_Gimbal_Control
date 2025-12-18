
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/DJI_Detector.cpp

#include "Component.h"

// ===== ZT_Lib =============================================================
#include "DJI_Gimbal.h"

#include "DJI_Detector.h"

// Public
/////////////////////////////////////////////////////////////////////////////

DJI_Detector::DJI_Detector() : mSystem(EthCAN::System::Create())
{
    assert(NULL != mSystem);

    mSystem->SetTraceStream(stdout);
}

DJI_Detector::~DJI_Detector()
{
    assert(NULL != mSystem);

    mSystem->Release();
}

// ===== IDetector ==========================================================

void DJI_Detector::Gamepads_Detect(GamepadList *)
{
}

void DJI_Detector::Gimbals_Detect(GimbalList *aList)
{
    assert(NULL != aList);

    assert(NULL != mSystem);

    EthCAN_Result lRet = mSystem->Detect();
    assert(EthCAN_OK == lRet);

    unsigned int lCount = mSystem->Device_GetCount();
    for (unsigned int i = 0; i < lCount; i ++)
    {
        DJI_Gimbal * lGimbal = new DJI_Gimbal(mSystem->Device_Get(i));
        assert(NULL != lGimbal);

        ZT::Result lResult = lGimbal->Connect();
        if (ZT::ZT_OK == lResult)
        {
            aList->push_back(lGimbal);
        }
        else
        {
            TRACE_WARNING(stderr, "DJI_Detector::Detect - Not a DJI gimbal");
            Result_Display(lResult, stderr);
            lGimbal->Debug(stderr);
            delete lGimbal;
        }
    }
}
