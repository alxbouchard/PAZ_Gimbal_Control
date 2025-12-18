
// Author / Auteur		:	KMS -	Martin Dubois, ing.
// Product / Produit	:	KmsBase
// File / Fichier		:	KmsLib/RLE.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>

#include <KmsLib/RLE.h>

// Constants / Constantes
/////////////////////////////////////////////////////////////////////////////

#define ESCAPE	(0xFF)

// Static function declaration / Declaration des fonctions statiques
/////////////////////////////////////////////////////////////////////////////

static void			Compress	(		unsigned char * aOut	, unsigned int aOutSize_byte, unsigned int aOutLimit_byte, unsigned int aCount,	unsigned char aValue		);
static unsigned int Compress_0	(const	unsigned char * aIn		, unsigned int aInSize_byte	, unsigned char * aOut,	unsigned int aOutSize_byte	);
static unsigned int Compress_1	(		unsigned char * aInOut	, unsigned int aInSize_byte	,						unsigned int aOutSize_byte	);
static unsigned int Compress_2	(		unsigned char * aInOut	, unsigned int aInSize_byte	,						unsigned int aOutSize_byte	);

static unsigned int Uncompress		(const	unsigned char * aIn		, unsigned int aInSize_byte, unsigned char * aOut,	unsigned int aOutSize_byte	);
static unsigned int Uncompress		(		unsigned char * aInOut	, unsigned int aOffset															);
static unsigned int Uncompress_0	(const	unsigned char * aIn		, unsigned int aInSize_byte, unsigned char * aOut,	unsigned int aOutSize_byte	);
static unsigned int Uncompress_1	(		unsigned char * aInOut	, unsigned int aInSize_byte,						unsigned int aOutSize_byte	);
static unsigned int Uncompress_2	(		unsigned char * aInOut	, unsigned int aInSize_byte,						unsigned int aOutSize_byte	);

namespace KmsLib
{

	// Functions / Fonctions
	/////////////////////////////////////////////////////////////////////////

	unsigned int RLE_Compress(const unsigned char * aIn, unsigned int aInSize_byte, unsigned char * aOut, unsigned int aOutSize_byte)
	{
		assert(NULL !=	aIn				);
		assert(0	<	aInSize_byte	);
		assert(NULL	!=	aOut			);
		assert(0	<	aOutSize_byte	);

		unsigned int lResult_byte;

		if (aIn != aOut)
		{
			lResult_byte = Compress_0(aIn, aInSize_byte, aOut, aOutSize_byte);
		}
		else
		{
			lResult_byte = Compress_1(aOut, aInSize_byte, aOutSize_byte);
		}

		return lResult_byte;
	}

	unsigned int RLE_Uncompress(const unsigned char * aIn, unsigned int aInSize_byte, unsigned char * aOut, unsigned int aOutSize_byte)
	{
		assert(NULL !=	aIn				);
		assert(0	<	aInSize_byte	);
		assert(NULL !=	aOut			);
		assert(0	<	aOutSize_byte	);

		unsigned int lResult_byte;

		if (aIn != aOut)
		{
			lResult_byte = Uncompress_0(aIn, aInSize_byte, aOut, aOutSize_byte);
		}
		else
		{
			lResult_byte = Uncompress_1(aOut, aInSize_byte, aOutSize_byte);
		}

		return lResult_byte;
	}

	unsigned int RLE_ComputeCompressedSize(const unsigned char * aIn, unsigned int aInSize_byte)
	{
		assert(NULL !=	aIn				);
		assert(0	<	aInSize_byte	);

		unsigned int lCount;
		unsigned int lResult_byte = 0;

		for (unsigned int i = 0; i < aInSize_byte; i+= lCount)
		{
			for (lCount = 1; (lCount < aInSize_byte - i) && (lCount < 256); lCount++)
			{
				if (aIn[i] != aIn[i + lCount])
				{
					break;
				}
			}

			if (ESCAPE == aIn[i])
			{
				lResult_byte += 3;
			}
			else
			{
				lResult_byte += (3 < lCount) ? 3 : lCount;
			}
		}

		return lResult_byte;
	}

	unsigned int RLE_ComputeUncompressedSize(const unsigned char * aIn, unsigned int aInSize_byte)
	{
		assert(NULL !=	aIn				);
		assert(0	<	aInSize_byte	);

		unsigned int lResult_byte = 0;

		for (unsigned int i = 0; i < aInSize_byte; i++)
		{
			if (ESCAPE == aIn[i])
			{
				i++;

				if ((0 == aIn[i]) || (ESCAPE == aIn[i]) || ((3 >= aIn[i]) && (ESCAPE != aIn[i+1])))
				{
					throw new Exception(Exception::CODE_INVALID_DATA, "Invalid data", NULL, __FILE__, __FUNCTION__, __LINE__, aIn[i]);
				}

				lResult_byte += aIn[i];
				i++;
			}
			else
			{
				lResult_byte++;
			}
		}

		return lResult_byte;
	}

}

// Static functions / Fonctions statiques
/////////////////////////////////////////////////////////////////////////////

// aOut				: [out]
// aOutSize_byte	:
// aOutLimit_byte	:
// aCount			:
// aValue			:
//
// Exception :	KmsLib::Exception	CODE_INVALID_BUFFER_SIZE
//									CODE_OVERFLOW
void Compress(unsigned char * aOut, unsigned int aOutSize_byte, unsigned int aOutLimit_byte, unsigned int aCount, unsigned char aValue)
{
	assert(NULL		!=	aOut	);
	assert(0		<	aCount	);
	assert(ESCAPE	>	aCount	);

	assert((3 < aCount) || (ESCAPE == aValue));

	if (3 > aOutSize_byte)
	{
		throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_BUFFER_SIZE, "Buffer too small", NULL, __FILE__, __FUNCTION__, __LINE__, aOutSize_byte);
	}

	if (3 > aOutLimit_byte)
	{
		throw new KmsLib::Exception(KmsLib::Exception::CODE_OVERFLOW, "OVERFLOW", NULL, __FILE__, __FUNCTION__, __LINE__, aOutSize_byte);
	}

	aOut[0] = ESCAPE;
	aOut[1] = aCount;
	aOut[2] = aValue;
}

// aIn				: [in ]
// aInSize_byte		:
// aOut				: [out]
// aOutSize_byte	:
//
// Return : Compressed size / Taille compressee
//
// Exception :	KmsLib::Exception	CODE_INVALID_BUFFER_SIZE
unsigned int Compress_0(const unsigned char * aIn, unsigned int aInSize_byte, unsigned char * aOut, unsigned int aOutSize_byte)
{
	assert(NULL !=	aIn				);
	assert(0	<	aInSize_byte	);
	assert(NULL	!=	aOut			);
	assert(aIn	!=	aOut			);
	assert(0	<	aOutSize_byte	);

	unsigned int lCount;
	unsigned int lResult_byte = 0;

	for (unsigned int i = 0; i < aInSize_byte; i += lCount)
	{
		for (lCount = 1; (lCount < aInSize_byte - i) && (lCount < (ESCAPE - 1)); lCount++)
		{
			if (aIn[i] != aIn[i + lCount])
			{
				break;
			}
		}

		if (ESCAPE == aIn[i])
		{
			Compress(aOut + lResult_byte, aOutSize_byte - lResult_byte, 0xffffffff, lCount, ESCAPE);

			lResult_byte+=3;
		}
		else
		{
			switch (lCount)
			{
			case 0: assert(false);

			case 1 :
				if (lResult_byte + lCount > aOutSize_byte)
				{
					throw new KmsLib::Exception(KmsLib::Exception::CODE_BUFFER_TOO_SMALL, "Buffer too small", NULL, __FILE__, __FUNCTION__, __LINE__, lResult_byte);
				}

				aOut[lResult_byte] = aIn[i];
				lResult_byte += lCount;
				break;

			case 2:
				if (lResult_byte + lCount > aOutSize_byte)
				{
					throw new KmsLib::Exception(KmsLib::Exception::CODE_BUFFER_TOO_SMALL, "Buffer too small", NULL, __FILE__, __FUNCTION__, __LINE__, lResult_byte);
				}

				aOut[lResult_byte		] = aIn[i		];
				aOut[lResult_byte + 1	] = aIn[i + 1	];
				lResult_byte += lCount;
				break;

			case 3:
				if (lResult_byte + lCount > aOutSize_byte)
				{
					throw new KmsLib::Exception(KmsLib::Exception::CODE_BUFFER_TOO_SMALL, "Buffer too small", NULL, __FILE__, __FUNCTION__, __LINE__, lResult_byte);
				}

				aOut[lResult_byte		] = aIn[i		];
				aOut[lResult_byte + 1	] = aIn[i + 1	];
				aOut[lResult_byte + 2	] = aIn[i + 2	];
				lResult_byte += lCount;
				break;

			default:
				assert(ESCAPE > lCount);

				Compress(aOut + lResult_byte, aOutSize_byte - lResult_byte, 0xffffffff, lCount, aIn[i]);

				lResult_byte += 3;
			}
		}
	}

	return lResult_byte;
}

// aInOut			: [in,out]
// aInSize_byte		:
// aOutSize_byte	:
//
// Return : Compressed size / Taille compressee
//
// Exception :	KmsLib::Exception	CODE_INVALID_BUFFER_SIZE
//									CODE_NOT_ENOUGH_MEMORY
unsigned int Compress_1(unsigned char * aInOut, unsigned int aInSize_byte, unsigned int aOutSize_byte)
{
	assert(NULL	!=	aInOut			);
	assert(0	<	aInSize_byte	);
	assert(0	<	aOutSize_byte	);

	unsigned int lCount;
	unsigned int lResult_byte = 0;

	try
	{
		for (unsigned int i = 0; i < aInSize_byte; i += lCount)
		{
			for (lCount = 1; (lCount < aInSize_byte - i) && (lCount < (ESCAPE - 1)); lCount++)
			{
				if (aInOut[i] != aInOut[i + lCount])
				{
					break;
				}
			}

			if (ESCAPE == aInOut[i])
			{
				Compress(aInOut + lResult_byte, aOutSize_byte - lResult_byte, i - lResult_byte + 1, lCount, ESCAPE);

				lResult_byte += 3;
			}
			else
			{
				switch (lCount)
				{
				case 0: assert(false);

				case 1:
					if (lResult_byte + lCount > aOutSize_byte)
					{
						throw new KmsLib::Exception(KmsLib::Exception::CODE_BUFFER_TOO_SMALL, "Buffer too small", NULL, __FILE__, __FUNCTION__, __LINE__, lResult_byte);
					}

					aInOut[lResult_byte] = aInOut[i];
					lResult_byte += lCount;
					break;

				case 2:
					if (lResult_byte + lCount > aOutSize_byte)
					{
						throw new KmsLib::Exception(KmsLib::Exception::CODE_BUFFER_TOO_SMALL, "Buffer too small", NULL, __FILE__, __FUNCTION__, __LINE__, lResult_byte);
					}

					aInOut[lResult_byte] = aInOut[i];
					aInOut[lResult_byte + 1] = aInOut[i + 1];
					lResult_byte += lCount;
					break;

				case 3:
					if (lResult_byte + lCount > aOutSize_byte)
					{
						throw new KmsLib::Exception(KmsLib::Exception::CODE_BUFFER_TOO_SMALL, "Buffer too small", NULL, __FILE__, __FUNCTION__, __LINE__, lResult_byte);
					}

					aInOut[lResult_byte] = aInOut[i];
					aInOut[lResult_byte + 1] = aInOut[i + 1];
					aInOut[lResult_byte + 2] = aInOut[i + 2];
					lResult_byte += lCount;
					break;

				default:
					assert(ESCAPE >= lCount);

					Compress(aInOut + lResult_byte, aOutSize_byte - lResult_byte, i - lResult_byte + 1, lCount, aInOut[i]);

					lResult_byte += 3;
				}
			}
		}
	}
	catch (KmsLib::Exception * eE)
	{
		if (KmsLib::Exception::CODE_OVERFLOW != eE->GetCode())
		{
			throw;
		}

		return Compress_2(aInOut, aInSize_byte, aOutSize_byte);
	}

	return lResult_byte;
}

// aInOut			: [in,out]
// aInSize_byte		:
// aOutSize_byte	:
//
// Return : Compressed size / Taille compressee
//
// Exception :	KmsLib::Exception	CODE_INVALID_BUFFER_SIZE
//									CODE_NOT_ENOUGH_MEMORY
unsigned int Compress_2(unsigned char * aInOut, unsigned int aInSize_byte, unsigned int aOutSize_byte)
{
	assert(NULL !=	aInOut			);
	assert(0	<	aInSize_byte	);
	assert(0	<	aOutSize_byte	);

	unsigned char * lBuffer = new unsigned char[aInSize_byte];
	if (NULL == lBuffer)
	{
		// NOT TESTED : Not easy to test!
		throw new KmsLib::Exception(KmsLib::Exception::CODE_NOT_ENOUGH_MEMORY, "Not enough memory", NULL, __FILE__, __FUNCTION__, __LINE__, aInSize_byte);
	}
	
	memcpy(lBuffer, aInOut, aInSize_byte);

	unsigned int lResult;
	
	try
	{
		lResult = Compress_0(lBuffer, aInSize_byte, aInOut, aOutSize_byte);
	}
	catch (...)
	{
		delete[] lBuffer;

		throw;
	}

	delete[] lBuffer;

	return lResult;
}

// aIn				: [in ]
// aInSize_byte		:
// aOut				: [out]
// aOutSize_byte	:
//
// Return : Uncompressed size / Taille non compresse
//
// Exception :	KmsLib::Exception	CODE_INVALID_BUFFER_SIZE
//									CODE_INVALID_DATA
//									CODE_OVERFLOW
unsigned int Uncompress(const unsigned char * aIn, unsigned int aInSize_byte, unsigned char * aOut, unsigned int aOutSize_byte)
{
	assert(NULL		!= aIn		);
	assert(ESCAPE	== aIn[0]	);
	assert(NULL		!= aOut		);

	if (3 > aInSize_byte)
	{
		throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "Invalid data", NULL, __FILE__, __FUNCTION__, __LINE__, aInSize_byte);
	}

	unsigned int	lResult_byte	= aIn[1];
	unsigned char	lValue			= aIn[2];

	if ((0 == lResult_byte) || (ESCAPE == lResult_byte) || ((3 >= lResult_byte) && (lValue != ESCAPE)))
	{
		throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "Invalid data", NULL, __FILE__, __FUNCTION__, __LINE__, lResult_byte);
	}

	if (aOutSize_byte < lResult_byte)
	{
		throw new KmsLib::Exception(KmsLib::Exception::CODE_BUFFER_TOO_SMALL, "Buffer too small", NULL, __FILE__, __FUNCTION__, __LINE__, lResult_byte);
	}

	for (unsigned int i = 0; i < lResult_byte; i++)
	{
		aOut[i] = lValue;
	}

	return lResult_byte;
}

// aInOut		: [in,out]
// aOffset_byte	:
//
// Return	:	Uncompressed size / Taille non compressee
//
// Exception :	KmsLib::Exception	CODE_OVERFLOW
unsigned int Uncompress(unsigned char * aInOut, unsigned int aOffset_byte)
{
	assert(NULL		!= aInOut		);
	assert(ESCAPE	== aInOut[0]	);
	assert(0		!= aInOut[1]	);
	assert(ESCAPE	!= aInOut[1]	);
	assert(3		<= aOffset_byte	);

	assert((3 < aInOut[1]) || (ESCAPE == aInOut[2]));

	unsigned int	lResult_byte	= aInOut[1];
	unsigned char	lValue			= aInOut[2];

	if (aOffset_byte < lResult_byte)
	{
		throw new KmsLib::Exception(KmsLib::Exception::CODE_OVERFLOW, "Overflow", NULL, __FILE__, __FUNCTION__, __LINE__, lResult_byte);
	}

	for (unsigned int i = aOffset_byte - lResult_byte; i < aOffset_byte; i++)
	{
		aInOut[i] = lValue;
	}

	return lResult_byte;
}

// aIn				: [in ]
// aInSize_byte		:
// aOut				: [out]
// aOutSize_byte	:
//
// Return : Uncompressed size / Taille non compressee
//
// Exception :	KmsLib::Exception	CODE_INVALID_BUFFER_SIZE
//									CODE_INVALID_DATA
unsigned int Uncompress_0(const unsigned char * aIn, unsigned int aInSize_byte, unsigned char * aOut, unsigned int aOutSize_byte)
{
	assert(NULL !=	aIn				);
	assert(0	<	aInSize_byte	);
	assert(NULL	!=	aOut			);
	assert(aIn	!=	aOut			);
	assert(0	<	aOutSize_byte	);

	unsigned int lResult_byte = 0;

	for (unsigned int i = 0; i < aInSize_byte; i++)
	{
		if (ESCAPE == aIn[i])
		{
			lResult_byte += Uncompress(aIn + i, aInSize_byte - i, aOut + lResult_byte, aOutSize_byte - lResult_byte);

			i += 2;
		}
		else
		{
			if (aOutSize_byte <= lResult_byte)
			{
				throw new KmsLib::Exception(KmsLib::Exception::CODE_BUFFER_TOO_SMALL, "Buffer too small", NULL, __FILE__, __FUNCTION__, __LINE__, lResult_byte);
			}

			aOut[lResult_byte] = aIn[i];  lResult_byte++;
		}
	}

	return lResult_byte;
}

// aInOut			: [in,out]
// aInSize_byte		:
// aOutSize_byte	:
//
// Return : Uncompressed size / Taille non compressee
//
// Exception :	KmsLib::Exception	CODE_INVALID_BUFFER_SIZE
//									CODE_INVALID_DATA
//									CODE_NOT_ENOUGH_MEMORY
unsigned int Uncompress_1(unsigned char * aInOut, unsigned int aInSize_byte, unsigned int aOutSize_byte)
{
	assert(NULL !=	aInOut			);
	assert(0	<	aInSize_byte	);
	assert(0	<	aOutSize_byte	);

	unsigned int lResult = KmsLib::RLE_ComputeUncompressedSize(aInOut, aInSize_byte);

	if (aOutSize_byte < lResult)
	{
		throw new KmsLib::Exception(KmsLib::Exception::CODE_BUFFER_TOO_SMALL, "Buffer too small", NULL, __FILE__, __FUNCTION__, __LINE__, lResult);
	}

	if (aInSize_byte > lResult)
	{
		return Uncompress_2(aInOut, aInSize_byte, aOutSize_byte);
	}

	try
	{
		int lC = aInSize_byte	- 1;
		int lU = lResult		- 1;

		while (0 <= lU)
		{
			switch (lC)
			{
			case 0:
			case 1:
				if (ESCAPE == aInOut[lC])
				{
					throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "Invalid Data", NULL, __FILE__, __FUNCTION__, __LINE__, aInOut[lC]);
				}

				aInOut[lU] = aInOut[lC];

				lC--;
				lU--;
				break;

			case 2:
			case 3:
				if (ESCAPE == aInOut[lC - 2])
				{
					lU -= Uncompress(aInOut + lC - 2, lU - lC + 3);
					lC -= 3;
				}
				else
				{
					if (ESCAPE == aInOut[lC])
					{
						throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "Invalid Data", NULL, __FILE__, __FUNCTION__, __LINE__, aInOut[lC]);
					}

					aInOut[lU] = aInOut[lC];

					lC--;
					lU--;
				}
				break;

			default:
				if ((ESCAPE == aInOut[lC - 2]) && (ESCAPE != aInOut[lC - 4]))
				{
					lU -= Uncompress(aInOut + lC - 2, lU - lC + 3);
					lC -= 3;
				}
				else
				{
					if (ESCAPE == aInOut[lC])
					{
						throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "Invalid Data", NULL, __FILE__, __FUNCTION__, __LINE__, aInOut[lC]);
					}

					aInOut[lU] = aInOut[lC];
					lC--;
					lU--;
				}
			}
		}

		assert(-1 == lC);
	}
	catch (KmsLib::Exception * eE)
	{
		if (KmsLib::Exception::CODE_OVERFLOW != eE->GetCode())
		{
			throw;
		}

		return Uncompress_2(aInOut, aInSize_byte, aOutSize_byte);
	}

	return lResult;
}

// aInOut			: [in,out]
// aInSize_byte		:
// aOutSize_byte	:
//
// Return : Uncompressed size / Taille non compressee
//
// Exception :	KmsLib::Exception	CODE_INVALID_BUFFER_SIZE
//									CODE_INVALID_DATA
//									CODE_NOT_ENOUGH_MEMORY
unsigned int Uncompress_2(unsigned char * aInOut, unsigned int aInSize_byte, unsigned int aOutSize_byte)
{
	assert(NULL !=	aInOut			);
	assert(0	<	aInSize_byte	);
	assert(0	<	aOutSize_byte	);

	unsigned char * lBuffer = new unsigned char[aInSize_byte];
	if (NULL == lBuffer)
	{
		// NOT TESTED : Not easy to test!
		throw new KmsLib::Exception(KmsLib::Exception::CODE_NOT_ENOUGH_MEMORY, "Not enough memory", NULL, __FILE__, __FUNCTION__, __LINE__, aInSize_byte);
	}

	memcpy(lBuffer, aInOut, aInSize_byte);

	unsigned int lResult;

	try
	{
		lResult = Uncompress_0(lBuffer, aInSize_byte, aInOut, aOutSize_byte);
	}
	catch (...)
	{
		delete[] lBuffer;

		throw;
	}

	delete[] lBuffer;

	return lResult;
}
