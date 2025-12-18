
// Author / Auteur     KMS - Martin Dubois, ing.
// Product / Produit   KmsBase
// File / Fichier      KmsLib/IgnoreList.cpp

// Last test coverage update / Derniere mise a jour de la couverture de test
// 2017-11-10

// TODO  KmsLib.IgnoreList
//       Supporter le \ pour escape des #<br>
//       Supporter le \ pour escape des trailling space<br>
//       Supporter le \ pour escape des !<br>
//       Supporter le ! pour nier une exclusion<br>
//       Supporter le **/<br>
//       Supporter le /**<br>
//       Supporter le /**/<br>
//       Supporter les [] dans les pattern

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>
#include <stdlib.h>

// ===== Includes/KmsLib ====================================================
#include <KmsLib/Exception.h>
#include <KmsLib/String.h>
#include <KmsLib/TextFile.h>

#include <KmsLib/IgnoreList.h>

// Static functions declarations / Declaration des fonctions statiques
/////////////////////////////////////////////////////////////////////////////

static bool MatchName(const char * aName, const char * aToMatch);

static bool Trim(char * aLine);

namespace KmsLib
{

	// Public
	/////////////////////////////////////////////////////////////////////////

	IgnoreList::IgnoreList(IgnoreList * aParent, const char * aPrefix) : mParent(aParent)
	{
		if (NULL != aPrefix)
		{
			assert(NULL != aParent);

			mPrefix = aPrefix;
		}
	}

	IgnoreList * IgnoreList::GetParent()
	{
		return mParent;
	}

	bool IgnoreList::IsFileIgnored(const char * aFileName) const
	{
		assert(NULL != aFileName);

		for (EntryList::const_iterator lIt = mList.begin(); lIt != mList.end(); lIt++)
		{
			if ((!lIt->mFlags.mFolder) && Match((*lIt), aFileName))
			{
				return true;
			}
		}

		if (NULL != mParent)
		{
			return mParent->IsFileIgnored(aFileName);
		}

		return false;
	}

	bool IgnoreList::IsFolderIgnored(const char * aFolderName) const
	{
		assert(NULL != aFolderName);

		for (EntryList::const_iterator lIt = mList.begin(); lIt != mList.end(); lIt++)
		{
			if (lIt->mFlags.mFolder && Match((*lIt), aFolderName))
			{
				return true;
			}
		}

		if (NULL != mParent)
		{
			return mParent->IsFolderIgnored(aFolderName);
		}

		return false;
	}

	void IgnoreList::ReadFromFile(const char * aFolderName, const char * aFileName)
	{
		assert(NULL != aFolderName);
		assert(NULL != aFileName  );

		char lFileName[2048];

		sprintf_s(lFileName, "%s" SLASH "%s", aFolderName, aFileName);

        TextFile  lTF;

        lTF.Open(lFileName);

		Entry  lEntry;

		char  lLine[2048];

		while (lTF.ReadLine(lLine, sizeof(lLine)))
		{
			lEntry.mFlags.mFolder  =         Trim	  (lLine);
			lEntry.mFlags.mPattern = String::IsPatern (lLine);

			switch (lLine[0])
			{
			case '#'	: // Comment / Commentaire
				break;

			case '\0'	: // Empty line / Ligne vide
				break;

			case '/'	: // Absolute entry / Entree absolue
				String::ReplaceSlash(lLine + 1);

				lEntry.mFlags.mAbsolute = true;
				lEntry.mString			= lLine + 1	;
				mList.push_back(lEntry);
				break;

			default		: // Relativ entry / Entree relative
				String::ReplaceSlash(lLine);

				lEntry.mFlags.mAbsolute = false;
				lEntry.mString			= lLine;
				mList.push_back(lEntry);
			}
		}
	}

	// Private
	/////////////////////////////////////////////////////////////////////////

	// aEntry      [---;R--]
	// aFileName   [---;R--]
	//
	// Return / Retour
	//	false
	//	true
	bool IgnoreList::Match(const Entry & aEntry, const char * aFileName) const
	{
		assert(NULL != (&aEntry));
		assert(NULL != aFileName);

		unsigned int lIndex = static_cast<unsigned int>(mPrefix.size());
		if (0 < lIndex)
		{
			if ((0 != strncmp(mPrefix.c_str(), aFileName, mPrefix.size())) || (SLASH_C != aFileName[lIndex]))
			{
				return false;
			}

			lIndex++;
		}

		bool lResult;

		const char * lPtr = aFileName + lIndex;

		// A sub folder may existe between the prefix and the relative entry.
		// The loop skip 0..n folder name and try. / Un nom de repertoire
		// peut exister entre le prefix et l'entree relative. La boucle saute
		// 0..n nom de repertoire et essaie.
		for ( ;; )
		{
			lResult = MatchNameOrPattern(aEntry, lPtr);

			if (aEntry.mFlags.mAbsolute || lResult)
			{
				break;
			}

			lPtr = strchr(lPtr, SLASH_C);
			if (NULL == lPtr)
			{
				break;
			}

			lPtr++;
		}

		return lResult;
	}

	// aEntry      [---;R--]  List entry / Entree de la liste
	// aFileName   [---;R--]  File or folder name / Nom de fichier ou de
	//                        dossier
	//
	// Return / Retour
	//	false
	//	true
	bool IgnoreList::MatchNameOrPattern(const Entry & aEntry, const char * aFileName) const
	{
		assert(NULL != (&aEntry));
		assert(NULL != aFileName);

		bool lResult;

		if (aEntry.mFlags.mPattern)
		{
			lResult = String::MatchPatern (aEntry.mString.c_str(), aFileName);
		}
		else
		{
			lResult =         MatchName   (aEntry.mString.c_str(), aFileName);
		}

		return lResult;
	}

}

// Fonctions statiques
/////////////////////////////////////////////////////////////////////////////

// aName     [---;R--]  Name to ignore / Nom a ignorer
// aToMatch  [---;R--]  File or folder name / Nom du fichier ou du dossier
//
// Return / Retour
//	false
//	true
bool MatchName(const char * aName, const char * aToMatch)
{
	assert(NULL != aName	);
	assert(NULL != aToMatch	);

	return (0 == strcmp(aName, aToMatch));
}

// aLine  [---;RW-]
//
// Return / Retour
//  false  It's a file name / C'est un nom de fichier
//  true   It's a folder name / C'est un nom de dossier
bool Trim(char * aLine)
{
	assert(NULL != aLine);

	size_t i = strlen(aLine);
	while (0 < i)
	{
		i--;

		switch (aLine[i])
		{
		case '/'	:
			aLine[i] = '\0';
			return true;

		case '\n' :
		case '\r' :
		case '\t' :
		case ' ' :
			aLine[i] = '\0';
			break;

		default :
			return false;
		}
	}

	return false;
}
