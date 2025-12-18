
// Product / Produit	:	KmsBase

/// \author	KMS -		Martin Dubois
/// \file	Includes/KmsLib/Windows/RegistryKey.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Windows =============================================================
#include <Windows.h>

#include <SetupAPI.h>

namespace KmsLib
{
	namespace Windows
	{

		// Class / Classe
		/////////////////////////////////////////////////////////////////////

		/// \cond	en
		/// \brief	Wrap a HKEY
		/// \endcond
		/// \cond	fr
		/// \brief	Encapsule un HKEY
		/// \endcond
		class RegistryKey
		{

		public:

			enum
			{
				OPEN_FLAG_ADMINISTRATOR	= 0x00000001,
				OPEN_FLAG_DRIVER_KEY	= 0x00000002,
			};

			/// \cond	en
			///	\brief	Constructor
			/// \endcond
			/// \cond	fr
			/// \brief	Constructeur
			/// \endcond
			RegistryKey();

			/// \cond	en
			///	\brief	Destructor
			/// \endcond
			/// \cond	fr
			/// \brief	Destructeur
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			~RegistryKey();

			/// \cond	en
			///	\brief	Cast operator
			/// \endcond
			/// \cond	fr
			/// \brief	Operateur de conversion
			/// \endcond
			operator HKEY ();

			/// \cond	en
			///	\brief	Does the sub key exist?
			/// \param	aSubKey	[in]	Name of the sub key
			/// \retval	false	The sub key does not exist.
			/// \retval	true	The sub key exist.
			/// \endcond
			/// \cond	fr
			/// \brief	La sous cle existe-t-elle?
			/// \param	aSubKey	[in]	Le nom de la sous cle
			/// \retval	false	La sous cle n'existe pas.
			/// \retval true	La sous cle existe.
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			bool	DoesSubKeyExist(const char * aSubKey);

			/// \cond	en
			///	\brief	Read a string value
			/// \param	aName	[in ]	Value name
			/// \param	aOut	[out]	Output buffer
			/// \param	aOutSize_byte	Output buffer size
			/// \endcond
			/// \cond	fr
			/// \brief	Lire une valeur texte
			/// \param	aName	[in ]	Nom de la valeur
			/// \param	aOut	[out]	L'espace memoire de sortie
			/// \param	aOutSize_byte	La taille de l'espace memoire de sortie
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void	GetValue(const char * aName, char * aOut, unsigned int aOutSize_byte);

			/// \cond	en
			///	\brief	Read a DWORD value
			/// \param	aName	[in]	Value name
			/// \param	aDefaultValue	Default value to use if the registry
			///							key does not contain the value.
			/// \return	This method returns the read value.
			/// \endcond
			/// \cond	fr
			/// \brief	Lire une valeur DWORD
			/// \param	aName	[in]	Nom de la valeur
			/// \param	aDefaultValue	La valeur par defaut a utiliser si la
			///							cle ne contient pas la valeur.
			/// \return	Cette methode retourne la valeur lue.
			/// \endcond
			DWORD	GetValue_DWORD(const char * aName, DWORD aDefaultValue);

			/// \cond	en
			///	\brief	Set the default value of a key
			/// \param	aValue	[in]	The value
			/// \endcond
			/// \cond	fr
			/// \brief	Ecrire la valeur par defaut d'une cle
			/// \param	aValue	[in]	La valeur
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void SetDefaultValue(const char						  * aValue);

			/// \cond	en
			///	\brief	Set a DWORD value
			/// \param	aName	[in]	Value name
			/// \param	aValue			The value
			/// \endcond
			/// \cond	fr
			/// \brief	Affecte une valeur de type DWORD
			/// \param	aName	[in]	Nom de la valeur
			/// \param	aValue			La valeur
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void SetValue(const char * aName, DWORD			aValue);

			/// \cond	en
			///	\brief	Set a string value
			/// \param	aName	[in]	Value name
			/// \param	aValue	[in]	The value
			/// \endcond
			/// \cond	fr
			/// \brief	Affacte une valeur de type chaine de caracteres
			/// \param	aName	[in]	Le nom de la valeur
			/// \param	aValue	[in]	La valeur
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void SetValue(const char * aName, const char   * aValue);

			/// \cond	en
			///	\brief	Set a value
			/// \param	aName	[in]	Value name
			/// \param	aType			See REG_...
			/// \param	aValue	[in]	The value
			/// \param	aValueSize_byte	The size of the value
			/// \endcond
			/// \cond	fr
			/// \brief	Affecte une valeur
			/// \param	aName	[in]	Le nom de la valeur
			/// \param	aType			Voir REG_...
			/// \param	aValue	[in]	La valeur
			/// \param	aValueSize_byte	La taille de la valeur
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void SetValue(const char * aName, DWORD aType, const void * aValue, unsigned int aValueSize_byte);

			/// \cond	en
			///	\brief	See RegCloseKey
			/// \endcond
			/// \cond	fr
			/// \brief	Voir RegCloseKey
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void Close();

			/// \cond	en
			///	\brief	See RegCreateKey
			/// \param	aKey			The parent key
			/// \param	aSubKey	[in]	The name of the sub key to create
			/// \endcond
			/// \cond	fr
			/// \brief	Voir RegCreateKey
			/// \param	aKey			La cle parent
			/// \param	aSubKey	[in]	Le nom de la sous cle a creer
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void Create(HKEY aKey, const char * aSubKey);

			/// \cond	en
			///	\brief	See RegDeleteKey
			/// \param	aSubKey	[in]	The name of the sub key to delete
			/// \endcond
			/// \cond	fr
			/// \brief	Voir RegDeleteKey
			/// \param	aSubKey	[in]	Le nom de la sous cle a detruire
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void DeleteSubKey(const char * aSubKey);

			/// \cond	en
			///	\brief	See RegDeleteValue
			/// \param	aName	The name of the value to delete
			/// \endcond
			/// \cond	fr
			/// \brief	Voir RegDeleteValue
			/// \param	aName	Le nom de la valeur a detruire
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void DeleteValue(const char * aName);

			/// \cond	en
			///	\brief	See RegOpenKey
			/// \param	aKey			The parent key
			/// \param	aSubKey	[in]	The name of the sub key to open
			/// \endcond
			/// \cond	fr
			/// \brief	Voir RegOpenKey
			/// \param	aKey			La cle parent
			/// \param	aSubKey	[in]	Le nom de la cle a ouvrir
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void Open(HKEY aKey, const char * aSubKey);

			/// \cond	en
			///	\brief	See RegOpenKey
			/// \param	aKey			The parent key
			/// \param	aSubKeyIndex	The index of the sub key to open
			/// \retval	false	The index is not valid
			/// \endcond
			/// \cond	fr
			/// \brief	Voir RegOpenKey
			/// \param	aKey			La cle parent
			/// \param	aSubKeyIndex	L'indice de la sous cle a ouvrir
			/// \retval	false	L'indice est invalid
			/// \endcond
			/// \retval	true	OK
			/// \exception	Exception	CODE_REGISTRY_ERROR
			bool Open(HKEY aKey, unsigned int aSubKeyIndex);

			/// \cond	en
			///	\brief	Open the registry key associated to a device
			/// \param	aDevInfoSet		Information set containing
			///							information	about the device
			/// \param	aDevInfoData	Information about the device.
			/// \param	aFlags			See OPEN_FLAG_...
			/// \endcond
			/// \cond	fr
			/// \brief	Ouvrir la cle de registre associee a un peripherique
			/// \param	aDevInfoSet		L'ensemble d'information contenant
			///							les informations au sujet du
			///							peripherique
			/// \param	aDevInfoData	Les informations au sujet du
			///							peripherique
			/// \param	aFlags			Voir OPEN_FLAG_...
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void Open(HDEVINFO aDevInfoSet, PSP_DEVINFO_DATA aDevInfoData, unsigned int aFlags);

		private:

			RegistryKey(const RegistryKey &);

			const RegistryKey & operator = (const RegistryKey &);

			HKEY	mKey;

		};

	}
}
