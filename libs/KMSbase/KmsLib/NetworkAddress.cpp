
// Author / Auteur		KMS	-	Martin Dubois, ing
// Product / Produit	KmsBase
// File / Fichier		KmsLib/NetworkAddress.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>

// ===== Windows ============================================================
#include <WS2tcpip.h>

// ===== Includes ===========================================================
#include "../Includes/KmsLib/Exception.h"

#include "../Includes/KmsLib/NetworkAddress.h"

namespace KmsLib
{

	// Public
	/////////////////////////////////////////////////////////////////////////

	NetworkAddress::NetworkAddress()
	{
		memset(&mAddr, 0, sizeof(mAddr));

		mAddr.sin_family = AF_INET;
	}

	NetworkAddress::operator sockaddr * ()
	{
		return reinterpret_cast<sockaddr *>(&mAddr);
	}

	NetworkAddress::operator const sockaddr * () const
	{
		return reinterpret_cast<const sockaddr *>(&mAddr);
	}

	NetworkAddress::operator sockaddr_in * ()
	{
		return reinterpret_cast<sockaddr_in *>(&mAddr);
	}

	NetworkAddress::operator const sockaddr_in * () const
	{
		return reinterpret_cast<const sockaddr_in *>(&mAddr);
	}

	bool NetworkAddress::IsBroadcast(unsigned int aNetMask) const
	{
		return (0xffffffff == (aNetMask | mAddr.sin_addr.S_un.S_addr));
	}

	bool NetworkAddress::IsLocalHost() const
	{
		return ((0x0000007f == (0x000000ff & mAddr.sin_addr.S_un.S_addr)) && (!IsBroadcast(0x000000ff)));
	}

	bool NetworkAddress::IsMulticast() const
	{
		return (0x000000e0 == (0x000000ff & mAddr.sin_addr.S_un.S_addr));
	}

	bool NetworkAddress::IsUnicast() const
	{
		return ((!IsBroadcast()) && (!IsMulticast()));
	}

	unsigned int NetworkAddress::GetAddr() const
	{
		return mAddr.sin_addr.S_un.S_addr;
	}

	unsigned short NetworkAddress::GetPort() const
	{
		return ntohs(mAddr.sin_port);
	}

	int NetworkAddress::GetSize() const
	{
		return sizeof(mAddr);
	}

	void NetworkAddress::Set(unsigned int aAddr)
	{
		mAddr.sin_addr.S_un.S_addr = aAddr;

		Validate();
	}

	void NetworkAddress::Set(unsigned int aAddr, unsigned short aPortNumber)
	{
		assert(aPortNumber != 0);

		SetPortNumber(aPortNumber);

		Set(aAddr);
	}

	void NetworkAddress::Set(const unsigned char * aAddr)
	{
		assert(NULL != aAddr);

		mAddr.sin_addr.S_un.S_un_b.s_b1 = aAddr[0];
		mAddr.sin_addr.S_un.S_un_b.s_b2 = aAddr[1];
		mAddr.sin_addr.S_un.S_un_b.s_b3 = aAddr[2];
		mAddr.sin_addr.S_un.S_un_b.s_b4 = aAddr[3];

		Validate();
	}

	void NetworkAddress::Set(const unsigned char * aAddr, unsigned short aPortNumber)
	{
		assert(NULL	!= aAddr		);
		assert(0	!= aPortNumber	);

		SetPortNumber(aPortNumber);

		Set(aAddr);
	}

	void NetworkAddress::Set(unsigned char aA, unsigned char aB, unsigned char aC, unsigned char aD)
	{
		mAddr.sin_addr.S_un.S_un_b.s_b1 = aA;
		mAddr.sin_addr.S_un.S_un_b.s_b2 = aB;
		mAddr.sin_addr.S_un.S_un_b.s_b3 = aC;
		mAddr.sin_addr.S_un.S_un_b.s_b4 = aD;

		Validate();
	}

	void NetworkAddress::Set(unsigned char aA, unsigned char aB, unsigned char aC, unsigned char aD, unsigned short aPortNumber)
	{
		assert(0 != aPortNumber);

		SetPortNumber(aPortNumber);

		Set(aA, aB, aC, aD);
	}

	void NetworkAddress::Set(const char * aAddr)
	{
		assert(NULL != aAddr);

		unsigned int	lAddr		;
		unsigned int	lPortNumber	;
		int				lRetI		;

		char lMsg	[1024];
		char lName	[1024];

		lRetI = sscanf_s(aAddr, "%[^: /\\\n\r\t]:%u", lName SIZE_INFO(sizeof(lName) / sizeof(lName[0])), &lPortNumber);
		switch (lRetI)
		{
		case 2 :
			if (0xffff < lPortNumber)
			{
				sprintf_s(lMsg, "%u is not a valid port number (%d)", lPortNumber, lRetI);

				throw new Exception(Exception::CODE_INVALID_PORT_NUMBER, "Invalid port number", lMsg, __FILE__, __FUNCTION__, __LINE__, lPortNumber);
			}

			SetPortNumber(lPortNumber);

			// No break;

		case 1 :
			if (1 != inet_pton(AF_INET, lName, &lAddr))
			{
				ADDRINFOA	lHints	;
				PADDRINFOA	lInfo	;

				memset(&lHints, 0, sizeof(lHints));

				lHints.ai_family = AF_INET;

				lRetI = getaddrinfo(lName, NULL, NULL, &lInfo);
				if (0 != lRetI)
				{
					sprintf_s(lMsg, "getaddrinfo( \"%s\", , ,  ) failed returning %d", lName, lRetI);

					throw new Exception(Exception::CODE_NETWORK_ERROR, "getaddrinfo( , , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, lRetI);
				}

				assert(AF_INET	== lInfo->ai_addr->sa_family	);
				assert(AF_INET	== lInfo->ai_family				);

				memcpy(&mAddr.sin_addr.S_un.S_addr, &reinterpret_cast<sockaddr_in *>(lInfo->ai_addr)->sin_addr.S_un.S_addr, sizeof(mAddr.sin_addr.S_un.S_addr));

				Validate();
			}
			else
			{
				Set(lAddr);
			}
			break;

		default :
			sprintf_s(lMsg, "\"%s\" is not a valid name", aAddr);

			throw new Exception(Exception::CODE_INVALID_ADDRESS, "Invalid name", lMsg, __FILE__, __FUNCTION__, __LINE__, lRetI);
		}
	}

	void NetworkAddress::SetPortNumber(unsigned short aPortNumber)
	{
		if (0 == aPortNumber)
		{
			throw new Exception(Exception::CODE_INVALID_PORT_NUMBER, "Invalid port", NULL, __FILE__, __FUNCTION__, __LINE__, aPortNumber);
		}

		mAddr.sin_port = htons(aPortNumber);
	}

	void NetworkAddress::ResetPort()
	{
		mAddr.sin_port = 0;
	}

	// Private
	/////////////////////////////////////////////////////////////////////////

	void NetworkAddress::Validate() const
	{
		if (0 == mAddr.sin_addr.S_un.S_addr)
		{
			throw new Exception(Exception::CODE_INVALID_ADDRESS, "The address is not valid", NULL, __FILE__, __FUNCTION__, __LINE__, mAddr.sin_addr.S_un.S_addr);
		}
	}

}
