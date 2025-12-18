
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/KmsLib/OpenCL/CommandQueue.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== OpenCL =============================================================
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS 

#include <CL/cl.hpp>

namespace KmsLib
{
	namespace OpenCL
	{

		// Class / Classe
		/////////////////////////////////////////////////////////////////////

		/// \cond	en
		/// \brief	This class extend the cl::CommandQueue class.
		/// \endcond
		/// \cond	fr
		/// \brief	Cette classe ettend les fonctionnalite de la classe
		///			cl::CommandQueue.
		/// \endcond
		class CommandQueue : public cl::CommandQueue
		{
		public:

			/// \cond	en
			/// \brief	See cl::CommandQueue::CommandQueue
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::CommandQueue::CommandQueue
			/// \endcond
			/// \param	aContext	[in ]
			/// \param	aDevice		[in ]
			/// \param	aProperties	[opt]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			CommandQueue(const cl::Context & aContext, const cl::Device & aDevice, cl_command_queue_properties aProperties = 0);

			/// \cond	en
			/// \brief	See cl::CommandQueue::enqueueNDRangeKernel
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::CommandQueue::enqueueNDRangeKernel
			/// \endcond
			/// \param	aKernel
			/// \param	aOffset
			/// \param	aGlobal
			/// \param	aLocal
			/// \param	aEvents		[in,keep,opt]
			///	\param	aNewEvent	[out    ,opt]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			void EnqueueNDRangeKernel(cl::Kernel aKernel, cl::NDRange aOffset, cl::NDRange aGlobal, cl::NDRange aLocal = cl::NullRange, const VECTOR_CLASS<cl::Event> * aEvents = NULL, cl::Event * aNewEvent = NULL);

			/// \cond	en
			/// \brief	See cl::CommandQueue::enqueueNDRangeKernel
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::CommandQueue::enqueueNDRangeKernel
			/// \endcond
			/// \param	aKernel
			/// \param	aOffset
			/// \param	aGlobal
			/// \param	aLocal
			/// \param	aEvents		[in,keep,opt]
			///	\param	aNewEvent	[out    ,opt]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			void EnqueueNDRangeKernel_1D(cl::Kernel aKernel, size_t aOffset, size_t aGlobal, size_t aLocal, const VECTOR_CLASS<cl::Event> * aEvents = NULL, cl::Event * aNewEvent = NULL);

			/// \cond	en
			/// \brief	See cl::CommandQueue::enqueueNDRangeKernel
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::CommandQueue::enqueueNDRangeKernel
			/// \endcond
			/// \param	aKernel
			/// \param	aOffset
			/// \param	aGlobal
			/// \param	aEvents		[in,keep,opt]
			///	\param	aNewEvent	[out    ,opt]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			void EnqueueNDRangeKernel_1D(cl::Kernel aKernel, size_t aOffset, size_t aGlobal, const VECTOR_CLASS<cl::Event> * aEvents = NULL, cl::Event * aNewEvent = NULL);

			/// \cond	en
			/// \brief	See cl::CommandQueue::enqueueReadBuffer
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::CommandQueue::enqueueReadBuffer
			/// \endcond
			/// \param	aBuffer
			/// \param	aBlocking
			/// \param	aOffset_byte
			/// \param	aSize_byte
			/// \param	aOut		[out,keep    ]
			///	\param	aEvents		[in ,keep,opt]
			/// \param	aNewEvents	[out     ,opt]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			void EnqueueReadBuffer(cl::Buffer aBuffer, cl_bool aBlocking, size_t aOffset_byte, size_t aSize_byte, void * aOut, const VECTOR_CLASS<cl::Event> * aEvents = NULL, cl::Event * aNewEvent = NULL);

			/// \cond	en
			/// \brief	See cl::CommandQueue::enqueueWriteBuffer
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::CommandQueue::enqueueWriteBuffer
			/// \endcond
			/// \param	aBuffer
			/// \param	aBlocking
			/// \param	aOffset_byte
			/// \param	aSize_byte
			/// \param	aIn			[in,keep    ]
			///	\param	aEvents		[in,keep,opt]
			/// \param	aNewEvent	[out    ,opt]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			void EnqueueWriteBuffer(cl::Buffer aBuffer, cl_bool aBlocking, size_t aOffset_byte, size_t aSize_byte, const void * aIn, const VECTOR_CLASS<cl::Event> * aEvents = NULL, cl::Event * aNewEvent = NULL);

		};

	}
}

