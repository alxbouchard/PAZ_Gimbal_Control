
// Author   KMS - Martin Dubois, ing.
// Product  KmsBase
// File     KmsLib/Linux/Windows.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ===== System =============================================================
#include <sys/types.h>
#include <sys/stat.h>

// ===== Includes ===========================================================
#include <KmsLib/Linux/Windows.h>

// Static function declaration / Declaration de la fonction statique
/////////////////////////////////////////////////////////////////////////////

static bool Match( const char * aPat, const char * aName );

// Data types / Types de donnees
/////////////////////////////////////////////////////////////////////////////

typedef struct
{
	const char	  * mFileName	;
	DIR			  * mFolder		;
	const char	  *	mFolderName	;
	char			mBuffer[ 2048 ];
}
FindFileContext;

// Functions / Fonctions
/////////////////////////////////////////////////////////////////////////////

bool CopyFile( const char * aExistingFileName, const char * aNewFileName, bool aFailIfExists )
{
	assert( NULL != aExistingFileName	);
	assert( NULL != aNewFileName		);

	if ( aFailIfExists && ( 0 == access( aNewFileName, F_OK ) ) )
	{
		return false;
	}

	char lCommand[ 2048 ];

	sprintf( lCommand, "cp -a%s %s %s", aFailIfExists ? "n" : "", aExistingFileName, aNewFileName );

	return ( 0 == system( lCommand ) );
}

bool FindClose ( HANDLE aHandle )
{
	assert( NULL != aHandle );

	FindFileContext * lContext = reinterpret_cast< FindFileContext * >( aHandle );

	int lRetI = closedir( lContext->mFolder );
	assert( 0 == lRetI );

	(void)( lRetI );

	delete lContext;
    
    return true;
}

HANDLE FindFirstFile( const char * aFileName, WIN32_FIND_DATA * aFindFileData )
{
	assert( NULL != aFileName		);
	assert( NULL != aFindFileData	);

	FindFileContext	* lResult = new FindFileContext;
	assert( NULL != lResult );

	memset( lResult, 0, sizeof( FindFileContext ) );

	strncpy( lResult->mBuffer, aFileName, sizeof( lResult->mBuffer ) - 1 );

	char * lPtr = strrchr( lResult->mBuffer, '/' );
	if ( NULL == lPtr )
	{
		lResult->mFileName		= lResult->mBuffer	;
		lResult->mFolderName	= "."				;
	}
	else
	{
		( * lPtr ) = '\0';

		lResult->mFileName		= lPtr + 1			;
		lResult->mFolderName	= lResult->mBuffer	;
	}

	lResult->mFolder = opendir( lResult->mFolderName );
	if ( NULL == lResult->mFolder )
	{
		delete lResult;
		return INVALID_HANDLE_VALUE;
	}

	if ( ! FindNextFile( lResult, aFindFileData ) )
	{
		FindClose( lResult );
		return INVALID_HANDLE_VALUE;
	}

	return lResult;
}

bool FindNextFile( HANDLE aHandle, WIN32_FIND_DATA * aFindFileData )
{
	assert( NULL != aHandle			);
	assert( NULL != aFindFileData	);

	memset( aFindFileData, 0, sizeof( WIN32_FIND_DATA ) );

	FindFileContext * lContext = reinterpret_cast< FindFileContext * >( aHandle );

	struct dirent * lEntry	;

	do
	{
		lEntry = readdir( lContext->mFolder );
		if ( NULL == lEntry )
		{
			return false;
		}
	}
	while ( ! Match( lContext->mFileName, lEntry->d_name ) );

	char lFileName[ 2048 ];

	sprintf( lFileName, "%s/%s", lContext->mFolderName, lEntry->d_name );

	struct stat lStat;

	int lRetI = stat( lFileName, & lStat );
	if ( 0 != lRetI )
	{
		return false;
	}

	strncpy( aFindFileData->cFileName, lEntry->d_name, sizeof( aFindFileData->cFileName ) - 1 );
	
	if ( 0 != ( lStat.st_mode & S_IFDIR ) )
	{
		aFindFileData->dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
	}

	aFindFileData->ftLastWriteTime = lStat.st_mtime;

	return true;
}

bool MoveFile( const char * aExistingFileName, const char * aNewFileName )
{
	assert( NULL != aExistingFileName	);
	assert( NULL != aNewFileName		);

	char lCommand[ 2048 ];

	sprintf( lCommand, "mv %s %s", aExistingFileName, aNewFileName );

	return ( 0 == system( lCommand ) );
}

// Static function / Fonction statique
/////////////////////////////////////////////////////////////////////////////

bool Match( const char * aPat, const char * aName )
{
	assert( NULL != aPat	);
	assert( NULL != aName	);

	for ( ;; )
	{
		switch ( * aPat )
		{
		case '\0' :
			return ( '\0' == ( * aName ) );

		case '?' :
			if ( '\0' == ( * aName ) )
			{
				return false;
			}

			aName	++;
			aPat	++;
			break;

		case '*' :
			if ( '\0' == ( * aName ) )
			{
				return false;
			}

			aName	++;
			aPat	++;

			if ( '\0' == ( * aPat ) )
			{
				return true;
			}

			while ( '\0' != ( * aName ) )
			{
				if ( Match( aPat, aName ) )
				{
					return true;
				}

				aName ++;
			}

			return false;

		default :
			if ( ( * aPat ) != ( * aName ) )
			{
				return false;
			}

			aPat	++;
			aName	++;
		}
	}
}

