
// Author / Auteur   KMS - Martin Dubois, ing.
// Project / Projet  KmsBase
// File / Fichier    KmsLib_Test/Windows/SystemLog.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>
#include <KmsTest.h>

#include <KmsLib/Windows/SystemLog.h>

// ===== Local ==============================================================
#include "../Messages.h"

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(SystemLog_Base)
{
    KmsLib::Windows::SystemLog lSL0("System", "KmsLib_Test");

    try
    {
        lSL0.IsConfigured();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_OPEN_ERROR, eE->GetCode());
    }

    try
    {
        lSL0.Configure("Useless");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_OPEN_ERROR, eE->GetCode());
    }

    try
    {
        lSL0.Unconfigure();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_OPEN_ERROR, eE->GetCode());
    }

    // Invalid argument / Argument invalid
    try
    {
        lSL0.LogEvent(EVENTLOG_INFORMATION_TYPE, 1, 1, 1, 0, NULL, NULL);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SYSTEM_LOG_ERROR, eE->GetCode());
    }
}
KMS_TEST_END_2

KMS_TEST_BEGIN(SystemLog_SetupB)
{
    KmsLib::Windows::SystemLog lSL0("System", "KmsLib_Test");

    char lMsgFile[2048];

    KMS_TEST_ASSERT_RETURN(0 < GetCurrentDirectory(sizeof(lMsgFile), lMsgFile));

#ifdef _WIN64
    strcat_s(lMsgFile, "\\x64");
#endif // _WIN64

#ifdef _DEBUG
    strcat_s(lMsgFile, "\\Debug");
#endif // _DEBUG

#ifdef NDEBUG
    strcat_s(lMsgFile, "\\Release");
#endif // NDEBUG

    strcat_s(lMsgFile, "\\KmsLib_Test.exe");

    printf("    Configuring the event sources...\n");
    lSL0.Configure(lMsgFile);
    printf("    Configured!\n");

    KMS_TEST_ASSERT(lSL0.IsConfigured());

    printf("    Loging event...\n");
    lSL0.LogEvent(EVENTLOG_INFORMATION_TYPE, 0, MSG_TEST, 0, 0, NULL, NULL);

    try
    {
        lSL0.LogEvent(EVENTLOG_INFORMATION_TYPE, 0, MSG_TEST, 1, 0, NULL, NULL);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SYSTEM_LOG_ERROR, eE->GetCode());
    }

    printf("    Unconfiguring the event sources...\n");
    lSL0.Unconfigure();
    printf("    Unconfigured!\n");

    KMS_TEST_ASSERT(!lSL0.IsConfigured());
}
KMS_TEST_END_2
