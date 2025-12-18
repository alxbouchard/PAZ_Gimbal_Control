
// Product 	KmsBase

/// \author  KMS - Martin Dubois, ing.
/// \file    Includes/KmsLib/DriverHandle.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Interface ==========================================================
#include <KmsLib/FileHandle.h>

#ifdef _KMS_WINDOWS_
    #include <KmsLib/Windows/RegistryKey.h>
#endif

namespace KmsLib
{

	// Class
	/////////////////////////////////////////////////////////////////////////

	/// \cond	en
	/// \brief	This class wrap a HANDLE to a driver
	/// \endcond
	/// \cond	fr
	/// \brief	Cette classe encapsule un HANDLE vers un pilote
	/// \endcond
	class DriverHandle : public FileHandle
	{

	public:

		/// \cond	en
		/// \brief	Connection flags
		/// \endcond
		/// \cond	fr
		/// \brief	Drapaux de connexion
		/// \endcond
		enum
		{
			CONNECT_FLAG_ADMINISTRATOR		= 0x00000001,
			CONNECT_FLAG_OPEN_DEVICE_KEY	= 0x00000002,
            CONNECT_FLAG_OVERLAPPED         = 0x00000004,
		};

		/// \cond	en
		/// \brief	Constructor
		/// \endcond
		/// \brief	Constructeur
		/// \cond	fr
		/// \endcond
		DriverHandle();

		/// \cond	en
		/// \brief	Destructor
		/// \endcond
		/// \cond	fr
		/// \brief	Destructeur
		/// \endcond
		virtual ~DriverHandle();

		/// \cond	en
		/// \brief	Cancel all request sent to the driver using this
		///			instance.
		/// \endcond
		/// \cond	fr
		/// \brief	Annuler toutes les requetes envoye a partir de cet
		///			instance
		/// \endcond
		/// \exception	Exception	CODE_IO_ERROR
		void	CancelAll();

        /// \cond	en
        ///	\brief	Connect to the driver using a symbolic link.
        /// \param	aLink	             The symbolic link
        /// \param	aDesiredAccess	     See GENERIC_... or O_...
        /// \param  aFlagsAndAttributes  See FILE_ATTRIBUTE_... and
        ///                              FILE_FLAG_...
        /// \endcond
        /// \cond	fr
        /// \brief	Connecter au pilote en utilisant un lien symbolique
        /// \param	aLink	          	 Le lien symbolique
        /// \param	aDesiredAccess	     Voir GENERIC_... ou O_...
        /// \param  aFlagsAndAttributes  Voir FILE_ATTRIBUTES_... and
        ///                              FILE_FLAG_...
        /// \endcond
        /// \exception	Exception	CODE_IO_ERROR
        void	Connect(const char * aLink, unsigned int aDesiredAccess, unsigned int aFlagsAndAttributes);

		/// \cond	en
		/// \brief	See DeviceIoControl
		/// \param	aCode
		/// \param	aIn		[in]	Input data
		/// \param	aInSize_byte	Size of input data
		/// \param	aOut	[out]	Output buffer
		/// \param	aOutSize_byte	Size of the output data
		/// \return	This method return the size of data the driver
		///			returned. 
		/// \endcond
		/// \cond	fr
		/// \brief	Voir DeviceIoControl
		/// \param	aCode
		/// \param	aIn		[in]	Donnees d'entrees
		/// \param	aInSize_byte	Taille des donnees d'entrees
		/// \param	aOut	[out]	Tampon de sortie
		/// \param	aOutSize_byte	Taille du tampon de sortie
		/// \return	Cette methode retourne la taille des donnees
		///			retournees par le pilote. 
		/// \endcond
		/// \exception	Exception	CODE_IOCTL_ERROR
		unsigned int	Control(unsigned int aCode, const void * aIn, unsigned int aInSize_byte, void * aOut, unsigned int aOutSize_byte);

        #ifdef _KMS_WINDOWS_

            /// \cond	en
            ///	\brief	Connect to the driver using an interface.
            /// \param	aInterface		The interface's GUID
            /// \param  aIndex          The device index
            /// \param	aDesiredAccess	See GENERIC_...
            /// \param	aFlags			See CONNECT_FLAG_...
            /// \endcond
            /// \cond	fr
            /// \brief	Connecter au pilote en utilisant une interface
            /// \param	aInterface		Le GUID de l'interface
            /// \param  aIndex          L'index du peripherique
            /// \param	aDesiredAccess	Voir GENERIC_...
            /// \param	aFlags			Void CONNECT_FLAG_...
            /// \endcond
            /// \exception	Exception	CODE_REGISTRY_ERROR, CODE_SETUP_API_ERROR
            void Connect(const GUID & aInterface, unsigned int aIndex, DWORD aDesiredAccess, unsigned int aFlags);

            /// \cond	en
            /// \brief	See CONNECT_FLAG_OPEN_DEVICE_KEY
            /// \endcond
            /// \cond	fr
            /// \brief	Voir CONNECT_FLAG_OPEN_DEVICE_KEY
            /// \endcond
            Windows::RegistryKey  mDeviceKey;

        #endif

	private:

		DriverHandle(const DriverHandle &);

		const DriverHandle & operator = (const DriverHandle &);

	};

}
