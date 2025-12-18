
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib_Test/ControlLink.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <ZT/IControlLink.h>
#include <ZT/IGamepad.h>
#include <ZT/ISystem.h>

// Tests
// //////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(ControlLink_Base)
{
    ZT::IControlLink * lC0 = ZT::IControlLink::Create();
    KMS_TEST_ASSERT_RETURN(NULL != lC0);

    KMS_TEST_COMPARE(ZT::ZT_ERROR_FILE_OPEN, lC0->ReadConfigFile("DoesNotExist"));

    KMS_TEST_COMPARE(ZT::ZT_ERROR_CONFIG, lC0->ReadConfigFile("ZT_Lib/Tests/Config_0.txt"));

    KMS_TEST_COMPARE(ZT::ZT_OK, lC0->ReadConfigFile("ZT_Lib/Tests/Config_1.txt"));

    lC0->Release();
}
KMS_TEST_END

KMS_TEST_BEGIN(ControlLink_SetupC)
{
    ZT::IControlLink * lC0 = ZT::IControlLink::Create();
    ZT::ISystem      * lS0 = ZT::ISystem     ::Create();
    KMS_TEST_ASSERT_RETURN(NULL != lC0);
    KMS_TEST_ASSERT_RETURN(NULL != lS0);

    KMS_TEST_COMPARE_RETURN(ZT::ZT_OK, lS0->Gamepads_Detect());
    KMS_TEST_COMPARE_RETURN(ZT::ZT_OK, lS0->Gimbals_Detect());

    ZT::IGamepad * lG0 = lS0->Gamepad_Get(0);
    KMS_TEST_ASSERT_RETURN(NULL != lG0);

    lC0->Gamepad_Set(lG0);
    lC0->Gimbals_Set(lS0);

    lC0->Start();

    sleep(60);

    lC0->Stop();

    lC0->Release();
    lG0->Release();
    lS0->Release();
}
KMS_TEST_END
