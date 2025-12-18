
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib_Test/FileHandle.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ===================================================================
#include <fcntl.h>

#ifdef _KMS_WINDOWS_
    // ===== Windows ============================================================
    #include <Windows.h>
#endif

// ===== Interface ==========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/FileHandle.h>
#include <KmsTest.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

    #define OPEN_EXISTING   0
    #define GENERIC_READ    O_RDONLY
    #define GENERIC_WRITE   O_WRONLY

#endif

#ifdef _KMS_WINDOWS_

#endif

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(FileHandle_Base)
{
    KmsLib::FileHandle	lFH0;

    lFH0.Create("KmsLib_Test" SLASH "FileHandle.cpp", GENERIC_READ, 0, OPEN_EXISTING, 0);

    char         lData[1024];
    unsigned int lRet;

    lRet = lFH0.Read(lData, 1);
    KMS_TEST_COMPARE(1, lRet);

    try
    {
        lFH0.Write(lData, 1);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_WRITE_FILE_ERROR, eE->GetCode());
    }

    #ifdef _KMS_WINDOWS_
        HANDLE lHandle = lFH0;
        KMS_TEST_ASSERT(INVALID_HANDLE_VALUE != lHandle);
    #endif

    lFH0.Create(".gitignore", GENERIC_WRITE, 0, OPEN_EXISTING, 0);

    try
    {
        lFH0.Read(lData, 1);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_READ_FILE_ERROR, eE->GetCode());
    }

    try
    {
        lFH0.Create("DoNotExist", GENERIC_READ, 0, OPEN_EXISTING, 0);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_CREATE_FILE_ERROR, eE->GetCode());
    }
}
KMS_TEST_END
