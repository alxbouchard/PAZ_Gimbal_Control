
// Auteur / Author		KMS -	Martin Dubois, ing.
// Product / Produit	KmsBase
// File / Fichier		KmsLib/Windows/Windows.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>

// ===== KmsBase ============================================================
#include <KmsLib/Windows/RegistryKey.h>

#include <KmsLib/Windows/Windows.h>

// Constants / Constantes
/////////////////////////////////////////////////////////////////////////////

// ===== HKCR - Sub Key =====================================================
#define DIRECTORY_SHELL		"Directory\\shell"

// ===== HKLM - Sub Key =====================================================
#define SOFTWARE_CLASSES	"SOFTWARE\\Classes"

// ===== HKLM\SOFTWARE\Classes\... - Sub Key ================================
#define SHELL				"shell"

// ===== HKLM\SOFTWARE\Classes\...\shell - Sub Key ==========================
#define COMMAND				"command"

namespace KmsLib
{
	namespace Windows
	{

		// Constantes
		/////////////////////////////////////////////////////////////////////

		const char * COMMAND_EDIT = "Edit";
		const char * COMMAND_OPEN = "Open";

		// Fonctions
		/////////////////////////////////////////////////////////////////////

		void Extension_Register(const char * aExt,
			const char * aClassId, const char * aCommandName, const char * aCommand)
		{
			assert(NULL != aExt			);
			assert(NULL != aClassId		);
			assert(NULL != aCommandName	);
			assert(NULL != aCommand		);

			RegistryKey	lKey;

			lKey.Open(HKEY_LOCAL_MACHINE, SOFTWARE_CLASSES);

			// TESTED	KmsLib_Text		Windows - Windows - Setup B

			RegistryKey	lKey0;
			RegistryKey	lKey1;

			lKey0.Create(lKey, aExt		);
			lKey1.Create(lKey, aClassId	);

			lKey.Close();

			RegistryKey	lKey2;
			RegistryKey	lKey3;

			lKey2.Create(lKey0, "OpenWithProgIds"	);
			lKey3.Create(lKey1, SHELL				);

			lKey0.Close();
			lKey1.Close();

			lKey2.SetValue	(aClassId, REG_BINARY, NULL, 0);
			lKey1.Create	(lKey3, aCommandName);

			lKey2.Close();
			lKey3.Close();

			lKey3.Create(lKey1, COMMAND);

			lKey1.Close();

			lKey3.SetDefaultValue(aCommand);

			lKey3.Close();
		}

		void Extension_Unregister(const char * aExt, const char * aClassId)
		{
			assert(NULL != aExt		);
			assert(NULL != aClassId	);

			RegistryKey	lKey0;

			lKey0.Open(HKEY_LOCAL_MACHINE, SOFTWARE_CLASSES);

			// TESTED	KmsLib_Text		Windows - Windows - Setup B

			// Maybe, this is not registered. / Ce n'est peut-etre pas
			// enregiste.
			if (lKey0.DoesSubKeyExist(aExt))
			{
				lKey0.DeleteSubKey(aExt);
			}

			// Maybe, this is not registered. / Ce n'est peut-etre pas
			// enregiste.
			if (lKey0.DoesSubKeyExist(aClassId))
			{
				lKey0.DeleteSubKey(aClassId);
			}
		}

		void Extension_Unregister(const char * aExt, const char * aClassId, const char * aCommandName)
		{
			assert(NULL != aExt);
			assert(NULL != aClassId);
			assert(NULL != aCommandName);

			RegistryKey	lKey0;

			lKey0.Open(HKEY_LOCAL_MACHINE, SOFTWARE_CLASSES);

			// TESTED	KmsLib_Text		Windows - Windows - Setup B

			// Maybe, this is not registered. / Ce n'est peut-etre pas
			// enregiste.
			if (lKey0.DoesSubKeyExist(aClassId))
			{
				RegistryKey	lKey1;

				lKey1.Open(lKey0, aClassId);

				lKey0.Close();

				lKey0.Open(lKey1, SHELL);

				lKey1.Close();

				// Maybe, this is not registered. / Ce n'est peut-etre pas
				// enregiste.
				if (lKey0.DoesSubKeyExist(aCommandName))
				{
					lKey0.DeleteSubKey(aCommandName);
				}
			}

			lKey0.Close();
		}

		void ShellDirectory_Register(const char * aSubKey, const char * aCaption, const char * aCommand)
		{
			assert(NULL != aSubKey	);
			assert(NULL != aCaption	);
			assert(NULL != aCommand	);

			RegistryKey lKey0;

			lKey0.Open(HKEY_CLASSES_ROOT, DIRECTORY_SHELL);

			// TESTED	KmsLib_Text		Windows - Windows - Setup B

			RegistryKey lKey1;

			lKey1.Create(lKey0, aSubKey);

			lKey0.Close();

			lKey1.SetDefaultValue(aCaption);

			RegistryKey lKey2;

			lKey2.Create(lKey1, COMMAND);

			lKey1.Close();

			lKey2.SetDefaultValue(aCommand);

			lKey2.Close();
		}

		void ShellDirectory_Unregister(const char * aSubKey)
		{
			assert(NULL != aSubKey);

			RegistryKey lKey0;

			lKey0.Open(HKEY_CLASSES_ROOT, DIRECTORY_SHELL);

			// TESTED	KmsLib_Text		Windows - Windows - Setup B

			// Maybe, this is not registered. / Ce n'est peut-etre pas
			// enregiste.
			if (lKey0.DoesSubKeyExist(aSubKey))
			{
				lKey0.DeleteSubKey(aSubKey);
			}

			lKey0.Close();
		}

	}
}
