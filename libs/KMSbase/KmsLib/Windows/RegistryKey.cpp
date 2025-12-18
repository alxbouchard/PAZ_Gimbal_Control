
// Author / Auteur		KMS -	Martin Dubois, ing.
// Product / Produit	KmsBase
// File / Fichier		KmsLib/Windows/RegistryKey.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>

// ===== Windows ============================================================
#include <Windows.h>
#include <Cfgmgr32.h>

// ===== Interface ==========================================================
#include <KmsLib/Exception.h>

#include <KmsLib/Windows/RegistryKey.h>

namespace KmsLib
{
	namespace Windows
	{
		// Public
		/////////////////////////////////////////////////////////////////////

		RegistryKey::RegistryKey() : mKey( NULL )
		{
		}

		RegistryKey::~RegistryKey()
		{
			if (NULL != mKey)
			{
				Close();
			}

			assert(NULL == mKey);
		}

		RegistryKey::operator HKEY ()
		{
			assert(NULL != mKey);

			return mKey;
		}

		bool RegistryKey::DoesSubKeyExist(const char * aSubKey)
		{
			assert(NULL != aSubKey);
			
			assert(NULL != mKey);

			HKEY lKey;

			LSTATUS lRetS = RegOpenKeyEx(mKey, aSubKey, 0, DELETE | KEY_READ | KEY_WRITE, &lKey);
			switch (lRetS)
			{
			case ERROR_SUCCESS :
				assert(NULL != lKey);

				lRetS = RegCloseKey(lKey);
				if (ERROR_SUCCESS != lRetS)
				{
					// NOTE TESTED   KmsLib.RegistryKey.ErrorHandling
                    //               RegCloseKey fail / RegCloseKey
                    //               echoue<br>
                    //               Realy not easy to test / Vraiment pas
                    //               facile a tester
					char lMsg[2048];

					sprintf_s(lMsg, "An error occured when closing the sub key %s", aSubKey);

					throw new Exception(Exception::CODE_REG_CLOSE_ERROR, "RegCloseKey(  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, lRetS);
				}
				return true;

			case ERROR_FILE_NOT_FOUND :
				break;

			default:
				throw new Exception(Exception::CODE_REG_OPEN_ERROR, "RegOpenKey( , ,  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, lRetS);
			}

			return false;
		}

		void RegistryKey::GetValue(const char * aName, char * aOut, unsigned int aOutSize_byte)
		{
			assert(NULL	!=	aOut			);
			assert(0	<	aOutSize_byte	);

			DWORD lSize_byte = aOutSize_byte;
			DWORD lType;

			LSTATUS lRetS = RegQueryValueEx(mKey, aName, NULL, &lType, reinterpret_cast<LPBYTE>(aOut), &lSize_byte);
			switch (lRetS)
			{
			case ERROR_SUCCESS:
				if ((REG_SZ != lType) || (aOutSize_byte < lSize_byte))
				{
					throw new KmsLib::Exception(KmsLib::Exception::CODE_REGISTRY_ERROR, "Invalid registry value",
						NULL, __FILE__, __FUNCTION__, __LINE__, lType);
				}
				break;

			default:
				throw new KmsLib::Exception(KmsLib::Exception::CODE_REG_QUERY_ERROR, "Registry error",
					NULL, __FILE__, __FUNCTION__, __LINE__, lRetS);
			}
		}

		DWORD RegistryKey::GetValue_DWORD(const char * aName, DWORD aDefaultValue)
		{
			assert(NULL != mKey);

			DWORD lResult						;
			DWORD lSize_byte = sizeof(lResult)	;
			DWORD lType							;

			LSTATUS lRetS = RegQueryValueEx(mKey, aName, NULL, &lType, reinterpret_cast<LPBYTE>(&lResult), &lSize_byte);
			switch (lRetS)
			{
			case ERROR_SUCCESS :
				if ((REG_DWORD != lType) || (sizeof(lResult) != lSize_byte))
				{
					throw new KmsLib::Exception(KmsLib::Exception::CODE_REGISTRY_ERROR, "Invalid registry value",
						NULL, __FILE__, __FUNCTION__, __LINE__, lType);
				}
				break;

			case ERROR_FILE_NOT_FOUND :
				lResult = aDefaultValue;
				break;

			default :
				throw new KmsLib::Exception(KmsLib::Exception::CODE_REG_QUERY_ERROR, "Registry error",
					NULL, __FILE__, __FUNCTION__, __LINE__, lRetS);
			}

			return lResult;
		}

		void RegistryKey::SetDefaultValue(const char * aValue)
		{
			assert(NULL != aValue);

			SetValue(NULL, aValue);
		}

		void RegistryKey::SetValue(const char * aName, DWORD aValue)
		{
			assert(NULL != aName);

			SetValue(aName, REG_DWORD, &aValue, sizeof(DWORD));
		}

		void RegistryKey::SetValue(const char * aName, const char * aValue)
		{
			assert(NULL != aValue	);

			SetValue(aName, REG_SZ, aValue, static_cast< unsigned int >( strlen(aValue) ) + 1);
		}

		void RegistryKey::SetValue(const char * aName, DWORD aType, const void * aValue, unsigned int aValueSize_byte)
		{
			assert(NULL != mKey);

			LSTATUS lRet = RegSetValueEx(mKey, aName, 0, aType, reinterpret_cast< const BYTE * >(aValue), aValueSize_byte);
			if (ERROR_SUCCESS != lRet)
			{
				char lMessage[2048];

				sprintf_s(lMessage, sizeof(lMessage), "RegSetValueEx( , \"%s\", , 0x%08x, , %u bytes ) failed\n",
					aName, aType, aValueSize_byte);

				throw new Exception(Exception::CODE_REG_SET_ERROR, "RegSetValueEx( , , , , ,  ) failed",
					lMessage, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

		void RegistryKey::Close()
		{
			assert(NULL != mKey);

			LSTATUS lRet = RegCloseKey(mKey);

			mKey = NULL;

			if (ERROR_SUCCESS != lRet)
			{
				// NOTE TESTED   KmsLib.RegistryKey.ErrorHandling
                //               RegCloseKey fail / RegCloseKey echoue<br>
                //               Realy not easy to test / Vraiment pas
                //               facile a tester
				throw new Exception(Exception::CODE_REGISTRY_ERROR, "RegCloseKey(  ) failed",
					NULL, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

		void RegistryKey::Create(HKEY aKey, const char * aSubKey)
		{
			assert(NULL != aKey		);
			assert(NULL != aSubKey	);

			if (NULL != mKey)
			{
				Close();
			}

			LSTATUS lRet = RegCreateKeyEx(aKey, aSubKey, 0, NULL, 0, DELETE | KEY_READ | KEY_WRITE, NULL, &mKey, NULL);
			if (ERROR_SUCCESS != lRet)
			{
				char lMessage[2048];

				sprintf_s(lMessage, sizeof(lMessage), "RegCreateKeyEx( , \"%s\", , , , , , ,  ) failed",
					aSubKey);

				throw new Exception(Exception::CODE_REG_CREATE_ERROR, "RegCreateKeyEx( , , , , , , , ,  ) failed",
					lMessage, __FILE__, __FUNCTION__, __LINE__, lRet);
			}

			assert(NULL != mKey);
		}

		void RegistryKey::DeleteSubKey(const char * aSubKey)
		{
			assert(NULL != aSubKey);
			
			assert(NULL != mKey);

			LSTATUS lRet = RegDeleteTree(mKey, aSubKey);
			if (ERROR_SUCCESS != lRet)
			{
				char lMessage[2048];

				sprintf_s(lMessage, sizeof(lMessage), "RegDeleteTree( , \"%s\" ) failed",
					aSubKey);

				throw new Exception(Exception::CODE_REG_DELETE_ERROR, "RegDeleteTree( ,  ) failed",
					lMessage, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

		void RegistryKey::DeleteValue(const char * aName)
		{
			assert(NULL != aName);

			assert(NULL != mKey);

			LSTATUS lRet = RegDeleteValue(mKey, aName);
			if (ERROR_SUCCESS != lRet)
			{
				char lMessage[2048];

				sprintf_s(lMessage, sizeof(lMessage), "RegDeleteValue( , \"%s\" ) failed",
					aName);

				throw new Exception(Exception::CODE_REG_DELETE_ERROR, "RegDeleteValue( ,  ) failed",
					lMessage, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

		void RegistryKey::Open(HKEY aKey, const char * aSubKey)
		{
			assert(NULL != aKey);
			assert(NULL != aSubKey);

			if (NULL != mKey)
			{
				Close();
			}

			LSTATUS lRet = RegOpenKeyEx(aKey, aSubKey, 0, DELETE | KEY_READ | KEY_WRITE, &mKey);
			if (ERROR_SUCCESS != lRet)
			{
				char lMessage[2048];

				sprintf_s(lMessage, sizeof(lMessage), "RegOpenKeyEx( , \"%s\", , ,  ) failed",
					aSubKey);

				throw new Exception(Exception::CODE_REG_OPEN_ERROR, "RegOpenKeyEx( , , , ,  ) failed",
					lMessage, __FILE__, __FUNCTION__, __LINE__, lRet);
			}

			assert(NULL != mKey);
		}

		bool RegistryKey::Open(HKEY aKey, unsigned int aSubKeyIndex)
		{
			assert(NULL != aKey);

			char lName[256];

			LONG lRet = RegEnumKey(aKey, aSubKeyIndex, lName, sizeof(lName));
			if (ERROR_SUCCESS != lRet)
			{
				return false;
			}

			Open(aKey, lName);

			return true;
		}

		// The test of the DriverHandle class is responsible for testing this
		// method / Le test de la class DriverHandle est responsable de
		// tester cette methode
		void RegistryKey::Open(HDEVINFO aDevInfoSet, PSP_DEVINFO_DATA aDevInfoData, unsigned int aFlags)
		{
			assert(NULL != aDevInfoSet		);
			assert(NULL != aDevInfoData		);

			if (NULL != mKey)
			{
				Close();
			}

			char lMsg[2048];

			if (0 != (aFlags & OPEN_FLAG_ADMINISTRATOR))
			{
				DWORD lFlags = (0 != (aFlags & OPEN_FLAG_DRIVER_KEY)) ? CM_REGISTRY_SOFTWARE : CM_REGISTRY_HARDWARE;

				CONFIGRET lRetCR = CM_Open_DevNode_Key(aDevInfoData->DevInst, KEY_QUERY_VALUE | KEY_SET_VALUE, 0, RegDisposition_OpenAlways, &mKey, lFlags);
				if (CR_SUCCESS != lRetCR)
				{
					sprintf_s(lMsg, "CM_Open_DevNode_key( , , , , , 0x%08x ) failed", lFlags);

					throw new Exception(Exception::CODE_REGISTRY_ERROR, "CM_Open_DevNode_Key( , , , , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, lRetCR);
				}

				// TESTED   KmsLib.RegistryKey
                //          KmsLib_Test.exe - DriverHandle - Setup C<br>
                //          Open the registry key associated to a device /
                //          Ouvre la cle de registre associee a un
                //          peripherique
			}
			else
			{
				DWORD lKeyType = (0 != (aFlags & OPEN_FLAG_DRIVER_KEY)) ? DIREG_DRV : DIREG_DEV;

				mKey = SetupDiOpenDevRegKey(aDevInfoSet, aDevInfoData, DICS_FLAG_GLOBAL, 0, KEY_QUERY_VALUE, lKeyType);
				if (INVALID_HANDLE_VALUE == mKey)
				{
					// NOT TESTED   KmsLib.RegistryKey.ErrorHandling
                    //              SetupDiOpenDevRegKey fail /
                    //              SetupDiOpenDevRegKey echoue<br>
                    //              Not easy to test / Pas facile a tester

					// This class use NULL, not INVALID_HANDLE_VALUE / Cette
					// classe utilise NULL et non INVALID_HANDLE_VALUE.
					mKey = NULL;

					sprintf_s(lMsg, "SetupDiOpenDevRegKey( , , , , , %u ) failed", lKeyType);

					throw new Exception(Exception::CODE_REGISTRY_ERROR, "SetupDiOpenDevRegKey( , , , , ,  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, 0);
				}
			}
		}

	}

}
