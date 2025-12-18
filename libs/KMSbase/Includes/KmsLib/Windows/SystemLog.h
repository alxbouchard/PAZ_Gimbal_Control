
// Product / Produit	:	KmsBase

/// \author	Kms -	Martin Dubois, ing
/// \file	Includes/KmsLib/Windows/SystemLog.h

#pragma once

namespace KmsLib
{
	namespace Windows
	{

		// Class / Classe
		/////////////////////////////////////////////////////////////////////

		/// \cond	en
		/// \brief	Let add events in the system log
		/// \endcond
		/// \cond	fr
		/// \brief	Permet d'ajouter des evennements dans le log system
		/// \endcond
		class SystemLog
		{

		public:

			/// \cond	en
			///	\brief	Constructor
			/// \param	aLogName	[in,keep]	The windows log name
			/// \param	aSourceName	[in,keep]	The name of the source of
			///									messages.
			/// \endcond
			/// \cond	fr
			/// \brief	Constructeur
			/// \param	aLogName	[in,keep]	Le nom du journal windows
			/// \param	aSourceName	[in,keep]	Le nom de de la source des
			///									messages
			/// \endcond
			SystemLog(const char * aLogName, const char * aSourceName);

			/// \cond	en
			///	\brief	Destructor
			/// \endcond
			/// \cond	fr
			/// \brief	Destructeur
			/// \endcond
			/// \exception	Exception	CODE_SYSTEM_LOG_ERROR
			~SystemLog();

			/// \cond	en
			///	\brief	Is the system log configured?
			/// \retval	false	The windows log is not configured
			/// \retval	true	The windows log is configured
			/// \endcond
			/// \cond	fr
			/// \brief	Le journal system est-il configure?
			/// \retval	false	Le journal windows n'est pas configure?
			/// \retval true	Le journal windows est configure?
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			bool	IsConfigured()	const;

			/// \cond	en
			///	\brief	Configure the windows log
			/// \param	aMessageFile	[in]	The name of the executable
			///									file containing the messages.
			/// \endcond
			/// \cond	fr
			/// \brief	Configure le journal windows
			/// \param	aMessageFile	[in]	Le nom de l'executable
			///									contenant les messages.
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void	Configure(const char * aMessageFile);

			/// \cond	en
			///	\brief	Unconfigure the windows log
			/// \endcond
			/// \cond	fr
			/// \brief	deconfigure le journal windows
			/// \endcond
			/// \exception	Exception	CODE_REGISTRY_ERROR
			void	Unconfigure();

			/// \cond	en
			///	\brief	Add an event to the log
			/// \param	aType			The event type
			/// \param	aCategory		The event category
			/// \param	aEventId		The event identifier
			/// \param	aStringCount	The number of strings associated to
			///							this event
			/// \param	aDataSize_byte	The size of data associated to this
			///							event
			/// \param	aString	[in]	The strings
			/// \param	aData			The data
			/// \endcond
			/// \cond	fr
			/// \brief	Ajoute un evenement au journal
			/// \param	aType			Le type de l'evennement
			/// \param	aCategory		La category de l'evennement
			/// \param	aEventId		L'identificateur de l'evennement
			/// \param	aStringCount	Le nombre de chaines de caractere
			///							attachees a l'evennement
			/// \param	aDataSize_byte	La taille des donnees attachees a
			///							l'evennement.
			/// \param	aString	[in]	Les chaines de caracteres
			/// \param	aData			Les donnees
			/// \endcond
			/// \exception	Exception	CODE_SYSTEM_LOG_ERROR
			void	LogEvent(WORD aType, WORD aCategory, DWORD aEventId, WORD aStringCount, DWORD aDataSize_byte, LPCTSTR * aString, LPVOID aData);

		private:

			void	Deregister	();
			void	Register	();

			HANDLE			mHandle		;
			const char	  * mLogName	;
			const char    * mSourceName	;

		};
	}
}
