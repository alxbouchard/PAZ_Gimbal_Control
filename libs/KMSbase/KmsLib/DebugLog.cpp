
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib/DebugLog.cpp

// Last test coverage update / Derniere mise a jour de la couverture de test
// 2017-10-31

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <time.h>

#ifdef _KMS_WINDOWS_
	#include <process.h>
#endif // _KMS_WINDOWS_

// ===== Includes ===========================================================
#include <KmsLib/DebugLog.h>

// Constants / Constantes
/////////////////////////////////////////////////////////////////////////////

static const char * DAY_NAMES[] =
{
	"Sunday"	,
	"Monday"	,
	"Tuesday"	,
	"Wednesday"	,
	"Thursday"	,
	"Friday"	,
	"Saturday"	,
};

static const char * MONTH_NAMES[] =
{
	"January"	,
	"February"	,
	"March"		,
	"April"		,
	"May"		,
	"June"		,
	"July"		,
	"August"	,
	"September"	,
	"October"	,
	"November"	,
	"December"	,
};

namespace KmsLib
{
	
	// Public
	/////////////////////////////////////////////////////////////////////////

	DebugLog::DebugLog(const char * aFolder, const char * aName)
	{
		assert(NULL != aFolder	);
		assert(NULL != aName	);

		sIndex++;

		char lFileName[2048];

		sprintf_s(lFileName, "%s/pid_%u_log_%s_%u.txt", aFolder, _getpid(), aName, sIndex);

        mTF.Create(lFileName, TextFile::FLAG_IGNORE_ERROR);

        Log(__FILE__, __FUNCTION__, __LINE__);
        LogTime();
        Log("Creating log file");
	}

	DebugLog::~DebugLog()
	{
		Log(__FILE__, __FUNCTION__, __LINE__);
		LogTime();
		Log("Closing log file");
	}

	DebugLog::operator FILE * ()
	{
		return (FILE *)(mTF);
	}

	bool DebugLog::IsEnabled() const
	{
		return mTF.IsOpened();
	}

	void DebugLog::Log(const Exception * aException)
	{
		assert(NULL != aException);

		if (mTF.IsOpened())
		{
			fprintf(mTF, "EXCEPTION");
			aException->Write(mTF);
		}
	}

	void DebugLog::Log(const char * aFile, const char * aFunction, unsigned int aLine)
	{
		assert(NULL != aFile    );
		assert(NULL != aFunction);

		if (mTF.IsOpened())
		{
			fprintf(mTF, "%s (%u) - %s\n", aFile, aLine, aFunction);
		}
	}

	void  DebugLog::Log(const  char  * aMessage)
	{
		assert(NULL != aMessage);

		if (mTF.IsOpened())
		{
			fprintf(mTF, "%s\n", aMessage);
		}
	}

	void  DebugLog::LogTime()
	{
		if (mTF.IsOpened())
		{
			time_t	lTime;

			time(&lTime);

			tm lLT;

			int  lRet = localtime_s(&lLT, &lTime);
			if (0 == lRet)
			{
				fprintf(mTF, "%s, %s %u %04u - %02u:%02u:%02u\n",
					DAY_NAMES[lLT.tm_wday], MONTH_NAMES[lLT.tm_mon], lLT.tm_mday, lLT.tm_year + 1900,
					lLT.tm_hour, lLT.tm_min, lLT.tm_sec);
			}
			else
			{
                // NOT TESTED   KmsLib.DebugLog.ErrorHandling
                //              localtime_s fail / localtime_s a echoue
				Log(__FILE__, __FUNCTION__, __LINE__);
				fprintf(mTF, "localtime_s( , 0x%08x%08x ) failed returning %u\n",
					static_cast<unsigned int>(lTime & 0xffffffff), static_cast<unsigned int>(lTime >> 32),
					lRet);
			}
		}
	}

	// Private
	/////////////////////////////////////////////////////////////////////////

	unsigned int DebugLog::sIndex = 0;

}
