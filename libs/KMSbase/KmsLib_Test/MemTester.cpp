
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/MemTester.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Interface ==========================================================
#include <KmsLib/MemTester.h>
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(MemTester_Base)
{
    unsigned char lData[2047];

    KmsLib::MemTester	lMT0;

    KMS_TEST_COMPARE((100 + 256 + 999), lMT0.GetCount());

    do
    {
        lMT0.GetIndex();
        lMT0.GetPatternType();

        unsigned int lProgress;

        lProgress = lMT0.GetProgress(true);
        KMS_TEST_ASSERT(1000 >= lProgress);

        lProgress = lMT0.GetProgress(false);
        KMS_TEST_ASSERT(1000 >= lProgress);

        lMT0.Generate(lData, sizeof(lData));

        KMS_TEST_COMPARE(0, lMT0.Verify(lData, sizeof(lData)));
    } while (lMT0.NextIndexAndPatternType());

    unsigned int lOffset_byte = 0;
    do
    {
        lMT0.Generate(lData, sizeof(lData));

        lData[lOffset_byte] ^= 0x01;
        lOffset_byte++;

        KMS_TEST_COMPARE(1, lMT0.Verify(lData, sizeof(lData)));
    } while (lMT0.NextIndexAndPatternType());
}
KMS_TEST_END
