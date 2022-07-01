#pragma once

#include "AFX_EXT_MISC.h"
#include <vector>

#include "xUtil.h"

/**********************************************************************************/
/*                                                                                */
/*    B i g  I n t e g e r                                                        */
/*                                                                                */
/*                                                                                */
/*                                                                                */
/*                                              made by 하얀악마(박원하)             */
/*                                                                                */
/*                                              2001. 10. 31. 시월의 마지막 밤에     */
/*                                              2012. 08. 14~20. 수정 및 x86 ASM   */
/*                                                                                */
/*--------------------------------------------------------------------------------*/
/*                                                                                */
/* 참고 : x86 ASM 추가(32bit 에서만 동자)                                            */
/*                                                                                */
/*                                                                                */
/**********************************************************************************/

#pragma pack(push, 8)
WARNING_PUSH_AND_DISABLE(4267)

#define __LITTLE_ENDIAN			// x86계열은 LittleEndian임.
#define __SUPPORT_INT64

#ifdef __SUPPORT_INT64
	typedef unsigned long int BigIntItem;	// 32 bit. 
	typedef uint64_t BigIntItem2;	// 64 bit. 32 bit연산 결과를 임시 저장
					// 64 bit integer가 지원될 경우 int64_t를 Compiler가 지원하는 적당한 keyword으로 수정해야 함.
#else
	typedef unsigned short int BigIntItem;	// 16 bit.
	typedef unsigned long int BigIntItem2;	// 32 bit. 16bit연산 결과를 저장
#endif

//extern const int s_nBitPerItem;

// End of Configuration
///////////////////////////////////////////////////////////////////////////////

#ifndef countof
	#define countof(arr) (sizeof(arr)/sizeof(arr[0]))		// returns # of array item
#endif

class AFX_EXT_CLASS_MISC CBigInt {
public:
	const static uintXX_t s_nBitPerItem;
	//									// 만약 MultiThread를 사용할때 이 변수를 사용하여 하나의 CBigInt개체를 다룰때는 Semaphore를 걸어주는 것이 좋을 듯.
protected:
	std::vector<BigIntItem> m_value;	// Value (Big Endian.)
	int m_eSign;						// Sign
public:
// Constructor
	static CBigInt zero;
	static CBigInt one;
	CBigInt() {							// Default Constructor
		InitValue();
	}
	CBigInt(const CBigInt& B) {			// Copy Constructor
		*this = B;
	}
	CBigInt(const int b) {
		*this = b;
	}
	CBigInt(const char* b) {
		*this = b;
	}
	//CBigInt(const wchar_t* b) {
	//	*this = b;
	//}
	void Init(uintXX_t nBit = 32, BOOL bRandom = FALSE, DWORD seed = 0);
	CBigInt& AssignZero();
protected:
	void InitValue(size_t nItem = 1);		// 초기화.

public:
// Destructor
	virtual ~CBigInt() {
	}

	size_t AdjustLength(size_t nItemNew);						// Bit Length 수정
	size_t GrowLength(size_t nAdditionalItem);				// Bit Length 확장
	size_t Trim() { return AdjustLength(0); }				// 변수 크기를 조절(Order에 맞춰 BitLength를 조절)

// operators
	// assign operators
	CBigInt& operator = (const CBigInt& B) {
		if (this == &B)
			return *this;
		m_value = B.m_value;
		m_eSign = B.m_eSign;
		return *this;
	}
	CBigInt& operator = (int b) {
		InitValue(1);
		m_eSign = b < 0 ? -1 : 1;
		if (m_eSign < 0) m_value[0] = -b;
		else m_value[0] = b;
		return *this;
	}
	CBigInt& operator = (const char* b);
	//CBigInt& operator = (const wchar_t* b);

	// type cast operators
	operator long () const { return m_value.size() ? ((int)m_value[0])*m_eSign : 0; }
	operator unsigned long const () { return m_value.size() ? (unsigned int)((int)m_value[0]*m_eSign) : 0; }
	char* Print(char* sz, int& nBufferSize, const char* szFormat = NULL/* Not Avaialble */);
	wchar_t* Print(wchar_t* sz, int& nBufferSize, const wchar_t* szFormat = NULL/* Not Avaialble */);
	char* PrintHex(char* sz, int& nBufferSize, BOOL bForDisplay = FALSE, const char* szFormat = NULL/* Not Avaialble */);
	wchar_t* PrintHex(wchar_t* sz, int& nBufferSize, BOOL bForDisplay = FALSE, const wchar_t* szFormat = NULL/* Not Avaialble */);
	void PrintHex(CStringA& str, BOOL bForDisplay = FALSE, const char* szFormat = NULL/* Not Avaialble */);
	void PrintHex(CStringW& str, BOOL bForDisplay = FALSE, const wchar_t* szFormat = NULL/* Not Avaialble */);

// Unary Operators
	// Negate Operator
	CBigInt operator - (void) const { CBigInt C(*this); C.m_eSign *= -1; return C; }
	CBigInt& Negate() { m_eSign *= -1; return *this; }

	// Logical Operators
	BOOL IsZero() const {
		if (!m_value.size()) return TRUE;
		for (unsigned int i = 0; i < m_value.size(); i++)
			if (m_value[i]) return FALSE;
		return TRUE;
	}
	BOOL operator == (const CBigInt& B) const { return (CompareValue(B) == 0); }
	BOOL operator != (const CBigInt& B) const { return CompareValue(B); }
	BOOL operator > (const CBigInt& B) const  { return CompareValue(B) > 0; }
	BOOL operator < (const CBigInt& B) const  { return CompareValue(B) < 0; }
	BOOL operator >= (const CBigInt& B) const { return CompareValue(B) >= 0; }
	BOOL operator <= (const CBigInt& B) const { return CompareValue(B) <= 0; }
	int CompareDigits(const CBigInt& B) const;			// (부호 무시)	-1 : this is smaller, 0 : equal, 1 : this is bigger
	int CompareValue(const CBigInt& B) const;			//				-1 : this is smaller, 0 : equal, 1 : this is bigger
	BOOL IsNegative() const { return !IsZero() && m_eSign < 0; }
	BOOL IsPositive() const { return !IsZero() && m_eSign > 0; }

	// Arithmatic Operators
	CBigInt operator + (const CBigInt& B) const { CBigInt C(*this); C += B; return C; }
	CBigInt operator - (const CBigInt& B) const { CBigInt C(*this); C -= B; return C; }
	CBigInt operator * (const CBigInt& B) const { CBigInt C(*this); Mul(B, C); return C; }
	CBigInt operator / (const CBigInt& B) const { CBigInt Q, R; Div(B, Q, R); return Q; }
	CBigInt operator % (const CBigInt& B) const { CBigInt Q, R; Div(B, Q, R); return R; }
	CBigInt& operator += (const CBigInt& B);
	CBigInt& operator -= (const CBigInt& B);
	CBigInt& operator *= (const CBigInt& B) { *this = *this * B; return *this; }
	CBigInt& operator /= (const CBigInt& B);
	CBigInt& operator %= (const CBigInt& B);
	CBigInt& AddDigits(const CBigInt& B);
	CBigInt& SubDigits(const CBigInt& B);
	BOOL Mul(const CBigInt& B, CBigInt& C) const ;						// *this * B --> C
	BOOL Div(const CBigInt& B, CBigInt& Q, CBigInt& R) const ;			// *this / B --> Q ... R

	// Bitwise Logical Operators
	CBigInt operator | (const CBigInt& B) { CBigInt C(*this); return C |= B; }
	CBigInt operator & (const CBigInt& B) { CBigInt C(*this); return C &= B; }
	CBigInt operator ^ (const CBigInt& B) { CBigInt C(*this); return C ^= B; }
	CBigInt& operator |= (const CBigInt& B);
	CBigInt& operator &= (const CBigInt& B);
	CBigInt& operator ^= (const CBigInt& B);
	CBigInt operator ~ ();

	// Bitwise Shift Operators
	CBigInt& operator << (unsigned int c) { CBigInt C(*this); return C <<= c; }
	CBigInt& operator >> (unsigned int c) { CBigInt C(*this); return C >>= c; }
	CBigInt& operator <<= (unsigned int c);
	CBigInt& operator >>= (unsigned int c);

	// Getting i'th bit
	int GetBit(unsigned int i) const { return (*this)(i); }
	int operator () (unsigned int i) const {
		if (i >= (unsigned)m_value.size()*s_nBitPerItem) return 0;
		int index = i / s_nBitPerItem;
		int nShift = i % s_nBitPerItem;
		return (m_value[index] >> nShift) & 0x01;
	}
	BOOL SetBit(unsigned int i, BOOL b) {
		if (i >= (unsigned)m_value.size()*s_nBitPerItem)
			AdjustLength(GetNItem(i+1));
		int iItem = i / s_nBitPerItem;
		int iBit = i % s_nBitPerItem;
		if (b)
			m_value[iItem] |= (0x01 << iBit);
		else
			m_value[iItem] &= ~(0x01 << iBit);
		return b;
	}

	const BigIntItem* GetValue() const { return m_value.data(); }
	BOOL GetValue(BigIntItem* stream, int* pCount) const {
		if (pCount) {
			if (*pCount < (int)m_value.size()) {
				*pCount = (int)m_value.size();
				return FALSE;
			}
		}
		if (!m_value.size() || !stream || !pCount)
			return FALSE;

		memmove(stream, m_value.data(), _min(*pCount, (int)(m_value.size()*sizeof(BigIntItem))));
		return TRUE;
	}
	BOOL SetValue(const BigIntItem* stream, size_t nCount) {
		if (nCount <= 0) {
			m_value.clear();
			return FALSE;
		}
		m_value.resize(nCount, 0);
		if (!stream)
			return FALSE;
		size_t n = _min((int)m_value.size(), nCount);
		for (size_t i = 0; i < n; i++, stream++)
			m_value[i] = *stream;
		return TRUE;
	}
	BOOL GetValue(BYTE* stream, int* pCount) const {
		if (pCount) {
			if (*pCount < (int)(m_value.size()*sizeof(BigIntItem))) {
				*pCount = (int)(m_value.size()*sizeof(BigIntItem));
				return FALSE;
			}
		}
		if (!m_value.size() || !stream || !pCount)
			return FALSE;
		memmove(stream, m_value.data(), _min((unsigned)*pCount, m_value.size()*sizeof(BigIntItem)));
		return TRUE;
	}
	BOOL SetValue(const BYTE* stream, size_t nCount) {
		if (nCount <= 0) {
			m_value.clear();
			return FALSE;
		}
		size_t nItem = (nCount-1) / sizeof(BigIntItem) + 1;
		m_value.resize(nItem, 0);
		AssignZero();
		if (!stream)
			return FALSE;
		memmove(m_value.data(), stream, _min(nCount, m_value.size()*sizeof(BigIntItem)));
		return TRUE;
	}

// Helpers
public:
	size_t GetOrder() const ;
	size_t GetNItem() const {
 		for (size_t i = m_value.size()-1; i >= 0; i--) if (m_value[i]) return i+1;
		return 0;
	}
	size_t GetCount() const { return m_value.size(); }
	size_t GetSize() const { return GetCount()*sizeof(BigIntItem); }
	BigIntItem& operator [] (int i) {
		if (i < 0)
			AfxThrowMemoryException();
		if (i >= (int)m_value.size())
			m_value.resize(i+1, 0);
		return m_value[i];
	}
	const BigIntItem& operator [] (int i) const {
		if ( (i < 0) || (i >= (int)m_value.size()) )
			AfxThrowMemoryException();
		return m_value[i];
	}

protected:
	static size_t GetNItem(size_t nBit);						// Bit갯수로 전체 길이를 계산

public:
	BOOL IsPrime(int nIteration) { return IsPrime(*this, nIteration); }

public:
	static BOOL GetOnePrime(CBigInt& p, uintXX_t nBitLen, BOOL bSetMSB);						// Miller-Rabin Algorithm
	static BOOL IsPrime(const CBigInt& n, int nIteration);								// Miller-Rabin Algorithm
	static CBigInt CalcGCD(CBigInt a, CBigInt b);										// Euclid Algorithm
	static CBigInt ModularExp(const CBigInt& x, const CBigInt& b, const CBigInt& n);	// Square And Multiply Algorithm
};

#pragma pack(pop)
