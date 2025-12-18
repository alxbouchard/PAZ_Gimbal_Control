
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/NetworkAddress.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Windows ============================================================
#include <WinSock2.h>

// ===== Interface ==========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/NetworkAddress.h>
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(NetworkAddress_Base)
{
    WSADATA lData;

    KMS_TEST_COMPARE(0, WSAStartup(MAKEWORD(2, 2), &lData));

    KmsLib::NetworkAddress lNA0;

    sockaddr	* lSA  = lNA0;
    sockaddr_in	* lSAI = lNA0;

    const sockaddr		* lCSA  = (const KmsLib::NetworkAddress)(lNA0);
    const sockaddr_in	* lCSAI = (const KmsLib::NetworkAddress)(lNA0);

    KMS_TEST_ASSERT(!lNA0.IsBroadcast());
    KMS_TEST_ASSERT(!lNA0.IsLocalHost());
    KMS_TEST_ASSERT(!lNA0.IsMulticast());
    KMS_TEST_ASSERT( lNA0.IsUnicast  ());

    KMS_TEST_COMPARE(0, lNA0.GetAddr());
    KMS_TEST_COMPARE(0, lNA0.GetPort());

    KMS_TEST_COMPARE(static_cast< int >( sizeof(sockaddr_in) ), lNA0.GetSize());

    lNA0.Set(0xffffffff);

    KMS_TEST_ASSERT(lNA0.IsBroadcast());

    KMS_TEST_COMPARE(0xffffffff, lNA0.GetAddr());

    lNA0.Set(0xffc0c1c2, 0xc2c1);

    KMS_TEST_ASSERT(!lNA0.IsBroadcast());
    KMS_TEST_ASSERT(lNA0.IsBroadcast(0x00ffffff));

    KMS_TEST_COMPARE(0xc2c1, lNA0.GetPort());

    unsigned char lArray[4];

    lArray[0] = 0xe0;
    lArray[1] = 0xe1;
    lArray[2] = 0xe2;
    lArray[3] = 0xe3;

    lNA0.Set(lArray);

    KMS_TEST_ASSERT(lNA0.IsMulticast());

    lNA0.Set(lArray, 0xe1e0);

    lNA0.Set(0xc3, 0xc4, 0xc5, 0xc6);
    lNA0.Set(0xc3, 0xc4, 0xc5, 0xc6, 0xc4c3);

    lNA0.Set("127.0.0.1");

    KMS_TEST_ASSERT(lNA0.IsLocalHost());

    lNA0.Set("127.0.0.1:8081");

    lNA0.Set("www.kms-quebec.com");

    lNA0.ResetPort();

    try
    {
        lNA0.Set("127.0.0.1:65537");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_INVALID_PORT_NUMBER, eE->GetCode());
    }

    try
    {
        lNA0.Set("DowsNotExist");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_NETWORK_ERROR, eE->GetCode());
    }

    try
    {
        lNA0.Set("");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_INVALID_ADDRESS, eE->GetCode());
    }

    try
    {
        lNA0.SetPortNumber(0);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_INVALID_PORT_NUMBER, eE->GetCode());
    }

    try
    {
        lNA0.Set("0.0.0.0");
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_INVALID_ADDRESS, eE->GetCode());
    }

    KMS_TEST_COMPARE(0, WSACleanup());
}
KMS_TEST_END_2
