
// Product / Produit	KmsBase

/// \author	KMS - Martin Dubois
/// \file	Includes/KmsLib/IgnoreList.h

#pragma once

// Inclusion
/////////////////////////////////////////////////////////////////////////////

// ===== C++ ================================================================
#include <list>
#include <string>

namespace KmsLib
{

	// Classe
	/////////////////////////////////////////////////////////////////////////

	/// \cond	en
	/// \brief	The IgnoreList class
	/// \endcond
	/// \cond	fr
	/// \brief	La classe IgnoreList
	/// \endcond
	class IgnoreList
	{

	public:

		/// \cond	en
		/// \brief	Constructor
		/// \param	aParent	[-K-;R--]	The parent list
		/// \param	aPrefix	[---;R--]	The relative folder name
		/// \endcond
		/// \cond	fr
		/// \brief	Constructeur
		/// \param	aParent	[-K-;R--]	La liste parent
		/// \param	aPrefix	[---;R--]	Le nom relatif du repetoire
		/// \endcond
		IgnoreList(IgnoreList * aParent, const char * aPrefix);

		/// \cond	en
		/// \brief	Parent list accessor
		/// \return	The parent list address
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur de la liste parent
		/// \retval	L'adresse de la liste parent
		/// \endcond
		IgnoreList * GetParent();

		/// \cond	en
		/// \brief	Is the file ignored?
		/// \param	aFileName	[---;R--]	The file name
		/// \retval	false	The file is not ignored.
		/// \retval	true	The file is ignored.
		/// \endcond
		/// \cond	fr
		/// \brief	Est-ce que le fichier est ignore?
		/// \param	aFileName	[---;R--]	Le nom du fichier
		/// \retval	false	Le fichier n'est pas ignore.
		/// \retval	true	Le fichier est ignore.
		/// \endcond
		bool IsFileIgnored(const char * aFileName) const;

		/// \cond	en
		/// \brief	Is the folder ignored?
		/// \param	aFolderName	[---;R--]	The folder name
		/// \retval	false	The folder is not ignored.
		/// \retval	true	The folder is ignored.
		/// \endcond
		/// \cond	fr
		/// \brief	Est-ce que le repertoire est ignore?
		/// \param	aFolderName	[---;R--]	Le nom du repertoire
		/// \retval	false	Le repertoire n'est pas ignore.
		/// \retval	true	Le repertoire est ignore.
		/// \endcond
		bool IsFolderIgnored(const char * aFolderName) const;

		/// \cond	en
		/// \brief	Read the ignore list from a file
		/// \param	aFolderName	[---;R--]	The absolute folder name
		/// \param	aFileName	[---;R--]	The list file name
		/// \endcond
		/// \cond	fr
		/// \brief	Lire la liste d'un fichier
		/// \param	aFolderName	[---;R--]	Le nom absolue du repetoire
		/// \param	aFileName	[---;R--]	Le nom du fichier
		/// \endcond
		/// \exception	KmsLib::Exception	CODE_IO_ERROR
		void ReadFromFile(const char * aFolderName, const char * aFileName);

	private:

		typedef struct
		{
			struct
			{
				unsigned	mAbsolute	: 1;
				unsigned	mFolder		: 1;
				unsigned	mPattern	: 1;

				unsigned	mReserved : 29;
			}
			mFlags;

			std::string  mString;
		}
		Entry;

		bool Match(const Entry & aEntry, const char * aFileName) const;

		bool MatchNameOrPattern(const Entry & aEntry, const char * aFileName) const;

		typedef std::list< Entry > EntryList;

		EntryList		mList	;
		IgnoreList	  * mParent	;
		std::string		mPrefix	;

	};

}
