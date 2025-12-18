
// Product / Produit	KmsBase

/// \author	KMS - Martin Dubois
/// \file	Includes/KmsLib/Socket.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C++ ================================================================
#include <list>

namespace KmsLib
{

	class SocketInternal;

	// Class / Classe
	/////////////////////////////////////////////////////////////////////////

	/// \cond	en
	/// \brief	The Socket class
	/// \endcond
	/// \cond	fr
	/// \brief	La classe Socket
	/// \endcond
	class Socket
	{

	public:

		/// \cond	en
		/// \brief	A socket list
		/// \endcond
		/// \cond	fr
		/// \brief	Un liste de socket
		/// \endcond
		typedef std::list<Socket *> SocketList;

		/// \brief	Cleanup a thread used as server
		/// \exception	KmsLib::Exception	CODE_NETWORK_ERROR
		static	void	Thread_Cleanup();

		/// \brief	Prepare a thread to act as a server
		/// \exception	KmsLib::Exception	CODE_NETWORK_ERROR
		static	void	Thread_Init();

		/// \cond	en
		/// \brief	See select.
		/// \param	aSockets	[---;R--]	Socket list
		/// \return	This method returns the first ready socket.
		/// \endcond
		/// \cond	fr
		/// \brief	Voir select.
		/// \param	aSockets	[---;R--]	Une liste de socket
		/// \return	Cette methode retourne l'adresse du socket qui doit etre
		///			servi.
		/// \endcond
		static Socket * Select(const SocketList & aSockets);

		/// \cond	en
		/// \brief	Constructor
		/// \endcond
		/// \cond	fr
		/// \brief	Constructeur
		/// \endcond
		Socket();

		/// \cond	en
		/// \brief	Assignation constructor
		/// \param	aInternal	[-K-;RW-]	The already created internal
		///									structure
		/// \endcond
		/// \cond	fr
		/// \brief	Constructeur utilisant un structure interne deja alloue
		/// \param	aInternal	[-K-;RW-]	La structure interne existante
		/// \endcond
		Socket(SocketInternal * aInternal);

		/// \cond	en
		/// \brief	Destructor
		/// \endcond
		/// \cond	fr
		/// \brief	Destructeur
		/// \endcond
		~Socket();

		/// \cond	en
		/// \brief	Local address accessor
		/// \return	This method return the local address with byte in network
		///			order. 0 indicates any addresses.
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour l'adresse locale
		/// \return	Cette methode retourne l'adresse locale avec les
		///			octets dans l'ordre reseau. Une valeur de 0 indique
		///			que toutes les adresses locales sont accepte.
		/// \endcond
		unsigned int GetLocalAddress() const;

		/// \cond	en
		/// \brief	Local port accessor
		/// \return	This method return the local port with byte in host
		///			order.
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le numbero de port local.
		/// \return	Cette methode retourne le numero de port local avec les
		///			octets dans l'ordre hote.
		/// \endcond
		unsigned short GetLocalPort() const;

		/// \cond	en
		/// \brief	See accept.
		/// \param	aFrom	Only accept connection from this address (byte in
		///					network order). 0 indicates to accept connection
		///					from all address.
		/// \return	Value to pass to an assignation constructor or NULL	if
		///			the connection is not accepted.
		/// \endcond
		/// \cond	fr
		/// \brief	Voir accept.
		/// \param	aFrom	Accepte seulement les connexion qui viennent de
		///					cet adresse. L'adresse doit etre indique avec les
		///					octets dans l'ordre reseau.
		/// \return	Cette method retourne un valeur a passer au constructeur
		///			d'un instance de cette meme classe.
		/// \endcond
		/// \exception	KmsLib::Exception	CODE_NETWORK_ERROR
		SocketInternal * Accept(unsigned int aFrom = 0);

		/// \cond	en
		/// \brief	See bind and listen.
		/// \param	aAddress	Local address with byte in network order. 0
		///						indicate to use any address.
		/// \param	aPort		Local port number with byte in host	order. 0
		///						indicate to use any port.
		/// \endcond
		/// \cond	fr
		/// \brief	Voir bind et listen.
		/// \param	aAddress	L'adresse locale a utiliser avec les octets
		///						dans l'ordre reseau. Une valeur de 0 indique
		///						d'utiliser n'importe la quelle des adresses
		///						locales.
		/// \param	aPort		Le numero de port local avec les octets dans
		///						l'ordre hote. Une valeur de 0 indique
		///						d'utiliser un numero de port assigne
		///						automatiquement.
		/// \endcond
		/// \exception	KmsLib::Exception	CODE_NETWORK_ERROR
		void	BindAndListen(unsigned int aAddress = 0, unsigned short aPort = 0);

		/// \cond	en
		/// \brief	See connect.
		/// \param	aAddress	Address with byte in network order.
		/// \param	aPort		Local port number with byte in host	order.
		/// \retval	false	Cannot connect
		/// \endcond
		/// \cond	fr
		/// \brief	See connect.
		/// \param	aAddress	L'adresse avec les octets en ordre reseau.
		/// \param	aPort		Le numero de port avec les octets en ordre
		///						hote.
		/// \retval	false	Impossible de connecter
		/// \endcond
		/// \retval	true	OK
		bool	Connect(unsigned int aAddress, unsigned short aPort);

		/// \cond	en
		/// \brief	See socket.
		/// \endcond
		/// \cond	fr
		/// \brief	Voir socket.
		/// \endcond
		/// \exception	KmsLib::Exception	CODE_NETWORK_ERROR
		void	Create();
		
		/// \cond	en
		/// \brief	See socket, bind and listen
		/// \param	aAddress	Local address with byte in network order. 0
		///						indicate to use any address.
		/// \param	aPort		Local port number with byte in host	order. 0
		///						indicate to use any port.
		/// \endcond
		/// \cond	fr
		/// \brief	Voir socket
		/// \param	aAddress	L'adresse locale a utiliser avec les octets
		///						dans l'ordre reseau. Une valeur de 0 indique
		///						d'utiliser n'importe la quelle des adresses
		///						locales.
		/// \param	aPort		Le numero de port local avec les octets dans
		///						l'ordre hote. Une valeur de 0 indique
		///						d'utiliser un numero de port assigne
		///						automatiquement.
		/// \endcond
		/// \exception	KmsLib::Exception	CODE_NETWORK_ERROR
		void	CreateBindAndListen(unsigned int aAddress = 0, unsigned short aPort = 0);

		/// \cond	en
		/// \brief	See recv.
		/// \param	aOut	[---;-W-]	Output buffer
		/// \param	aOutSize_byte		Output buffer size
		/// \return	This metho returns the size of received data.
		/// \endcond
		/// \cond	fr
		/// \brief	Void recv.
		/// \param	aOut	[---;-W-]	Espace memoire de sortie
		/// \param	aOutSize_byte		Taille de l'espace memoire de sortie
		/// \endcond
		/// \exception	KmsLib::Exception	CODE_NETWORK_ERROR
		unsigned int Receive(void * aOut, unsigned int aOutSize_byte);
		
		/// \cond	en
		/// \brief	See send.
		/// \param	aIn		[---;R--]	Data to send
		/// \param	aInSize_byte		Size of data to send
		/// \endcond
		/// \cond	fr
		/// \brief	Voir send.
		/// \param	aIn		[---;R--]	Donnees a transmettre
		/// \param	aInSize_byte		Taille des donnees a transmettre
		/// \endcond
		/// \exception	KmsLib::Exception	CODE_NETWORK_ERROR
		void Send(const void * aIn, unsigned int aInSize_byte);
		
	// internal:

		SocketInternal * GetInternal();

	private:

		Socket(const Socket &);

		const Socket & operator = (const Socket &);

		void	Bind	(unsigned int aAddress, unsigned short aPort);
		void	Listen	();

		SocketInternal * mInternal;

	};

}
