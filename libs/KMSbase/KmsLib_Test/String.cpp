
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/String.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/String.h>
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(String_Base)
{
    char lStr[4];
    strcpy_s(lStr, "ABC");

    KmsLib::String::Trunk(lStr, 'C');
    KMS_TEST_COMPARE(0, strcmp("AB", lStr));

    KmsLib::String::Trunk(lStr, "B");
    KMS_TEST_COMPARE(0, strcmp("A", lStr));
}
KMS_TEST_END_2
