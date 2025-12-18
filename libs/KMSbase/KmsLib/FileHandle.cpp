
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib/FileHandle.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
    // ===== System =========================================================
    #include <sys/fcntl.h>
#endif

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

// ===== Interface ==========================================================
#include <KmsLib/Exception.h>

#include <KmsLib/FileHandle.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
    #define INVALID_HANDLE_VALUE (-1)
#endif

namespace KmsLib
{

	// Public
	/////////////////////////////////////////////////////////////////////////

	FileHandle::FileHandle() : mHandle(INVALID_HANDLE_VALUE)
	{
	}

	FileHandle::~FileHandle()
	{
		if (INVALID_HANDLE_VALUE != mHandle)
		{
			Close();
		}
	}

	void FileHandle::Close()
	{
		assert(INVALID_HANDLE_VALUE != mHandle);

        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

            close(mHandle);

        #endif

        #ifdef _KMS_WINDOWS_

            BOOL lRet = ::CloseHandle(mHandle);

            if (!lRet)
            {
                // PAS TESTE : Difficile de faire echouer CloseHandle.
                    throw Exception(Exception::CODE_CLOSE_HANDLE_ERROR, "CloseHandle(  ) failed",
                    NULL, __FILE__, __FUNCTION__, __LINE__, 0);
            }

        #endif

        mHandle = INVALID_HANDLE_VALUE;
	}

	void FileHandle::Create(const char * aFileName, unsigned int aDesiredAccess, unsigned int aSharedMode, unsigned int aCreationDisposition, unsigned int aFlagsAndAttributes)
	{
		assert(NULL != aFileName);

        if (INVALID_HANDLE_VALUE != mHandle)
		{
			Close();
		}

        assert(INVALID_HANDLE_VALUE == mHandle);

        char lMessage[2048];

        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

            mHandle = open( aFileName, aDesiredAccess | aSharedMode | aCreationDisposition | aFlagsAndAttributes );
            if (INVALID_HANDLE_VALUE == mHandle)
            {
                sprintf_s(lMessage SIZE_INFO(sizeof(lMessage)), "open( \"%s\", 0x%08x ) failed",
                    aFileName, aDesiredAccess | aSharedMode | aCreationDisposition | aFlagsAndAttributes);

                throw new Exception(Exception::CODE_CREATE_FILE_ERROR, "open( ,  ) failed",
                    lMessage, __FILE__, __FUNCTION__, __LINE__, 0 );
            }

        #endif

        #ifdef _KMS_WINDOWS_

            mHandle = CreateFile(aFileName, aDesiredAccess, aSharedMode, NULL, aCreationDisposition, aFlagsAndAttributes, NULL);
            if (INVALID_HANDLE_VALUE == mHandle)
            {
                sprintf_s(lMessage, sizeof(lMessage), "CreateFile( \"%s\", 0x%08x, 0x%08x, , 0x%08x, 0x%08x,  ) failed",
                    aFileName, aDesiredAccess, aSharedMode, aCreationDisposition, aFlagsAndAttributes);

                throw new Exception(Exception::CODE_CREATE_FILE_ERROR, "CreateFile( , , , , , ,  ) failed",
                    lMessage, __FILE__, __FUNCTION__, __LINE__, 0 );
            }

        #endif
	}

	unsigned int FileHandle::Read(void * aOut, unsigned int aOutSize_byte)
	{
		assert(NULL !=	aOut			);
		assert(0	<	aOutSize_byte	);

        char lMessage[2048];

        unsigned int lResult_byte;

        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

            int lRet = read(mHandle, aOut, aOutSize_byte);
            if (0 > lRet)
            {
                sprintf_s(lMessage SIZE_INFO(sizeof(lMessage)), "read( %d, , %u bytes ) failed",
                    mHandle, aOutSize_byte);

                throw new Exception(Exception::CODE_READ_FILE_ERROR, "read( , ,  ) failed",
                    lMessage, __FILE__, __FUNCTION__, __LINE__, 0);
            }

            lResult_byte = lRet;

        #endif

        #ifdef _KMS_WINDOWS_

            if (!ReadFile(mHandle, aOut, aOutSize_byte, reinterpret_cast<LPDWORD>(&lResult_byte), NULL))
            {
                sprintf_s(lMessage, sizeof(lMessage), "ReadFile( , , %u bytes, ,  ) failed",
                    aOutSize_byte);

                throw new Exception(Exception::CODE_READ_FILE_ERROR, "ReadFile( , , , ,  ) failed",
                    lMessage, __FILE__, __FUNCTION__, __LINE__, 0);
            }

        #endif

        assert(aOutSize_byte >= lResult_byte);

        return lResult_byte;
	}

	void FileHandle::Write(const void * aIn, unsigned int aInSize_byte)
	{
		assert(NULL != aIn);
		assert(0    <  aInSize_byte);

        unsigned int lInfo_byte;
        char         lMessage[2048];
        
        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

            int lRet = write(mHandle, aIn, aInSize_byte);
            if (0 > lRet)
            {
                sprintf_s(lMessage, "write( %d, , %u bytes ) failed",
                    mHandle, aInSize_byte);

                throw new Exception(Exception::CODE_WRITE_FILE_ERROR, "write( , ,  ) failed",
                    lMessage, __FILE__, __FUNCTION__, __LINE__, 0);
            }

            lInfo_byte = lRet;

        #endif

        #ifdef _KMS_WINDOWS_

            if (!WriteFile(mHandle, aIn, aInSize_byte, reinterpret_cast<LPDWORD>(&lInfo_byte), NULL))
            {
                sprintf_s(lMessage, sizeof(lMessage), "WriteFile( , , %u bytes, ,  ) failed",
                    aInSize_byte);

                throw new Exception(Exception::CODE_WRITE_FILE_ERROR, "WriteFile( , , , ,  ) failed",
                    lMessage, __FILE__, __FUNCTION__, __LINE__, 0);
            }

        #endif

		// PAS TESTE : Modifier un fichier complexifierait le test.

        if (aInSize_byte != lInfo_byte)
		{
			sprintf_s(lMessage SIZE_INFO(sizeof(lMessage)), "WriteFile did not write all data (To write = %u byte, Written = %u byte)",
				aInSize_byte, lInfo_byte);

			throw new Exception(Exception::CODE_WRITE_FILE_ERROR, "WriteFile did not write all the data",
				lMessage, __FILE__, __FUNCTION__, __LINE__, lInfo_byte);
		}
	}

    #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

        FileHandle::operator int ()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            return mHandle;
        }

    #endif

    #ifdef _KMS_WINDOWS_

        FileHandle::operator HANDLE ()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            return mHandle;
        }

    #endif

}
