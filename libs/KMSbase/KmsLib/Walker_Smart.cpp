
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib/Walker_Smart.cpp

// Include
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

#ifdef _KMS_WINDOWS_
	#include <io.h>
#endif // _KMS_WINDOWS_

// ===== Windows ============================================================
#ifdef _KMS_WINDOWS_
	#include <Windows.h>
#endif // _KMS_WINDOWS_

// ===== KmsBase ============================================================
#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
	#include <KmsLib/Linux/Windows.h>
#endif // _KMS_LINUX_

#include <KmsLib/IgnoreList.h>

#include <KmsLib/Walker_Smart.h>

namespace KmsLib
{

	// Public
	/////////////////////////////////////////////////////////////////////////////

	Walker_Smart::Walker_Smart() : mIgnoreList(NULL)
	{
	}

	void Walker_Smart::AddIgnoreFileName(const char * aFileName)
	{
		assert(NULL != aFileName);

		mIgnoreFiles.push_back(aFileName);
	}

	void Walker_Smart::OnFolder_Smart(const char * aFolderName)
	{
		Walker::OnFolder(aFolderName);
	}

	// ===== Walker =============================================================

	void Walker_Smart::OnFile(const char * aFileName, const FILETIME & aLastWrite)
	{
		assert(NULL != aFileName);

		if ((NULL == mIgnoreList) || (!mIgnoreList->IsFileIgnored(aFileName)))
		{
			OnFile_Smart(aFileName, aLastWrite);
		}
	}

	void Walker_Smart::OnFolder(const char * aFolderName)
	{
		if ((NULL == aFolderName) || (NULL == mIgnoreList) || (!mIgnoreList->IsFolderIgnored(aFolderName)))
		{
			bool lCreated = false;

			for (std::list< std::string >::iterator lIt = mIgnoreFiles.begin(); lIt != mIgnoreFiles.end(); lIt++)
			{
				if (FileExist(aFolderName, lIt->c_str()))
				{
					char lFolderName[2048];

					GetAbsolute(lFolderName, sizeof(lFolderName) / sizeof(lFolderName[0]), aFolderName);

					if (!lCreated)
					{
						mIgnoreList	= new IgnoreList(mIgnoreList, aFolderName);
						lCreated	= true;
					}

					mIgnoreList->ReadFromFile(lFolderName, lIt->c_str());
				}
			}

			OnFolder_Smart(aFolderName);

			if (lCreated)
			{
				IgnoreList * lToDelete = mIgnoreList;

				mIgnoreList = mIgnoreList->GetParent();

				delete lToDelete;
			}
		}
	}

}
