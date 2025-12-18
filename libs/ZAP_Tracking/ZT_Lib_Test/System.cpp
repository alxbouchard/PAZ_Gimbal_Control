
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib_Test/System.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <ZT/ISystem.h>
#include <ZT/Result.h>

// Tests
// //////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(System_Base)

    // Create
    ZT::ISystem * lS0 = ZT::ISystem::Create();
    KMS_TEST_ASSERT(NULL != lS0);

    // Gamepads_Detect
    KMS_TEST_COMPARE(ZT::ZT_OK, lS0->Gamepads_Detect());
    
    // Gimbals_Detect
    KMS_TEST_COMPARE(ZT::ZT_OK, lS0->Gimbals_Detect());

    // Gimbal_Find_IPv4
    KMS_TEST_ASSERT(NULL == lS0->Gimbal_Find_IPv4(static_cast<const char *>(NULL)));
    KMS_TEST_ASSERT(NULL == lS0->Gimbal_Find_IPv4("Invalid"));
    KMS_TEST_ASSERT(NULL == lS0->Gimbal_Find_IPv4("0.0.0.0"));
    KMS_TEST_ASSERT(NULL == lS0->Gimbal_Find_IPv4(0x01010101));

    // Gimbal_Get
    KMS_TEST_ASSERT(NULL == lS0->Gimbal_Get(0));

    // Release
    lS0->Release();

KMS_TEST_END
