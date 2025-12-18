
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/Windows/RegistryKey.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ++ ===============================================================
#include <exception>

// ===== Interface ==========================================================
#include <KmsLib\Exception.h>
#include <KmsLib\Windows\RegistryKey.h>
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

// HKEY_CURRENT_USER\Software
//		KMS
//			KmsLib_Test	= KmsLib_Test
//				ValueA = 1
//				KeyA
//				KeyB
KMS_TEST_BEGIN(RegistryKey_Base)
{
    char lValueSZ[64];

    // ----------------------------------------------------------------------
    KmsLib::Windows::RegistryKey lKey0;

    try
    {
        // The test program do not have access to the HKEY_LOCAL_MACHINE
        // key / Le programme de test n'a pas la permission d'ecrire dans
        // HKEY_LOCAL_MACHINE
        lKey0.Create(HKEY_LOCAL_MACHINE, "KmsLib_Test");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_CREATE_ERROR, eE->GetCode());
    }

    try
    {
        // The key do not exist / La cle n'existe pas
        lKey0.Open(HKEY_LOCAL_MACHINE, "DoNotExist");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_OPEN_ERROR, eE->GetCode());
    }

    lKey0.Open(HKEY_CURRENT_USER, "Software");

    KMS_TEST_ASSERT(!lKey0.DoesSubKeyExist("DoesNotExist"));

    try
    {
        // The key do not exist / La cle n'existe pas
        lKey0.DeleteSubKey("DoNotExist");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_DELETE_ERROR, eE->GetCode());
    }

    try
    {
        // The value do not exist / La valeur n'existe pas
        lKey0.DeleteValue("DoNotExist");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_DELETE_ERROR, eE->GetCode());
    }

    try
    {
        // The key name is not valid / Le nom de la cle n'est pas valide
        lKey0.DoesSubKeyExist("\\NotValid\\");
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
        // The value do not exist / La valeur n'existe pas
        lKey0.GetValue("DoNotExist", lValueSZ, sizeof(lValueSZ));
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_QUERY_ERROR, eE->GetCode());
    }

    // ----------------------------------------------------------------------
    KmsLib::Windows::RegistryKey lKey1;

    lKey1.Create(lKey0, "KMS");

    KMS_TEST_ASSERT(lKey0.DoesSubKeyExist("KMS"));


    // ----------------------------------------------------------------------
    KmsLib::Windows::RegistryKey lKey2;

    lKey2.Create(lKey1, "KmsLib_Test");

    lKey2.SetDefaultValue("DefaultValue");
    lKey2.SetValue("ValueA", 1);

    lKey2.GetValue(NULL, lValueSZ, sizeof(lValueSZ));
    KMS_TEST_COMPARE(0, strcmp("DefaultValue", lValueSZ));

    try
    {
        // ValueA is a DWORD / ValueA est un DWORD
        lKey2.GetValue("ValueA", lValueSZ, sizeof(lValueSZ));
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REGISTRY_ERROR, eE->GetCode());
    }

    KMS_TEST_COMPARE(1, lKey2.GetValue_DWORD("ValueA", 0));
    KMS_TEST_COMPARE(1, lKey2.GetValue_DWORD("DoNotExist", 1));

    try
    {
        // The value is larger than a DWORD / La valeur est plus grande qu'un
        // DWORD
        lKey2.GetValue_DWORD(NULL, 0);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_QUERY_ERROR, eE->GetCode());
    }

    lKey2.SetDefaultValue("");

    try
    {
        // The default value is a SZ / La valeur par defaut est un SZ
        lKey2.GetValue_DWORD(NULL, 0);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REGISTRY_ERROR, eE->GetCode());
    }

    lKey2.DeleteValue("ValueA");

    // ----------------------------------------------------------------------
    KmsLib::Windows::RegistryKey lKey3;

    lKey3.Create(lKey2, "KeyA");
    lKey3.Open  (lKey2, "KeyA");
    lKey3.Create(lKey2, "KeyB");

    KMS_TEST_ASSERT( lKey3.Open(lKey2, static_cast<unsigned int>(0)));
    KMS_TEST_ASSERT( lKey3.Open(lKey2, 1));
    KMS_TEST_ASSERT(!lKey3.Open(lKey2, 2));

    lKey2.DeleteSubKey("KeyA");
    lKey2.DeleteSubKey("KeyB");

    try
    {
        // KeyB no more exist / La cle KeyB n'existe plus
        lKey3.SetValue("KmsLib_Test", 2);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_REG_SET_ERROR, eE->GetCode());
    }
}
KMS_TEST_END
