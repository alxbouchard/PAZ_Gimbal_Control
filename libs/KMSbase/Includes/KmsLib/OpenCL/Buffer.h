
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/KmsLib/OpenCL/Buffer.h

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
		/// \brief	This class extend the cl::Buffer class.
		/// \endcond
		/// \cond	fr
		/// \brief	Cette classe ettend les fonctionnalite de la classe
		///			cl::Buffer.
		/// \endcond
		class Buffer : public cl::Buffer
		{
		public:

			/// \cond	en
			/// \brief	See cl::Buffer::Buffer
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Buffer::Buffer
			/// \endcond
			/// \param	aContext	[in]
			/// \param	aFlags
			/// \param	aSize_byte
			/// \param	aHostPtr	[in,keep,opt,out]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			Buffer(const cl::Context & aContext, cl_mem_flags aFlags, size_t aSize_byte, void * aHostPtr = NULL);

		};

	}
}

