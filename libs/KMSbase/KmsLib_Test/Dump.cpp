
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/Dump.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Interface ==========================================================
#include <KmsLib/Dump.h>
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Dump_Base)
{
    const unsigned char  DATA_1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
    const unsigned short DATA_2[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    const unsigned int   DATA_4[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    KmsLib::Dump (stdout, DATA_1, sizeof(DATA_1));
    KmsLib::Dump1(stdout, DATA_1, sizeof(DATA_1));
    KmsLib::Dump2(stdout, DATA_2, sizeof(DATA_2));
    KmsLib::Dump4(stdout, DATA_4, sizeof(DATA_4));
    KmsLib::DumpC(stdout, DATA_1, sizeof(DATA_1));
}
KMS_TEST_END
