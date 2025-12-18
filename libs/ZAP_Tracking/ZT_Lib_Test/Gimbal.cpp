
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib_Test/Gimbal.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <ZT/IGimbal.h>
#include <ZT/ISystem.h>
#include <ZT/Result.h>

// Tests
// //////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Gimbal_SetupA)

    FILE * lNull = fopen("/dev/null", "wb");

    ZT::ISystem * lS0 = ZT::ISystem::Create();

    KMS_TEST_COMPARE(ZT::ZT_OK, lS0->Gimbals_Detect());

    ZT::IGimbal * lG0 = lS0->Gimbal_Get(0);
    KMS_TEST_ASSERT_RETURN(NULL != lG0);

    ZT::IGimbal::Config   lConfig;
    ZT::IGimbal::Info     lInfo;
    ZT::IGimbal::Position lPos;
    ZT::IGimbal::Speed    lSpeed;
    
    // Activate
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Activate());

    // Config_Get
    lG0->Config_Get(&lConfig);
    ZT::IGimbal::Display(stdout, lConfig);

    // Config_Set
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Config_Set(lConfig));

    // Focus_Cal
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Focus_Cal(ZT::IGimbal::OPERATION_CAL_AUTO_ENABLE));
    sleep(1);
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Focus_Cal(ZT::IGimbal::OPERATION_CAL_STOP));

    // Focus_Set
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Focus_Position_Set(50.0));
    sleep(10);
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Focus_Position_Set(30.0));
    sleep(10);
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Focus_Position_Set(50.0));

    // Info_Get
    lG0->Info_Get(&lInfo);
    ZT::IGimbal::Display(stdout, lInfo);

    // Position_Get
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Position_Get(&lPos));
    ZT::IGimbal::Display(stdout, lPos);

    // Position_Set
    lPos.mAxis_deg[0] -= 10.0;
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Position_Set(lPos));
    sleep(1);

    // Speed_Set
    lSpeed.mAxis_deg_s[ZT::IGimbal::AXIS_PITCH] = 0.0;
    lSpeed.mAxis_deg_s[ZT::IGimbal::AXIS_ROLL ] = 0.0;
    lSpeed.mAxis_deg_s[ZT::IGimbal::AXIS_YAW  ] = 5.0;
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Speed_Set(lSpeed));
    sleep(1);

    // Speed_Stop
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Speed_Stop());
    lPos.mAxis_deg[0] += 10.0;
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Position_Set(lPos));

    // Debug
    lG0->Debug(stdout);
    
    // Release
    lG0->Release();

    lS0->Release();

KMS_TEST_END

KMS_TEST_BEGIN(Gimbal_Focus_SetupA)
{
    ZT::ISystem * lS0 = ZT::ISystem::Create();

    KMS_TEST_COMPARE(ZT::ZT_OK, lS0->Gimbals_Detect());

    ZT::IGimbal * lG0 = lS0->Gimbal_Get(0);
    KMS_TEST_ASSERT_RETURN(NULL != lG0);

    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Activate());

    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Focus_Cal(ZT::IGimbal::OPERATION_CAL_AUTO_ENABLE));
    sleep(1);
    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Focus_Cal(ZT::IGimbal::OPERATION_CAL_STOP));

    for (unsigned int i = 0; i < 100; i ++)
    {
        KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Focus_Position_Set(i));
        usleep(100000);
    }

    lG0->Release();
    lS0->Release();
}
KMS_TEST_END
