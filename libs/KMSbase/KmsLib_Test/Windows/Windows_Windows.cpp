
// Auteur   KMS - Martin Dubois, ing.
// Produit  KmsBase
// Fichier  KmsLib_Test/Windows_Windows_Base.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Windows ============================================================
#include <Windows.h>

// ===== KmsBase ============================================================
#include <KmsLib/Exception.h>
#include <KmsLib/Windows/Windows.h>
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Windows_Windows_Base)
{
    try
    {
        KmsLib::Windows::Extension_Register(".kmslibtest", "kmslibtest", KmsLib::Windows::COMMAND_EDIT, "notepad.exe %1");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_OPEN_ERROR, eE->GetCode());
    }

    try
    {
        KmsLib::Windows::Extension_Unregister(".kmslibtest", "kmslibtest");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_OPEN_ERROR, eE->GetCode());
    }

    try
    {
        KmsLib::Windows::ShellDirectory_Register("kmslibtest", "kmslibtest", "notepad.exe %1");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_OPEN_ERROR, eE->GetCode());
    }

    try
    {
        KmsLib::Windows::ShellDirectory_Unregister("kmslibtest");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_OPEN_ERROR, eE->GetCode());
    }
}
KMS_TEST_END_2


KMS_TEST_BEGIN(Windows_Windows_SetupB)
{
    KmsLib::Windows::Extension_Unregister(".kmslibtest", "kmslibtest", KmsLib::Windows::COMMAND_EDIT);
    KmsLib::Windows::Extension_Register  (".kmslibtest", "kmslibtest", KmsLib::Windows::COMMAND_EDIT, "notepad.exe %1");
    KmsLib::Windows::Extension_Unregister(".kmslibtest", "kmslibtest", KmsLib::Windows::COMMAND_EDIT);
    KmsLib::Windows::Extension_Unregister(".kmslibtest", "kmslibtest");

    KmsLib::Windows::ShellDirectory_Unregister("kmslibtest");
    KmsLib::Windows::ShellDirectory_Register  ("kmslibtest", "kmslibtest", "notepad.exe %1");
    KmsLib::Windows::ShellDirectory_Unregister("kmslibtest");
}
KMS_TEST_END_2
