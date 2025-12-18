
// Author / Auteur		:	KMS	-	Martin Dubois, ing.
// Product / Produit	:	KmsBase
// File / Fichier		:	KmsLib/OpenCL/CommandQueue.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/OpenCL/CommandQueue.h>

namespace KmsLib
{
	namespace OpenCL
	{

		// Public
		/////////////////////////////////////////////////////////////////////

		CommandQueue::CommandQueue(const cl::Context & aContext, const cl::Device & aDevice, cl_command_queue_properties aProperties)
		{
			assert(NULL != (&aContext	));
			assert(NULL != (&aDevice	));

			cl_int lRet;

			(*dynamic_cast<cl::CommandQueue *>(this)) = cl::CommandQueue(aContext, aDevice, aProperties, &lRet);

			if (CL_SUCCESS != lRet)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::CommandQueue::CommandQueue( , , ,  ) failed indicating %d", lRet);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::CommandQueue::CommandQueue( , , ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

		void CommandQueue::EnqueueNDRangeKernel(cl::Kernel aKernel, cl::NDRange aOffset, cl::NDRange aGlobal, cl::NDRange aLocal, const VECTOR_CLASS<cl::Event> * aEvents, cl::Event * aNewEvent)
		{
			cl_int lRet = enqueueNDRangeKernel(aKernel, aOffset, aGlobal, aLocal, aEvents, aNewEvent);
			if (CL_SUCCESS != lRet)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::CommandQueue::enqueueNDRangeKernel( , , , , ,  ) failed returning %d", lRet);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::CommandQueue::enqueueNDRangeKernel( , , , , ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

		void CommandQueue::EnqueueNDRangeKernel_1D(cl::Kernel aKernel, size_t aOffset, size_t aGlobal, size_t aLocal, const VECTOR_CLASS<cl::Event> * aEvents, cl::Event * aNewEvent)
		{
			assert(0 < aGlobal	);
			assert(0 < aLocal	);

			EnqueueNDRangeKernel(aKernel, cl::NDRange(aOffset), cl::NDRange(aGlobal), cl::NDRange(aLocal), aEvents, aNewEvent);
		}

		void CommandQueue::EnqueueNDRangeKernel_1D(cl::Kernel aKernel, size_t aOffset, size_t aGlobal, const VECTOR_CLASS<cl::Event> * aEvents, cl::Event * aNewEvent)
		{
			assert(0 < aGlobal	);

			EnqueueNDRangeKernel(aKernel, cl::NDRange(aOffset), cl::NDRange(aGlobal), cl::NullRange, aEvents, aNewEvent);
		}

		void CommandQueue::EnqueueReadBuffer(cl::Buffer aBuffer, cl_bool aBlocking, size_t aOffset_byte, size_t aSize_byte, void * aOut, const VECTOR_CLASS<cl::Event> * aEvents, cl::Event * aNewEvent)
		{
			assert(0	<	aSize_byte	);
			assert(NULL	!=	aOut		);

			cl_int lRet = enqueueReadBuffer(aBuffer, aBlocking, aOffset_byte, aSize_byte, aOut, aEvents, aNewEvent);
			if (CL_SUCCESS != lRet)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::CommandQueue::EnqueueReadBuffer( , %s, %u bytes, %u bytes, , ,  ) failed returning %d",
					aBlocking ? "CL_TRUE" : "CL_FALSE", static_cast<unsigned int>(aOffset_byte), static_cast<unsigned int>(aSize_byte),	lRet);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::CommandQueue::EnqueueReadBuffer( , , , , , ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
			}
		}

		void CommandQueue::EnqueueWriteBuffer(cl::Buffer aBuffer, cl_bool aBlocking, size_t aOffset_byte, size_t aSize_byte, const void * aIn, const VECTOR_CLASS<cl::Event> * aEvents, cl::Event * aNewEvent)
		{
			assert(0	<	aSize_byte	);
			assert(NULL	!=	aIn			);

			cl_int lRet = enqueueWriteBuffer(aBuffer, aBlocking, aOffset_byte, aSize_byte, aIn, aEvents, aNewEvent);
			if (CL_SUCCESS != lRet)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "cl::CommandQueue::EnqueueWriteBuffer( , %s, %u bytes, %u bytes, , ,  ) failed returning %d",
					aBlocking ? "CL_TRUE" : "CL_FALSE", static_cast<unsigned int>(aOffset_byte), static_cast<unsigned int>(aSize_byte), lRet);

				throw new Exception(Exception::CODE_OPEN_CL_ERROR, "cl::CommandQueue::EnqueueWriteBuffer( , , , , , ,  ) failed",
					lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
			}

		}

	}
}
