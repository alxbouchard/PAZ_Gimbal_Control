
// Author / Auteur		:	KMS	-	Martin Dubois, ing.
// Product / Produit	:	KmsBase
// File / Fichier		:	KmsLib/CmdLineParser.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <KmsLib/CmdLineParser.h>

// Declarations des fonctions statique
/////////////////////////////////////////////////////////////////////////////

static unsigned int ConvertToUInt(const char * aIn, int aRadix);

namespace KmsLib
{

	// Public
	/////////////////////////////////////////////////////////////////////////

	CmdLineParser::CmdLineParser(const char * aOptNoData, const char * aOptWithData) :
		mArgCount		(0				),
		mErrorFlags		(0				),
		mOptNoData		(aOptNoData		),
		mOptWithData	(aOptWithData	)
	{
		assert(NULL != aOptNoData	);
		assert(NULL != aOptWithData	);
	}

	CmdLineParser::~CmdLineParser()
	{
	}

	bool CmdLineParser::IsPresent(int aArgOpt) const
	{
		return (mOptions.end() != mOptions.find(aArgOpt));
	}

	unsigned int CmdLineParser::GetArgumentCount() const
	{
		return mArgCount;
	}

	void CmdLineParser::GetData(int aArgOpt, char * aOut, unsigned int aOutSize_byte, const char * aDefault) const
	{
		assert(NULL	!=	aOut			);
		assert(0	<	aOutSize_byte	);

		Map::const_iterator lIt = mOptions.find(aArgOpt);
		if (mOptions.end() == lIt)
		{
			if (NULL == aDefault)
			{
				throw new Exception(Exception::CODE_INVALID_COMMAND_LINE,
					"An mandatory argument or option is not present",
					NULL, __FILE__, __FUNCTION__, __LINE__, aArgOpt);
			}

			strncpy_s(aOut SIZE_INFO( aOutSize_byte ), aDefault, aOutSize_byte);
		}
		else
		{
			strncpy_s(aOut SIZE_INFO( aOutSize_byte ), lIt->second.c_str(), aOutSize_byte);
		}
	}

	void CmdLineParser::GetData(int aArgOpt, unsigned int * aOut, int aRadix) const
	{
		assert(NULL != aOut);

		Map::const_iterator lIt = mOptions.find(aArgOpt);
		if (mOptions.end() == lIt)
		{
			throw new Exception(Exception::CODE_INVALID_COMMAND_LINE,
				"An mandatory argument or option is not present",
				NULL, __FILE__, __FUNCTION__, __LINE__, aArgOpt);
		}
		else
		{
			(*aOut) = ConvertToUInt(lIt->second.c_str(), aRadix);
		}
	}

	void CmdLineParser::GetData(int aArgOpt, unsigned int * aOut, int aRadix, unsigned int aDefault) const
	{
		assert(NULL != aOut);

		Map::const_iterator lIt = mOptions.find(aArgOpt);
		if (mOptions.end() == lIt)
		{
			(*aOut) = aDefault;
		}
		else
		{
			(*aOut) = ConvertToUInt(lIt->second.c_str(), aRadix);
		}
	}

	void CmdLineParser::GetData(int aArgOpt, FILE ** aOut, const char * aMode, FILE * aDefault) const
	{
		assert(NULL != aOut		);
		assert(NULL != aMode	);

		Map::const_iterator lIt = mOptions.find(aArgOpt);
		if (mOptions.end() == lIt)
		{
			if (NULL == aDefault)
			{
				throw new Exception(Exception::CODE_INVALID_COMMAND_LINE,
					"An mandatory argument or option is not present",
					NULL, __FILE__, __FUNCTION__, __LINE__, aArgOpt);
			}

			(*aOut) = aDefault;
		}
		else
		{
			errno_t lRet = fopen_s(aOut, lIt->second.c_str(), aMode);
			if (0 != lRet)
			{
				char lMsg[2048];

				sprintf_s(lMsg, "Cannot open %s (%s)", lIt->second.c_str(), aMode);

				throw new Exception(Exception::CODE_FILE_OPEN_ERROR, "Cannot open file",
					lMsg, __FILE__, __FUNCTION__, __LINE__, aArgOpt);
			}
		}
	}

	unsigned int CmdLineParser::GetErrorFlags() const
	{
		return mErrorFlags;
	}

	void CmdLineParser::Parse(int aCount, const char ** aVector)
	{
		assert(1	<=	aCount		);
		assert(NULL !=	aVector		);
		assert(NULL !=	aVector[0]	);

		for (unsigned int i = 1; i < static_cast<unsigned int>(aCount); i++)
		{
			if ('-' == aVector[i][0])
			{
				unsigned int j = 1;

				do
				{
					if ('\0' == aVector[i][j])
					{
						mArgCount++;
						break;
					}

					if (NULL != strchr(mOptWithData, aVector[i][j]))
					{
						if ('\0' != aVector[i][j + 1])
						{
							mErrorFlags |= ERROR_FLAG_IGNORED_OPTIONS;
						}

						i++;
						if (static_cast<unsigned int>(aCount) <= i)
						{
							mErrorFlags |= ERROR_FLAG_INCOMPLET_OPTION;
							return;
						}

						Map::value_type lVal(aVector[i - 1][j], aVector[i]);
						mOptions.insert(lVal);
						break;
					}

					if (NULL != strchr(mOptNoData, aVector[i][j]))
					{
						Map::value_type lVal(aVector[i][j], "");
						mOptions.insert(lVal);
					}
					else
					{
						mErrorFlags |= ERROR_FLAG_INVALID_OPTION;
					}

					j++;
				}
				while ('\0' != aVector[i][j]);
			}
			else
			{
				Map::value_type lVal(mArgCount, aVector[i]);
				mOptions.insert(lVal);
				mArgCount++;
			}
		}
	}

}

// Fonctions statiques
/////////////////////////////////////////////////////////////////////////////

unsigned int ConvertToUInt(const char * aIn, int aRadix)
{
	assert(NULL != aIn);

	char * lPtr;

	unsigned int lResult = static_cast< unsigned int >( strtoul(aIn, &lPtr, aRadix) );

	if ('\0' != (*lPtr))
	{
		throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_COMMAND_LINE,
			"An argument or option contains ignored data",
			NULL, __FILE__, __FUNCTION__, __LINE__, 0);
	}

	return lResult;
}
