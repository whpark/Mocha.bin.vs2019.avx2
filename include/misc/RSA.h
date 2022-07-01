#pragma once

#include "AFX_EXT_MISC.h"

/**********************************************************************************/
/*                                                                                */
/*    R S A                                                                       */
/*                                                                                */
/*                                                                                */
/*                                                                                */
/*                                              made by 하얀악마(박원하)             */
/*                                                                                */
/*                                              2001. 10. 31. 시월의 마지막 밤에     */
/*                                              2012. 08. 20~22. 수정 중           */
/*                                                                                */
/**********************************************************************************/

#include "BigInt.h"

#pragma pack(push, 8)

class AFX_EXT_CLASS_MISC CRSA {
protected:
	uintXX_t m_nBitLen;
	uintXX_t m_nBitLenEncoding;
	CBigInt m_N;			// n
	CBigInt m_keyPublic;	// b (Public Key)
	CBigInt m_keySecret;	// a (Secret Key)

public:
// Constructor & Destructor
	CRSA();
	CRSA(uintXX_t nBitLen, const CBigInt& N = CBigInt::zero, const CBigInt& keyPublic = CBigInt::zero, const CBigInt& keySecret = CBigInt::zero) {
		Setup(nBitLen, N, keyPublic, keySecret);
	}
	CRSA(const CRSA& B) { *this = B; }
	CRSA& operator = (const CRSA& B) {
		if (this != &B) {
			m_nBitLen = B.m_nBitLen;
			m_nBitLenEncoding = B.m_nBitLenEncoding;
			m_N = B.m_N;
			m_keyPublic = B.m_keyPublic;
			m_keySecret = B.m_keySecret;
		}
		return *this;
	}
	virtual ~CRSA();

	uintXX_t	GetBitLen()				const { return m_nBitLen; }
	uintXX_t	GetBitLenEncoding()		const { return m_nBitLenEncoding; }
	CBigInt GetN()					const { return m_N; }
	CBigInt GetPublicKey()			const { return m_keyPublic; }
	CBigInt GetSecretKey()			const { return m_keySecret; }

// Implementation
public:

	void Clear();
																				// Setup RSA
	BOOL Setup(uintXX_t nBitLen, const CBigInt& N = CBigInt::zero, const CBigInt& keyPublic = CBigInt::zero, const CBigInt& keySecret = CBigInt::zero);

	sizeXX_t Encrypt(const BYTE* stream, sizeXX_t nLen, std::vector<BYTE>& code) const;	// Encrypt ( stream -> cipher )
	sizeXX_t Decrypt(const BYTE* stream, sizeXX_t nLen, std::vector<BYTE>& text) const;	// Decrypt ( stream -> plain )

	sizeXX_t Encrypt(const std::vector<BYTE>& stream, std::vector<BYTE>& code) const;	// Encrypt ( stream -> cipher )
	sizeXX_t Decrypt(const std::vector<BYTE>& stream, std::vector<BYTE>& text) const;	// Decrypt ( stream -> plain )

protected:
	CBigInt CalcInverse(const CBigInt& b, const CBigInt& n);					// Extended Euclid Algorithm
};

#pragma pack(pop)
