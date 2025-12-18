
// Author / Auteur		:	KMS	-	Martin Dubois, ing.
// Product / Produit	:	KmsBase
// File / Fichier		:	KmsLib/OpenCL/Kernel.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/OpenCL/Kernel.h>

namespace KmsLib
{
	namespace OpenCL
	{

		// Public
		/////////////////////////////////////////////////////////////////////

		Kernel::Kernel(const cl::Program & aProgram, const char * aName)
		{
			assert(NULL != (&aProgram)	);
			assert(NULL	<	aName		);

			cl_int lRet;

			(*dynamic_cast<cl::Kernel *>(this)) = cl::Kernel(aProgram, aName, &lRet);

			if (CL_SUCCESS != lRet)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::Kernel::Kernel( , \"%s\" ) failed indicating %d", aName, lRet);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::Kernel::Kernel( , ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

		void Kernel::SetArg(cl_uint aIndex, cl::Buffer aBuffer)
		{
			cl_int lRet = setArg(aIndex, aBuffer);
			if (CL_SUCCESS != lRet)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::Kernel::setArg( %u,  ) failed returning %d", aIndex, lRet);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::Kernel::setArg( ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

		void Kernel::GetWorkGroupInfo(const cl::Device & aDevice, cl_kernel_work_group_info aName, size_t * aOut)
		{
			assert(NULL != (&aDevice)	);
			assert(NULL != aOut			);

			cl_int lRet = getWorkGroupInfo(aDevice, aName, aOut);
			if (CL_SUCCESS != lRet)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::Kernel::getWorkGroupInfo( , %u,  ) failed returning %d", aName, lRet);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::Kernel::getWorkGroupInfo( , ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

	}
}
