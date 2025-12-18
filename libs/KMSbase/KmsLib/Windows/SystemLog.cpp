
// Author / Auteur	:	KMS -	Martin Dubois, ing.
// Project / Projet	:	KmsBase
// File / Fichier	:	KmsLib/Windows/SystemLog.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/Windows/RegistryKey.h>

#include <KmsLib/Windows/SystemLog.h>

// Constants / Constantes
/////////////////////////////////////////////////////////////////////////////

#define REG_KEY_EVENT_LOG	"SYSTEM\\CurrentControlSet\\services\\eventlog"

namespace KmsLib
{
	namespace Windows
	{

		// Public
		/////////////////////////////////////////////////////////////////////

		SystemLog::SystemLog(const char * aLogName, const char * aSourceName) : mHandle(NULL), mLogName(aLogName), mSourceName(aSourceName)
		{
			assert(NULL != mLogName);
			assert(NULL != mSourceName);
		}

		SystemLog::~SystemLog()
		{
			if (NULL != mHandle)
			{
				Deregister();
			}

			assert(NULL == mHandle);
		}

		bool SystemLog::IsConfigured() const
		{
			assert(NULL != mLogName);
			assert(NULL != mSourceName);

			RegistryKey lKey0;

			lKey0.Open(HKEY_LOCAL_MACHINE, REG_KEY_EVENT_LOG);

			// TESTED : KmsLib_Test.exe - SystemLog - Setup B

			RegistryKey lKey1;

			lKey1.Open(lKey0, mLogName);

			return lKey1.DoesSubKeyExist(mSourceName);
		}

		void SystemLog::Configure(const char * aMessageFile)
		{
			assert(NULL != aMessageFile);

			assert(NULL != mLogName		);
			assert(NULL != mSourceName	);

			RegistryKey lKey0;

			lKey0.Open(HKEY_LOCAL_MACHINE, REG_KEY_EVENT_LOG);

			// TESTED : KmsLib_Test.exe - SystemLog - Setup B

			RegistryKey lKey1;

			lKey1.Open		(lKey0, mLogName	);
			lKey0.Create	(lKey1, mSourceName	);

			lKey0.SetValue("EventMessageFile"	, REG_EXPAND_SZ, aMessageFile, static_cast<unsigned int>(strlen(aMessageFile)) + 1);
			lKey0.SetValue("TypeSupported"		, 0x00000007);
		}

		void SystemLog::Unconfigure()
		{
			assert(NULL != mLogName);
			assert(NULL != mSourceName);

			RegistryKey lKey0;

			lKey0.Open(HKEY_LOCAL_MACHINE, REG_KEY_EVENT_LOG);

			// TESTED : KmsLib_Test.exe - SystemLog - Setup B

			RegistryKey lKey1;

			lKey1.Open(lKey0, mLogName);

			lKey1.DeleteSubKey(mSourceName);
		}

		void SystemLog::LogEvent(WORD aType, WORD aCategory, DWORD aEventId, WORD aStringCount, DWORD aDataSize_byte, LPCTSTR * aString, LPVOID aData)
		{
			if (NULL == mHandle)
			{
				Register();

				// TESTED : KmsLib_Test.exe - SystemLog - Setup B
			}

			// TESTED : KmsLib_Test.exe - SystemLog - Setup B

			if (!ReportEvent(mHandle, aType, aCategory, aEventId, NULL, aStringCount, aDataSize_byte, aString, aData))
			{
				char lMsg[2048];

				sprintf_s(lMsg, "Cannot log the event (Type=%u, Category=%u, Event=%u, String=%u, Size=%u bytes)", aType, aCategory, aEventId, aStringCount, aDataSize_byte);

				throw new Exception(Exception::CODE_SYSTEM_LOG_ERROR,
					"ReportEvent( , , , , , , , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
			}
		}

		// Private
		/////////////////////////////////////////////////////////////////////

		// Exception : KmsLib::Exception	CODE_SYSTEM_LOG_ERROR
		void SystemLog::Deregister()
		{
			// TESTED : KmsLib_Test.exe - SystemLog - Setup B

			assert(NULL != mHandle);

			BOOL lRetB = DeregisterEventSource(mHandle);
			mHandle = NULL;
			if (!lRetB)
			{
				// NOTE TESTED : Not easy to test / Pas facile a tester
				throw new Exception(Exception::CODE_SYSTEM_LOG_ERROR,
					"DeregisterEventSource(  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, 0);
			}
		}

		// Exception : KmsLib::Exception	CODE_SYSTEM_LOG_ERROR
		void SystemLog::Register()
		{
			assert(NULL == mHandle);
			assert(NULL != mSourceName);

			mHandle = RegisterEventSource(NULL, mSourceName);
			if (NULL == mHandle)
			{
				char lMsg[2048];

				sprintf_s(lMsg, "Cannot register the event source %s", mSourceName);

				throw new Exception(Exception::CODE_SYSTEM_LOG_ERROR,
					"RegisterEventSource( ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
			}

			// TESTED : KmsLib_Test.exe - SystemLog - Setup B
		}

	}
}
