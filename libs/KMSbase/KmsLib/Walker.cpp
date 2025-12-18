
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib/Walker.cpp

// Inclusions
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>

#ifdef _KMS_WINDOWS_
	#include <direct.h>
	#include <io.h>
#endif // _KMS_WINDOWS_

// ===== System =============================================================
#include <sys/stat.h>
#include <sys/types.h>

// ===== Windows ============================================================
#ifdef _KMS_WINDOWS_
	#include <Windows.h>
#endif // _KMS_WINDOWS_

// ===== Includes/KmsLib ====================================================
#include <KmsLib/Exception.h>
#include <KmsLib/File.h>

#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
	#include <KmsLib/Linux/Windows.h>
#endif // _KMS_LINUX_

#include <KmsLib/Walker.h>

namespace KmsLib
{

	// Public
	/////////////////////////////////////////////////////////////////////////////

	void Walker::Walk(const char * aFolderName)
	{
		assert(NULL != aFolderName);

		mRoot = aFolderName;

		OnFolder(NULL);
	}

	// Protected
	/////////////////////////////////////////////////////////////////////////////

	void Walker::GetAbsolute(char * aOut, unsigned int aOutLen, const char * aRoot, const char * aName)
	{
		assert(NULL	!=	aOut	);
		assert(0	<	aOutLen	);
		assert(NULL	!=	aRoot	);

		if (NULL == aName)
		{
#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
			sprintf(aOut, "%s", aRoot);
#endif // _KMS_LINUX_

#ifdef _KMS_WINDOWS_
			sprintf_s(aOut, aOutLen, "%s", aRoot);
#endif // _KMS_WINDOWS_
		}
		else
		{
#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
			sprintf(aOut, "%s" SLASH "%s", aRoot, aName);
#endif // _KMS_LINUX_

#ifdef _KMS_WINDOWS_
			sprintf_s(aOut, aOutLen, "%s" SLASH "%s", aRoot, aName);
#endif // _KMS_WINDOWS_
		}
	}

	bool Walker::GetFolder(char * aOut, unsigned int aOutLen, const char * aName)
	{
		assert(NULL    != aOut    );
		assert(0       <  aOutLen );
		assert(NULL    != aName   );
		assert('\0'    != (*aName));
		assert(SLASH_C != (*aName));

		const char * lPtr = strrchr(aName, SLASH_C);
		if (NULL == lPtr)
		{
			return false;
		}

		unsigned int lCount = (unsigned int)(lPtr - aName);

		if (aOutLen <= lCount)
		{
			throw new KmsLib::Exception(KmsLib::Exception::CODE_BUFFER_TOO_SMALL,
				"Buffer too short", NULL, __FILE__, __FUNCTION__, __LINE__, lCount);
		}

		memcpy(aOut, aName, lCount);

		aOut[lCount] = '\0';

		return true;
	}

	bool Walker::Exist(const char * aRoot, const char * aName)
	{
		assert(NULL != aRoot);
		assert(NULL != aName);

		char lFolder[PATH_LENGTH];

		GetAbsolute(lFolder, PATH_LENGTH, aRoot, aName);

		return KmsLib::File::Exist(lFolder);
	}

	void Walker::OnFolder(const char * aFolderName)
	{
		assert(!mRoot.empty());

		char lPatern[PATH_LENGTH];

		if (NULL == aFolderName)
		{
			sprintf_s(lPatern, "%s" SLASH "*", mRoot.c_str());
		}
		else
		{
			sprintf_s(lPatern, "%s" SLASH "%s" SLASH "*", mRoot.c_str(), aFolderName);
		}

		WIN32_FIND_DATA lData;

		HANDLE lHandle = FindFirstFile(lPatern, &lData);
		if (INVALID_HANDLE_VALUE != lHandle)
		{
			do
			{
				char lName[PATH_LENGTH];

				if (NULL == aFolderName)
				{
					sprintf_s(lName, "%s", lData.cFileName);
				}
				else
				{
					sprintf_s(lName, "%s" SLASH "%s", aFolderName, lData.cFileName);
				}

				if (0 != (lData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					if (	(0 != strcmp(lData.cFileName, "."	))
						&&	(0 != strcmp(lData.cFileName, ".."	)))
					{
						OnFolder(lName);
					}
				}
				else
				{
					OnFile(lName, lData.ftLastWriteTime);
				}
			}
			while (FindNextFile(lHandle, &lData));

			BOOL lBool = FindClose(lHandle);
			assert(lBool);
		}
	}

	void Walker::GetAbsolute(char * aOut, unsigned int aOutLen, const char * aName) const
	{
		assert(NULL	!=	aOut	);
		assert(0	<	aOutLen	);

		GetAbsolute(aOut, aOutLen, mRoot.c_str(), aName);
	}

	bool Walker::FileExist(const char * aFolder, const char * aFile) const
	{
		assert(NULL != aFile);

		assert(!mRoot.empty());

		char lFileName[PATH_LENGTH];

		if (NULL == aFolder)
		{
			sprintf_s(lFileName, "%s" SLASH "%s", mRoot.c_str(), aFile);
		}
		else
		{
			sprintf_s(lFileName, "%s" SLASH "%s" SLASH "%s", mRoot.c_str(), aFolder, aFile);
		}

		return KmsLib::File::Exist(lFileName, FILE_EXIST_FLAG_READ);
	}

	void Walker::CreateFolder(const char * aToRoot, const char * aFolder)
	{
		assert(NULL != aToRoot);
		assert(NULL != aFolder);

		char lFolder[PATH_LENGTH];

		if (GetFolder(lFolder, PATH_LENGTH, aFolder))
		{
			if (!Exist(aToRoot, lFolder))
			{
				CreateFolder(aToRoot, lFolder);
			}
		}

		GetAbsolute(lFolder, PATH_LENGTH, aToRoot, aFolder);

		int lRetI = _mkdir(lFolder);
		if (0 != lRetI)
		{
			char lMessage[2048];

			sprintf_s(lMessage, "mkdir( %s, ) failed", lFolder);

			throw new KmsLib::Exception(KmsLib::Exception::CODE_DIRECTORY_ERROR,
				"mkdir( ,  ) failed", lMessage, __FILE__, __FUNCTION__, __LINE__, 0);
		}
	}

}
