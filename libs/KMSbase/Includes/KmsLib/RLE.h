
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/KmsLib/RLE.h

#pragma once

namespace KmsLib
{

	// Functions / Fonctions
	/////////////////////////////////////////////////////////////////////////

	/// \cond	en
	/// \brief	Compress using the RLE algorithm
	/// \param	aIn		[in]	Uncompressed data
	/// \param	aInSize_byte	Size of uncompressed data
	/// \param	aOut	[out]	Compressed data
	/// \param	aOutSize_byte	Maximum size of compressed data
	/// \return	The size of compressed data in bytes
	/// \endcond
	/// \cond	fr
	/// \brief	Compresser en utilisant l'algorithme RLE
	/// \param	aIn		[in]	Donnees non compressees
	/// \param	aInSize_byte	Taille des donnees non compressees
	/// \param	aOut	[out]	Donnees compressees
	/// \param	aOutSize_byte	Taille maximale des donnees compressees
	/// \return	La taille des donnees compressees en octets
	/// \endcond
	/// \exception	Exception	CODE_INVALID_BUFFER_SIZE,
	///							CODE_NOT_ENOUGH_MEMORY
	extern unsigned int	RLE_Compress(const unsigned char * aIn, unsigned int aInSize_byte, unsigned char * aOut, unsigned int aOutSize_byte);

	/// \cond	en
	/// \brief	Uncompress using the RLE algorithm
	/// \param	aIn		[in]	Compressed data
	/// \param	aInSize_byte	Size of compressed data
	/// \param	aOut	[out]	Uncompressed data
	/// \param	aOutSize_byte	Maximum size of uncompressed data
	/// \return	Size of uncompressed data in bytes
	/// \endcond
	/// \cond	fr
	/// \brief	Decrompress en utilisant l'algorithme RLE
	/// \param	aIn		[in]	Donnees compressees
	/// \param	aInSize_byte	Taille des donnees compressees
	/// \param	aOut	[out]	Donnees non compressees
	/// \param	aOutSize_byte	Taille maximale des donnees non compressees
	/// \return	Taille des donnees non compressees en octets
	/// \endcond
	/// \exception	Exception	CODE_INVALID_BUFFER_SIZE,
	///							CODE_INVALID_DATA,
	///							CODE_NOT_ENOUGH_MEMORY
	extern unsigned int	RLE_Uncompress(const unsigned char * aIn, unsigned int aInSize_byte, unsigned char * aOut, unsigned int aOutSize_byte);

	/// \cond	en
	/// \brief	Compute the size of compressed data
	/// \param	aIn		[in]	Uncompressed data
	/// \param	aInSize_byte	Size of uncompressed data
	/// \endcond
	/// \cond	fr
	/// \brief	Calcul la taille des donnees compressees
	/// \param	aIn		[in]	Donnees non compressees
	/// \param	aInSize_byte	Taille des donnees non compressees
	/// \endcond
	extern unsigned int	RLE_ComputeCompressedSize(const unsigned char * aIn, unsigned int aInSize_byte);

	/// \cond	en
	/// \brief	Compute the size of uncompressed data
	/// \param	aIn		[in]	Compressed data
	/// \param	aInSize_byte	Size of compressed data
	/// \return	Size of uncompressed data in bytes
	/// \endcond
	/// \cond	fr
	/// \brief	Calcul la tailled des donnees non compressees
	/// \param	aIn		[in]	Donnees compressees
	/// \param	aInSize_byte	Taille des donnees compressees
	/// \return	Taille des donnees non compressees en octets
	/// \endcond
	/// \exception	Exception	CODE_INVALID_DATA
	extern unsigned int	RLE_ComputeUncompressedSize(const unsigned char * aIn, unsigned int aInSize_byte);

}
