
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/RLE.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Interface ==========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/RLE.h>
#include <KmsTest.h>

// Data types / Type de donnees
/////////////////////////////////////////////////////////////////////////////

typedef struct
{
	KmsLib::Exception::Code	mCode					;
	const char			  * mCompressed				;
	unsigned int			mCompressedSize_byte	;
	const char			  * mUncompressed			;
	unsigned int			mUncompressedSize_byte	;
}
TestCaseError;

typedef struct
{
	const char    * mCompressed				;
	unsigned int	mCompressedSize_byte	;
	const char    * mUncompressed			;
	unsigned int	mUncompressedSize_byte	;
}
TestCaseOK;

// Constants / Constantes
/////////////////////////////////////////////////////////////////////////////

static const TestCaseError	TEST_CASE_ERROR[] =
{
	{ KmsLib::Exception::CODE_BUFFER_TOO_SMALL     	, NULL								, 1, "ab"		, 2		},
	{ KmsLib::Exception::CODE_INVALID_BUFFER_SIZE	, NULL								, 1, "\377"		, 1		},
	{ KmsLib::Exception::CODE_BUFFER_TOO_SMALL      , NULL								, 1, "aa"		, 2		},
	{ KmsLib::Exception::CODE_BUFFER_TOO_SMALL      , NULL								, 1, "aaa"		, 3		},
	{ KmsLib::Exception::CODE_INVALID_BUFFER_SIZE	, NULL								, 1, "aaaa"		, 4		},
	{ KmsLib::Exception::CODE_BUFFER_TOO_SMALL      , "aa"								, 2, NULL		, 1		},
	{ KmsLib::Exception::CODE_INVALID_DATA			, "\377\002a"						, 3, NULL		, 1		},
	{ KmsLib::Exception::CODE_BUFFER_TOO_SMALL      , NULL								, 3, "\377ab"	, 3		},
	{ KmsLib::Exception::CODE_INVALID_DATA			, "\377"							, 1, NULL		, 1		},
	{ KmsLib::Exception::CODE_BUFFER_TOO_SMALL      , "\377\004a"						, 3, NULL		, 3		},
	{ KmsLib::Exception::CODE_INVALID_DATA			, "\377\377a"						, 3, NULL		, 512	},
	{ KmsLib::Exception::CODE_INVALID_DATA			, "\377\001\377\377\007a\377\004"	, 7, NULL		, 256	},
	{ KmsLib::Exception::CODE_INVALID_DATA			, "\377\001"						, 2, NULL		, 256	},
	{ KmsLib::Exception::CODE_INVALID_DATA			, "ab\377\377"						, 4, NULL		, 512	},
};

#define TEST_CASE_ERROR_QTY	(sizeof(TEST_CASE_ERROR)/sizeof(TEST_CASE_ERROR[0]))

static const TestCaseOK TEST_CASE_OK[] =
{
	{ "\0"						, 1, "\0"			, 1		},
	{ "\377\001\377"			, 3, "\377"			, 1		},
	{ "aa"						, 2, "aa"			, 2		},
	{ "aab"						, 3, "aab"			, 3		},
	{ "aaa"						, 3, "aaa"			, 3		},
	{ "\377\004a"				, 3, "aaaa"			, 4		},
	{ "\377\011a\377\001\377"	, 6, "aaaaaaaaa\377", 10	},
	{ "\377\001\377\377\007a"	, 6, "\377aaaaaaa"	, 8		},
	{ "abcdefg"					, 7, "abcdefg"		, 7     },		
};

#define TEST_CASE_OK_QTY	(sizeof(TEST_CASE_OK)/sizeof(TEST_CASE_OK[0]))

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(RLE_Base)
{
    unsigned char	lBuffer[512];
    unsigned int	lIndex;
    unsigned int	lSize_byte;

    // ===== Count ==========================================================
    for (lIndex = 0; lIndex < TEST_CASE_OK_QTY; lIndex++)
    {
        lSize_byte = KmsLib::RLE_ComputeCompressedSize(reinterpret_cast<const unsigned char *>(TEST_CASE_OK[lIndex].mUncompressed), TEST_CASE_OK[lIndex].mUncompressedSize_byte);
        KMS_TEST_COMPARE(TEST_CASE_OK[lIndex].mCompressedSize_byte, lSize_byte);

        lSize_byte = KmsLib::RLE_ComputeUncompressedSize(reinterpret_cast<const unsigned char *>(TEST_CASE_OK[lIndex].mCompressed), TEST_CASE_OK[lIndex].mCompressedSize_byte);
        KMS_TEST_COMPARE(TEST_CASE_OK[lIndex].mUncompressedSize_byte, lSize_byte);
    }

    // ===== OK - 2 buffers =================================================
    for (lIndex = 0; lIndex < TEST_CASE_OK_QTY; lIndex++)
    {
        lSize_byte = KmsLib::RLE_Compress(reinterpret_cast<const unsigned char *>(TEST_CASE_OK[lIndex].mUncompressed), TEST_CASE_OK[lIndex].mUncompressedSize_byte, lBuffer, sizeof(lBuffer));
        KMS_TEST_COMPARE(TEST_CASE_OK[lIndex].mCompressedSize_byte, lSize_byte);
        KMS_TEST_COMPARE(0, memcmp(TEST_CASE_OK[lIndex].mCompressed, lBuffer, lSize_byte));

        lSize_byte = KmsLib::RLE_Uncompress(reinterpret_cast<const unsigned char *>(TEST_CASE_OK[lIndex].mCompressed), TEST_CASE_OK[lIndex].mCompressedSize_byte, lBuffer, sizeof(lBuffer));
        KMS_TEST_COMPARE(TEST_CASE_OK[lIndex].mUncompressedSize_byte, lSize_byte);
        KMS_TEST_COMPARE(0, memcmp(TEST_CASE_OK[lIndex].mUncompressed, lBuffer, lSize_byte));
    }

    // ===== Error - 2 buffer ===============================================
    for (lIndex = 0; lIndex < TEST_CASE_ERROR_QTY; lIndex++)
    {
        try
        {
            if (NULL == TEST_CASE_ERROR[lIndex].mCompressed)
            {
                lSize_byte = KmsLib::RLE_Compress(reinterpret_cast<const unsigned char *>(TEST_CASE_ERROR[lIndex].mUncompressed), TEST_CASE_ERROR[lIndex].mUncompressedSize_byte, lBuffer, TEST_CASE_ERROR[lIndex].mCompressedSize_byte);
            }
            else
            {
                lSize_byte = KmsLib::RLE_Uncompress(reinterpret_cast<const unsigned char *>(TEST_CASE_ERROR[lIndex].mCompressed), TEST_CASE_ERROR[lIndex].mCompressedSize_byte, lBuffer, TEST_CASE_ERROR[lIndex].mUncompressedSize_byte);
            }

            KMS_TEST_ERROR();
        }
        catch (KmsLib::Exception * eE)
        {
            KMS_TEST_ERROR_INFO;
            eE->Write(stdout);
            KMS_TEST_COMPARE(TEST_CASE_ERROR[lIndex].mCode, eE->GetCode());
        }
    }

    // ===== OK - 1 buffer ==================================================
    for (lIndex = 0; lIndex < TEST_CASE_OK_QTY; lIndex++)
    {
        memcpy(lBuffer, TEST_CASE_OK[lIndex].mUncompressed, TEST_CASE_OK[lIndex].mUncompressedSize_byte);
        lSize_byte = KmsLib::RLE_Compress(lBuffer, TEST_CASE_OK[lIndex].mUncompressedSize_byte, lBuffer, sizeof(lBuffer));
        KMS_TEST_COMPARE(TEST_CASE_OK[lIndex].mCompressedSize_byte, lSize_byte);
        KMS_TEST_COMPARE(0, memcmp(TEST_CASE_OK[lIndex].mCompressed, lBuffer, lSize_byte));

        memcpy(lBuffer, TEST_CASE_OK[lIndex].mCompressed, TEST_CASE_OK[lIndex].mCompressedSize_byte);
        lSize_byte = KmsLib::RLE_Uncompress(lBuffer, TEST_CASE_OK[lIndex].mCompressedSize_byte, lBuffer, sizeof(lBuffer));
        KMS_TEST_COMPARE(TEST_CASE_OK[lIndex].mUncompressedSize_byte, lSize_byte);
        KMS_TEST_COMPARE(0, memcmp(TEST_CASE_OK[lIndex].mUncompressed, lBuffer, lSize_byte));
    }

    // ===== Error - 1 buffer ===============================================
    for (lIndex = 0; lIndex < TEST_CASE_ERROR_QTY; lIndex++)
    {
        try
        {
            if (NULL == TEST_CASE_ERROR[lIndex].mCompressed)
            {
                memcpy(lBuffer, TEST_CASE_ERROR[lIndex].mUncompressed, TEST_CASE_ERROR[lIndex].mUncompressedSize_byte);
                lSize_byte = KmsLib::RLE_Compress(lBuffer, TEST_CASE_ERROR[lIndex].mUncompressedSize_byte, lBuffer, TEST_CASE_ERROR[lIndex].mCompressedSize_byte);
            }
            else
            {
                memcpy(lBuffer, TEST_CASE_ERROR[lIndex].mCompressed, TEST_CASE_ERROR[lIndex].mCompressedSize_byte);
                lSize_byte = KmsLib::RLE_Uncompress(lBuffer, TEST_CASE_ERROR[lIndex].mCompressedSize_byte, lBuffer, TEST_CASE_ERROR[lIndex].mUncompressedSize_byte);
            }

            KMS_TEST_ERROR();
        }
        catch (KmsLib::Exception * eE)
        {
            KMS_TEST_ERROR_INFO;
            eE->Write(stdout);
            KMS_TEST_COMPARE(TEST_CASE_ERROR[lIndex].mCode, eE->GetCode());
        }
    }
}
KMS_TEST_END_2
