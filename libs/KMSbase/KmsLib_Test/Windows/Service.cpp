
// Author / Auteur   KMS - Martin Dubois, ing.
// Project / Projet  KmsBase
// File / Ficheir    KmsLib_Test/Windows/Service.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Windows ============================================================
#include <Windows.h>

// ===== Interface ==========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/Windows/SystemLog.h>

#include <KmsLib/Windows/Service.h>

#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Service_Base)
{
    KmsLib::Windows::Service lS0("Useless");

    try
    {
        lS0.IsEnabled();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS0.IsInstalled();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS0.IsRunning();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS0.GetDelayedAutoStart();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS0.SetDelayedAutoStart(false);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS0.Create("Useless", "Useless");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS0.Delete();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS0.Disable();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS0.Enable();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }
}
KMS_TEST_END_2


KMS_TEST_BEGIN(Service_SetupB)
{
    KmsLib::Windows::Service	lS0("KmsLib_Test");
    KmsLib::Windows::Service	lS1("DoNotExist");
    KmsLib::Windows::SystemLog	lSL0("System", "TODO");

    char lBinary[2048];

    KMS_TEST_COMPARE_RETURN(0, GetCurrentDirectory(sizeof(lBinary), lBinary));

#ifdef _WIN64
    strcat_s(lBinary, "\\x64");
#endif

#ifdef _DEBUG
    strcat_s(lBinary, "\\Debug");
#endif // _DEBUG

#ifdef NDEBUG
    strcat_s(lBinary, "\\Release");
#endif // NDWBUG

    strcat_s(lBinary, "\\Service.exe");

    printf("    ----- Test using the template service -----\n");

    if (lS0.IsInstalled())
    {
        printf("    Test service already existing, deleting it...\n");
        lS0.Delete();
    }

    printf("    Configuring the event sources...\n");
    lSL0.Configure(lBinary);

    printf("    Creating the test service...\n");
    lS0.Create("KmsLib_Test", lBinary);

    KMS_TEST_ASSERT(!lS0.IsEnabled  ());
    KMS_TEST_ASSERT( lS0.IsInstalled());
    KMS_TEST_ASSERT(!lS0.IsRunning  ());

    KMS_TEST_ASSERT(!lS0.GetDelayedAutoStart());

    lS0.SetDelayedAutoStart(true);

    // Disabling a disabled service is OK / Desactiver un service desactive
    // est correct.
    lS0.Disable();

    printf("    Enabling the test service...\n");
    lS0.Enable();

    printf("    Starting the test service...\n");
    lS0.Start();

    printf("    Deleting the test service...\n");
    lS0.Delete();

    printf("    Unconfiguring event sources...\n");
    lSL0.Unconfigure();

    printf("    ----- Test using a not existing service -----\n");

    try
    {
        lS1.IsEnabled();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    KMS_TEST_ASSERT(!lS1.IsInstalled());

    try
    {
        lS1.IsRunning();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS1.Create("", "");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS1.Delete();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS1.Disable();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS1.Enable();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }

    try
    {
        lS1.Start();
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_SERVICE_MANAGER_ERROR, eE->GetCode());
    }
}
KMS_TEST_END_2
