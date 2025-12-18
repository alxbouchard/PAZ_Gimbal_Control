
// Product  KmsBase

/// \author	KMS	- Martin Dubois, ing.
/// \file	Includes/KmsLib/Windows/ComPortHandle.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Includes ===========================================================
#include <KmsLib/FileHandle.h>

namespace KmsLib
{
	namespace Windows
	{

		// Classe
		/////////////////////////////////////////////////////////////////////

		/// \cond	en
		/// \brief	This class wrap a HANDLE to a COM port
		/// \endcond
		/// \cond	fr
		/// \brief	Cette classe encapsule un HANDLE vers un port COM
		/// \endcond
		class ComPortHandle : public FileHandle
		{

		public:

			typedef enum
			{
				BAUD_RATE_1200		= 1200		,
				BAUD_RATE_2400		= 2400		,
				BAUD_RATE_4800		= 4800		,
				BAUD_RATE_9600		= 9600		,
				BAUD_RATE_19200		= 19200		,
				BAUD_RATE_38400		= 38400		,
				BAUD_RATE_115200	= 115200	,

				BAUD_RATE_DEFAULT	= 115200	,
				BAUD_RATE_MAX		= 115200
			}
			BaudRate;

			/// \cond	en
			/// \brief	Constructor
			/// \endcond
			/// \cond	fr
			/// \brief	Constructeur
			/// \endcond
			ComPortHandle();

			/// \cond	en
			/// \brief	Destructor
			/// \endcond
			/// \cond	fr
			/// \brief	Destructeur
			/// \endcond
			virtual ~ComPortHandle();

			/// \cond	en
			/// \brief	Baud rate accessor
			/// \return	This method returns the baud rate
			/// \endcond
			/// \cond	fr
			/// \brief	Accesseur pour la vitesse de transmission
			/// \return	Cette methode retourne la vitesse de transmission
			/// \endcond
			BaudRate	GetBaudRate() const;

			/// \cond	en
			/// \brief	Baud rate accessor
			/// \param	aBaudRate	The baud rate
			/// \endcond
			/// \cond	fr
			/// \brief	Accesseur pour la vitesse de transmission
			/// \param	aBaudRate	La nouvelle vitesse de transmission
			/// \endcond
			/// \exception	Exception	CODE_COMMUNICATION_ERROR
			void			SetBaudRate(BaudRate aBaudRate);

			/// \cond	en
			/// \brief	Connect to the first available COM port by number.
			/// \param	aDesiredAccess	See GENERIC_...
			/// \endcond
			/// \cond	fr
			/// \brief	Connecter au premier port COM disponible.
			/// \param	aDesiredAccess	Voir GENERIC_...
			/// \endcond
			/// \exception	Exception	CODE_IO_ERROR
			void Connect(DWORD aDesiredAccess);

			/// \cond	en
			/// \brief	Connect to a COM port by name.
			/// \param	aName			The port name
			/// \param	aDesiredAccess	See GENERIC_...
			/// \endcond
			/// \cond	fr
			/// \brief	Connecter a un port COM par son nom.
			/// \param	aName			Le nom du port
			/// \param	aDesiredAccess	Voir GENERIC_...
			/// \endcond
			/// \exception	Exception	CODE_IO_ERROR
			void Connect(const char * aName, DWORD aDesiredAccess);

			/// \cond	en
			/// \brief	Connect to a COM port by number.
			/// \param	aNumber			The port number
			/// \param	aDesiredAccess	See GENERIC_...
			/// \endcond
			/// \cond	fr
			/// \brief	Connecter a un port COM par son numero.
			/// \param	aNumber			Le numero de port
			/// \param	aDesiredAccess	Voir GENERIC_...
			/// \endcond
			/// \exception	Exception	CODE_IO_ERROR
			void Connect_ByNumber(unsigned int aNumber, DWORD aDesiredAccess);

		private:

			void SetBaudRate_Internal();
			
			BaudRate	mBaudRate;

		};

	}

}
