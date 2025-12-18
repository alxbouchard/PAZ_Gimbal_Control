
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/KmsLib/MemTester.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

namespace KmsLib
{

	// Class / Classe
	/////////////////////////////////////////////////////////////////////////

	/// \cond	en
	/// \brief	This class provide memory test pattern generation and
	///			verification.
	/// \endcond
	/// \cond	fr
	/// \brief	Cette classe fournis une fonctionnalite de generation et de
	///			verification de patron pour le test de memoire.
	/// \endcond
	class MemTester
	{

	public:

		typedef enum
		{
			PATTERN_TYPE_START = 0,

			PATTERN_TYPE_KMS	= 0,
			PATTERN_TYPE_RANDOM = 1,
			PATTERN_TYPE_SIMPLE = 2,

			PATTERN_TYPE_QTY = 3
		}
		PatternType;

		/// \cond	en
		/// \brief	Constructor
		/// \endcond
		/// \cond	fr
		/// \brief	Constructeur
		/// \endcond
		MemTester();

		/// \cond	en
		/// \brief	Destructor
		/// \endcond
		/// \cond	fr
		/// \brief	Destructeur
		/// \endcond
		~MemTester();

		/// \cond	en
		/// \brief	Go to the first index or the first type
		/// \endcond
		/// \cond	fr
		/// \brief	Aller au premier indice du premier type
		/// \endcond
		void Reset();

		/// \cond	en
		/// \brief	Pattern count accessor
		/// \return	The total pattern count
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le nombre de patron
		/// \return	Le nombre total de patron
		/// \endcond
		unsigned int	GetCount() const;

		/// \cond	en
		/// \brief	Pattern count accessor
		/// \param	aType	See Pattern	Type
		/// \return	The pattern count
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le nombre de patron
		/// \param	aType	Voir PatternType
		/// \return	Le nombre total de patron
		/// \endcond
		unsigned int	GetCount(PatternType aType) const;

		/// \cond	en
		/// \brief	Index accessor
		/// \return	The current index
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour l'indice
		/// \return	L'indice courrant
		/// \endcond
		unsigned int	GetIndex() const;

		/// \cond	en
		/// \brief	Index accessor
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le type
		/// \endcond
		void	ResetIndex();

		/// \cond	en
		/// \brief	Pattern type accessor
		/// \return	See PatternType
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le type de patron
		/// \return	Voir PatternType
		/// \endcond
		PatternType	GetPatternType() const;

		/// \cond	en
		/// \brief	Pattern type accessor
		/// \param	aType	See PatternType
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le type de patron
		/// \param	aType	Void PatternType
		/// \endcond
		void	SetPatternType(PatternType aType);

		/// \cond	en
		/// \brief	Progress accessor
		/// \param	aGlobal	If the caller set this parameter to true, the
		///					method returns the progress for all patter types.
		///					If it set it to false, the method returns the
		///					progress for the current pattern type.
		/// \return	This method returns the progress in 0.1 %.
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le progres
		/// \param	aGlobal	Si la valeur est true, la methode retourne le
		///					progres pour tous les types de patrons. Si elle
		///					est false, la methode retourne le progress pour
		///					le type de patron en cours.
		/// \return	Cette methode retourne le progress en 0.1 %.
		/// \endcond
		unsigned int GetProgress(bool aGlobal = true) const;

		/// \cond	en
		/// \brief	Generate the pattern
		/// \param	aOut	[out]	Output buffer
		/// \param	aOutSize_byte	Output buffer size
		/// \endcond
		/// \cond	fr
		/// \brief	Genere a patron
		/// \param	aOut	[out]	L'espace memoire de sortie
		/// \param	aOutSize_byte	La taille de l'espace de memoire de sortie
		/// \endcond
		void	Generate(unsigned char * aOut, unsigned int aOutSize_byte) const;

		/// \cond	en
		/// \brief	Go to the next index
		/// \retval	false	No more index for the current type
		/// \endcond
		/// \cond	fr
		/// \brief	Passer au prochain indice
		/// \retval	false	Pas d'autre indice
		/// \endcond
		/// \retval	true	OK
		bool	NextIndex();

		/// \cond	en
		/// \brief	Go to the next index or pattern type
		/// \retval	false	No more type
		/// \endcond
		/// \cond	fr
		/// \brief	Passer au prochain indice ou type de patron
		/// \retval	false	Pas d'autre type
		/// \endcond
		/// \retval	true	OK
		bool	NextIndexAndPatternType();

		/// \cond	en
		/// \brief	Verify a patter
		///	\param	aIn		[in]	Input buffer
		/// \param	aInSize_byte	Input buffer size
		/// \return	This method returns the number of error it detected.
		/// \endcond
		/// \cond	fr
		/// \brief	Verifie un patron
		/// \param	aIn		[in]	L'espace memoire d'entree
		/// \param	aInSize_byte	La taille de l'espace memoire d'entree
		/// \return	Cette methode retourne le nombre d'erreurs detectes.
		/// \endcond
		unsigned int	Verify(const unsigned char * aIn, unsigned int aInSize_byte) const;

	private:

		void	Generate_KMS	(unsigned char * aOut, unsigned int aOutSize_byte) const;
		void	Generate_Random	(unsigned char * aOut, unsigned int aOutSize_byte) const;
		void	Generate_Simple	(unsigned char * aOut, unsigned int aOutSize_byte) const;

		unsigned int	Verify_KMS		(const unsigned char * aIn, unsigned int aInSize_byte) const;
		unsigned int	Verify_Random	(const unsigned char * aIn, unsigned int aInSize_byte) const;
		unsigned int	Verify_Simple	(const unsigned char * aIn, unsigned int aInSize_byte) const;

		unsigned int	mIndex			;
		unsigned int	mPatternType	;

	};

}
