
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/KmsLib/Dump.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ==== C ===================================================================
#include <stdio.h>

namespace KmsLib
{

	// Function
	/////////////////////////////////////////////////////////////////////////

	/// \cond	en
	/// \brief	Dump binary data in hexadecimal format
	/// \param	aOut	[in,out]	The output stream
	/// \param	aIn		[in]		The data to dump
	///	\param	aInSize_byte		The size of data to dump
	/// \endcond
	/// \cond	fr
	/// \brief	Ecrit des donnees binaire en format hexadecimal
	/// \param	aOut	[in,out]	Le fichier de sortie
	/// \param	aIn		[in]		Les donnees a ecrire
	/// \param	aInSize_byte		La taille des donnees a ecrire
	/// \endcond
	extern void Dump(FILE * aOut, const unsigned char * aIn, unsigned int aInSize_byte);

	/// \cond	en
	/// \brief	Dump binary data in hexadecimal format
	/// \param	aOut	[in,out]	The output stream
	/// \param	aIn		[in]		The data to dump
	///	\param	aInSize_byte		The size of data to dump
	/// \endcond
	/// \cond	fr
	/// \brief	Ecrit des donnees binaire en format hexadecimal
	/// \param	aOut	[in,out]	Le fichier de sortie
	/// \param	aIn		[in]		Les donnees a ecrire
	/// \param	aInSize_byte		La taille des donnees a ecrire
	/// \endcond
	extern void Dump1(FILE * aOut, const unsigned char * aIn, unsigned int aInSize_byte);

	/// \cond	en
	/// \brief	Dump binary data in hexadecimal format
	/// \param	aOut	[in,out]	The output stream
	/// \param	aIn		[in]		The data to dump
	///	\param	aInSize_byte		The size of data to dump
	/// \endcond
	/// \cond	fr
	/// \brief	Ecrit des donnees binaire en format hexadecimal
	/// \param	aOut	[in,out]	Le fichier de sortie
	/// \param	aIn		[in]		Les donnees a ecrire
	/// \param	aInSize_byte		La taille des donnees a ecrire
	/// \endcond
	extern void Dump2(FILE * aOut, const unsigned short * aIn, unsigned int aInSize_byte);

	/// \cond	en
	/// \brief	Dump binary data in hexadecimal format
	/// \param	aOut	[in,out]	The output stream
	/// \param	aIn		[in]		The data to dump
	///	\param	aInSize_byte		The size of data to dump
	/// \endcond
	/// \cond	fr
	/// \brief	Ecrit des donnees binaire en format hexadecimal
	/// \param	aOut	[in,out]	Le fichier de sortie
	/// \param	aIn		[in]		Les donnees a ecrire
	/// \param	aInSize_byte		La taille des donnees a ecrire
	/// \endcond
	extern void Dump4(FILE * aOut, const unsigned int * aIn, unsigned int aInSize_byte);

	/// \cond	en
	/// \brief	Dump binary data in hexadecimal format
	/// \param	aOut	[in,out]	The output stream
	/// \param	aIn		[in]		The data to dump
	///	\param	aInSize_byte		The size of data to dump
	/// \endcond
	/// \cond	fr
	/// \brief	Ecrit des donnees binaire en format hexadecimal
	/// \param	aOut	[in,out]	Le fichier de sortie
	/// \param	aIn		[in]		Les donnees a ecrire
	/// \param	aInSize_byte		La taille des donnees a ecrire
	/// \endcond
	extern void DumpC(FILE * aOut, const unsigned char * aIn, unsigned int aInSize_byte);
}
