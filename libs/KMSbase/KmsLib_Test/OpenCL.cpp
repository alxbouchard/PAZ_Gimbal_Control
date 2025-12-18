
// Author / Auteur		:	KMS -	Martin Dubois, ing.
// Product / Produit	:	KmsBase
// File / Fichier		:	KmsLib_Test/OpenCL.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// #include <KmsBase.h>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/OpenCL/Buffer.h>
#include <KmsLib/OpenCL/CommandQueue.h>
#include <KmsLib/OpenCL/Context.h>
#include <KmsLib/OpenCL/Kernel.h>
#include <KmsLib/OpenCL/Program.h>
#include <KmsTest.h>

// Kernels
/////////////////////////////////////////////////////////////////////////////

static const char * KERNEL0[] =
{
	"__kernel void Kernel0()\n",
	"{\n"
	"}\n"
};

static const char * KERNEL1[] =
{
	"Error!\n",
};

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(OpenCL_Base)

	unsigned char				lBuffer[128];
	VECTOR_CLASS<cl::Device>	lDevices	;
	size_t						lSize		;
	std::string					lString		;

	// ===== Context ========================================================

	KmsLib::OpenCL::Context lC0(CL_DEVICE_TYPE_CPU);

	try
	{
		KmsLib::OpenCL::Context lC1(CL_DEVICE_TYPE_ACCELERATOR);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	try
	{
		KmsLib::OpenCL::Context lC2(lDevices);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	cl_uint lDeviceCount;

	lC0.GetInfo(CL_CONTEXT_NUM_DEVICES, &lDeviceCount);
	KMS_TEST_ASSERT(0 < lDeviceCount);

	try
	{
		lC0.GetInfo(0, &lDeviceCount);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	lC0.GetInfo(CL_CONTEXT_DEVICES, &lDevices);

	try
	{
		lC0.GetInfo(0, &lDevices);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	// ===== Buffer =========================================================
	// Un Context est necessaire pour tester cette class. / A Context is
	// needed in order to test this class.

	KmsLib::OpenCL::Buffer lB0(lC0, 0, 1024);

	try
	{
		KmsLib::OpenCL::Buffer lB1(cl::Context(), 0, 1024);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	// ===== Program ========================================================

	KmsLib::OpenCL::Program lP0(lC0, KERNEL0, sizeof(KERNEL0) / sizeof(KERNEL0[0]));

	try
	{
		KmsLib::OpenCL::Program lP1(cl::Context(), KERNEL0, sizeof(KERNEL0) / sizeof(KERNEL0[0]));
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	try
	{
		KmsLib::OpenCL::Program lP2(cl::Context(), lDevices, "DoNotExist", false );
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_IO_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	KmsLib::OpenCL::Program lP3(lC0, lDevices, "KmsLib_Test/OpenCL.cpp", false	);

	try
	{
		KmsLib::OpenCL::Program lP4(lC0, lDevices, "KmsLib_Test/OpenCL.cpp", true);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	lP0.GetBuildInfo(lDevices.front(), CL_PROGRAM_BUILD_LOG, &lString);

	try
	{
		lP0.GetBuildInfo(lDevices.front(), 0, &lString);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	lP0.Build(lDevices);

	KmsLib::OpenCL::Program lP1(lC0, KERNEL1, sizeof(KERNEL1) / sizeof(KERNEL1[0]));

	try
	{
		lP1.Build(lDevices);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	// ===== Kernel =========================================================
	// Un Program est necessaire pour tester cette class. / A Program is
	// needed in order to test this class.

	KmsLib::OpenCL::Kernel lK0(lP0, "Kernel0");

	try
	{
		lK0.SetArg(0, lB0);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	lK0.GetWorkGroupInfo(lDevices.front(), CL_KERNEL_WORK_GROUP_SIZE, &lSize);
	KMS_TEST_ASSERT(0 < lSize);

	try
	{
		lK0.GetWorkGroupInfo(lDevices.front(), 0, &lSize);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	// ===== CommandQueue ===================================================
	// Un Kernel est necessaire pour tester cette class. / A Kernel is needed
	// in order to test this class.

	KmsLib::OpenCL::CommandQueue lCQ0(lC0, lDevices.front());

	try
	{
		KmsLib::OpenCL::CommandQueue lCQ0(cl::Context(), lDevices.front());
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	lCQ0.EnqueueNDRangeKernel_1D(lK0, 0, 1);

	try
	{
		lCQ0.EnqueueNDRangeKernel_1D(cl::Kernel(), 0, 1, 1);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	lCQ0.EnqueueReadBuffer(lB0, CL_TRUE, 0, sizeof(lBuffer), lBuffer);

	try
	{
		lCQ0.EnqueueReadBuffer(cl::Buffer(), CL_TRUE, 0, sizeof(lBuffer), lBuffer);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

	lCQ0.EnqueueWriteBuffer(lB0, CL_TRUE, 0, sizeof(lBuffer), lBuffer);

	try
	{
		lCQ0.EnqueueWriteBuffer(cl::Buffer(), CL_TRUE, 0, sizeof(lBuffer), lBuffer);
		KMS_TEST_ASSERT(false);
	}
	catch (KmsLib::Exception * eE)
	{
		KMS_TEST_ASSERT(KmsLib::Exception::CODE_OPEN_CL_ERROR == eE->GetCode());
		KMS_TEST_ERROR_INFO;
		eE->Write(stderr);
	}

KMS_TEST_END_2
