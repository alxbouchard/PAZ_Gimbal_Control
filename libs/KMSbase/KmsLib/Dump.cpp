
// Auteur	:	KMS -	Martin Dubois, ing.
// Projet	:	KmsBase
// Fichier	:	Includes/KmsLib/Dump.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>

// ===== Interface ==========================================================
#include <KmsLib/Dump.h>

namespace KmsLib
{

	// Function
	/////////////////////////////////////////////////////////////////////////

	void Dump(FILE * aOut, const unsigned char * aIn, unsigned int aInSize_byte)
	{
		assert(NULL	!=	aOut			);
		assert(NULL	!=	aIn				);
		assert(0	<	aInSize_byte	);

		Dump1(aOut, aIn, aInSize_byte);
	}

	void Dump1(FILE * aOut, const unsigned char * aIn, unsigned int aInSize_byte)
	{
		assert(NULL !=	aOut			);
		assert(NULL !=	aIn				);
		assert(0	<	aInSize_byte	);

		for (unsigned int i = 0; i < aInSize_byte; i++)
		{
			fprintf(aOut, "%02x", aIn[i]);

			switch (i % 16)
			{
			case 3	:
			case 11	:
				fprintf(aOut, "  ");
				break;

			case 7	: fprintf(aOut, "    "	); break;
			case 15	: fprintf(aOut, "\n"	); break;

			default:
				fprintf(aOut, " ");
			}
		}

		fprintf(aOut, "\n");
	}

	void Dump2(FILE * aOut, const unsigned short * aIn, unsigned int aInSize_byte)
	{
		assert(NULL != aOut);
		assert(NULL != aIn);
		assert(0	<	aInSize_byte);

		unsigned int lCount = aInSize_byte / sizeof(unsigned short);

		for (unsigned int i = 0; i < lCount; i++)
		{
			fprintf(aOut, "%04x", aIn[i]);

			switch (i % 8)
			{
			case 3 : fprintf(aOut, "  "); break;
			case 7 : fprintf(aOut, "\n"); break;

			default:
				fprintf(aOut, " ");
			}
		}

		fprintf(aOut, "\n");
	}

	void Dump4(FILE * aOut, const unsigned int * aIn, unsigned int aInSize_byte)
	{
		assert(NULL !=	aOut			);
		assert(NULL !=	aIn				);
		assert(0	<	aInSize_byte	);

		unsigned int lCount = aInSize_byte / sizeof(unsigned int);

		for (unsigned int i = 0; i < lCount; i++)
		{
			fprintf(aOut, "%08x", aIn[i]);

			switch (i % 4)
			{
			case 3: fprintf(aOut, "\n"); break;

			default:
				fprintf(aOut, " ");
			}
		}

		fprintf(aOut, "\n");
	}

	void DumpC(FILE * aOut, const unsigned char * aIn, unsigned int aInSize_byte)
	{
		assert(NULL !=	aOut			);
		assert(NULL !=	aIn				);
		assert(0	<	aInSize_byte	);

		for (unsigned int i = 0; i < aInSize_byte; i ++)
		{
			fprintf(aOut, "0x%02x,", aIn[i]);

			switch (i % 8)
			{
			case 7: fprintf(aOut, "\n"); break;

			default :
				fprintf(aOut, " ");
			}
		}

		fprintf(aOut, "\n");
	}
}
