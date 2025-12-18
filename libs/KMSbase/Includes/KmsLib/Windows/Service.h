
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/KmsLib/Windows/Service.h

#pragma once

namespace KmsLib
{
	namespace Windows
	{

		// Class / Classe
		/////////////////////////////////////////////////////////////////////

		/// \cond	en
		/// \brief	Control a Windows' service
		/// \endcond
		/// \cond	fr
		/// \brief	Controle un service windows
		/// \endcond
		class Service
		{

		public:

			/// \cond	en
			///	\brief	Constructor
			/// \endcond
			/// \cond	fr
			/// \brief	Constructeur
			/// \endcond
			Service(const char * aServiceName);

			/// \cond	en
			///	\brief	Destructor
			/// \endcond
			/// \cond	fr
			/// \brief	Destructeur
			/// \endcond
			~Service();

			/// \cond	en
			///	\brief	Is the service enabled?
			/// \retval	false	The service is disabled.
			/// \retval	true	The service is enabled.
			/// \endcond
			/// \cond	fr
			/// \brief	Le service est-il actif?
			/// \retval	false	Le service est innactif
			/// \retval	true	Le service est actif
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			bool	IsEnabled();

			/// \cond	en
			///	\brief	Is the service installed?
			/// \retval	false	The service is not installed.
			/// \retval	true	The service is installed.
			/// \endcond
			/// \cond	fr
			/// \brief	Le service est-il installe?
			/// \retval	false	Le service n'est pas installe.
			/// \retval	true	Le service est installe.
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			bool	IsInstalled();

			/// \cond	en
			///	\brief	Is the service running?
			/// \retval	false	The service is stopped.
			/// \retval	true	The service is running.
			/// \endcond
			/// \cond	fr
			/// \brief	Le service est-il en cours d'execution?
			/// \retval	false	Le service est arrete.
			/// \retval	true	Le service est en execution.
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			bool	IsRunning();

			/// \cond	en
			///	\brief	Retrieve the "DelayedAutoStart" setting
			/// \retval	false	The service start as soon as possible.
			/// \retval	true	The service start is delayed.
			/// \endcond
			/// \cond	fr
			/// \brief	Optient la valeur de "DelayAutoStart"
			/// \retval	false	Le service demarre aussi tot que possible.
			/// \retval	true	Le service demarre apres un certain delai.
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			bool			GetDelayedAutoStart();

			/// \cond	en
			///	\brief	Retrieve the start type
			/// \return	See SERVICE_...
			/// \endcond
			/// \cond	fr
			/// \brief	Optient le type de demarrage
			/// \return	Voir SERVICE_...
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			unsigned int	GetStartType();

			/// \cond	en
			///	\brief	Set the "DelayedAutoStart" setting
			/// \param	aValue	See GetDelayedAutoStart
			/// \endcond
			/// \cond	fr
			/// \brief	Change la valeur de "DelayedAutoStart"
			/// \param	aValue	Voir GetDelayedAutoStart
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			void	SetDelayedAutoStart(bool			aValue);

			/// \cond	en
			///	\brief	Set the start type
			/// \param	aValue	See SERVICE_...
			/// \endcond
			/// \cond	fr
			/// \brief	Change le type de demarrage
			/// \param	aValue	Voir SERVICE_...
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			void	SetStartType(unsigned int	aValue);

			/// \cond	en
			///	\brief	Create the service in the database
			/// \param	aDisplayName	[in]	The display name
			/// \param	aBinary			[in]	The executable name
			/// \endcond
			/// \cond	fr
			/// \brief	Cree le service dans la base de donnees
			/// \param	aDisplayName	[in]	Le nom visible par l'utilisateur
			/// \param	aBinary			[in]	Le nom de l'executable
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			void	Create(const char * aDisplayName, const char * aBinary);

			/// \cond	en
			///	\brief	Remove the service from the database
			/// \endcond
			/// \cond	fr
			/// \brief	Retire le service de la base de donnees
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			void	Delete();

			/// \cond	en
			///	\brief	Start the service
			/// \endcond
			/// \cond	fr
			/// \brief	Lance l'execution du service
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			void	Start();

			/// \cond	en
			///	\brief	Diable the service
			/// \endcond
			/// \cond	fr
			/// \brief	Desactive le service
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			void	Disable();

			/// \cond	en
			///	\brief	Enable the service
			/// \endcond
			/// \cond	fr
			/// \brief	Active le service
			/// \endcond
			/// \exception	Exception	CODE_SERVICE_MANAGER_ERROR
			void	Enable();

		private:

			void	ConnectAndOpenIfNeeded	();
			void	ConnectIfNeeded			();

			void	Connect	();
			void	Control	(DWORD aControl);
			void	Open	();
			void	Stop	();
			void	Wait	(DWORD aPending, DWORD aFinal);

			void	UpdateConfig	();
			void	UpdateStatus	();

			QUERY_SERVICE_CONFIG  * mConfig				;
			unsigned int            mConfigSize_byte	;
			SC_HANDLE				mHandle				;
			SC_HANDLE				mManager			;
			const char			  * mName				;
			SERVICE_STATUS_PROCESS	mStatus				;

		};

	}
}
