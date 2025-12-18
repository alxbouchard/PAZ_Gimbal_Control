
// Author / Auteur		:	KMS	-	Martin Dubois, ing.
// Product / Produit	:	KmsBase
// File / Fichier		:	KmsLib/OpenCL/Buffer.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/OpenCL/Buffer.h>

namespace KmsLib
{
	namespace OpenCL
	{

		// Public
		/////////////////////////////////////////////////////////////////////

		Buffer::Buffer(const cl::Context & aContext, cl_mem_flags aFlags, size_t aSize_byte, void * aHostPtr)
		{
			assert(NULL !=	(&aContext)	);
			assert(0	<	aSize_byte	);

			cl_int lRet;

			(* dynamic_cast<cl::Buffer *>(this)) = cl::Buffer(aContext, aFlags, aSize_byte, aHostPtr, &lRet);

			if (CL_SUCCESS != lRet)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::Buffer::Buffer( , 0x%08x, %u bytes, ,  ) failed indicating %d",
					static_cast<unsigned int>(aFlags), static_cast<unsigned int>(aSize_byte), lRet);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::Buffer::Buffer( , , , ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

	}
}
