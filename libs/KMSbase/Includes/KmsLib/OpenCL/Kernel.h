
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/KmsLib/OpenCL/Kernel.h

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
		/// \brief	This class extend the cl::kernel class.
		/// \endcond
		/// \cond	fr
		/// \brief	Cette classe ettend les fonctionnalite de la classe
		///			cl::kernel.
		/// \endcond
		class Kernel : public cl::Kernel
		{
		public:

			/// \cond	en
			/// \brief	See cl::Kernel::Kernel
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Kernel::Kernel
			/// \endcond
			/// \param	aProgram	[in]
			/// \param	aBuffer		[in]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			Kernel(const cl::Program & aProgram, const char * aName);

			/// \cond	en
			/// \brief	See cl::Kernel::setArg
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Kernel::setArg
			/// \endcond
			/// \param	aIndex
			/// \param	aBuffer
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			void SetArg(cl_uint aIndex, cl::Buffer aBuffer);

			/// \cond	en
			/// \brief	See cl::Kernel::getWorkGroupInfo
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Kernel::getWorkGroupInfo
			/// \endcond
			/// \param	aDevice	[in ]
			/// \param	aName
			/// \param	aOut	[out]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			void GetWorkGroupInfo(const cl::Device & aDevice, cl_kernel_work_group_info aName, size_t * aOut);

		};

	}
}

