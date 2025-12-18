
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/Walker_Smart.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Windows ============================================================
#ifdef _KMS_WINDOWS_
	#include <Windows.h>
#endif // _KMS_WINDOWS_

// ===== KmsBase ============================================================
#include <KmsTest.h>

#ifdef _KMS_LINUX_
	#include <KmsLib/Linux/Windows.h>
#endif // _KMS_LINUX_

#include <KmsLib/Walker_Smart.h>

// Test class / Classe de test
/////////////////////////////////////////////////////////////////////////////

class Walker_Smart_Test : public KmsLib::Walker_Smart
{

public:

	// ===== KmsLib::Walker =================================================

	virtual void OnFile_Smart(const char * aFileName, const FILETIME & aLastWrite);

	unsigned int mTestIndex;

	bool	mTestResult[1];

};

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Walker_Smart_Base)
{
    Walker_Smart_Test lWT0;

    lWT0.mTestIndex = 0;

    lWT0.AddIgnoreFileName(".gitignore");

    lWT0.Walk("KmsLib_Test");

    KMS_TEST_COMPARE(static_cast< unsigned int >(sizeof(lWT0.mTestResult) / sizeof(lWT0.mTestResult[0])), lWT0.mTestIndex);
    for (unsigned int i = 0; i < lWT0.mTestIndex; i++)
    {
        KMS_TEST_ASSERT(lWT0.mTestResult[i]);
    }
}
KMS_TEST_END

// Test class code / Code de la classe de test
/////////////////////////////////////////////////////////////////////////////

void Walker_Smart_Test::OnFile_Smart(const char * aFile, const FILETIME & aLastWrite)
{
	assert(NULL != aFile			);
	assert(NULL != (&aLastWrite)	);

	switch (mTestIndex)
	{
	case 0:
		mTestResult[mTestIndex] = (0 == strcmp(".gitignore", aFile));
		mTestIndex++;
		break;
	}
}