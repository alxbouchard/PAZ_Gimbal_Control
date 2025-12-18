
// Product / Produit	KmsBase

/// \author	KMS - Martin Dubois
/// \file	Includes/KmsLib/Walker.h

#pragma once

// Inclusions
/////////////////////////////////////////////////////////////////////////////

// ===== STL ================================================================
#include <list>
#include <string>

// ===== KmsBase ============================================================
#include <KmsLib/Walker.h>

namespace KmsLib
{

	class IgnoreList;

	// Classe
	/////////////////////////////////////////////////////////////////////////

	/// \cond	en
	/// \brief	The Walker_Smart class
	/// \endcond
	/// \cond	fr
	/// \brief	La classe Walker_Smart
	/// \endcond
	class Walker_Smart : public Walker
	{

	public:

		/// \cond	en
		/// \brief	Constructor
		/// \endcond
		/// \cond	fr
		/// \brief	Constructeur
		/// \endcond
		Walker_Smart();

		/// \cond	en
		/// \brief	Add a name of a file containing a list of file to ignore.
		/// \param	aFileName	[---;R--]	The file name
		/// \endcond
		/// \cond	fr
		/// \brief	Ajouter un nom de fichier contenant une liste de fichier
		///			a ignorer
		/// \param	aFileName	[---;R--]	Le nom de fichier
		/// \endcond
		void AddIgnoreFileName(const char * aFileName);

		/// \cond	en
		/// \brief	Called for every file
		/// \param	aFile	[---;R--]	The file name
		/// \param	aLastWrite			The last write date
		/// \endcond
		/// \cond	fr
		/// \brief	Appeler pour chacun des fichier
		/// \param	aFile	[---;R--]	Le nom du fichier
		/// \param	aLastWrite			La date de derniere ecriture
		/// \endcond
		virtual void OnFile_Smart(const char * aFile, const FILETIME & aLastWrite) = 0;

		/// \cond	en
		/// \brief	Called for every folder
		/// \param	aFolder	[---;R--]	The folder name
		/// \endcond
		/// \cond	fr
		/// \brief	Appeler pour chacun des repertoire
		/// \param	aFolder	[---;R--]	Le nom du repertoire
		/// \endcond
		virtual void OnFolder_Smart(const char * aFolder);

		// ===== Walker =========================================================

		virtual void OnFile(const char * aFile, const FILETIME & aLastWrite);
		virtual void OnFolder(const char * aFolder);

	private:

		Walker_Smart(const Walker_Smart &);

		const Walker_Smart & operator = (const Walker_Smart &);

		std::list< std::string > mIgnoreFiles;

		IgnoreList * mIgnoreList;

	};

}
