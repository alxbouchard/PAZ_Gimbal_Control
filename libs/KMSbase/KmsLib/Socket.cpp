
// Author / Auteur		KMS	-	Martin Dubois, ing.
// Product / Produit	KmsBase
// File / Fichier		KmsLib/Socket.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// ===== C++ ================================================================
#include <exception>

// ===== Windows ============================================================
#include <WinSock2.h>

// ===== KmsBase ============================================================
#include <KmsLib/Exception.h>

#include <KmsLib/Socket.h>

namespace KmsLib
{

	class SocketInternal
	{

	public:

		sockaddr_in	mLocalAddress;
		SOCKET		mSocket;

	};

	// Public
	/////////////////////////////////////////////////////////////////////////

	void Socket::Thread_Cleanup()
	{
		int lRet = WSACleanup();
		if (0 != lRet)
		{
			char lMsg[1024];

			sprintf_s(lMsg, "WSACleanup() failed returning %d", lRet);

			throw new Exception(Exception::CODE_WINSOCK_ERROR, "WSACleanup() failed", lMsg, __FILE__, __FUNCTION__, __LINE__, WSAGetLastError());
		}
	}

	void Socket::Thread_Init()
	{
		WORD    lVersion = MAKEWORD(2, 2);
		WSADATA lData;

		int lRet = WSAStartup(lVersion, &lData);
		if (0 != lRet)
		{
			// NOT TESTED : Not easy to test
			char lMsg[1024];

			sprintf_s(lMsg, "WSAStartup( ,  ) failed returning %d", lRet);

			throw new Exception(Exception::CODE_WINSOCK_ERROR, "WSACleanup() failed", lMsg, __FILE__, __FUNCTION__, __LINE__, WSAGetLastError());
		}
	}

	Socket * Socket::Select(const SocketList & aRead)
	{
		assert(NULL != (&aRead));

		// NOT TESTED : Not easy to test

		fd_set	lRead;

		FD_ZERO(&lRead);

		SocketList::const_iterator lIt;

		for (lIt = aRead.begin(); lIt != aRead.end(); lIt++)
		{
			FD_SET((*lIt)->GetInternal()->mSocket, &lRead);
		}

		int lRet = select(0, &lRead, NULL, NULL, NULL);
		assert(0 < lRet);

		for (lIt = aRead.begin(); lIt != aRead.end(); lIt++)
		{
			if (FD_ISSET((*lIt)->GetInternal()->mSocket, &lRead))
			{
				return (*lIt);
			}
		}

		assert(false);
		return NULL;
	}

	Socket::Socket() : mInternal(new SocketInternal)
	{
		assert(NULL != mInternal);

		// TODO  Normal  Optimise - Eviter l allocation dynamique

		memset(mInternal, 0, sizeof(SocketInternal));

		mInternal->mSocket = INVALID_SOCKET;
	}

	Socket::Socket(SocketInternal * aInternal) : mInternal(aInternal)
	{
		assert(NULL != mInternal);

		assert(INVALID_SOCKET != mInternal->mSocket);

		// NOT TESTED : Testing it require 2 threads and this is the
		//				normal case.
	}

	Socket::~Socket()
	{
		assert(NULL != mInternal);

		if (INVALID_SOCKET != mInternal->mSocket)
		{
			closesocket(mInternal->mSocket);
		}

		delete mInternal;
	}

	unsigned int Socket::GetLocalAddress() const
	{
		assert(NULL != mInternal);

		return mInternal->mLocalAddress.sin_addr.S_un.S_addr;
	}

	unsigned short Socket::GetLocalPort() const
	{
		assert(NULL != mInternal);

		return ntohs(mInternal->mLocalAddress.sin_port);
	}

	SocketInternal * Socket::Accept(unsigned int aRemoteAddress)
	{
		assert(NULL != mInternal);

		assert(INVALID_SOCKET != mInternal->mSocket);

		sockaddr_in lRemote;

		int lSize_byte = sizeof(lRemote);

		SOCKET lConnect = accept(mInternal->mSocket, reinterpret_cast<sockaddr *>(&lRemote), &lSize_byte);
		if ((INVALID_SOCKET == lConnect) || (sizeof(lRemote) != lSize_byte))
		{
			char lMsg[1024];

			sprintf_s(lMsg, "accept( , ,  ) failed returning 0x%08x", static_cast<unsigned int>(lConnect));

			throw new Exception(Exception::CODE_SOCKET_ERROR, "accept( , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, WSAGetLastError());
		}

		// NOT TESTED :	Testing it require 2 threads and this is the
		//				normal case.
		if ((0 != aRemoteAddress) && (aRemoteAddress != lRemote.sin_addr.S_un.S_addr))
		{
			// We do not accept the connection!
			int lRetI = closesocket(lConnect);
			if (0 != lRetI)
			{
				char lMsg[1024];

				sprintf_s(lMsg, "closesocket(  ) failed returning %d", lRetI);

				throw new Exception(Exception::CODE_SOCKET_ERROR, "closesocket(  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, WSAGetLastError());
			}

			return NULL;
		}

		// TODO  Normal  Optimise - Eviter l'allocation dynamique
		
		SocketInternal * lResult = new SocketInternal;
		assert(NULL != lResult);

		lResult->mLocalAddress	= mInternal->mLocalAddress;
		lResult->mSocket		= lConnect;

		return lResult;
	}

	void Socket::BindAndListen(unsigned int aAddress, unsigned short aPort)
	{
		assert(NULL != mInternal);

		assert(INVALID_SOCKET != mInternal->mSocket);

		Bind(aAddress, aPort);

		int	lSize_byte = sizeof(mInternal->mLocalAddress);

		int lRet = getsockname(mInternal->mSocket, reinterpret_cast<sockaddr *>(&mInternal->mLocalAddress), &lSize_byte);
		if ((0 != lRet) || (sizeof(mInternal->mLocalAddress) != lSize_byte))
		{
			// NOT TESTED	: Not easy to test
			char lMsg[1024];

			sprintf_s(lMsg, "getsockname( , , ) failed returning %d", lRet);

			throw new Exception(Exception::CODE_NETWORK_ERROR, "gethostname( , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, WSAGetLastError());
		}

		Listen();
	}

	bool Socket::Connect(unsigned int aAddress, unsigned short aPort)
	{
		assert(0 != aAddress	);
		assert(0 != aPort		);
		
		sockaddr_in lAddr;

		lAddr.sin_addr.S_un.S_addr	= aAddress		;
		lAddr.sin_family			= AF_INET		;
		lAddr.sin_port				= htons(aPort)	;

		int lRetI = connect(mInternal->mSocket, reinterpret_cast<sockaddr *>(&lAddr), sizeof(lAddr));
		if (0 != lRetI)
		{
			return false;
		}

		// NOT TESTED :	Testing it require 2 threads and this is the
		//				normal case.
		return true;
	}

	void Socket::Create()
	{
		assert(NULL != mInternal);

		assert(INVALID_SOCKET == mInternal->mSocket);

		mInternal->mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == mInternal->mSocket)
		{
			throw new Exception(Exception::CODE_SOCKET_ERROR, "socket( , ,  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, WSAGetLastError());
		}
	}

	void Socket::CreateBindAndListen(unsigned int aAddress, unsigned short aPort)
	{
		Create();
		BindAndListen(aAddress, aPort);
	}

	unsigned int Socket::Receive(void * aOut, unsigned int aOutSize_byte)
	{
		assert(NULL	!=	aOut			);
		assert(0	<	aOutSize_byte	);
		
		assert(NULL != mInternal);

		assert(INVALID_SOCKET != mInternal->mSocket);

		int lResult = recv(mInternal->mSocket, reinterpret_cast<char *>(aOut), aOutSize_byte, 0);
		if (0 > lResult)
		{
			char lMsg[1024];

			sprintf_s(lMsg, "recv( , , %u bytes,  ) failed returning %d", aOutSize_byte, lResult);

			throw new Exception(Exception::CODE_SOCKET_ERROR, "recv( , , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, WSAGetLastError());
		}

		// NOT TESTED :	Testing it require 2 threads and this is the
		//				normal case.
		return lResult;
	}

	void Socket::Send(const void * aIn, unsigned int aInSize_byte)
	{
		assert(NULL	!=	aIn				);
		assert(0	<	aInSize_byte	);

		assert(NULL != mInternal);

		assert(INVALID_SOCKET != mInternal->mSocket);

		int lRet = send(mInternal->mSocket, reinterpret_cast<const char *>(aIn), aInSize_byte, 0);
		if (aInSize_byte != lRet)
		{
			char lMsg[1024];

			sprintf_s(lMsg, "send( , , %u bytes,  ) failed returning %d", aInSize_byte, lRet);

			throw new Exception(Exception::CODE_SOCKET_ERROR, "send( , , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, WSAGetLastError());
		}

		// NOT TESTED :	Testing it require 2 threads and this is the
		//				normal case.
	}

	// Internal
	/////////////////////////////////////////////////////////////////////////

	SocketInternal * Socket::GetInternal()
	{
		assert(NULL != mInternal);

		return mInternal;
	}

	// Private
	/////////////////////////////////////////////////////////////////////////

	// aAddress		Bytes are in network order
	// aPort		Bytes are in host order
	//
	// Exception	KmsLib::Exception	CODE_NETWORK_ERROR
	void Socket::Bind(unsigned int aAddress, unsigned short aPort)
	{
		assert(NULL != mInternal);

		assert(INVALID_SOCKET != mInternal->mSocket);

		memset(&mInternal->mLocalAddress, 0, sizeof(mInternal->mLocalAddress));

		mInternal->mLocalAddress.sin_addr.S_un.S_addr	= aAddress		;
		mInternal->mLocalAddress.sin_family				= AF_INET		;
		mInternal->mLocalAddress.sin_port				= htons(aPort)	;

		int lRet = bind(mInternal->mSocket, reinterpret_cast<sockaddr *>(&mInternal->mLocalAddress), sizeof(mInternal->mLocalAddress));
		if (0 != lRet)
		{
			char lMsg[1024];

			sprintf_s(lMsg, "bind( , %u.%u.%u.%u:%u,  ) failed returning %d",
				mInternal->mLocalAddress.sin_addr.S_un.S_un_b.s_b1,
				mInternal->mLocalAddress.sin_addr.S_un.S_un_b.s_b2,
				mInternal->mLocalAddress.sin_addr.S_un.S_un_b.s_b3,
				mInternal->mLocalAddress.sin_addr.S_un.S_un_b.s_b4,
				ntohs(mInternal->mLocalAddress.sin_port),
				lRet);

			throw new Exception(Exception::CODE_SOCKET_ERROR, "bind( , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, WSAGetLastError());
		}
	}

	// Exception	KmsLib::Exception	CODE_NETWORK_ERROR
	void Socket::Listen()
	{
		assert(NULL != mInternal);

		assert(INVALID_SOCKET != mInternal->mSocket);

		int lRet = listen(mInternal->mSocket, 2);
		if (0 != lRet)
		{
			// NOT TESTED	: Not easy to test
			char lMsg[1024];

			sprintf_s(lMsg, "listen( ,  ) failed returning %d", lRet);

			throw new Exception(Exception::CODE_SOCKET_ERROR, "listen( ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, WSAGetLastError());
		}
	}

}
