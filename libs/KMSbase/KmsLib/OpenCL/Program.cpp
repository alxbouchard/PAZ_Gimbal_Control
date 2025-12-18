
// Author / Auteur		:	KMS	-	Martin Dubois, ing.
// Product / Produit	:	KmsBase
// File / Fichier		:	KmsLib/OpenCL/Program.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/OpenCL/Program.h>

// Static function declarations / Declaration des fonctions statiques
/////////////////////////////////////////////////////////////////////////////

static bool ReadBinaryFile(FILE * aIn, cl::Program::Binaries	* aOut);
static void ReadSourceFile(FILE * aIn, cl::Program::Sources		* aOut);

namespace KmsLib
{
	namespace OpenCL
	{

		// Public
		/////////////////////////////////////////////////////////////////////

		Program::Program(const cl::Context & aContext, const cl::Program::Sources & aSources)
		{
			assert(NULL != (&aContext));
			assert(NULL	!= (&aSources));

			cl_int lRet;

			(*dynamic_cast<cl::Program *>(this)) = cl::Program(aContext, aSources, &lRet);

			if (CL_SUCCESS != lRet)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::Program::Program( ,  ) failed indicating %d", lRet);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::Program::Program( , , , ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

		Program::Program(const cl::Context & aContext, const char ** aLines, unsigned int aLineCount)
		{
			assert(NULL !=	(&aContext)	);
			assert(NULL !=	aLines		);
			assert(0	<	aLineCount	);

			cl::Program::Sources lSources;

			for (unsigned int i = 0; i < aLineCount; i++)
			{
				assert(NULL != aLines[i]);

				lSources.push_back(std::pair<const char*, ::size_t>(aLines[i], strlen(aLines[i])));
			}

			(*this) = Program(aContext, lSources);
		}

		Program::Program(const cl::Context & aContext, const VECTOR_CLASS<cl::Device> & aDevices, const char * aFileName, bool aBin)
		{
			assert(NULL != (&aContext)	);
			assert(NULL != (&aDevices)	);
			assert(NULL != aFileName	);

			FILE		* lFile;
			const char	* lMode = aBin ? "rb" : "r";

			int lRetI = fopen_s(&lFile, aFileName, lMode);
			if (0 != lRetI)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "fopen_s( , \"%s\", \"%s\" ) failed returning %d", aFileName, lMode, lRetI);

				throw new Exception(Exception::CODE_IO_ERROR, "fopen_s( , , ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRetI);
			}

			assert(NULL != lFile);

			cl::Program::Binaries	lBinaries	;
			cl::Program::Sources	lSources	;

			bool lRetB;

			if (aBin)
			{
				lRetB = ReadBinaryFile(lFile, &lBinaries);
			}
			else
			{
				ReadSourceFile(lFile, &lSources);
				lRetB = true;
			}

			lRetI = fclose(lFile);
			if (0 != lRetI)
			{
				throw new Exception(Exception::CODE_IO_ERROR, "fclose(  ) failed",
					NULL, __FILE__, __FUNCTION__, __LINE__, lRetI);
			}

			if (!lRetB)
			{
				throw new Exception(Exception::CODE_INVALID_DATA, "ReadBinaryFile( ,  ) failed",
					NULL, __FILE__, __FUNCTION__, __LINE__, 0);
			}

			VECTOR_CLASS<cl_int> lBinStatus;

			if (aBin)
			{
				(*dynamic_cast<cl::Program *>(this)) = cl::Program(aContext, aDevices, lBinaries, &lBinStatus, &lRetI);
			}
			else
			{
				(*dynamic_cast<cl::Program *>(this)) = cl::Program(aContext, lSources, &lRetI);
			}

			if (CL_SUCCESS != lRetI)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::Program::Program( %s  ) failed indicating %d", aBin ? ", , , ," : ", ,", lRetI);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::Program( , , , ,  ) or cl::Program( , , ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRetI);
			}

			if (aBin)
			{
				for (VECTOR_CLASS<cl_int>::iterator lIt = lBinStatus.begin(); lIt != lBinStatus.end(); lIt++)
				{
					if (CL_SUCCESS != (*lIt))
					{
						throw new Exception(Exception::CODE_INVALID_DATA, "cl::Program( , , , ,  ) reported and error",
							NULL, __FILE__, __FUNCTION__, __LINE__, (*lIt));

					}
				}
			}
		}

		void Program::GetBuildInfo(const cl::Device & aDevice, cl_program_build_info aName, std::string * aOut)
		{
			assert(NULL != (&aDevice)	);
			assert(NULL != aOut			);

			cl_int lRet = getBuildInfo(aDevice, aName, aOut);
			if (CL_SUCCESS != lRet)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::Program::getBuildInfo( , %u,  ) failed indicating %d", aName, lRet);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::Program::Program( , ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

		void Program::Build(const VECTOR_CLASS<cl::Device> & aDevices, const char * aOptions, void(CL_CALLBACK * aNotify)(cl_program, void *), void * aData) const
		{
			assert(NULL !=	(&aDevices)		);
			assert(0	<	aDevices.size()	);

			cl_int lRet = build(aDevices, aOptions, aNotify, aData);
			if (CL_SUCCESS != lRet)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::Program::build( , \"%s\", ,  ) failed indicating %d", aOptions, lRet);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::Program::build( , , ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}
	}
}

// Static functions / Fonctions statiques
/////////////////////////////////////////////////////////////////////////////

bool ReadBinaryFile(FILE * aIn, cl::Program::Binaries * aOut)
{
	int lRet = fseek(aIn, 0, SEEK_END);
	if (0 != lRet)
	{
		return false;
	}

	long lSize_byte = ftell(aIn);
	if (0 >= lSize_byte)
	{
		return false;
	}

	unsigned char * lBuffer = new unsigned char[lSize_byte];
	assert(NULL != lBuffer);

	aOut->push_back(std::pair<const void *, ::size_t>(lBuffer, lSize_byte));

	return true;
}

void ReadSourceFile(FILE * aIn, cl::Program::Sources * aOut)
{
	assert(NULL != aIn	);
	assert(NULL != aOut	);

	char lLine[1024];

	while (NULL != fgets(lLine, sizeof(lLine), aIn))
	{
		aOut->push_back(std::pair<const char*, ::size_t>(lLine, strlen(lLine)));
	}
}
