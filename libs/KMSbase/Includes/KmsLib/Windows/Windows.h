
// Product / Produit	KmsBase

/// \author	KMS	-	Martin Dubois, ing.
/// \file	Includes/KmsLib/Windows/Windows.h

#pragma once

namespace KmsLib
{

	namespace Windows
	{

		// Constantes
		/////////////////////////////////////////////////////////////////////

		/// \cond	en
		/// \brief	The name of "edit" command
		/// \endcond
		/// \cond	fr
		/// \brief	Le nom de la command "edit"
		/// \endcond
		extern const char * COMMAND_EDIT;

		/// \cond	en
		/// \brief	The name of "open" command
		/// \endcond
		/// \cond	fr
		/// \brief	Le nom de la command "open"
		/// \endcond
		extern const char * COMMAND_OPEN;

		// Fonctions
		/////////////////////////////////////////////////////////////////////

		/// \cond	en
		/// \brief	Register a file extension
		/// \param	aExt			[---;R--]	The file extension to
		///										register
		/// \param	aClassId		[---;R--]	The name of the class used
		///										for the registration
		/// \param	aCommandName	[---;R--]	See COMMAND_...
		/// \param	aCommand		[---;R--]	The command to execute
		/// \endcond
		/// \cond	fr
		/// \brief	Enregistrer une extension de fichier
		/// \param	aExt			[---;R--]	L'extension de fichier a
		///										enregistrer
		/// \param	aClassId		[---;R--]	Le nom de classe a utiliser
		///										pour l'enregistrement
		/// \param	aCommandName	[---;R--]	Voir COMMAND_...
		/// \param	aCommand		[---;R--]	La commande a executer
		/// \endcond
		/// \exception	Exception	CODE_REGISTRY_ERROR
		extern void Extension_Register(const char * aExt, const char * aClassId, const char * aCommandName, const char * aCommand );

		/// \cond	en
		/// \brief	Unregister a file extension
		/// \param	aExt			[---;R--]	The file extension to
		///										register
		/// \param	aClassId		[---;R--]	The name of the class used
		///										for	the	registration
		/// \endcond
		/// \cond	fr
		/// \brief	Desenregistrer une extension de fichier
		/// \param	aExt			[---;R--]	L'extension de fichier a
		///										enregistrer
		/// \param	aClassId		[---;R--]	Le nom de classe a utiliser
		///										pour l'enregistrement
		/// \endcond
		/// \exception	Exception	CODE_REGISTRY_ERROR
		extern void Extension_Unregister(const char * aExt, const char * aClassId);

		/// \cond	en
		/// \brief	Unregister a file extension
		/// \param	aExt			[---;R--]	The file extension to
		///										register
		/// \param	aClassId		[---;R--]	The name of the class used
		///										for the	registration
		/// \param	aCommandName	[---;R--]	See COMMAND_...
		/// \endcond
		/// \cond	fr
		/// \brief	Desenregistrer une extension de fichier
		/// \param	aExt			[---;R--]	L'extension de fichier a
		///										enregistrer
		/// \param	aClassId		[---;R--]	Le nom de classe a utiliser
		///										pour l'enregistrement
		/// \param	aCommandName	[---;R--]	Voir COMMAND_...
		/// \endcond
		/// \exception	Exception	CODE_REGISTRY_ERROR
		extern void Extension_Unregister(const char * aExt,	const char * aClassId, const char * aCommandName );

		/// \cond en
		/// \brief	Register a command to run from the windows explorer at
		///			directory level
		/// \endcond
		/// \cond fr
		/// \brief	Enregistrer une commande a execute a partir de
		///			l'explorateur windows au niveau d'un repertoire
		/// \endcond
		/// \param	aSubKey		[---;R--]
		/// \param	aCaption	[---;R--]
		/// \param	aCommand	[---;R--]
		/// \exception	Exception	CODE_REGISTRY_ERROR
		extern void ShellDirectory_Register(const char * aSubKey, const char * aCaption, const char * aCommand);

		/// \cond en
		/// \brief	Unregister a command to run from the windows explorer at
		///			directory level
		/// \endcond
		/// \cond fr
		/// \brief	Desenregistrer une commande a execute a partir de
		///			l'explorateur windows au niveau d'un repertoire
		/// \endcond
		/// \param	aSubKey		[---;R--]
		/// \exception	Exception	CODE_REGISTRY_ERROR
		extern void ShellDirectory_Unregister(const char * aSubKey);

	}
}
