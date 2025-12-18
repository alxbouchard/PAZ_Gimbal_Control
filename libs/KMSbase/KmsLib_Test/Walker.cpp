
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib_Test/Walker.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Windows ============================================================
#ifdef _KMS_WINDOWS_
#include <Windows.h>
#endif // _KMS_WINDOWS_

// ===== KmsBase ============================================================
#include <KmsLib/Exception.h>
#include <KmsTest.h>

#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
#include <KmsLib/Linux/Windows.h>
#endif // _KMS_LINUX_

#include <KmsLib/Walker.h>

// Test class / Classe de test
/////////////////////////////////////////////////////////////////////////////

class Walker_Test : public KmsLib::Walker
{

public:

	// ===== KmsLib::Walker =================================================

	virtual void OnFile(const char * aFileName, const FILETIME & aLastWrite );

	unsigned int mTestIndex;

	bool	mTestResult[9];

};

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Walker_Base)
{
    Walker_Test	lWT0;

    lWT0.mTestIndex = 0;

    lWT0.Walk("KmsLib_Test");

    KMS_TEST_COMPARE( static_cast< unsigned int >(sizeof(lWT0.mTestResult) / sizeof(lWT0.mTestResult[0])), lWT0.mTestIndex);
    for (unsigned int i = 0; i < lWT0.mTestIndex; i++)
    {
		printf("%u\n", i);

        KMS_TEST_ASSERT(lWT0.mTestResult[i]);
    }
}
KMS_TEST_END

// Test class code / Code de la classe de test
/////////////////////////////////////////////////////////////////////////////

void Walker_Test::OnFile(const char * aFile, const FILETIME & aLastWrite)
{
	assert(NULL != aFile			);
	assert(NULL != (&aLastWrite)	);

	char			lOut[2048];
	unsigned int	lOutLen = sizeof(lOut) / sizeof(lOut[0]);

	switch (mTestIndex)
	{
	case 0 :
        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
			mTestResult[mTestIndex] = true;
		#endif
		#ifdef _KMS_WINDOWS_
			mTestResult[mTestIndex] = (0 == strcmp(".gitignore", aFile));
		#endif
		mTestIndex++;
		break;

	case 1 :
		GetAbsolute(lOut, lOutLen, aFile);
        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
			mTestResult[mTestIndex] = true;
		#endif
		#ifdef _KMS_WINDOWS_
			mTestResult[mTestIndex] = (0 == strcmp("KmsLib_Test" SLASH "Clean.sh", lOut));
		#endif
		mTestIndex++;
		break;

	case 2 :
		mTestResult[mTestIndex] = FileExist(NULL, aFile);
		mTestIndex++;
		break;

	case 3 :
		GetAbsolute(lOut, lOutLen, NULL);
		mTestResult[mTestIndex] = (0 == strcmp("KmsLib_Test", lOut));
		mTestIndex++;
		break;

	case 4 :
		mTestResult[mTestIndex] = GetFolder(lOut, lOutLen, "Toto" SLASH "Tata") && (0 == strcmp("Toto", lOut));
		mTestIndex++;
		break;

	case 5 :
		mTestResult[mTestIndex] = !GetFolder(lOut, lOutLen, "Tutu");
		mTestIndex++;
		break;

	case 6 :
		mTestResult[mTestIndex] = !FileExist("Debug", "DoesNotExist");
		mTestIndex++;
		break;

	case 7 :
		mTestResult[mTestIndex] = !Exist("Debug", "DoesNotExist");
		mTestIndex++;
		break;

	case 8 :
		try
		{
			GetFolder(lOut, 4, "Titi" SLASH "Tata");
			mTestResult[mTestIndex] = false;
		}
		catch (KmsLib::Exception * eE)
		{
			KMS_TEST_ERROR_INFO;
			eE->Write(stdout);
			mTestResult[mTestIndex] = KmsLib::Exception::CODE_BUFFER_TOO_SMALL == eE->GetCode();
		}
		mTestIndex++;
		break;
	}
}
