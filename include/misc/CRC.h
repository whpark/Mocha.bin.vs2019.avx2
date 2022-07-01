#pragma once

#include "AFX_EXT_MISC.h"

#pragma pack(push, 8)

//=============================================================================
//
class AFX_EXT_CLASS_MISC IDataCheck {
protected:
	int m_nBit;		// CRC Bit
	BOOL m_bBigEndian;
	uint64_t m_value;
	uint64_t m_fBitMask;
public:
	// Constructor & Destructor
	IDataCheck(int nBit = 8, uint64_t value0 = 0, BOOL bBigEndian = FALSE, uint64_t fBitMask = 0) : m_nBit(0), m_bBigEndian(FALSE), m_value(0), m_fBitMask(0) {
		Init(nBit, value0, bBigEndian, fBitMask);
	}
	// Use Default copy constructor and default assign operator.
	//IDataCheck(const IDataCheck& B) { *this = B; }
	//IDataCheck& operator = (const IDataCheck& B) {
	//	if (this != &B) {
	//		m_nBit = B.m_nBit; m_value = B.m_value; m_bBigEndian = B.m_bBigEndian; m_fBitMask = B.m_fBitMask;
	//	}
	//	return *this;
	//}
	virtual ~IDataCheck() { }
	virtual BOOL Init(int nBit = 8, uint64_t value0 = 0, BOOL bBigEndian = FALSE, uint64_t fBitMask = 0);

public:
	// Operation
	virtual BOOL Update(const void* pData, sizeXX_t nLength) = NULL;				// Calc & Update CheckSum
	uint64_t GetValue() const { return m_value & m_fBitMask; }						// Get Current CheckSum
	operator uint64_t () const { return GetValue(); }								// Get CRC

public:
	// Attributes
	int GetBitCount() const { return m_nBit; }
	BOOL IsBigEndian() const { return m_bBigEndian; }
	BOOL IsLittleEndian() const { return !m_bBigEndian; }
	uint64_t GetCurrentValue() const { return m_value; }
};

//-----------------------------------------------------------------------------
//

class AFX_EXT_CLASS_MISC CDataCheckSum : public IDataCheck {
public:
	CDataCheckSum(int nBit = 8, uint64_t value0 = 0, BOOL bBigEndian = FALSE, uint64_t fBitMask = -1i64) : IDataCheck(nBit, value0, bBigEndian, fBitMask) {}

	virtual BOOL Update(const void* pData, sizeXX_t nLength);
};

class AFX_EXT_CLASS_MISC CDataCheckXor : public IDataCheck {
public:
	CDataCheckXor(int nBit = 8, uint64_t value0 = 0, BOOL bBigEndian = FALSE, uint64_t fBitMask = -1i64) : IDataCheck(nBit, value0, bBigEndian, fBitMask) {}

	virtual BOOL Update(const void* pData, sizeXX_t nLength);
};

//=============================================================================
//
class AFX_EXT_CLASS_MISC ICyclicRedundancyCheck {
protected:
	int m_nBit;		// CRC Bit
	uint64_t m_vCRC;
	uint64_t m_ePolynomialMSB;
	uint64_t m_fXORMask;
	uint64_t m_fBitMask;
public:
	// Constructor & Destructor
	ICyclicRedundancyCheck(int nBit = 16, uint64_t vCRC0 = -1i64, uint64_t ePolynomialMSB = 0x1021, uint64_t fXORMask = 0) { Init(nBit, vCRC0, ePolynomialMSB, fXORMask); }
	// Use Default copy constructor and default assign operator.
	//ICyclicRedundancyCheck(const ICyclicRedundancyCheck& B) { *this = B; }
	//ICyclicRedundancyCheck& operator = (const ICyclicRedundancyCheck& B) {
	//	if (this != &B) {
	//		m_nBit = B.m_nBit; m_vCRC = B.m_vCRC; m_ePolynomialMSB = B.m_ePolynomialMSB; m_fBitMask = B.m_fBitMask; m_fXORMask = B.m_fXORMask;
	//	}
	//	return *this;
	//}
	virtual ~ICyclicRedundancyCheck() { }
	virtual BOOL Init(int nBit = 16, uint64_t vCRC0 = -1i64, uint64_t ePolynomialMSB = 0x1021, uint64_t fXORMask = 0);

public:
	// Operation
	virtual BOOL UpdateCRC(const void* pData, sizeXX_t nLength) = NULL;				// Calc & Update CRC
	uint64_t GetCRC() const { return (GetFinalCRC() ^ m_fXORMask) & m_fBitMask; }	// Get CRC
	operator uint64_t () const { return GetCRC(); }									// Get CRC

public:
	// Attributes
	int GetCRCBit() const { return m_nBit; }
	uint64_t GetCurrentCRC() const { return m_vCRC; }
	uint64_t GetPolynomial() const { return m_ePolynomialMSB; }
	static uint64_t GetPolynomialReverted(uint64_t ePolynomial, int nBit);

protected:
	virtual uint64_t GetFinalCRC() const = NULL;
};

//-----------------------------------------------------------------------------
//
class AFX_EXT_CLASS_MISC CCRCGeneral : public ICyclicRedundancyCheck {
public:
	CCRCGeneral(int nBit = 16, uint64_t vCRC0 = -1i64, uint64_t ePolynomialMSB = 0x1021, uint64_t fXORMask = 0) : ICyclicRedundancyCheck(nBit, vCRC0, ePolynomialMSB, fXORMask) { }

public:
	virtual BOOL UpdateCRC(const void* pData, sizeXX_t nLength);
protected:
	virtual uint64_t GetFinalCRC() const;
};

//-----------------------------------------------------------------------------
//
class AFX_EXT_CLASS_MISC CCRCGeneralLSB : public ICyclicRedundancyCheck {
protected:
	uint64_t m_ePolynomialLSB;
public:
	CCRCGeneralLSB(int nBit = 16, uint64_t vCRC0 = -1i64, uint64_t ePolynomialMSB = 0x8005, uint64_t fXORMask = 0) : ICyclicRedundancyCheck(nBit, vCRC0, ePolynomialMSB, fXORMask), m_ePolynomialLSB(0) {
		m_ePolynomialLSB = GetPolynomialReverted(ePolynomialMSB, m_nBit);
	}

	virtual BOOL Init(int nBit = 16, uint64_t vCRC0 = -1i64, uint64_t ePolynomialMSB = 0x1021, uint64_t fXORMask = 0);

public:
	virtual BOOL UpdateCRC(const void* pData, sizeXX_t nLength);
protected:
	virtual uint64_t GetFinalCRC() const { return m_vCRC; }
};



//=============================================================================
//


//-----------------------------------------------------------------------------
//

// General for (1~64 bit-CRC)
AFX_EXT_API_MISC uint64_t CalcCRCGeneral(const void* pData, sizeXX_t nLength, int nBit = 16, uint64_t vCRC0 = -1i64, uint64_t ePolynomialMSB = 0x1021/*MSB-first*/);
AFX_EXT_API_MISC uint64_t CalcCRCGeneralLSB(const void* pData, sizeXX_t nLength, int nBit = 16, uint64_t vCRC0 = 0, uint64_t ePolynomialMSB = 0x8005/*MSB-first*/);

// Standard CRC
AFX_EXT_API_MISC uint8_t  CalcCRC8CCITT(const void* pData, sizeXX_t nLength);
AFX_EXT_API_MISC uint16_t CalcCRC16CCITT(const void* pData, sizeXX_t nLength);
AFX_EXT_API_MISC uint16_t CalcCRC16IBM(const void* pData, sizeXX_t nLength);
AFX_EXT_API_MISC uint32_t CalcCRC32(const void* pData, sizeXX_t nLength);
AFX_EXT_API_MISC uint64_t CalcCRC64ISO(const void* pData, sizeXX_t nLength);	// N/A
AFX_EXT_API_MISC uint64_t CalcCRC64ECMA(const void* pData, sizeXX_t nLength);	// N/A

#pragma pack(pop)
