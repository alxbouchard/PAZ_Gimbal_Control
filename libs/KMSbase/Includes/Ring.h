
// Produit	:	KmsBase

/// \author	KMS	-	Martin Dubois, ing.
/// \file	Includes/Ring.h

#pragma once

// Class
/////////////////////////////////////////////////////////////////////////////

/// \cond	en
/// \brief	Circulare buffer
/// \endcond
/// \cond	fr
/// \brief	Tampon circulaire
/// \endcond
template<typename T, unsigned int N>
class Ring
{

public:

	Ring();

	bool	IsEmpty	() const;
	bool	IsFull	() const;

	T			  * In();
	unsigned int	In_GetIndex() const;
	void			In_Next();
	void			In_Push(const T & aIn);

	const T		  & Out() const;
	unsigned int	Out_GetIndex() const;
	void			Out_Next();

	void	Reset();

private:

	bool	mEmpty;

	unsigned int	mIn		;
	unsigned int	mOut	;

	T	mRing[N];

};

// Public
/////////////////////////////////////////////////////////////////////////////

template<typename T, unsigned int N>
inline Ring<T, N>::Ring()
{
	Reset();
}

template<typename T, unsigned int N>
inline bool Ring<T, N>::IsEmpty() const
{
	return mEmpty;
}

template<typename T, unsigned int N>
inline bool Ring<T, N>::IsFull() const
{
	assert(N > mIn	);
	assert(N > mOut	);

	return ((!mEmpty) && (mIn == mOut));
}

template<typename T, unsigned int N>
inline T * Ring<T, N>::In()
{
	assert(N > mIn);

	return (mRing + mIn);
}

template<typename T, unsigned int N>
inline unsigned int Ring<T, N>::In_GetIndex() const
{
	assert(N > mIn);

	return mIn;
}

template<typename T, unsigned int N>
inline void Ring<T, N>::In_Next()
{
	assert(N > mIn);

	mEmpty = false;

	mIn = (mIn + 1) % N;
}

template<typename T, unsigned int N>
inline void Ring<T, N>::In_Push(const T & aIn)
{
	assert(NULL != (&aIn));
	
	assert(N > mIn);

	mRing[mIn] = aIn;
	
	In_Next();
}

template<typename T, unsigned int N>
inline const T & Ring<T, N>::Out() const
{
	assert(N > mOut);

	return (mRing[mOut]);
}

template<typename T, unsigned int N>
inline unsigned int Ring<T, N>::Out_GetIndex() const
{
	assert(N > mOut);

	return mOut;
}

template<typename T, unsigned int N>
inline void Ring<T, N>::Out_Next()
{
	assert(N > mOut);

	mOut = (mOut + 1) % N;

	if (mIn == mOut)
	{
		mEmpty = true;
	}
}

template<typename T, unsigned int N>
inline void Ring<T, N>::Reset()
{
	mEmpty	= true	;
	mIn		= 0		;
	mOut	= 0		;
}
