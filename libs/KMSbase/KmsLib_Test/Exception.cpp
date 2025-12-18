
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib_Test/DebugLog.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Windows ============================================================
#ifdef _WIN32
    #include <Windows.h>
#endif // _WIN32

// ===== Interface ==========================================================
#include <KmsLib/Exception.h>
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Exception_Base)
{
    // ===== RegisterTranslator =============================================

    void * lPrevTranslator = KmsLib::Exception::RegisterTranslator();

    #ifdef _KMS_WINDOWS_

        try
        {
            *reinterpret_cast<unsigned int *>(NULL) = 0;

            KMS_TEST_ERROR();
        }
        catch (KmsLib::Exception * eE)
        {
            KMS_TEST_ERROR_INFO;
            eE->Write(stdout);
            KMS_TEST_COMPARE(KmsLib::Exception::CODE_ACCESS_VIOLATION, eE->GetCode());
        }

        try
        {
            volatile unsigned int lA = 1;
            volatile unsigned int lB = 0;

            lA /= lB;

            KMS_TEST_ERROR();
        }
        catch (KmsLib::Exception * eE)
        {
            KMS_TEST_ERROR_INFO;
            eE->Write(stdout);
            KMS_TEST_COMPARE(KmsLib::Exception::CODE_INTEGER_DIVIDE_BY_ZERO, eE->GetCode());
        }

    #endif // _KMS_WINDOWS_

    // ===== RestoreTranslator ==============================================
    KmsLib::Exception::RestoreTranslator(lPrevTranslator);

    // ===== DefineCode =====================================================
    KmsLib::Exception::Code lCodeZero = KmsLib::Exception::DefineCode(0, "Zero");

    // ===== GetCodeName ====================================================

    KMS_TEST_COMPARE(0, strcmp("CODE_IO_ERROR", KmsLib::Exception::GetCodeName(KmsLib::Exception::CODE_IO_ERROR)));
    KMS_TEST_COMPARE(0, strcmp("Zero"         , KmsLib::Exception::GetCodeName(lCodeZero)));

    KMS_TEST_ASSERT(NULL == KmsLib::Exception::GetCodeName(KmsLib::Exception::CODE_QTY));

    // ===== SetCodeName ====================================================
    KmsLib::Exception::SetCodeName(lCodeZero, "0" );
    KmsLib::Exception::SetCodeName(lCodeZero, NULL);

    // ===== Exception ======================================================
    KmsLib::Exception * lE = new KmsLib::Exception(KmsLib::Exception::CODE_UNKNOWN, "Unknown", NULL, __FILE__, __FUNCTION__, __LINE__, 0);

    // ===== GetCode ========================================================
    KMS_TEST_COMPARE(KmsLib::Exception::CODE_UNKNOWN, lE->GetCode());

    // ===== GetFile ========================================================
    KMS_TEST_COMPARE(0, strcmp(__FILE__, lE->GetFile()));

    // ===== GetFunction ====================================================
    KMS_TEST_COMPARE(0, strcmp(__FUNCTION__, lE->GetFunction()));

    // ===== GetInfoA =======================================================
    KMS_TEST_COMPARE(0, lE->GetInfoA    ());

    // ===== GetLastError ===================================================

    #ifdef _KMS_LINUX_
        KMS_TEST_COMPARE(25, lE->GetLastError());
    #endif

    #ifdef _KMS_OS_X_
        KMS_TEST_COMPARE(19, lE->GetLastError());
    #endif

    #ifdef _KMS_WINDOWS_
        KMS_TEST_COMPARE(0, lE->GetLastError());
    #endif

    // ===== GetLine ========================================================
    KMS_TEST_COMPARE(__LINE__ - 29, lE->GetLine());

    // ===== GetMessage =====================================================
    KMS_TEST_COMPARE(0, strcmp("Unknown"   , lE->GetMessage ()));

    // ===== Write ==========================================================
    lE->Write(stdout);

    // ===== what ===========================================================
    KMS_TEST_COMPARE(0, strcmp("Unknown", lE->what()));
}
KMS_TEST_END_2
