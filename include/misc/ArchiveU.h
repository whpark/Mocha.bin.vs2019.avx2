#pragma once
#include "AFX_EXT_MISC.h"
#include "afx.h"
#include "TRefPointer.hpp"
#include "TString.h"
//#include <winsock2.h>

#pragma pack(push, 8)

//-----------------------------------------------------------------------------
//
class AFX_EXT_CLASS_MISC CArchiveU : public CArchive {
public:
	enum eENCODING { E_NONE = -1, E_AUTO, E_ANSI, E_UNICODE, E_UTF8, };
	enum eBYTE_ORDER { BO_DEFAULT, BO_LITTLE_ENDIAN/*x86, LSB First*/, BO_BIG_ENDIAN/*MSB First*/, };
protected:
	eENCODING m_eEncoding;
	BOOL m_bConvertByteOrder;
public:
	CArchiveU(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL, eBYTE_ORDER eByteOrder = BO_DEFAULT) : CArchive(pFile, nMode, nBufSize, lpBuf) {
		m_eEncoding = E_NONE;
		SetByteOrdering(eByteOrder);
	}
	CArchiveU(CArchive& ar) : CArchive(ar.GetFile(), ar.IsStoring() ? CArchive::store : CArchive::load) {
		SetByteOrdering(BO_DEFAULT);
	}

public:	
	void SetByteOrdering(eBYTE_ORDER eByteOrder = BO_DEFAULT);
	static eBYTE_ORDER GetByteOrdering();
	BOOL GetConvertByteOrder() { return m_bConvertByteOrder; }

	int64_t				GetSignedInt64(int iIndex = 0) const;
	uint64_t	GetUnsignedInt64(int iIndex = 0) const;
#define CONV_BO(v) { if (m_bConvertByteOrder) SwapByteOrder(v); }
	CArchiveU& operator >> (char& c)		{ CArchive::operator >> (c); return *this; }
	CArchiveU& operator >> (BYTE& b)		{ CArchive::operator >> (b); return *this; }
	CArchiveU& operator >> (short& s)		{ CArchive::operator >> (s); CONV_BO(s); return *this; }
	CArchiveU& operator >> (WORD& w)		{ CArchive::operator >> (w); CONV_BO(w); return *this; }
	//CArchiveU& operator >> (int& i)			{ CArchive::operator >> (i); CONV_BO(i); return *this; }
	CArchiveU& operator >> (long& l)		{ CArchive::operator >> (l); CONV_BO(l); return *this; }
	CArchiveU& operator >> (DWORD& dw)		{ CArchive::operator >> (dw); CONV_BO(dw); return *this; }
	CArchiveU& operator >> (int32_t& l)		{ CArchive::operator >> (l); CONV_BO(l); return *this; }
	CArchiveU& operator >> (uint32_t& dw)	{ CArchive::operator >> (dw); CONV_BO(dw); return *this; }
	CArchiveU& operator >> (int64_t& i64)	{ Read(&i64, sizeof(i64)); CONV_BO(i64); return *this; }
	CArchiveU& operator >> (uint64_t& u64)	{ Read(&u64, sizeof(u64)); CONV_BO(u64); return *this; }
//#if !_M_64_
//	CArchiveU& operator >> (size_t& size)	{ Read(&size, sizeof(size)); CONV_BO(size); return *this; }
//#endif
	CArchiveU& operator >> (float& f)		{ CArchive::operator >> (f); return *this; }
	CArchiveU& operator >> (double& d)		{ CArchive::operator >> (d); return *this; }
	CArchiveU& operator >> (bool& b)		{ CArchive::operator >> (b); return *this; }
	CArchiveU& operator >> (CString& str)	{ CArchive::operator >> (str); return *this; }


	CArchiveU& operator << (char c)			{ CArchive::operator << (c); return *this; }
	CArchiveU& operator << (BYTE b)			{ CArchive::operator << (b); return *this; }
	CArchiveU& operator << (short s)		{ CONV_BO(s); CArchive::operator << (s); return *this; }
	CArchiveU& operator << (WORD w)			{ CONV_BO(w); CArchive::operator << (w); return *this; }
	//CArchiveU& operator << (int i)			{ CONV_BO(i); CArchive::operator << (i); return *this; }
	CArchiveU& operator << (long l)			{ CONV_BO(l); CArchive::operator << (l); return *this; }
	CArchiveU& operator << (DWORD dw)		{ CONV_BO(dw); CArchive::operator << (dw); return *this; }
	CArchiveU& operator << (int32_t l)		{ CONV_BO(l); CArchive::operator << (l); return *this; }
	CArchiveU& operator << (uint32_t dw)	{ CONV_BO(dw); CArchive::operator << (dw); return *this; }
	CArchiveU& operator << (int64_t& i64)	{ CONV_BO(i64); Write(&i64, sizeof(i64)); return *this; }
	CArchiveU& operator << (uint64_t& u64)	{ CONV_BO(u64); Write(&u64, sizeof(u64)); return *this; }
#if !_M_64_
	CArchiveU& operator << (size_t& size)	{ CONV_BO(size); Write(&size, sizeof(size)); return *this; }
#endif
	CArchiveU& operator << (float f)		{ CArchive::operator << (f); return *this; }
	CArchiveU& operator << (double d)		{ CArchive::operator << (d); return *this; }
	CArchiveU& operator << (bool& b)		{ CArchive::operator << (b); return *this; }
	CArchiveU& operator << (const CString& str)	{ CArchive::operator << (str); return *this; }


	template < typename TYPE > CArchiveU& StreamIO(TYPE& v) {
		if (IsStoring())	operator << (v); else operator >> (v);
		return *this;
	}
	template < typename TYPE > CArchiveU& StreamIO(TYPE& v, BOOL bConvertByteOrder) {
		if (IsStoring()) {
			if (bConvertByteOrder) {
				TYPE v2(v);
				SwapByteOrder(v2);
				operator << (v2);
			} else {
				operator << (v);
			}
		} else {
			operator >> (v);
			if (bConvertByteOrder)
				SwapByteOrder(v);
		}

		return *this;
	}
	CArchiveU& operator || (char& v)		{ return StreamIO(v); }
	CArchiveU& operator || (BYTE& v)		{ return StreamIO(v); }
	CArchiveU& operator || (short& v)		{ return StreamIO(v, m_bConvertByteOrder); }
	CArchiveU& operator || (WORD& v)		{ return StreamIO(v, m_bConvertByteOrder); }
	CArchiveU& operator || (int& v)			{ return StreamIO(v, m_bConvertByteOrder); }
	CArchiveU& operator || (long& v)		{ return StreamIO(v, m_bConvertByteOrder); }
	CArchiveU& operator || (DWORD& v)		{ return StreamIO(v, m_bConvertByteOrder); }
	//CArchiveU& operator || (int32_t& v)		{ return StreamIO(v, m_bConvertByteOrder); }
#if _M_64_
	CArchiveU& operator || (uint32_t& v)	{ return StreamIO(v, m_bConvertByteOrder); }
#else
	CArchiveU& operator || (uint32_t& v)	{ return StreamIO<DWORD>((DWORD&)v, m_bConvertByteOrder); }
#endif
	CArchiveU& operator || (int64_t& v)		{ return StreamIO(v, m_bConvertByteOrder); }
	CArchiveU& operator || (uint64_t& v)	{ return StreamIO(v, m_bConvertByteOrder); }
	//CArchiveU& operator || (size_t& v)		{ return StreamIO(v, m_bConvertByteOrder); }
	CArchiveU& operator || (float& v)		{ return StreamIO(v); }
	CArchiveU& operator || (double& v)		{ return StreamIO(v); }
	CArchiveU& operator || (bool& v)		{ return StreamIO(v); }
	CArchiveU& operator || (CString& str)	{ return StreamIO(str); }

#undef CONV_BO

public:
	BOOL WriteEncodingSignature(eENCODING eEncoding);
	eENCODING ReadEncodingSignature();
	eENCODING GetEncoding() const { return m_eEncoding; }
	eENCODING SetEncoding(eENCODING eEncoding);

public:
	BOOL ReadStringU(CStringA& str);
	BOOL ReadStringU(CStringW& str);
	void WriteStringU(LPCSTR psz);
	void WriteStringU(LPCWSTR psz);
protected:
	BOOL ReadString(eENCODING eEncoding, CStringA& str);
	BOOL ReadString(eENCODING eEncoding, CStringW& str);
	void WriteString(eENCODING eEncoding, LPCSTR psz);
	void WriteString(eENCODING eEncoding, LPCWSTR psz);

	// Read/Write as is.
public:
	LPSTR  ReadStringA(LPSTR lpsz, sizeXX_t nMax)  { return ReadStringT<char>(lpsz, nMax); }		/* nMax : include NULL terminator*/
	LPWSTR ReadStringW(LPWSTR lpsz, sizeXX_t nMax)  { return ReadStringT<wchar_t>(lpsz, nMax); }	/* nMax : include NULL terminator*/
	BOOL   ReadStringA(CStringA& rString) { return ReadStringT<char, CStringA>(rString); }
	BOOL   ReadStringW(CStringW& rString) { return ReadStringT<wchar_t, CStringW>(rString); }

	void WriteStringA(LPCSTR sz, sizeXX_t nLen = 0)	{ WriteStringT(sz, nLen); }
	void WriteStringW(LPCWSTR wsz, sizeXX_t nLen = 0)	{ WriteStringT(wsz, nLen); }
	void WriteStringA(const CStringA& str) { if (str.GetLength()) Write((const char*)str, str.GetLength()); }
	void WriteStringW(const CStringW& str) { if (str.GetLength()) Write((const wchar_t*)str, str.GetLength()*sizeof(wchar_t)); }

#if _M_64_
	size64_t Read(void* lpBuf, size64_t nMax);
	void Write(const void* lpBuf, size64_t nMax);
#endif

protected:
	template <typename tchar> tchar* ReadStringT(tchar* lpsz, sizeXX_t nMax/*including NULL terminator*/) {
		if ((!nMax) || (!lpsz))
			return lpsz;
		tchar c;
		tchar* pos = lpsz;
		BOOL bRead = FALSE;
		while ( (Read(&c, sizeof(c)) > 0) && (--nMax != 0) ) {
			bRead = TRUE;
			if (c == '\r')
				continue;
			if (c == '\n')
				break;
			*pos++ = c;
		}
		*pos = 0;
		if (!bRead)
			return NULL;
		return lpsz;
	}
	template <typename tchar, class cstring> BOOL ReadStringT(cstring& rString) {
		tchar c;
		rString.Empty();
		BOOL bRead = FALSE;
		while (Read(&c, sizeof(c)) > 0) {
			bRead = TRUE;
			if (c == '\r')
				continue;
			if (c == '\n')
				break;
			rString += c;
		}

		return bRead;
	}
	template <typename tchar> void WriteStringT(const tchar* lpsz, sizeXX_t nLen) {
		if (!lpsz)
			return ;
		if (!nLen)
			nLen = tszlen(lpsz);
		Write(lpsz, nLen*sizeof(tchar));
//WARNING_POP()
	}
};

template <class ARCHIVE, class T>
BOOL StoreTo(ARCHIVE& ar, const std::vector<T>& B) {
#if _X86_COMPATIBLE_MODE_FOR_IO_
	DWORD size = (uint32_t)B.size();
#else
	sizeXX_t size = B.size();
#endif

	ar << size;

	if (!size)
		return TRUE;
	for (uintXX_t i = 0; i < size; i++) {
		__if_exists(T::SerializeDynamic) {
			T::SerializeDynamic(ar, &(B[i]));
		}
		__if_not_exists(T::SerializeDynamic) {
			ar << B[i];
		}
	}
	return TRUE;
}
template <class ARCHIVE, class T>
BOOL LoadFrom(ARCHIVE& ar, std::vector<T>& B, const T& v0 = T()) {

	B.clear();

#if _X86_COMPATIBLE_MODE_FOR_IO_
	DWORD size = 0;
#else
	sizeXX_t size = 0;
#endif

	ar >> size;

	if (!size)
		return TRUE;

	B.reserve(size);
	for (uintXX_t i = 0; i < size; i++) {
		__if_exists(T::SerializeDynamic) {
			T* p = T::SerializeDynamic(ar, (T*)NULL);
			B.push_back(*p);
			delete p;
		}
		__if_not_exists(T::SerializeDynamic) {
			B.push_back(v0);
			ar >> B[i];
		}
	}
	return TRUE;
}
template <class ARCHIVE, class T>
BOOL StoreTo(ARCHIVE& ar, const std::vector<TRefPointer<T>>& B) {
#if _X86_COMPATIBLE_MODE_FOR_IO_
	DWORD size = (DWORD)B.size();
#else
	sizeXX_t size = B.size();
#endif

	ar << size;

	if (!size)
		return TRUE;
	for (uintXX_t i = 0; i < size; i++) {
		__if_exists(T::SerializeDynamic) {
			T::SerializeDynamic(ar, B[i]);
		}
		__if_not_exists(T::SerializeDynamic) {
			ar << *B[i];
		}
	}
	return TRUE;
}
template <class ARCHIVE, class T>
BOOL LoadFrom(ARCHIVE& ar, std::vector<TRefPointer<T>>& B) {

	B.clear();

#if _X86_COMPATIBLE_MODE_FOR_IO_
	uint32_t size = 0;
#else
	sizeXX_t size = 0;
#endif

	ar >> size;

	if (!size)
		return TRUE;

	B.reserve(size);
	for (uintXX_t i = 0; i < size; i++) {
		__if_exists(T::SerializeDynamic) {
			TRefPointer<T> p = T::SerializeDynamic(ar, (T*)NULL);
			B.push_back(p);
		}
		__if_not_exists(T::SerializeDynamic) {
			B.push_back(TRefPointer<T>(new T));
			ar >> *B[i];
		}
	}
	return TRUE;
}

template <class T> CArchive&  operator << (CArchive& ar,  const std::vector<T>& B)				{ StoreTo<CArchive, T>(ar, B);   return ar; }
template <class T> CArchive&  operator >> (CArchive& ar,  std::vector<T>& B)					{ LoadFrom<CArchive, T>(ar, B);  return ar; }
template <class T> CArchive&  operator << (CArchive& ar,  const std::vector<TRefPointer<T>>& B)	{ StoreTo<CArchive, T>(ar, B);   return ar; }
template <class T> CArchive&  operator >> (CArchive& ar,  std::vector<TRefPointer<T>>& B)		{ LoadFrom<CArchive, T>(ar, B);  return ar; }
template <class T> CArchiveU& operator << (CArchiveU& ar, const std::vector<T>& B)				{ StoreTo<CArchiveU, T>(ar, B);  return ar; }
template <class T> CArchiveU& operator >> (CArchiveU& ar, std::vector<T>& B)					{ LoadFrom<CArchiveU, T>(ar, B); return ar; }
template <class T> CArchiveU& operator << (CArchiveU& ar, const std::vector<TRefPointer<T>>& B)	{ StoreTo<CArchiveU, T>(ar, B);  return ar; }
template <class T> CArchiveU& operator >> (CArchiveU& ar, std::vector<TRefPointer<T>>& B)		{ LoadFrom<CArchiveU, T>(ar, B); return ar; }

// std::string
template <class TCHAR>
CArchive& StoreTo(CArchive& ar, const std::basic_string<TCHAR>& str) {
	uint32_t nLen = (uint32_t)str.length();
	ar << nLen;
	if (nLen > 0)
		ar.Write(str.c_str(), nLen * sizeof(TCHAR));
	return ar;
}
template <class TCHAR>
CArchive& LoadFrom(CArchive& ar, std::basic_string<TCHAR>& str) {
	uintXX_t nLen = 0;
	ar >> nLen;
	str.assign((size_t)nLen, 0);
	if (nLen > 0) {
		std::vector<TCHAR> buf(nLen, 0);
		ar.Read(buf.data(), nLen * sizeof(TCHAR));
		str.assign(buf.data(), buf.end());
	} else {
		str.clear();
	}
	return ar;
}
template <class TCHAR> CArchive& operator << (CArchive& ar, const std::basic_string<TCHAR>& str)	{ return StoreTo(ar, str); }
template <class TCHAR> CArchive& operator >> (CArchive& ar, std::basic_string<TCHAR>& str)			{ return LoadFrom(ar, str); }
template <class TCHAR> CArchive& operator << (CArchiveU& ar, const std::basic_string<TCHAR>& str)	{ return (CArchiveU&)StoreTo((CArchive&)ar, str); }
template <class TCHAR> CArchive& operator >> (CArchiveU& ar, std::basic_string<TCHAR>& str)			{ return (CArchiveU&)LoadFrom((CArchive&)ar, str); }


////-----------------------------------------------------------------------------
//// CArchiveDummy for Size Calc.
//class AFX_EXT_CLASS_MISC CArchiveDummy {
//protected:
//	CArchiveU::eENCODING m_eEncoding;
//	int m_nLength;
//public:
//	CArchiveDummy(UINT nMode = CArchive::store, CArchiveU::eENCODING eEncoding = CArchiveU::E_AUTO) : m_nLength(0), m_eEncoding(eEncoding) { }
//	CArchiveDummy(const CArchiveDummy& B) { *this = B; }
//	CArchiveDummy& operator = (const CArchiveDummy& B) {
//		if (this != &B) { m_eEncoding = B.m_eEncoding; m_nLength = B.m_nLength; }
//		return *this;
//	}
//
//	void Abort() { m_nLength = 0; }
//	void Close() { m_nLength = 0; }
//	size_t GetCurrentLength() const { return m_nLength; }
//
//public:	
//	CArchiveDummy& operator >> (char& c)		{ m_nLength += sizeof(c);	return *this; }
//	CArchiveDummy& operator >> (BYTE& b)		{ m_nLength += sizeof(b);	return *this; }
//	CArchiveDummy& operator >> (short& s)		{ m_nLength += sizeof(s);	return *this; }
//	CArchiveDummy& operator >> (WORD& w)		{ m_nLength += sizeof(w);	return *this; }
//	CArchiveDummy& operator >> (int& i)			{ m_nLength += sizeof(i);	return *this; }
//	CArchiveDummy& operator >> (long& l)		{ m_nLength += sizeof(l);	return *this; }
//	CArchiveDummy& operator >> (DWORD& dw)		{ m_nLength += sizeof(dw);	return *this; }
//	CArchiveDummy& operator >> (int64_t& i64)	{ m_nLength += sizeof(i64);	return *this; }
//	CArchiveDummy& operator >> (uint64_t& u64)	{ m_nLength += sizeof(u64);	return *this; }
//	CArchiveDummy& operator >> (float& f)		{ m_nLength += sizeof(f);	return *this; }
//	CArchiveDummy& operator >> (double& d)		{ m_nLength += sizeof(d);	return *this; }
//
//	CArchiveDummy& operator << (char& c)		{ m_nLength += sizeof(c);	return *this; }
//	CArchiveDummy& operator << (BYTE& b)		{ m_nLength += sizeof(b);	return *this; }
//	CArchiveDummy& operator << (short& s)		{ m_nLength += sizeof(s);	return *this; }
//	CArchiveDummy& operator << (WORD& w)		{ m_nLength += sizeof(w);	return *this; }
//	CArchiveDummy& operator << (int& i)			{ m_nLength += sizeof(i);	return *this; }
//	CArchiveDummy& operator << (long& l)		{ m_nLength += sizeof(l);	return *this; }
//	CArchiveDummy& operator << (DWORD& dw)		{ m_nLength += sizeof(dw);	return *this; }
//	CArchiveDummy& operator << (int64_t& i64)	{ m_nLength += sizeof(i64);	return *this; }
//	CArchiveDummy& operator << (uint64_t& u64)	{ m_nLength += sizeof(u64);	return *this; }
//	CArchiveDummy& operator << (float& f)		{ m_nLength += sizeof(f);	return *this; }
//	CArchiveDummy& operator << (double& d)		{ m_nLength += sizeof(d);	return *this; }
//
//
//public:
////	BOOL WriteEncodingSignature(eENCODING eEncoding) {
////		if (eEncoding == CArchiveU::E_AUTO) {
////#ifdef _UNICODE
////			eEncoding = CArchiveU::E_UNICODE;
////#else
////			eEncoding = CArchiveU::E_ANSI;
////#endif
////		}
////		switch (eEncoding) {
////		case CArchiveU::E_ANSI :
////			break;
////		case CArchiveU::E_UNICODE :
////			m_nLength += sizeof(USHORT);
////			break;
////		case CArchiveU::E_UTF8 :
////			m_nLength += 3; //strlen(UTF8_BOM)
////			break;
////		default :
////			return FALSE;
////		}
////		SetEncoding(eEncoding);
////		return TRUE;
////	}
////	eENCODING ReadEncodingSignature();
////	eENCODING GetEncoding() const { return m_eEncoding; }
////	eENCODING SetEncoding(eENCODING eEncoding);
//
//public:
//	//BOOL ReadStringU(CStringA& str);
//	//BOOL ReadStringU(CStringW& str);
//	//void WriteStringU(LPCSTR psz);
//	//void WriteStringU(LPCWSTR psz);
//protected:
//	//BOOL ReadString(eENCODING eEncoding, CStringA& str);
//	//BOOL ReadString(eENCODING eEncoding, CStringW& str);
//	//void WriteString(eENCODING eEncoding, LPCSTR psz);
//	//void WriteString(eENCODING eEncoding, LPCWSTR psz);
//
//	// Read/Write as is.
//public:
//	LPSTR  ReadString(LPSTR lpsz, sizeXX_t nMax)  { m_nLength += nMax; return lpsz; }
//	void WriteString(LPCTSTR psz) { m_nLength += _tcslen(psz) * sizeof(TCHAR); }
//
//	size_t Read(void* lpBuf, size_t nMax) { m_nLength += nMax; return nMax; }
//	size_t Write(void* lpBuf, size_t nMax) { m_nLength += nMax; return nMax; }
//
//};

#pragma pack(pop)
