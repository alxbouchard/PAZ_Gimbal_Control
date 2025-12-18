
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib_Test/Gamepad.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <ZT/IGamepad.h>
#include <ZT/IMessageReceiver.h>
#include <ZT/ISystem.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_GAMEPAD (1)

// Test class
// //////////////////////////////////////////////////////////////////////////

class Tester : public ZT::IMessageReceiver
{

public:

    // ===== ZT::IMessageReceiver ==========================================

    virtual bool ProcessMessage(void * aSender, unsigned int aCode, const void * aData);

};

// Tests
// //////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Gamepad_SetupB)
{
    ZT::IGamepad * lG0;
    Tester         lTester;

    ZT::ISystem * lS0 = ZT::ISystem::Create();
    KMS_TEST_ASSERT_RETURN(NULL != lS0);

    KMS_TEST_COMPARE_GOTO(ZT::ZT_OK, lS0->Gamepads_Detect(), Cleanup);

    lG0 = lS0->Gamepad_Get(0);
    KMS_TEST_ASSERT_GOTO(NULL != lG0, Cleanup);

    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Receiver_Start(&lTester, MSG_GAMEPAD));

    sleep(30);

    KMS_TEST_COMPARE(ZT::ZT_OK, lG0->Receiver_Stop());

    lG0->Release();

Cleanup:
    lS0->Release();
}
KMS_TEST_END

// Public
// //////////////////////////////////////////////////////////////////////////

bool Tester::ProcessMessage(void * aSender, unsigned int aCode, const void * aData)
{
    assert(NULL != aSender);
    assert(MSG_GAMEPAD == aCode);
    assert(NULL != aData);

    printf("Tester::ProcessMessage( , ,  )\n");

    const ZT::IGamepad::Event * lEvent = reinterpret_cast<const ZT::IGamepad::Event *>(aData);

    printf("   Action  : %s\n", ZT::IGamepad::ACTION_NAMES [lEvent->mAction ]);
    printf("   Control : %s\n", ZT::IGamepad::CONTROL_NAMES[lEvent->mControl]);
    printf("   Value   : %f\n", lEvent->mValue_pc);

    return true;
}
