
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/SafeAPI.h

#pragma once

// Includes
//////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// Macro
//////////////////////////////////////////////////////////////////////////////

/// \cond	en
/// \brief	This macro should be used when indicating the size to a function
///			of the windows Safe API.
/// \endcond
/// \cond	fr
/// \brief	Cette macro devrait etre utilise quand il faut indiquer la taille
///			a une fonction du Windows Sage API.
/// \endcond

#if defined( _KMS_LINUX_ ) || defined( _KMS_OS_X_ )
	#define SIZE_INFO(S)
#endif // _KMS_LINUX_ || _LMS_OS_X_

#ifdef _KMS_WINDOWS_
	#define SIZE_INFO(S)	, static_cast<unsigned int>(S)
#endif // _KMS_WINDOWS_
