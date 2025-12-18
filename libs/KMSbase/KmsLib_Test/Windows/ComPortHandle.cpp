
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/Windows/ComPortHandle.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Windows ============================================================
#include <Windows.h>

// ===== Interface ==========================================================
#include <KmsLib\Exception.h>
#include <KmsLib\Windows\ComPortHandle.h>
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(ComPortHandle_Base)
{
    KmsLib::Windows::ComPortHandle lCPH0;

    lCPH0.SetBaudRate(KmsLib::Windows::ComPortHandle::BAUD_RATE_9600);

    KMS_TEST_COMPARE(KmsLib::Windows::ComPortHandle::BAUD_RATE_9600, lCPH0.GetBaudRate());

    try
    {
        lCPH0.Connect("", GENERIC_READ);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_CREATE_FILE_ERROR, eE->GetCode());
    }

    try
    {
        lCPH0.Connect_ByNumber(19, GENERIC_READ);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_CREATE_FILE_ERROR, eE->GetCode());
    }

    try
    {
        lCPH0.Connect("NUL", GENERIC_READ);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_COM_PORT_ERROR, eE->GetCode());
    }
}
KMS_TEST_END_2


KMS_TEST_BEGIN(ComPortHandle_SetupA)
{
    KmsLib::Windows::ComPortHandle lCPH0;

    lCPH0.Connect(GENERIC_READ);

    lCPH0.SetBaudRate(KmsLib::Windows::ComPortHandle::BAUD_RATE_DEFAULT);
}
KMS_TEST_END_2
