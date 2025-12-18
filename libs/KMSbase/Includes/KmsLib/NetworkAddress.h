
// Author / Auteur		:	KMS	-	Martin Dubois, ing
// Product / Produit	:	KmsBase
// File / Fichier		:	Includes/KmsLib/NetworkAddress.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

namespace KmsLib
{

	// Class / Classe
	/////////////////////////////////////////////////////////////////////////

	/// \cond	en
	/// \brief	This class wrap a sockaddr_in structure.
	/// \endcond
	/// \cond	fr
	/// \brief	Cette classe encapsule une structure sockaddr_in.
	/// \endcond
	class NetworkAddress
	{

	public:

		/// \cond	en
		/// \brief	Constructor
		/// \endcond
		/// \cond	fr
		/// \brief	Constructeur
		/// \endcond
		NetworkAddress();

		/// \cond	en
		/// \brief	Cast operator
		/// \endcond
		/// \cond	fr
		/// \brief	Operateur de conversion
		/// \endcond
		operator sockaddr * ();

		/// \cond	en
		/// \brief	Cast operator
		/// \endcond
		/// \cond	fr
		/// \brief	Operateur de conversion
		/// \endcond
		operator const sockaddr * () const;

		/// \cond	en
		/// \brief	Cast operator
		/// \endcond
		/// \cond	fr
		/// \brief	Operateur de conversion
		/// \endcond
		operator sockaddr_in * ();

		/// \cond	en
		/// \brief	Cast operator
		/// \endcond
		/// \cond	fr
		/// \brief	Operateur de conversion
		/// \endcond
		operator const sockaddr_in * () const;

		/// \cond	en
		/// \brief	Is the address a broadcast address?
		/// \param	aNetMask	Net mask to use
		/// \retval	false
		/// \retval	true
		/// \endcond
		/// \cond	fr
		/// \brief	Est-ce que l'adresse est une adresse de broadcast?
		/// \param	aNetMask	Le masque de sous reseau a utiliser
		/// \retval	false
		/// \retval	true
		/// \endcond
		bool	IsBroadcast(unsigned int aNetMask = 0) const;

		/// \cond	en
		/// \brief	Is the address a local host address?
		/// \retval	false
		/// \retval	true
		/// \endcond
		/// \cond	fr
		/// \brief	Est-ce que l'adresse est une adresse de l'ordinateur local?
		/// \retval	false
		/// \retval	true
		/// \endcond
		bool	IsLocalHost() const;

		/// \cond	en
		/// \brief	Is the address a multicast address?
		/// \retval	false
		/// \retval	true
		/// \endcond
		/// \cond	fr
		/// \brief	Est-ce que l'adresse est une adresse de multicast?
		/// \retval	false
		/// \retval	true
		/// \endcond
		bool	IsMulticast() const;

		/// \cond	en
		/// \brief	Is the address a unicast address?
		/// \retval	false
		/// \retval	true
		/// \endcond
		/// \cond	fr
		/// \brief	Est-ce que l'adresse est une adresse de unicast?
		/// \retval	false
		/// \retval	true
		/// \endcond
		bool	IsUnicast() const;

		/// \cond	en
		/// \brief	Address accessor
		/// \return	This method return the 32 bits address with byte in
		///			network order.
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour l'adresse
		/// \return	Cette methode retourne l'adresse de 32 bits avec les
		///			octets ordonnee dans l'ordre reseau.
		/// \endcond
		unsigned int	GetAddr() const;

		/// \cond	en
		/// \brief	Port accessor
		/// \return	This method return the 16 bits port number with byte in
		///			host order.
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le port
		/// \return	Cette methode retourne le numero de port de 16 bits avec
		///			les	octets ordonnee dans l'ordre hote.
		/// \endcond
		unsigned short	GetPort() const;

		/// \cond	en
		/// \brief	Size accessor
		/// \return	This method return the size of the structure in bytes.
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour la taille
		/// \return	Cette methode retourne la taille de la structure en
		///			octets.
		/// \endcond
		int	GetSize() const;

		/// \cond	en
		/// \brief	Address accessor
		/// \param	aAddr	32 bits address with byte in network order
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour l'adresse
		/// \param	aAddr	Adresse de 32 bits avec les octets en ordre
		///					reseau
		/// \endcond
		/// \exception	Exception	CODE_INVALID_ARGUMENT
		void	Set(unsigned int aAddr);

		/// \cond	en
		/// \brief	Address accessor
		/// \param	aAddr		32 bits address with byte in network order
		/// \param	aPortNumber	16 bits port number with byte in host order
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour l'adresse
		/// \param	aAddr		Adresse de 32 bits avec les octets en ordre
		///						reseau
		/// \param	aPortNumber	Numero de port de 16 bits avec les octets en
		///						ordre hote
		/// \endcond
		/// \exception	Exception	CODE_INVALID_ARGUMENT
		void	Set(unsigned int aAddr, unsigned short aPortNumber);

		/// \cond	en
		/// \brief	Address accessor
		/// \param	aAddr	Array containing the 32 bits address with byte in
		///					network order.
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour l'adresse
		/// \param	aAddr	Vecteur contenant l'adresse de 32 bits avec les
		///					octets dans l'ordre reseau
		/// \endcond
		/// \exception	Exception	CODE_INVALID_ARGUMENT
		void	Set(const unsigned char * aAddr);

		/// \cond	en
		/// \brief	Address accessor
		/// \param	aAddr		Array containing the 32 bits address with
		///						byte in	network order.
		/// \param	aPortNumber	16 bits port number with byte in host order
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour l'adresse
		/// \param	aAddr		Vecteur contenant l'adresse de 32 bits avec
		///						les	octets dans l'ordre reseau
		/// \param	aPortNumber	Numero de port de 16 bits avec les octets en
		///						ordre hote
		/// \endcond
		/// \exception	Exception	CODE_INVALID_ARGUMENT
		void	Set(const unsigned char * aAddr, unsigned short aPortNumber);

		/// \cond	en
		/// \brief	Address accessor
		/// \param	aA	Byte 1
		/// \param	aB	Byte 2
		/// \param	aC	Byte 3
		/// \param	aD	Byte 4
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour l'adresse
		/// \param	aA	Octet 1
		/// \param	aB	Octet 2
		/// \param	aC	Octet 3
		/// \param	aD	Octet 4
		/// \endcond
		/// \exception	Exception	CODE_INVALID_ARGUMENT
		void	Set(unsigned char aA, unsigned char aB, unsigned char aC, unsigned char aD);

		/// \cond	en
		/// \brief	Address accessor
		/// \param	aA			Byte 1
		/// \param	aB			Byte 2
		/// \param	aC			Byte 3
		/// \param	aD			Byte 4
		/// \param	aPortNumber	16 bits port number with byte in host order
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour l'adresse
		/// \param	aA			Octet 1
		/// \param	aB			Octet 2
		/// \param	aC			Octet 3
		/// \param	aD			Octet 4
		/// \param	aPortNumber	Numero de port de 16 bits avec les octets en
		///						ordre hote
		/// \endcond
		/// \exception	Exception	CODE_INVALID_ARGUMENT
		void	Set(unsigned char aA, unsigned char aB, unsigned char aC, unsigned char aD, unsigned short aPortNumber);

		/// \cond	en
		/// \brief	Address accessor
		/// \param	aIn	Address in a human readable format.
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour l'adresse
		/// \param	aIn	L'adresse dans un format lisible.
		/// \endcond
		/// \exception	Exception	CODE_INVALID_ARGUMENT, CODE_NETWORK_ERROR
		void	Set(const char * aIn);

		/// \cond	en
		/// \brief	Port accessor
		/// \param	aPortNumber	16 bits port number with bytes in host order.
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le port
		/// \param	aPortNumber	Le numero de port de 16 bit avec les octets
		///						dans l'ordre hote.
		/// \endcond
		/// \exception	Exception	CODE_INVALID_ARGUMENT
		void	SetPortNumber(unsigned short aPortNumber);

		/// \cond	en
		/// \brief	Port accessor
		/// \endcond
		/// \cond	fr
		/// \brief	Accesseur pour le port
		/// \endcond
		void	ResetPort();

	private:

		void	Validate()	const;

		sockaddr_in	mAddr;

	};

}
