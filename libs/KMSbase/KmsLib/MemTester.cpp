
// Author / Auteur		:	KMS -	Martin Dubois, ing.
// Product / Produit	:	KmsBase
// File / Fichier		:	KmsLib/MemTester.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <memory.h>
#include <stdlib.h>

// ===== Includes ===========================================================
#include <KmsLib/MemTester.h>

// Constants / Constantes
/////////////////////////////////////////////////////////////////////////////

#define COUNT_KMS		(100)
#define COUNT_RANDOM	(999)
#define COUNT_SIMPLE	(256)

static unsigned int COUNT[KmsLib::MemTester::PATTERN_TYPE_QTY] = { COUNT_KMS, COUNT_RANDOM, COUNT_SIMPLE };

namespace KmsLib
{

	// Public
	/////////////////////////////////////////////////////////////////////////

	MemTester::MemTester()
	{
		Reset();
	}

	MemTester::~MemTester()
	{
	}

	void MemTester::Reset()
	{
		mIndex			= 0				;
		mPatternType	= PATTERN_TYPE_START	;
	}

	unsigned int MemTester::GetCount() const
	{
		unsigned int lResult = 0;

		for (unsigned int i = PATTERN_TYPE_START; i < PATTERN_TYPE_QTY; i++)
		{
			lResult += GetCount(static_cast<PatternType>(i));
		}

		return lResult;
	}

	unsigned int MemTester::GetCount(PatternType aPatternType) const
	{
		assert(PATTERN_TYPE_QTY > aPatternType);

		return COUNT[aPatternType];
	}

	unsigned int MemTester::GetIndex() const
	{
		return mIndex;
	}

	void MemTester::ResetIndex()
	{
		mIndex = 0;
	}

	MemTester::PatternType MemTester::GetPatternType() const
	{
		assert(PATTERN_TYPE_QTY > mPatternType);

		return static_cast<PatternType>(mPatternType);
	}

	void MemTester::SetPatternType(PatternType aPatternType)
	{
		assert(PATTERN_TYPE_QTY > aPatternType);

		mIndex			= 0				;
		mPatternType	= aPatternType	;
	}

	unsigned int MemTester::GetProgress( bool aGlobal ) const
	{
		assert(PATTERN_TYPE_QTY > mPatternType);

		unsigned int lCount;
		unsigned int lDone = 0;

		if (aGlobal)
		{
			lCount = GetCount();

			for (unsigned int i = 0; i < mPatternType; i++)
			{
				lDone += GetCount(static_cast<PatternType>(i));
			}
		}
		else
		{
			lCount = GetCount(static_cast<const PatternType>(mPatternType));
		}

		lDone += mIndex;

		assert(lCount >= lDone);

		return ((lDone * 1000) / lCount);
	}

	void MemTester::Generate(unsigned char * aOut, unsigned int aOutSize_byte) const
	{
		assert(NULL !=	aOut			);
		assert(0	<	aOutSize_byte	);

		switch (mPatternType)
		{
		case PATTERN_TYPE_KMS		: Generate_KMS		(aOut, aOutSize_byte); break;
		case PATTERN_TYPE_RANDOM	: Generate_Random	(aOut, aOutSize_byte); break;
		case PATTERN_TYPE_SIMPLE	: Generate_Simple	(aOut, aOutSize_byte); break;

		default: assert(false);
		}
	}

	bool MemTester::NextIndex()
	{
		assert(PATTERN_TYPE_QTY > mPatternType);

		unsigned int lCount = COUNT[mPatternType];

		assert(lCount > mIndex);

		mIndex++;

		bool lResult = (lCount > mIndex);
		if (!lResult)
		{
			mIndex = 0;
		}

		return lResult;
	}

	bool MemTester::NextIndexAndPatternType()
	{
		bool lResult = NextIndex();
		if (!lResult)
		{
			mPatternType++;

			if (PATTERN_TYPE_QTY > mPatternType)
			{
				lResult = true;
			}
			else
			{
				mPatternType = PATTERN_TYPE_START;
			}
		}

		return lResult;
	}

	unsigned int MemTester::Verify(const unsigned char * aIn, unsigned int aInSize_byte) const
	{
		assert(NULL	!=	aIn				);
		assert(0	<	aInSize_byte	);

		unsigned int lResult;

		switch (mPatternType)
		{
		case PATTERN_TYPE_KMS		: lResult = Verify_KMS		(aIn, aInSize_byte); break;
		case PATTERN_TYPE_RANDOM	: lResult = Verify_Random	(aIn, aInSize_byte); break;
		case PATTERN_TYPE_SIMPLE	: lResult = Verify_Simple	(aIn, aInSize_byte); break;

		default: assert(false);
		}

		return lResult;
	}

	// Private
	/////////////////////////////////////////////////////////////////////////

	void MemTester::Generate_KMS(unsigned char * aOut, unsigned int aOutSize_byte) const
	{
		assert(NULL !=	aOut			);
		assert(0	<	aOutSize_byte	);

		assert(COUNT_KMS > mIndex);

		unsigned int	lCount	= aOutSize_byte / sizeof(unsigned int);
		unsigned int	lData	= 0			;
		unsigned int  * lDst	= reinterpret_cast<unsigned int *>(aOut);
		unsigned int	lIndex	= mIndex	;

		for (unsigned int i = 0; i < lCount; i++)
		{
			switch (lIndex % COUNT_KMS)
			{
			case 0: lData = 0x00000000; break;
			case 1: lData = 0x55555555; break;
			case 3: lData = 0x00000001; break;
			case 35: lData = 0x00000001; break;
			case 99: lData = 0xffffffff; break;
			default:
				if		(35 > lIndex)	{ lData <<= 1; }
				else if	(67 > lIndex)	{ lData <<= 1; lData |= 0x00000001; }
				else					{ lData <<= 1; }
			}

			lIndex++;

			*lDst = lData;

			lDst++;
		}
	}

	void MemTester::Generate_Random(unsigned char * aOut, unsigned int aOutSize_byte) const
	{
		assert(NULL !=	aOut			);
		assert(0	<	aOutSize_byte	);

		assert(COUNT_RANDOM > mIndex);

		srand(mIndex);

		for (unsigned int i = 0; i < aOutSize_byte; i++)
		{
			aOut[i] = (rand() & 0x00ff);
		}
	}

	void MemTester::Generate_Simple(unsigned char * aOut, unsigned int aOutSize_byte) const
	{
		assert(NULL != aOut				);
		assert(0 != aOutSize_byte	);

		assert(COUNT_SIMPLE > mIndex);

		memset(aOut, mIndex, aOutSize_byte);
	}

	unsigned int MemTester::Verify_KMS(const unsigned char * aIn, unsigned int aInSize_byte) const
	{
		assert(NULL	!=	aIn				);
		assert(0	<	aInSize_byte	);

		unsigned int	lCount	= aInSize_byte / sizeof(unsigned int);
		unsigned int	lData	= 0			;
		unsigned int	lIndex	= mIndex	;
		unsigned int	lResult	= 0			;

		const unsigned int  * lSrc = reinterpret_cast<const unsigned int *>(aIn);

		for (unsigned int i = 0; i < lCount; i++)
		{
			switch (lIndex % COUNT_KMS)
			{
			case 0: lData = 0x00000000; break;
			case 1: lData = 0x55555555; break;
			case 3: lData = 0x00000001; break;
			case 35: lData = 0x00000001; break;
			case 99: lData = 0xffffffff; break;
			default:
				if (35 > lIndex)	{ lData <<= 1; }
				else if (67 > lIndex)	{ lData <<= 1; lData |= 0x00000001; }
				else					{ lData <<= 1; }
			}

			lIndex++;

			if ((*lSrc) != lData)
			{
				lResult++;
			}

			lSrc++;
		}

		return lResult;
	}

	unsigned int MemTester::Verify_Simple(const unsigned char * aIn, unsigned int aInSize_byte) const
	{
		assert(NULL !=	aIn				);
		assert(0	<	aInSize_byte	);

		assert(COUNT_SIMPLE > mIndex);

		unsigned int lResult = 0;

		for (unsigned int i = 0; i < aInSize_byte; i++)
		{
			if (mIndex != aIn[i])
			{
				lResult++;
			}
		}

		return lResult;
	}

	unsigned int MemTester::Verify_Random(const unsigned char * aIn, unsigned int aInSize_byte) const
	{
		assert(NULL !=	aIn				);
		assert(0	<	aInSize_byte	);

		assert(COUNT_RANDOM > mIndex);

		srand(mIndex);

		unsigned int lResult = 0;

		for (unsigned int i = 0; i < aInSize_byte; i++)
		{
			if ((rand() & 0x00ff) != aIn[i])
			{
				lResult++;
			}
		}

		return lResult;
	}

}
