
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/IgnoreList.cpp

// Include
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <stdlib.h>

// ===== KmsBase ============================================================
#include <KmsLib/Exception.h>
#include <KmsTest.h>

#include <KmsLib/IgnoreList.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(IgnoreList_Base)
{
    KmsLib::IgnoreList lIL0(NULL, NULL);

    KMS_TEST_ASSERT(NULL == lIL0.GetParent());

    try
    {
        lIL0.ReadFromFile(".", "DoesNotExist.txt");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_FILE_OPEN_ERROR, eE->GetCode());
    }

    lIL0.ReadFromFile(".", ".gitignore");

    KMS_TEST_ASSERT(!lIL0.IsFileIgnored("Test.cmd"));
    KMS_TEST_ASSERT( lIL0.IsFileIgnored("Linux_Windows_Test0.txt"));

    KMS_TEST_ASSERT(!lIL0.IsFolderIgnored("KmsLib_Test"));
    KMS_TEST_ASSERT( lIL0.IsFolderIgnored("Release"));

    KmsLib::IgnoreList lIL1(&lIL0, "KmsLib_Test");

    KMS_TEST_ASSERT((&lIL0) == lIL1.GetParent());

    lIL1.ReadFromFile("." SLASH "KmsLib_Test", ".gitignore");

    KMS_TEST_ASSERT(!lIL1.IsFileIgnored("IgnoreList.cpp"));

    KMS_TEST_ASSERT(!lIL1.IsFolderIgnored("KmsLib_Test" SLASH "Test0"));
    KMS_TEST_ASSERT( lIL1.IsFolderIgnored("KmsLib_Test" SLASH "Test00"));

    KMS_TEST_ASSERT(lIL1.IsFolderIgnored("KmsLib_Test" SLASH "Release"));

    KMS_TEST_ASSERT( lIL1.IsFileIgnored("KmsLib_Test" SLASH				"Test1" SLASH "Toto"));
    KMS_TEST_ASSERT(!lIL1.IsFileIgnored("KmsLib_Test" SLASH "Toto" SLASH" Test1" SLASH "Toto"));

    KMS_TEST_ASSERT(lIL1.IsFolderIgnored("KmsLib_Test" SLASH				"Test2" SLASH "TotoA"));
    KMS_TEST_ASSERT(lIL1.IsFolderIgnored("KmsLib_Test" SLASH "Toto" SLASH	"Test2" SLASH "TotoB"));
}
KMS_TEST_END_2
