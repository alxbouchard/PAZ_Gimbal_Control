
// Author / Auteur		:	KMS	-	Martin Dubois, ing.
// Product / Produit	:	KmsBase
// File / Fichier		:	KmsLib/Windows/ComPortHandle.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/Windows/RegistryKey.h>

#include <KmsLib/Windows/ComPortHandle.h>

namespace KmsLib
{

	namespace Windows
	{

		// Public
		/////////////////////////////////////////////////////////////////////

		ComPortHandle::ComPortHandle() : mBaudRate(BAUD_RATE_DEFAULT)
		{
		}

		ComPortHandle::~ComPortHandle()
		{
		}

		// TODO  Normal  Ajouter une methode pour valider le baud rate;

		// TODO  Normal  Ajouter des methode pour lire et changer le timeout;

		// TODO  Normal  Ajouter des methode pour le flow control

		ComPortHandle::BaudRate ComPortHandle::GetBaudRate() const
		{
			return mBaudRate;
		}

		void ComPortHandle::SetBaudRate(BaudRate aBaudRate)
		{
			assert(BAUD_RATE_MAX >= aBaudRate);

			assert(BAUD_RATE_MAX >= mBaudRate);

			// TODO  Normal  Valider le baud rate

			mBaudRate = aBaudRate;

			if (IsOpen())
			{
				SetBaudRate_Internal();
			}
		}

		void ComPortHandle::Connect(DWORD aDesiredAccess)
		{
			assert(0 != aDesiredAccess);

			for (unsigned int i = 1; i < 20; i++)
			{
				try
				{
					Connect_ByNumber(i, aDesiredAccess);

					return;
				}
				catch (Exception * eE)
				{
					if (Exception::CODE_COM_PORT_ERROR != eE->GetCode())
					{
						throw;
					}
				}
			}

			throw Exception(Exception::CODE_NO_COM_PORT, "No available COM port", NULL, __FILE__, __FUNCTION__, __LINE__, 0);
		}

		void ComPortHandle::Connect(const char * aName, DWORD aDesiredAccess)
		{
			assert(NULL	!= aName			);
			assert(0	!= aDesiredAccess	);

			FileHandle::Create(aName, aDesiredAccess, 0, OPEN_EXISTING, 0);

			SetBaudRate_Internal();
		}

		void ComPortHandle::Connect_ByNumber(unsigned int aNumber, DWORD aDesiredAccess)
		{
			assert(0 <	aNumber			);
			assert(0 !=	aDesiredAccess	);

			char lName[16];

			sprintf_s(lName, "COM%u", aNumber);

			Connect(lName, aDesiredAccess);
		}

		// Private
		/////////////////////////////////////////////////////////////////////

		void ComPortHandle::SetBaudRate_Internal()
		{
			assert(INVALID_HANDLE_VALUE != mHandle);

			COMMCONFIG	lCC;
			DWORD		lSize_byte = sizeof(lCC);

			if (!GetCommConfig(mHandle, &lCC, &lSize_byte))
			{
				throw new Exception(Exception::CODE_COM_PORT_ERROR, "GetCommConfig failed", NULL, __FILE__, __FUNCTION__, __LINE__, lSize_byte);
			}

			lCC.dcb.BaudRate = mBaudRate;

			if (!SetCommConfig(mHandle, &lCC, lSize_byte))
			{
				// NOT TESTED   KmsLib.ComPortHandle.ErrorHandling
                //              SetCommConfig fail<br>
                //              Not easy to test because the error popup at
				//				the beginning of the method.
				throw new Exception(Exception::CODE_COM_PORT_ERROR, "SetCommConfig failed", NULL, __FILE__, __FUNCTION__, __LINE__, lSize_byte);
			}
		}

	}

}
