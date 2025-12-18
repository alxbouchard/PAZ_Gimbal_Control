
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/WindowsToLinux.h

#pragma once

// Includes
//////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ===================================================================
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ===== sys =================================================================
#include <sys/types.h>

// Data types / Types de donnees
//////////////////////////////////////////////////////////////////////////////

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
typedef int	errno_t;

// Macros
//////////////////////////////////////////////////////////////////////////////

#define __FUNCTION__	""

// ===== _... ================================================================

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define _S_IFDIR S_IFDIR

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define _access		access

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define _getpid()	getpid()

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define _mkdir(N)	mkdir(N, 0700)

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define _stricmp	strcasecmp

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define _strnicmp	strncasecmp

// ===== ..._s ===============================================================

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define fopen_s(O,F,M)		((NULL==((*(O))=fopen(F,M)))?1:0)

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define localtime_s(O,I)	(NULL==localtime_r((I),(O))?2:0)

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define scanf_s             scanf

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define sprintf_s			sprintf

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define sscanf_s			sscanf

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define strcat_s            strcat

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define strcpy_s			strcpy

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define strncpy_s			strncpy

