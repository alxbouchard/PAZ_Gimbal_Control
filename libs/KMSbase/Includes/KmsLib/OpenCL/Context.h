
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/KmsLib/OpenCL/Context.h

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
		/// \brief	This class extend the cl::Context class.
		/// \endcond
		/// \cond	fr
		/// \brief	Cette classe ettend les fonctionnalite de la classe
		///			cl::Context.
		/// \endcond
		class Context : public cl::Context
		{
		public:

			/// \cond	en
			/// \brief	See cl::Context::Context
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Context::Context
			/// \endcond
			/// \param	aDevices	[in    ]
			/// \param	aProperties	[in,opt]
			/// \param	aNotify		[   opt]
			/// \param	aData		[   opt]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			Context(const VECTOR_CLASS<cl::Device> & aDevices, cl_context_properties * aProperties = NULL, void (CL_CALLBACK * aNotify)(const char *, const void *, size_t, void *) = NULL, void * aData = NULL);

			/// \cond	en
			/// \brief	See cl::Context::Context
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Context::Context
			/// \endcond
			/// \param	aDeviceType
			/// \param	aDevices	[out]
			/// \param	aProperties	[in,opt]
			/// \param	aNotify		[   opt]
			/// \param	aData		[   opt]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			Context(cl_device_type aDeviceType, cl_context_properties * aProperties = NULL, void (CL_CALLBACK * aNotify)(const char *, const void *, size_t, void *) = NULL, void * aData = NULL);

			/// \cond	en
			/// \brief	See cl::Context::getInfo
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Context::getInfo
			/// \endcond
			/// \param	aName
			/// \param	aOut	[out]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			void GetInfo(cl_context_info aName, cl_uint * aOut);

			/// \cond	en
			/// \brief	See cl::Context::getInfo
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Context::getInfo
			/// \endcond
			/// \param	aName
			/// \param	aOut	[out]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			void GetInfo(cl_context_info aName, VECTOR_CLASS<cl::Device> * aOut);
		};

	}
}

