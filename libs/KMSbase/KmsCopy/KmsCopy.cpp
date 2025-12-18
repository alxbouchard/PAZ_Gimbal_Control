
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsCopy/KmsCopy.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifdef _KMS_WINDOWS_
    #include <direct.h>
#endif

// ===== System =============================================================
#include <sys/stat.h>

#ifdef _KMS_WINDOWS_
    // ===== Windows ============================================================
    #include <Windows.h>
#endif

// ===== Includes ===========================================================
#include "../Includes/KmsTool.h"

#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
    #include "../Includes/KmsLib/Linux/Windows.h"
#endif

// ===== Common =============================================================
#include "../Common/Version.h"

// Static function declaration / Declaration des fonctions statiques
/////////////////////////////////////////////////////////////////////////////

static int	Copy_File(const char * aSrcRoot, const char * aSrcFolder, const char * aSrcFile, const char * aDstRoot, const char * aDstFolder, const char * aDstFile);

static int	Execute(FILE * aFile, const char * aSrcRoot, const char * aDstRoot);

static int	Validate_DstFolder	(const char * aDstFolder	);
static int	Validate_DstFolder	(const char * aDstRoot, const char * aDstFolder);
static int	Validate_SrcRoot	(const char * aSrcRoot		);

// Entry point / Point d'entree
/////////////////////////////////////////////////////////////////////////////

int main(int aCount, const char ** aVector)
{
	assert(1	<= aCount	);
	assert(NULL != aVector	);

    KMS_TOOL_BANNER("KmsBase", "KmsCopy", VERSION_STR, VERSION_TYPE);

	if (4 != aCount)
	{
		fprintf(stderr,
			"\nUSER ERROR : Invalid command line\n"
			"Usage : KmsCopy {SrcRoot} {DstRoot} {FileList}\n");
		return 1;
	}

	int lResult = Validate_SrcRoot(aVector[1]);
	if (0 == lResult)
	{
		lResult = Validate_DstFolder(aVector[2]);
		if (0 == lResult)
		{
			printf("Parsing %s ...\n", aVector[3]);

			FILE * lFile;

			errno_t lErr = fopen_s(&lFile, aVector[3], "r");
			if (0 != lErr)
			{
				fprintf(stderr, "\nFATAL ERROR : fopen_s( , \"%s\",  ) failed returning %d\n", aVector[3], lErr);
				return 2;
			}

			lResult = Execute(lFile, aVector[1], aVector[2]);

			int lRetI = fclose(lFile);
			if (0 != lRetI)
			{
				fprintf(stderr, "\nWARNING : fclose(  ) failed returning %d", lRetI);
			}
		}
	}

	return lResult;
}

// Static functions / Fonctions statiques
/////////////////////////////////////////////////////////////////////////////

// aSrcRoot		[---;R--]
// aSrcFolder	[---;R--]
// aSrcFile		[---;R--]
// aDstRoot		[---;R--]
// aDstFolder	[---;R--]
// aDstFile		[---;R--]
//
// Retour
//	0	=	OK
//	8	=	Error / Erreur
int	Copy_File(const char * aSrcRoot, const char * aSrcFolder, const char * aSrcFile, const char * aDstRoot, const char * aDstFolder, const char * aDstFile)
{
	assert(NULL != aSrcRoot		);
	assert(NULL != aSrcFolder	);
	assert(NULL != aSrcFile		);
	assert(NULL != aDstRoot		);
	assert(NULL != aDstFolder	);
	assert(NULL != aDstFile		);

	char lDst[2048];
	char lSrc[2048];

	int lRetI = sprintf_s(lSrc, "%s" SLASH "%s" SLASH "%s", aSrcRoot, aSrcFolder, aSrcFile);
	assert(5 <= lRetI);

	lRetI = sprintf_s(lDst, "%s" SLASH "%s" SLASH "%s", aDstRoot, aDstFolder, aDstFile);
	assert(5 <= lRetI);

	int lResult;

	if (CopyFile(lSrc, lDst, false))
	{
		lResult = 0;
	}
	else
	{
		fprintf(stderr, "\nERROR : Cannot copy %s to %s\n", lSrc, lDst);
		lResult = 8;
	}

	return lResult;
}

// aFile	[---;RW-]
// aSrcRoot	[---;R--]
// aDstRoot	[---;R--]
//
// Retour
//	0				=	OK
//	Other / Autre	=	Error / Erreur
int Execute(FILE * aFile, const char * aSrcRoot, const char * aDstRoot)
{
	assert(NULL != aFile	);
	assert(NULL != aSrcRoot	);
	assert(NULL != aDstRoot	);

	char lDstFolder	[1024];
	char lLine		[2048];
	char lSrcFolder	[1024];

	unsigned int lCopyCount		= 0;
	unsigned int lErrorCount	= 0;
	unsigned int lLineNumber	= 0;

	int lResult = 0;
	int lRetI;

	while (NULL != fgets(lLine, sizeof(lLine) / sizeof(lLine[0]), aFile))
	{
		lLineNumber++;

		switch (lLine[0])
		{
		case ' '	:
		case '\t'	:	// File
			char lDstFile[1024];
			char lSrcFile[1024];

			switch (sscanf_s(lLine, " %[^ \n\r\t] %[^ \n\r\t]", lSrcFile SIZE_INFO(sizeof(lSrcFile) / sizeof(lSrcFile[0])), lDstFile SIZE_INFO(sizeof(lDstFile) / sizeof(lDstFile[0]))))
			{
			case EOF: break; // Empty line

			case 1:
				strcpy_s(lDstFile, lSrcFile);
				// no break

			case 2:
				lRetI = Copy_File(aSrcRoot, lSrcFolder, lSrcFile, aDstRoot, lDstFolder, lDstFile);
				if (0 == lRetI)
				{
					printf(".");
					lCopyCount++;
				}
				else
				{
					lErrorCount++;
					lResult |= lRetI;
				}
				break;

			default:
				fprintf(stderr, "\nERROR : The line %u is not valid\n", lLineNumber);
				lErrorCount ++;
				lResult |= 16;
			}
			break;

		case '\n' :
		case '\r' :	// Empty line
			break;

		case '#' : // Comment
			break;

		default : // Folder
			switch (sscanf_s(lLine, "%[^ \n\r\t] %[^ \n\r\t]", lSrcFolder SIZE_INFO(sizeof(lSrcFolder) / sizeof(lSrcFolder[0])), lDstFolder SIZE_INFO(sizeof(lDstFolder) / sizeof(lDstFolder[0]))))
			{
			case 1:
				strcpy_s(lDstFolder, lSrcFolder);
				// no break;

			case 2:
				lRetI = Validate_DstFolder(aDstRoot, lDstFolder);
				if (0 != lRetI)
				{
					lResult |= lRetI;
					goto End;
				}
				break;

			default:
				fprintf(stderr, "\nFATAL ERROR : The line %u is not valid\n", lLineNumber);
				lResult |= 3;
				goto End;
			}
		}
	}

End:

	printf(
		"\n"
		"    %u parsed lines\n"
		"    %u copied files\n"
		"    %u errors\n"
		"    (0x%08x)\n",
		lLineNumber,
		lCopyCount,
		lErrorCount,
		lResult);

	return lResult;
}

// aDstFolder	[---;R--]
//
// Retour
//	0				=	OK
//	Other / Autre	=	Error / Erreur
int Validate_DstFolder(const char * aDstFolder)
{
	assert(NULL != aDstFolder);

	struct stat lStat;

	int lRetI = stat(aDstFolder, &lStat);
	if (0 == lRetI)
	{
		if (0 == (lStat.st_mode & _S_IFDIR))
		{
			fprintf(stderr, "\nUSER ERROR : %s already exist and is not a folder\n", aDstFolder);
			return 4;
		}
	}
	else
	{
		lRetI = _mkdir(aDstFolder);
		if (0 != lRetI)
		{
			fprintf(stderr, "\nFATAL ERROR : _mkdir( \"%s\" ) failed returning %d\n", aDstFolder, lRetI);
			return 5;
		}
	}

	return 0;
}

// aDstRoot		[---;R--]
// aDstFolder	[---;R--]
int	Validate_DstFolder(const char * aDstRoot, const char * aDstFolder)
{
	assert(NULL != aDstRoot		);
	assert(NULL != aDstFolder	);

	char lDst[2048];

	int lRetI = sprintf_s(lDst, "%s" SLASH "%s", aDstRoot, aDstFolder);
	assert(3 <= lRetI);

	return Validate_DstFolder(lDst);
}

// aSrcRoot	[---;R--]
//
// Retour
//	0				=	OK
//	Other / Autre	=	Error / Erreur
int Validate_SrcRoot(const char * aSrcRoot)
{
	assert(NULL != aSrcRoot);

	struct stat lStat;

	int lRetI = stat(aSrcRoot, &lStat);
	if ((0 != lRetI) || (0 == (lStat.st_mode & _S_IFDIR)))
	{
		fprintf(stderr, "\nUSER ERROR : %s is not a valid source root (%d)\n", aSrcRoot, lRetI);
		return 6;
	}

	return 0;
}
