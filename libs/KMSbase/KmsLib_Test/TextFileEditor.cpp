
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/TextFileEditor.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Includes ===========================================================
#include <KmsTest.h>

// ----- KmsLib -------------------------------------------------------------
#include <KmsLib/Exception.h>
#include <KmsLib/File.h>
#include <KmsLib/TextFileEditor.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

#define TEST_FOLDER "KmsLib_Test\\Tests"

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(TextFileEditor_Base)
{
    char lLine[1024];

    KmsLib::TextFileEditor lTFE;

    // ===== GetLineNumber_Read =============================================
    KMS_TEST_COMPARE(0, lTFE.GetLineNumber_Read());

    // ===== GetLineNumber_Write ============================================
    KMS_TEST_COMPARE(0, lTFE.GetLineNumber_Write());

    // ===== IsOpened =======================================================
    KMS_TEST_ASSERT(!lTFE.IsOpened());

    // ===== Open ===========================================================

    try
    {
        lTFE.Open("DoesNotExist");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_MOVE_FILE_ERROR, eE->GetCode());
    }

    KmsLib::File::Copy(TEST_FOLDER "\\TextFileEditor.txt", TEST_FOLDER "\\TestA.txt");
    KmsLib::File::Copy(TEST_FOLDER "\\TextFileEditor.txt", TEST_FOLDER "\\TestB.txt");

    lTFE.Open(TEST_FOLDER "\\TestA.txt");
    lTFE.Open(TEST_FOLDER "\\TestB.txt");

    // ===== ReadLine / WriteLine ===========================================

    while (lTFE.ReadLine(lLine, sizeof(lLine)))
    {
        lTFE.WriteLine(lLine);
    }

    lTFE.Close();

    KMS_TEST_COMPARE(0xffffffff, KmsLib::TextFile::FindFirstDiff(TEST_FOLDER "\\TextFileEditor.txt", TEST_FOLDER "\\TestB.txt"));
    KMS_TEST_COMPARE(0xffffffff, KmsLib::TextFile::FindFirstDiff(TEST_FOLDER "\\TextFileEditor.txt", TEST_FOLDER "\\TestB.txt.bak"));
}
KMS_TEST_END_2
