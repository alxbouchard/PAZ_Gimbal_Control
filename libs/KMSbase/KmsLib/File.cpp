
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib/File.cpp

// TEST COVERAGE 2017-11-10 KMS - Martin Dubois, P.Eng

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

#ifdef _KMS_WINDOWS_
	#include <io.h>
#endif

#ifdef _KMS_OS_X_
    // ===== System =========================================================
    #include <sys/errno.h>
#endif

#ifdef _KMS_WINDOWS_
	// ===== Windows ========================================================
	#include <Windows.h>
#endif

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>

#ifdef _KMS_LINUX_
	#include <WindowsToLinux.h>
#endif

#include <KmsLib/File.h>

namespace KmsLib
{

    namespace File
    {

        // Functions / Fonctions
        /////////////////////////////////////////////////////////////////////

		#ifdef _KMS_WINDOWS_

        void Copy(const char * aSrc, const char * aDst, unsigned int aFlags)
        {
            assert(NULL != aSrc);
            assert(NULL != aDst);

            bool lFailIfExist = (0 != (aFlags & FILE_COPY_FLAG_FAIL_IF_EXIST));

            if (!CopyFile(aSrc, aDst, lFailIfExist))
            {
                char lMsg[2048];

                sprintf_s(lMsg, "CopyFile( \"%s\", \"%s\", %s ) failed", aSrc, aDst, lFailIfExist ? "TRUE" : "FALSE");

                throw new KmsLib::Exception(KmsLib::Exception::CODE_COPY_FILE_ERROR,
                    "CopyFile( , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, aFlags);
            }
        }

        void Delete(const char * aName, unsigned int aFlags)
        {
            assert(NULL != aName);

            if (!DeleteFile(aName))
            {
                char lMsg[2048];

                sprintf_s(lMsg, "DeleteFile( \"%s\" ) failed", aName);

                throw new KmsLib::Exception(KmsLib::Exception::CODE_DELETE_FILE_ERROR,
                    "DeleteFile(  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, aFlags);
            }
        }

        #endif

        bool Exist(const char * aName, unsigned int aFlags)
        {
            assert(NULL != aName);

            int lFlags = 0;

            if (0 != (FILE_EXIST_FLAG_READ & aFlags)) { lFlags |= 4; }

            int lRet = _access(aName, lFlags);
            if (0 == lRet)
            {
                return true;
            }

            switch (errno)
            {
            case EACCES:
            case ENOENT:
                return false;

            default:
                // NOT TESTED   KmsLib.File.ErrorHandling
                //              _access failed / _access a echoue

                char lMsg[2048];

                sprintf_s(lMsg, "_access( \"%s\", 0x%08x ) failed returning %d", lMsg, lFlags, lRet);

                throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_PARAMETER,
                    "_access( ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, aFlags);
            }
        }

		#ifdef _KMS_WINDOWS_

        bool IsEqual(const char * aA, const char * aB, unsigned int aFlags)
        {
            assert(NULL != aA);
            assert(NULL != aB);

            HANDLE lA = CreateFile(aA, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
            HANDLE lB = CreateFile(aB, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

            if ((INVALID_HANDLE_VALUE == lA) && (INVALID_HANDLE_VALUE == lB))
            {
                return true;
            }

            bool lResult;

            if (GetFileSize(lA, NULL) == GetFileSize(lB, NULL))
            {
                if ((INVALID_HANDLE_VALUE != lA) && (INVALID_HANDLE_VALUE != lB))
                {
                    for (; ; )
                    {
                        unsigned char lBufferA[4096];
                        unsigned char lBufferB[4096];

                        DWORD lSizeA_byte;
                        DWORD lSizeB_byte;

                        BOOL lRetA = ReadFile(lA, lBufferA, sizeof(lBufferA), &lSizeA_byte, NULL);
                        BOOL lRetB = ReadFile(lB, lBufferB, sizeof(lBufferB), &lSizeB_byte, NULL);

                        if ((lRetA != lRetB) || (lSizeA_byte != lSizeB_byte))
                        {
                            // NOT TESTED   KmsLib.File.ErrorHandling
                            //              ReadFile fail / ReadFile echoue
                            lResult = false;
                            break;
                        }

                        if (0 != memcmp(lBufferA, lBufferB, lSizeA_byte))
                        {
                            lResult = false;
                            break;
                        }

                        if (sizeof(lBufferA) > lSizeA_byte)
                        {
                            lResult = true;
                            break;
                        }
                    }
                }
            }
            else
            {
                lResult = false;
            }

            if (INVALID_HANDLE_VALUE != lA) { CloseHandle(lA); }
            if (INVALID_HANDLE_VALUE != lB) { CloseHandle(lB); }

            return lResult;
        }

        void Move(const char * aSrc, const char * aDst, unsigned int aFlags)
        {
            assert(NULL != aSrc);
            assert(NULL != aDst);

            if ((0 == (FILE_MOVE_FLAG_FAIL_IF_EXIST & aFlags)) && Exist(aDst))
            {
                Delete(aDst);
            }

            if (!MoveFile(aSrc, aDst))
            {
                char lMsg[2048];

                sprintf_s(lMsg, "MoveFile( \"%s\", \"%s\" ) failed", aSrc, aDst);

                throw new KmsLib::Exception(KmsLib::Exception::CODE_MOVE_FILE_ERROR,
                    "MoveFile( ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, aFlags);
            }
        }

        #endif

    }

}
