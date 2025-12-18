
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/Linux_Windows_Base.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Windows ============================================================
#ifdef _KMS_WINDOWS_
	#include <Windows.h>
#endif // _KMS_WINDOWS_

// ===== KmsBase ============================================================
#if defined( _KMS_LINUX_ ) || defined( _KMS_OS_X_ )
	#include <KmsLib/Linux/Windows.h>
#endif // _KMS_LINUX_ || _KMS_OS_X_ 

#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Linux_Windows_Base)
{
    KMS_TEST_ASSERT_RETURN(!CopyFile("DoNotExist"                         , "Linux_Windows_Test0.txt", false));
    KMS_TEST_ASSERT_RETURN( CopyFile("KmsLib_Test/Linux/Linux_Windows.cpp", "Linux_Windows_Test0.txt", false));
    KMS_TEST_ASSERT_RETURN(!CopyFile("KmsLib_Test/Linux/Linux_Windows.cpp", "Linux_Windows_Test0.txt", true ));

    WIN32_FIND_DATA lData;
    HANDLE			lHandle;

    lHandle = FindFirstFile("DoNotExist", &lData);
    KMS_TEST_ASSERT(INVALID_HANDLE_VALUE == lHandle);

    lHandle = FindFirstFile("KmsLib_Test", &lData);
    KMS_TEST_ASSERT_RETURN(INVALID_HANDLE_VALUE != lHandle);

    KMS_TEST_COMPARE(0, strcmp("KmsLib_Test", lData.cFileName));

    KMS_TEST_ASSERT(0 != (lData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));

    KMS_TEST_ASSERT(!FindNextFile(lHandle, &lData));

    KMS_TEST_ASSERT(FindClose(lHandle));

    lHandle = FindFirstFile("KmsLib_Test/Linux/?*.cpp", &lData);
    KMS_TEST_ASSERT_RETURN(INVALID_HANDLE_VALUE != lHandle);

    KMS_TEST_COMPARE(0, strcmp("Linux_Windows.cpp", lData.cFileName));
    KMS_TEST_COMPARE(0, (lData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));

    KMS_TEST_ASSERT(!FindNextFile(lHandle, &lData));

    KMS_TEST_ASSERT(FindClose(lHandle));

    KMS_TEST_ASSERT(!MoveFile("DoNotExist", "Linux_Windows_Test0.txt"));
}
KMS_TEST_END
