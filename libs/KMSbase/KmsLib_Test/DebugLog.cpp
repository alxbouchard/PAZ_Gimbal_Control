
// Author / Auteur    KMS -	Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/DebugLog.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Interface ==========================================================
#include <KmsLib/DebugLog.h>
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(DebugLog_Base)
{
    KmsLib::Exception lE(KmsLib::Exception::CODE_PRIVATE, "Test", NULL, __FILE__, __FUNCTION__, __LINE__, 0);

    KmsLib::DebugLog lDL0("DoesNotExist", "Test");

    KMS_TEST_ASSERT(!lDL0.IsEnabled());

    lDL0.Log(&lE);
    lDL0.Log(__FILE__, __FUNCTION__, __LINE__);
    lDL0.Log("Test");

    lDL0.LogTime();

    KmsLib::DebugLog lDL1("KmsLib_Test", "Test");

    lDL1.Log(&lE);
    lDL1.Log(__FILE__, __FUNCTION__, __LINE__);
    lDL1.Log("Test");

    lDL1.LogTime();

    FILE * lF1 = lDL1;
    KMS_TEST_ASSERT(NULL != lF1);
}
KMS_TEST_END_2
