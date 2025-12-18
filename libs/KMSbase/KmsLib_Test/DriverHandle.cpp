
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib_Test/DriverHandle.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <fcntl.h>

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>

    #pragma warning ( disable : 4005 )
    #include <ntddser.h>
#endif

// ===== Interface ==========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/DriverHandle.h>
#include <KmsTest.h>

// Constants / Constantes
/////////////////////////////////////////////////////////////////////////////

#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

    #define NULL_DEVICE     "/dev/null"
    #define READ_AND_WRITE  O_RDWR

#endif

#ifdef _KMS_WINDOWS_

    static const GUID GUID_INTERFACE_KEYBOARD = { 0x884b96c3, 0x56ef, 0x11d1, { 0xbc, 0x8c, 0x00, 0xa0, 0xc9, 0x14, 0x05, 0xdd } };

    #define NULL_DEVICE     "nul:"
    #define READ_AND_WRITE  GENERIC_READ | GENERIC_WRITE

#endif

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(DriverHandle_Base)
{
    KmsLib::DriverHandle	lDH0;

    // Always possible to open the NUL device / Toujours possible d'ouvrir
    // le peripherique NUL.
    lDH0.Connect(NULL_DEVICE, READ_AND_WRITE, 0);

    #ifdef _KMS_WINDOWS_
    
        // Nothing to cancel / Rien a canceller
        try
        {
            lDH0.CancelAll();
            KMS_TEST_ERROR();
        }
        catch (KmsLib::Exception * eE)
        {
            KMS_TEST_ERROR_INFO;
            eE->Write(stdout);
            KMS_TEST_COMPARE(KmsLib::Exception::CODE_IO_CANCEL_ERROR, eE->GetCode());
        }
    
    #endif

    // Invalid IOCTL / IOCTL invalid
    try
    {
        unsigned int lInfo_byte = lDH0.Control(0, NULL, 0, NULL, 0);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_IOCTL_ERROR, eE->GetCode());
    }

    #ifdef _KMS_WINDOWS_
    
        // Invalid interface GUID / Identifiant d'interface invalid
        try
        {
            static const GUID GUID_ZERO = { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
    
            lDH0.Connect(GUID_ZERO, 0, GENERIC_READ | GENERIC_WRITE, 0);
            KMS_TEST_ERROR();
        }
        catch (KmsLib::Exception * eE)
        {
            KMS_TEST_ERROR_INFO;
            eE->Write(stdout);
            KMS_TEST_COMPARE(KmsLib::Exception::CODE_NO_SUCH_DEVICE, eE->GetCode());
        }
    
        // No permission / Pas de permission
        try
        {
            lDH0.Connect(GUID_INTERFACE_KEYBOARD, 0, GENERIC_READ | GENERIC_WRITE, 0);
            KMS_TEST_ERROR();
        }
        catch (KmsLib::Exception * eE)
        {
            KMS_TEST_ERROR_INFO;
            eE->Write(stdout);
            KMS_TEST_COMPARE(KmsLib::Exception::CODE_CREATE_FILE_ERROR, eE->GetCode());
        }

        lDH0.mDeviceKey.Open(HKEY_CURRENT_USER, "SOFTWARE");

        try
        {
            lDH0.Connect(GUID_INTERFACE_KEYBOARD, 0, GENERIC_READ | GENERIC_WRITE, KmsLib::DriverHandle::CONNECT_FLAG_OPEN_DEVICE_KEY);
            KMS_TEST_ERROR();
        }
        catch (KmsLib::Exception * eE)
        {
            KMS_TEST_ERROR_INFO;
            eE->Write(stdout);
            KMS_TEST_COMPARE(KmsLib::Exception::CODE_CREATE_FILE_ERROR, eE->GetCode());
        }

        // No permission on the device registry key / Pas de permission pour le
        // cle de registre du peripherique
        try
        {
            lDH0.Connect(GUID_INTERFACE_KEYBOARD, 0, GENERIC_READ | GENERIC_WRITE, KmsLib::DriverHandle::CONNECT_FLAG_OPEN_DEVICE_KEY | KmsLib::DriverHandle::CONNECT_FLAG_ADMINISTRATOR);
            KMS_TEST_ERROR();
        }
        catch (KmsLib::Exception * eE)
        {
            KMS_TEST_ERROR_INFO;
            eE->Write(stdout);
            KMS_TEST_COMPARE(KmsLib::Exception::CODE_REGISTRY_ERROR, eE->GetCode());
        }

    #endif

}
KMS_TEST_END_2

#ifdef _KMS_WINDOWS_

    KMS_TEST_BEGIN(DriverHandle_SetupA)
    {
        KmsLib::DriverHandle	lDH0;

        // Connect to RS232-USB adapted / Connexion a l'adaptateur RS232-USB
        lDH0.Connect(GUID_DEVINTERFACE_COMPORT, 0, GENERIC_READ | GENERIC_WRITE, 0);

        unsigned int lInfo_byte = lDH0.Control(IOCTL_SERIAL_CLEAR_STATS, NULL, 0, NULL, 0);

        KMS_TEST_COMPARE(0, lInfo_byte);

        lDH0.Connect(GUID_DEVINTERFACE_COMPORT, 0, GENERIC_READ | GENERIC_WRITE, KmsLib::DriverHandle::CONNECT_FLAG_OPEN_DEVICE_KEY);

        // No permission on the device registry key / Pas de permission pour le
        // cle de registre du peripherique
        try
        {
            lDH0.Connect(GUID_DEVINTERFACE_COMPORT, 0, GENERIC_READ | GENERIC_WRITE, KmsLib::DriverHandle::CONNECT_FLAG_OPEN_DEVICE_KEY | KmsLib::DriverHandle::CONNECT_FLAG_ADMINISTRATOR);
            KMS_TEST_ERROR();
        }
        catch (KmsLib::Exception * eE)
        {
            KMS_TEST_ERROR_INFO;
            eE->Write(stdout);
            KMS_TEST_COMPARE(KmsLib::Exception::CODE_REGISTRY_ERROR, eE->GetCode());
        }
    }
    KMS_TEST_END_2

    KMS_TEST_BEGIN(DriverHandle_SetupC)
    {
        KmsLib::DriverHandle	lDH0;

        // Connect to RS232-USB adapted / Connexion a l'adaptateur RS232-USB
        lDH0.Connect(GUID_DEVINTERFACE_COMPORT, 0, GENERIC_READ | GENERIC_WRITE, KmsLib::DriverHandle::CONNECT_FLAG_OPEN_DEVICE_KEY);
    }
    KMS_TEST_END_2

#endif
