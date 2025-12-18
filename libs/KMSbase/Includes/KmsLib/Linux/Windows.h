
// Auteur	:	KMS	-	Martin Dubois, ing.
// Produit	:	KmsBase
// Fichier	:	Includes/Linux/Windows.h

#pragma once

// Constants / Constantes
/////////////////////////////////////////////////////////////////////////////

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define FALSE						(0)

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define FILE_ATTRIBUTE_DIRECTORY	(0x00000001)

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define INVALID_HANDLE_VALUE		reinterpret_cast< void * >(0xffffffff)

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
#define TRUE						(1)

// Data type / Types de donnees
/////////////////////////////////////////////////////////////////////////////

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
typedef bool			BOOL		;

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
typedef unsigned long	DWORD		;

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
typedef time_t			FILETIME	;

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
typedef void		  * HANDLE		;

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
typedef struct
{
	char		cFileName[ 256 ]	;
	DWORD		dwFileAttributes	;
	FILETIME	ftLastWriteTime		;
}
WIN32_FIND_DATA;

// Function declarations / Declarations des fonctions
/////////////////////////////////////////////////////////////////////////////

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
extern BOOL		CopyFile		( const char * aExistingFileName, const char * aNewFileName, BOOL aFailIfExists );

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
extern BOOL		FindClose		( HANDLE		aHandle );

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
extern HANDLE	FindFirstFile	( const char  * aFileName	, WIN32_FIND_DATA * aFindFileData );

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
extern BOOL		FindNextFile	( HANDLE		aHandle		, WIN32_FIND_DATA * aFindFileData );

/// \cond	en
/// \brief	See the Microsoft documentation
/// \endcond
/// \cond	fr
/// \brief	Voir la documentation Microsoft
/// \endcond
extern BOOL		MoveFile		( const char * aExistingFileName, const char * aNewFileName );

