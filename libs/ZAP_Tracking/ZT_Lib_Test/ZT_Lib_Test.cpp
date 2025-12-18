
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib_Test/ZT_Lib_Test.cpp

#include "Component.h"

// Tests
// //////////////////////////////////////////////////////////////////////////

// Setup A  At least one Gimbal is connected to the network and is turned on
// Setup B  At least one Gamepad is connected to the computer and an operator
//          follows the instruction
// Setup C  At least one Gamepad and one Gimbal are connected to the computer
//          ans an operator follows the instruction
KMS_TEST_GROUP_LIST_BEGIN
    KMS_TEST_GROUP_LIST_ENTRY("Base")
    KMS_TEST_GROUP_LIST_ENTRY("Setup-A")
    KMS_TEST_GROUP_LIST_ENTRY("Setup-B")
    KMS_TEST_GROUP_LIST_ENTRY("Setup-C")
KMS_TEST_GROUP_LIST_END

extern int ControlLink_Base();
extern int ControlLink_SetupC();
extern int Gamepad_SetupB();
extern int Gimbal_SetupA();
extern int Gimbal_Focus_SetupA();
extern int System_Base();

KMS_TEST_LIST_BEGIN
    KMS_TEST_LIST_ENTRY(ControlLink_Base   , "ControlLink - Base"      , 0, 0)
    KMS_TEST_LIST_ENTRY(ControlLink_SetupC , "ControlLink - Setup-C"   , 3, KMS_TEST_FLAG_INTERACTION_NEEDED)
    KMS_TEST_LIST_ENTRY(Gamepad_SetupB     , "Gamepad - Setup-B"       , 2, KMS_TEST_FLAG_INTERACTION_NEEDED)
    KMS_TEST_LIST_ENTRY(Gimbal_SetupA      , "Gimbal - Setup-A"        , 1, KMS_TEST_FLAG_INTERACTION_NEEDED)
    KMS_TEST_LIST_ENTRY(Gimbal_Focus_SetupA, "Gimbal - Focus - Setup-A", 1, KMS_TEST_FLAG_INTERACTION_NEEDED)
    KMS_TEST_LIST_ENTRY(System_Base        , "System - Base"           , 0, 0)
KMS_TEST_LIST_END

KMS_TEST_MAIN
