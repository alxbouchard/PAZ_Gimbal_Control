
// Product / Produit	KmsBase

/// \author	KMS - Martin Dubois
/// \file	Includes/KmsLib/Walker.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C++ ================================================================
#include <string>

namespace KmsLib
{

	// Class / Classe
	/////////////////////////////////////////////////////////////////////////////

	/// \cond	en
	/// \brief	The Walker class
	/// \endcond
	/// \cond	fr
	/// \brief	La classe Walker
	/// \endcond
	class Walker
	{

	public:

		/// \cond	en
		/// \brief	Walk a directory
		/// \param	aStart	[---;R--]	The start point
		/// \endcond
		/// \cond	fr
		/// \brief	Marche un repertoire
		/// \param	aStart	[---;R--]	Le point de depart
		/// \endcond
		void Walk(const char * aStart);

	protected:

		enum
		{
			PATH_LENGTH = 2048,
		};

		/// \cond	en
		/// \brief	Absolut path accessor
		/// \param	aOut	[---;-W-]	Output buffer
		/// \param	aOutLen				Output buffer's length
		/// \param	aRoot	[---;R--]	Root
		/// \param	aName	[---;R--]	File name
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour un chemin absolue
		/// \param	aOut	[---;-W-]	Espace memoire de sortie
		/// \param	aOutLen				Longueur de l'espace memoire de
		///								sortie
		/// \param	aRoot	[---;R--]	Racine
		/// \param	aName	[---;R--]	Nom du fichier
		/// \endcond
		static void GetAbsolute(char * aOut, unsigned int aOutLen, const char * aRoot, const char * aName);

		/// \cond	en
		/// \brief	Folder name accessor
		/// \param	aOut	[---;-W-]	Output buffer
		/// \param	aOutLen				Output buffer's length
		/// \param	aName	[---;R--]	File name
		/// \retval	false	No folder name returned
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le nom d'un repertoire
		/// \param	aOut	[---;-W-]	Espace memoire de sortie
		/// \param	aOutLen				Longueur de l'espace memoire de
		///								sortie
		/// \param	aName	[---;R--]	Nom du fichier
		/// \retval	false	Pas de nom de repetoire retourne
		/// \endcond
		/// \retval	true	OK
		/// \exception	KmsLib::Exception	CODE_INVALID_BUFFER_SIZE
		static bool GetFolder(char * aOut, unsigned int aOutLen, const char * aName);

		/// \cond	en
		/// \brief	Does the file exist?
		/// \param	aRoot	[---;R--]	Root
		/// \param	aName	[--O;R--]	File name or NULL to test the
		///								existence of the root folder.
		/// \retval	false	The file does not exist.
		/// \retval	true	The file esist.
		/// \endcond
		/// \cond	fr
		/// \brief	Est-ce que le fichier exist?
		/// \param	aRoot	[---;R--]	Racine
		/// \param	aName	[--O;R--]	Nom du fichier
		/// \retval	false	Le fichier n'existe pas.
		/// \retval	true	Le fichier existe.
		/// \endcond
		static bool Exist(const char * aRoot, const char * aName);

		/// \cond	en
		/// \brief	Called for every file
		/// \param	aFile	[---;R--]	The file name
		/// \param	aLastWrite			The last write date
		/// \endcond
		/// \cond	fr
		/// \brief	Appele pour chacun des fichier
		/// \param	aFile	[---;R--]	Le nom du fichier
		/// \param	aLastWrite			La date de derniere ecriture
		/// \endcond
		virtual void OnFile(const char * aFile, const FILETIME & aLastWrite) = 0;

		/// \cond	en
		/// \brief	Called for every folder
		/// \param	aFolder	[--O;R--]	The folder name or NULL for the root
		///								folder
		/// \endcond
		/// \cond	fr
		/// \brief	Appele pour chacun des repertoire
		/// \param	aFolder	[--O;R--]	Le nom du repertoire ou NULL pour le
		///								repertoire racine
		/// \endcond
		virtual void OnFolder(const char * aFolder);

		/// \cond	en
		/// \brief	Absolute file name accessor
		/// \param	aOut	[---;-W-]	Output buffer
		/// \param	aOutLen				Output buffer's length
		/// \param	aName	[--O;R--]	File name or NULL to get the root
		///								folder
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le nom de fichier absolu
		/// \param	aOut	[---;-W-]	Espace memoire de sortie
		/// \param	aOutLen				Longueur de l'espace memoire de
		///								sortie
		/// \param	aName	[--O;R--]	Nom du fichier ou NULL pour obtenir
		///								le repertoire racine
		/// \endcond
		void GetAbsolute(char * aOut, unsigned int aOutLen, const char * aName) const;

		/// \cond	en
		/// \brief	Create a folder
		/// \param	aToRoot	[---;R--]	Name of the folder where to create
		///								the new one
		/// \param	aFolder	[---;R--]	Name of the new folder
		/// \endcond
		/// \cond	fr
		/// \brief	Cree un repertoire
		/// \param	aToRoot	[---;R--]	Nom du repertoire ou creer le nouveau
		///								repertoire
		/// \param	aFolder	[---;R--]	Nom du nouveau repertoire
		/// \endcond
		void CreateFolder(const char * aToRoot, const char * aFolder);

		/// \cond	en
		/// \brief	Does the file exist?
		/// \param	aFolder	[---;R--]	Folder where to look
		/// \param	aFile	[---;R--]	File name
		/// \retval	false	The file does not exist.
		/// \retval	true	The file esist.
		/// \endcond
		/// \cond	fr
		/// \brief	Est-ce que le fichier exist?
		/// \param	aFolder	[---;R--]	Repertoire ou regarder
		/// \param	aFile	[---;R--]	Nom du fichier
		/// \retval	false	Le fichier n'existe pas.
		/// \retval	true	Le fichier existe.
		/// \endcond
		bool FileExist(const char * aFolder, const char * aFile) const;

	private:

		std::string mRoot;

	};

}
