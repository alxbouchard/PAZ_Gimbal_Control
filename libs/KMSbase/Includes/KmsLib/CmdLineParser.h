
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/KmsLib/CmdLineParser.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <stdio.h>

// ===== C++ ================================================================
#include <map>
#include <string>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>

namespace KmsLib
{

	// Class / Classe
	/////////////////////////////////////////////////////////////////////////

	/// \cond	en
	/// \brief	This class provide command line argument parsing
	///			functionnality.
	/// \endcond
	/// \cond	fr
	/// \brief	Cette classe fournis une fonctionnalite d'interpretation des
	///         ligne argument de la ligne de commande.
	/// \endcond
	class CmdLineParser
	{

	public:

		typedef enum
		{
			ERROR_FLAG_IGNORED_OPTIONS	= 0x01,
			ERROR_FLAG_INCOMPLET_OPTION	= 0x02,
			ERROR_FLAG_INVALID_OPTION	= 0x04,
		}
		ErrorFlag;

		/// \cond	en
		/// \brief	Constructor
		/// \param	aOptNoData		[in,keep]	This string contains all
		///										letter usable as option
		///										without data.
		/// \param	aOptWithData	[in,keep]	This string contains all
		///										letter usable as option
		///										with data.
		/// \endcond
		/// \cond	fr
		/// \brief	Constructeur
		/// \param	aOptNoData		[in,keep]	Cette chaine de caracteres
		///										contient toutes les lettres
		///										utilisees comme option dans
		///										donnees.
		/// \param	aOptWithData	[in,keep]	Cette chaine de caracteres
		///										contient toutes les lettres
		///										utilisees comme option avec
		///										donnees.
		/// \endcond
		CmdLineParser(const char * aOptNoData, const char * aOptWithData = "");

		/// \cond	en
		/// \brief	Destructor
		/// \endcond
		/// \cond	fr
		/// \brief	Destructeur
		/// \endcond
		~CmdLineParser();

		/// \cond	en
		/// \brief	Is the specified option present?
		/// \param	aArgOpt	Argument index or option letter
		/// \retval	false	No
		/// \retval	true	Yes
		/// \endcond
		/// \cond	fr
		/// \brief	L'option specifie est-il present?
		/// \param	aArgOpt	L'indice de l'argument ou la lettre de l'option
		/// \retval	false	Non
		/// \retval	true	Oui
		/// \endcond
		bool IsPresent(int aArgOpt) const;

		/// \cond	en
		/// \brief	Argument count accessor
		/// \return	This method return the number of argument
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le nombre d'argument
		/// \return	Cette methode retourne le nombre d'argument
		/// \endcond
		unsigned int GetArgumentCount() const;

		/// \cond	en
		/// \brief	Data accessor
		/// \param	aArgOpt			Argument index or option letter
		/// \param	aOut	[out]	Output buffer
		/// \param	aOutSize_byte	Output buffer size
		/// \param	aDefault		Default value to use if the argument or
		///							option is not present
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour les donnees
		/// \param	aArgOpt			L'index de l'arguement ou la lettre de
		///							l'option
		/// \param	aOut	[out]	L'espace memoire de sortie
		/// \param	aOutSize_byte	Taille de l'espace memoire de sortie
		/// \param	aDefault		Valeur a utiliser si l'argument ou
		///							l'option n'est pas present
		/// \endcond
		/// \exception	Exception	CODE_INVALID_COMMAND_LINE
		void GetData(int aArgOpt, char * aOut, unsigned int aOutSize_byte, const char * aDefault = NULL) const;

		/// \cond	en
		/// \brief	Data accessor
		/// \param	aArgOpt			Argument index or option letter
		/// \param	aOut	[out]	Output buffer
		/// \param	aRadix			See strtoul
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour les donnees
		/// \param	aArgOpt			L'index de l'arguement ou la lettre de l'option
		/// \param	aOut	[out]	L'espace memoire de sortie
		/// \param	aRadix			Voir strtoul
		/// \endcond
		/// \exception	Exception	CODE_INVALID_COMMAND_LINE
		void GetData(int aArgOpt, unsigned int * aOut, int aRadix) const;

		/// \cond	en
		/// \brief	Data accessor
		/// \param	aArgOpt			Argument index or option letter
		/// \param	aOut	[out]	Output buffer
		/// \param	aRadix			See strtoul
		/// \param	aDefault		Default value to use if the argument or
		///							option is not present
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour les donnees
		/// \param	aArgOpt			L'index de l'arguement ou la lettre de l'option
		/// \param	aOut	[out]	L'espace memoire de sortie
		/// \param	aRadix			Voir strtoul
		/// \param	aDefault		Valeur a utiliser si l'argument ou
		///							l'option n'est pas present
		/// \endcond
		void GetData(int aArgOpt, unsigned int * aOut, int aRadix, unsigned int aDefault) const;

		/// \cond	en
		/// \brief	Data accessor
		/// \param	aArgOpt			Argument index or option letter
		/// \param	aOut	[out]	Output buffer
		/// \param	aMode	[in ]	See fopen
		/// \param	aDefault		Default value to use if the argument or
		///							option is not present
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour les donnees
		/// \param	aArgOpt			L'index de l'arguement ou la lettre de
		///							l'option
		/// \param	aOut	[out]	L'espace memoire de sortie
		/// \param	aMode	[in ]	Voir fopen
		/// \param	aDefault		Valeur a utiliser si l'argument ou
		///							l'option n'est pas present
		/// \endcond
		/// \exception	Exception	CODE_INVALID_COMMAND_LINE, CODE_IO_ERROR
		void GetData(int aArgOpt, FILE ** aOut, const char * aMode, FILE * aDefault = NULL) const;

		/// \cond	en
		/// \brief	Error flags accessor
		/// \return	See ErrorFlag
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour les drapeau d'erreur
		/// \return	Voir ErrorFlag
		/// \endcond
		unsigned int GetErrorFlags() const;

		/// \cond	en
		/// \brief	Parse the command line arguments
		/// \param	aCount			See main
		/// \param	aVector	[in]	See main
		/// \endcond
		/// \cond	fr
		/// \brief	Interpret les arguments de la ligne de commande
		/// \param	aCount			Voir main
		/// \param	aVector	[in]	Void main
		/// \endcond
		void Parse(int aCount, const char ** aVector);

	private:

		typedef std::map<int, std::string> Map;

		unsigned int mArgCount		;
		unsigned int mErrorFlags	;

		const char * mOptNoData		;
		const char * mOptWithData	;

		Map mOptions;

	};

}
