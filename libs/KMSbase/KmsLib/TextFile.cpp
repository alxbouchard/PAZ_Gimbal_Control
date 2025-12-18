
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib/TextFile.cpp

// Last test coverage update / Derniere mise a jour de la couverture de test
// 2017-11-11

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include  <assert.h>

// ===== Includes/KmsLib ====================================================
#include  <KmsLib/Exception.h>

#include  <KmsLib/TextFile.h>

namespace KmsLib
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    unsigned int TextFile::FindFirstDiff(const char * aA, const char * aB)
    {
        assert(NULL != aA);
        assert(NULL != aB);

        TextFile lTFA;
        TextFile lTFB;

        lTFA.Open(aA);
        lTFB.Open(aB);

        char lLineA[4096];
        char lLineB[4096];

        unsigned int lResult = 0xffffffff;

        while (lTFA.ReadLine(lLineA, sizeof(lLineA)))
        {
            if (!lTFB.ReadLine(lLineB, sizeof(lLineB)))
            {
                return lTFB.GetLineNumber();
            }

            if (0 != strcmp(lLineA, lLineB))
            {
                return lTFB.GetLineNumber();
            }
        }

        if (lTFB.ReadLine(lLineB, sizeof(lLineB)))
        {
            return lTFB.GetLineNumber();
        }

        return 0xffffffff;
    }

    TextFile::TextFile() : mFile(NULL)
    {
    }

    TextFile::~TextFile()
    {
        if (IsOpened())
        {
            Close();
        }
    }

    TextFile::operator FILE * ()
    {
        assert(NULL != mFile);

        return mFile;
    }

    unsigned int TextFile::GetLineNumber() const
    {
        return mLineNumber;
    }

    bool TextFile::IsOpened() const
    {
        return (NULL != mFile);
    }

    void TextFile::Close()
    {
        assert(NULL != mFile);

        errno_t lErrNo = fclose(mFile);
        assert(0 == lErrNo);

        mFile = NULL;
    }

    void TextFile::Create(const char * aFileName, unsigned int aFlags)
    {
        assert(NULL != aFileName);

        if (IsOpened())
        {
            Close();
        }

        assert(NULL == mFile);

        errno_t lErrNo = fopen_s(&mFile, aFileName, "w");
        if (0 == lErrNo)
        {
            assert(mFile != NULL);

            mLineNumber = 0;
        }
        else
        {
            if (0 == (FLAG_IGNORE_ERROR & aFlags))
            {
                char lMessage[2048];

                sprintf_s(lMessage, "fopen_s( , \"%s\",  ) failed returning %d", aFileName, lErrNo);

                throw  new  KmsLib::Exception(KmsLib::Exception::CODE_FILE_OPEN_ERROR,
                    "fopen_s( , ,  ) failed", lMessage, __FILE__, __FUNCTION__, __LINE__, aFlags);
            }
        }
    }

    void TextFile::Open(const char * aFileName, unsigned int aFlags)
    {
        assert(NULL != aFileName);

        if (IsOpened())
        {
            Close();
        }

        assert(NULL == mFile);

        errno_t lErrNo = fopen_s(&mFile, aFileName, "r");
        if (0 == lErrNo)
        {
            assert(NULL != mFile);

            mLineNumber = 0;
        }
        else
        {
            if (0 == (FLAG_IGNORE_ERROR & aFlags))
            {
                char lMessage[2048];

                sprintf_s(lMessage, "fopen_s( , \"%s\",  ) failed returning %d", aFileName, lErrNo);

                throw  new  KmsLib::Exception(KmsLib::Exception::CODE_FILE_OPEN_ERROR,
                    "fopen_s( , ,  ) failed", lMessage, __FILE__, __FUNCTION__, __LINE__, aFlags);
            }
        }
    }

    bool TextFile::ReadLine(char * aOut, unsigned int aOutSize_byte)
    {
        assert(NULL != aOut         );
        assert(0    <  aOutSize_byte);

        assert(NULL != mFile);

        char * lRet = fgets(aOut, aOutSize_byte, mFile);
        if (NULL == lRet)
        {
            return false;
        }

        mLineNumber++;

        return true;
    }

    void TextFile::WriteLine(const char * aIn)
    {
        assert(NULL != aIn);

        assert(NULL != mFile);

        int lRet = fputs(aIn, mFile);
        if (0 > lRet)
        {
            // NOT TESTED  KmsLib.ErrorHandling
            //             fputs field / fputs a echoue

            char lMsg[2048];

            sprintf_s(lMsg, "fputs( \"%s\",  ) failed returning %d", aIn, lRet);

            throw  new  KmsLib::Exception(KmsLib::Exception::CODE_FILE_WRITE_ERROR,
                "fputs( ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
        }

        mLineNumber++;
    }

}
