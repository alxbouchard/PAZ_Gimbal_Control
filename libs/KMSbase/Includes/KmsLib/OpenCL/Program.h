
// Product / Produit	:	KmsBase

/// \author	KMS -	Martin Dubois, ing.
/// \file	Includes/KmsLib/OpenCL/Program.h

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
		/// \brief	This class extend the cl::Program class.
		/// \endcond
		/// \cond	fr
		/// \brief	Cette classe ettend les fonctionnalite de la classe
		///			cl::Program.
		/// \endcond
		class Program : public cl::Program
		{
		public:

			/// \cond	en
			/// \brief	See cl::Program::Program
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Program::Program
			/// \endcond
			/// \param	aContext	[in]
			/// \param	aSources	[in]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			Program(const cl::Context & aContext, const cl::Program::Sources & aSources);

			/// \cond	en
			/// \brief	See cl::Program::Program
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Program::Program
			/// \endcond
			/// \param	aContext	[in]
			/// \param	aLines		[in]
			/// \param	aLineCount
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			Program(const cl::Context & aContext, const char ** aLines, unsigned int aLineCount);

			/// \cond	en
			/// \brief	See cl::Program::Program
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Program::Program
			/// \endcond
			/// \param	aContext	[in]
			/// \param	aFileName	[in]
			/// \param	aBin
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR, CODE_IO_ERROR
			Program(const cl::Context & aContext, const VECTOR_CLASS<cl::Device> & aDevices, const char * aFileName, bool aBin);

			/// \cond	en
			/// \brief	See cl::Program::getBuildInfo
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Program::getBuildInfo
			/// \endcond
			/// \param	aDevices	[in ]
			/// \param	aName
			/// \param	aOut		[out]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			void	GetBuildInfo(const cl::Device & aDevice, cl_program_build_info aName, std::string * aOut);

			/// \cond	en
			/// \brief	See cl::Program::build
			/// \endcond
			/// \cond	fr
			/// \brief	Voir cl::Program::build
			/// \endcond
			/// \param	aDevices	[in		 ]
			/// \param	aOptions	[in  ,opt]
			/// \param	aNotify		[keep,opt]
			/// \param	aData		[keep,opt]
			/// \exception	KmsLib::Exception	CODE_OPEN_CL_ERROR
			void	Build(const VECTOR_CLASS<cl::Device> & aDevices, const char * aOptions = NULL, void (CL_CALLBACK * aNotify)(cl_program, void *) = NULL, void * aData = NULL) const;

		};

	}
}

