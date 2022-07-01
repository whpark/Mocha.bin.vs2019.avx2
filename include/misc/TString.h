#ifndef __TSTRING_H__
#define __TSTRING_H__

#include "AFX_EXT_MISC.h"

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "TList.hpp"

#ifndef ASSERT
#	define ASSERT assert
#endif

template < typename TCHAR > sizeXX_t tszlen(const TCHAR* psz);
template < typename TCHAR > TCHAR* tszcpy(TCHAR* pszDest, const TCHAR* pszSrc);
template < typename TCHAR > TCHAR* tszncpy(TCHAR* pszDest, const TCHAR* pszSrc, sizeXX_t nLen);	// nLen : Size of Buffer. always pads NULL at the end of 'pszDest'. (differs from strncpy)
template < typename TCHAR > TCHAR* tszcat(TCHAR* pszDest, const TCHAR* pszSrc);

template < typename TCHAR > int tszcmp(const TCHAR* pszA, const TCHAR* pszB);
template < typename TCHAR > int tszncmp(const TCHAR* pszA, const TCHAR* pszB, sizeXX_t nLen);
template < typename TCHAR > int tszicmp(const TCHAR* pszA, const TCHAR* pszB);
template < typename TCHAR > int tsznicmp(const TCHAR* pszA, const TCHAR* pszB, sizeXX_t nLen);

template < typename TCHAR > int tdszcmp(const TCHAR* pszA, const TCHAR* pszB);					// Compare strings (including digits)
template < typename TCHAR > int tdszicmp(const TCHAR* pszA, const TCHAR* pszB);				// Compare strings (including digits)

template < typename TCHAR > TCHAR* tszupr(TCHAR* psz);
template < typename TCHAR > TCHAR* tszlwr(TCHAR* psz);
template < typename TCHAR > TCHAR* tszrev(TCHAR* psz);
template < typename TCHAR > TCHAR* tszsearch(const TCHAR* string, int c );
template < typename TCHAR > TCHAR* tszsearch(const TCHAR* psz, const TCHAR* pszSearch);
template < typename TCHAR, typename T > T ttsztoi(const TCHAR* psz);		// atoi
template < typename TCHAR, typename T > T ttsztoi16(const TCHAR* psz);		// _tcstoul
template < typename TCHAR > intXX_t tsztoi(const TCHAR* psz);		// atoi
template < typename TCHAR > intXX_t tsztoi16(const TCHAR* psz);	// _tcstoul
inline int32_t  tsztol   (const char* psz,    char** ppszEnd = NULL,    int radix = 0) { return strtol    (psz, ppszEnd, radix); }
inline int32_t  tsztol   (const wchar_t* psz, wchar_t** ppszEnd = NULL, int radix = 0) { return wcstol    (psz, ppszEnd, radix); }
inline uint32_t tsztoul  (const char* psz,    char** ppszEnd = NULL,    int radix = 0) { return strtoul   (psz, ppszEnd, radix); }
inline uint32_t tsztoul  (const wchar_t* psz, wchar_t** ppszEnd = NULL, int radix = 0) { return wcstoul   (psz, ppszEnd, radix); }
inline int64_t  tsztoi64 (const char* psz,    char** ppszEnd = NULL,    int radix = 0) { return _strtoi64 (psz, ppszEnd, radix); }
inline int64_t  tsztoi64 (const wchar_t* psz, wchar_t** ppszEnd = NULL, int radix = 0) { return _wcstoi64 (psz, ppszEnd, radix); }
inline uint64_t tsztoui64(const char* psz,    char** ppszEnd = NULL,    int radix = 0) { return _strtoui64(psz, ppszEnd, radix); }
inline uint64_t tsztoui64(const wchar_t* psz, wchar_t** ppszEnd = NULL, int radix = 0) { return _wcstoui64(psz, ppszEnd, radix); }
inline double   tsztod   (const char* psz,    char** ppszEnd = NULL)                   { return strtod    (psz, ppszEnd); }
inline double   tsztod   (const wchar_t* psz, wchar_t** ppszEnd = NULL)                { return wcstod    (psz, ppszEnd); }

inline int __tolower(int c) { if ( (c >= 'A') && (c <= 'Z') ) return c-'A'+'a'; return c; }
inline int __toupper(int c) { if ( (c >= 'a') && (c <= 'z') ) return c-'a'+'A'; return c; }
inline int __isdigit(const int c)  { return (c >= '0') && (c <= '9'); }
inline int __isodigit(const int c) { return (c >= '0') && (c <= '7'); }
inline int __isxdigit(const int c) { return ((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')); }
inline int __isspace(const int c)  { return (c == '\t') || (c == '\r') || (c == '\n') || (c == ' '); }

template < typename TCHAR > int tszrmchar(TCHAR* psz, int chRemove);	// Remove Charactor from str. returns str length


#ifndef _FORCE_USE_CODEPAGE_949
#	define _FORCE_USE_CODEPAGE_949 FALSE
#endif

AFX_EXT_API_MISC BOOL UnicodeToUTF8		(LPCWSTR pwszSource,	LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bPrintable = FALSE);
AFX_EXT_API_MISC BOOL UnicodeToAnsi949	(LPCWSTR pwszSource,	LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bPrintable = FALSE);
AFX_EXT_API_MISC BOOL Ansi949ToUnicode	(LPCSTR pszSource,		LPWSTR pwszOut,	sizeXX_t& nCharCount/*wchar_t count including NULL terminater*/,	BOOL bPrintable = FALSE);
AFX_EXT_API_MISC BOOL Ansi949ToUTF8		(LPCSTR pszSource,		LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bPrintable = FALSE);
AFX_EXT_API_MISC BOOL UTF8ToUnicode		(LPCSTR pszSource,		LPWSTR pwszOut,	sizeXX_t& nCharCount/*wchar_t count including NULL terminater*/,	BOOL bPrintable = FALSE);
AFX_EXT_API_MISC BOOL UTF8ToAnsi949		(LPCSTR pszSource,		LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bPrintable = FALSE);

AFX_EXT_API_MISC BOOL ConvUnicodeToUTF8		(LPCWSTR pwszSource,	LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvUnicodeToAnsi		(LPCWSTR pwszSource,	LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvAnsiToUnicode		(LPCSTR pszSource,		LPWSTR pwszOut,	sizeXX_t& nCharCount/*wchar_t count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvAnsiToUTF8		(LPCSTR pszSource,		LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvUTF8ToUnicode		(LPCSTR pszSource,		LPWSTR pwszOut,	sizeXX_t& nCharCount/*wchar_t count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvUTF8ToAnsi		(LPCSTR pszSource,		LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);

AFX_EXT_API_MISC BOOL ConvUnicodeToUTF8		(LPCWSTR pwszSource,	TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvUnicodeToAnsi		(LPCWSTR pwszSource,	TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvAnsiToUnicode		(LPCSTR pszSource,		TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvAnsiToUTF8		(LPCSTR pszSource,		TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvUTF8ToUnicode		(LPCSTR pszSource,		TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvUTF8ToAnsi		(LPCSTR pszSource,		TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);

AFX_EXT_API_MISC BOOL ConvUnicodeToUTF8		(LPCWSTR pwszSource,	CStringA& strA,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvUnicodeToAnsi		(LPCWSTR pwszSource,	CStringA& strA,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvAnsiToUnicode		(LPCSTR pszSource,		CStringW& strW,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvAnsiToUTF8		(LPCSTR pszSource,		CStringA& strA,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvUTF8ToUnicode		(LPCSTR pszSource,		CStringW& strW,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
AFX_EXT_API_MISC BOOL ConvUTF8ToAnsi		(LPCSTR pszSource,		CStringA& strA,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);

inline CStringA CONV_U8(LPCSTR pszSource,  BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949) { CStringA strU8; ConvAnsiToUTF8(pszSource, strU8);    return std::move(strU8); }		// move constructor 없지만..
inline CStringA CONV_U8(LPCWSTR pszSource, BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949) { CStringA strU8; ConvUnicodeToUTF8(pszSource, strU8); return std::move(strU8); }

AFX_EXT_API_MISC BOOL IsUTF8String(const char sz[], int* pOutputBufferCount = NULL, BOOL* pbIsMSBSet = NULL);
AFX_EXT_API_MISC BOOL Conv8BitToUnicode(LPCSTR pszSource, CStringW& strW);	// UTF8 or MBCS to


//------------------------------------------------------------------------------------------

WARNING_PUSH_AND_DISABLE(4018 4244 4267)

template < typename TCHAR, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t >
class TString {
protected:
	TCHAR* m_pszData;
	SIZE_T m_nBufferSize;		// Memory Allocation Size
	SIZE_T m_nLen;				// String Length

public:
//	DECLARE_DYNAMIC_CREATE0(TString)
	TString()                 : m_pszData(NULL), m_nBufferSize(0), m_nLen(0) { }
	TString(const TCHAR B)    : m_pszData(NULL), m_nBufferSize(0), m_nLen(0) { *this = B; }
	TString(const TString<char>& B) : m_pszData(NULL), m_nBufferSize(0), m_nLen(0) { *this = B; }
	TString(const TString<wchar_t>& B) : m_pszData(NULL), m_nBufferSize(0), m_nLen(0) { *this = B; }
	explicit TString(const char* B)   : m_pszData(NULL), m_nBufferSize(0), m_nLen(0) { *this = B; }
	explicit TString(const wchar_t* B)   : m_pszData(NULL), m_nBufferSize(0), m_nLen(0) { *this = B; }
	~TString() { if (m_pszData) delete [] m_pszData; }

	TCHAR* Attach(TCHAR* psz, SIZE_T nBufferSize);
	TCHAR* Detach();
protected:
	BOOL Init(SIZE_T nMinLen = 0);
	TCHAR* PrepareBuffer(SIZE_T nNewLen, BOOL bKeepOriginal = TRUE);

public:
	SIZE_T GetLength() const { return m_nLen; }
	BOOL IsEmpty() const { return m_nLen == 0; }
	void Empty() { Init(); }

	TCHAR GetAt(INDEX_T index) const { ASSERT(index < m_nLen); return m_pszData[index]; }
	void SetAt(INDEX_T index, TCHAR ch) { ASSERT(index < m_nLen); m_pszData[index] = ch; }
	TCHAR& operator[](INDEX_T index) const { ASSERT(index < m_nLen); return m_pszData[index]; }
	operator const TCHAR*() const { return m_pszData; }
	operator TCHAR*() { return m_pszData; }

	TString& operator = (const TString<char>& B);
	TString& operator = (const TString<wchar_t>& B);
	TString& operator = (const TCHAR ch);
	TString& operator = (const char* psz);
	TString& operator = (const wchar_t* psz);

	TString& operator += (const TString& B);
	TString& operator += (TCHAR ch);
	TString& operator += (const TCHAR* psz);

	friend TString operator + (const TString& A, const TString& B)  {
		TString<TCHAR> str;
		SIZE_T nLen = A.m_nLen + B.m_nLen;
		str.Init(nLen);
		if (A.m_nLen)
			memmove(str.m_pszData, A.m_pszData, sizeof(TCHAR)*A.m_nLen);
		if (B.m_nLen)
			memmove(str.m_pszData+A.m_nLen, B.m_pszData, sizeof(TCHAR)*B.m_nLen);
		str.m_nLen = nLen;
		return str;
	}
	friend TString operator + (const TString& A, const TCHAR B) {
		if (!B)
			return A;

		TString<TCHAR> str;
		SIZE_T nLen = A.m_nLen + 1;
		str.Init(nLen);
		if (A.m_nLen)
			memmove(str.m_pszData, A.m_pszData, sizeof(TCHAR)*A.m_nLen);
		str.m_pszData[A.m_nLen] = B;
		str.m_nLen = nLen;
		return str;
	}
	friend TString operator + (const TString& A, const TCHAR* B) {
		SIZE_T nLenB = tszlen(B);
		if (!nLenB)
			return A;

		TString<TCHAR> str;
		SIZE_T nLen = A.m_nLen + nLenB;
		str.Init(nLen);
		if (A.m_nLen)
			memmove(str.m_pszData, A.m_pszData, sizeof(TCHAR)*A.m_nLen);
		if (nLenB)
			memmove(str.m_pszData+A.m_nLen, B, sizeof(TCHAR)*nLenB);
		str.m_nLen = nLen;

		return str;
	}
	friend TString operator + (const TCHAR A, const TString& B) {
		if (!A)
			return B;

		TString<TCHAR> str;
		SIZE_T nLen = 1 + B.m_nLen;
		str.Init(nLen);
		str.m_pszData[0] = A;
		if (B.m_nLen)
			memmove(str.m_pszData+1, B.m_pszData, sizeof(TCHAR)*B.m_nLen);
		str.m_nLen = nLen;
		return str;
	}
	friend TString operator + (const TCHAR* A, const TString& B) {
		TString<TCHAR> str;
		SIZE_T nLenA = tszlen(A);
		SIZE_T nLen = nLenA + B.m_nLen;
		str.Init(nLen);
		if (nLenA)
			memmove(str.m_pszData, A, nLenA);
		if (B.m_nLen)
			memmove(str.m_pszData+nLenA, B.m_pszData, sizeof(TCHAR)*B.m_nLen);
		str.m_nLen = nLen;
		return str;
	}

	// String Compare
	SIZE_T Compare(const TCHAR* psz) const { return tszcmp(m_pszData, psz); }
	SIZE_T CompareNoCase(const TCHAR* psz) const { return tszicmp(m_pszData, psz); }
	friend BOOL operator == (const TString& A, const TString& B)  { return A.Compare(B) == 0; }
	friend BOOL operator == (const TString& A, const TCHAR* B)    { return A.Compare(B) == 0; }
	friend BOOL operator == (const TCHAR* A,   const TString& B)  { return B.Compare(A) == 0; }
	friend BOOL operator != (const TString& A, const TString& B)  { return A.Compare(B) != 0; }
	friend BOOL operator != (const TString& A, const TCHAR* B)    { return A.Compare(B) != 0; }
	friend BOOL operator != (const TCHAR* A,   const TString& B)  { return B.Compare(A) != 0; }
	friend BOOL operator  < (const TString& A, const TString& B)  { return A.Compare(B)  < 0; }
	friend BOOL operator  < (const TString& A, const TCHAR* B)    { return A.Compare(B)  < 0; }
	friend BOOL operator  < (const TCHAR* A,   const TString& B)  { return B.Compare(A)  > 0; }
	friend BOOL operator  > (const TString& A, const TString& B)  { return A.Compare(B)  > 0; }
	friend BOOL operator  > (const TString& A, const TCHAR* B)    { return A.Compare(B)  > 0; }
	friend BOOL operator  > (const TCHAR* A,   const TString& B)  { return B.Compare(A)  < 0; }
	friend BOOL operator <= (const TString& A, const TString& B)  { return A.Compare(B) <= 0; }
	friend BOOL operator <= (const TString& A, const TCHAR* B)    { return A.Compare(B) <= 0; }
	friend BOOL operator <= (const TCHAR* A,   const TString& B)  { return B.Compare(A) >= 0; }
	friend BOOL operator >= (const TString& A, const TString& B)  { return A.Compare(B) >= 0; }
	friend BOOL operator >= (const TString& A, const TCHAR* B)    { return A.Compare(B) >= 0; }
	friend BOOL operator >= (const TCHAR* A,   const TString& B)  { return B.Compare(A) <= 0; }

	// SubString
	TString Mid(INDEX_T nFirst, SIZE_T nCount = (SIZE_T)-1) const;
	TString Left(SIZE_T nCount) const;
	TString Right(SIZE_T nCount) const;

	TString SpanIncluding(const TCHAR* pszCharSet) const;
	TString SpanExcluding(const TCHAR* pszCharSet) const;

	void MakeUpper() { tszupr(m_pszData); }
	void MakeLower() { tszlwr(m_pszData); }
	void MakeReverse() { tszrev(m_pszData); }

	void Trim() { TrimLeft(); TrimRight(); }
	void TrimRight(TCHAR chTarget);
	void TrimRight(const TCHAR* pszTargets = NULL);
	void TrimLeft(TCHAR chTarget);
	void TrimLeft(const TCHAR* pszTargets = NULL);

	SIZE_T Replace(TCHAR chOld, TCHAR chNew);
	SIZE_T Replace(const TCHAR* pszOld, const TCHAR* pszNew);
	SIZE_T Remove(TCHAR chRemove);
	SIZE_T Insert(INDEX_T index, TCHAR ch);
	SIZE_T Insert(INDEX_T index, const TCHAR* psz);
	INDEX_T Delete(INDEX_T index, SIZE_T nCount = 1);

	INDEX_T Find(TCHAR ch, INDEX_T nStart = 0) const;
	INDEX_T Find(const TCHAR* psz, INDEX_T nStart = 0) const;
	INDEX_T ReverseFind(TCHAR ch, INDEX_T nStart = -1) const;
	INDEX_T FindOneOf(const TCHAR* pszCharSet) const;

	// simple formatting
	void Format(const TCHAR* pszFormat, ...);	// Maximum Length : 1024, Thread Unsafe
	void FormatV(const TCHAR* pszFormat, va_list argList);

	//// input and output
	//template < class STREAM >
	//friend STREAM& operator<<(STREAM& ar, const TString& string) {
	//	dfasdjf
	//	return ar;
	//}
	//template < class STREAM >
	//friend STREAM& operator>>(STREAM& ar, TString& string) {
	//	sdsd
	//	return ar;
	//}


//	BOOL LoadString(UINT nID);

	TCHAR* GetBuffer(SIZE_T nLen) { return PrepareBuffer(nLen, FALSE); }
	TCHAR* GetBufferSetLength(SIZE_T nLen) { if (!PrepareBuffer(nLen, FALSE)) return NULL; m_nLen = nLen; return m_pszData; }
	BOOL ReleaseBuffer() { m_nLen = tszlen(m_pszData); ASSERT(m_nLen <= m_nBufferSize); return (m_nLen <= m_nBufferSize); }

	template < class Archive, typename INDEX_T, typename SIZE_T >
	friend Archive& operator << (Archive& ar, const TString<TCHAR, INDEX_T, SIZE_T>& str) {
		if (str.GetLength() == 0) {
			ar << (BYTE)0;
			return ar;
		}
		if (sizeof(TCHAR) == sizeof(char)) {
		} else if (sizeof(TCHAR) == sizeof(wchar_t)) {
			ar << (BYTE)0xff;
			ar << (WORD)0xfffe;
		} else {
			ASSERT(FALSE);
			return ar;
		}

		if (str.GetLength() < 255)
			ar << (BYTE)str.GetLength();
		else if (str.GetLength() < 0xfffe) {
			ar << (BYTE)0xff;
			ar << (WORD)str.GetLength();
		} else {
			ar << (BYTE)0xff;
			ar << (WORD)0xffff;
			ar << (DWORD)str.GetLength();
		}
		SIZE_T nLen = str.GetLength()*sizeof(TCHAR);
		ar.Write(str, nLen);
		return ar;
	}

	template < class Archive >
	DWORD GetTStringLengthFromArchive(Archive& ar) {
		DWORD dwLength = 0;
		BYTE b;
		ar >> b;
		if (b == 0xff) {
			WORD w;
			ar >> w;
			if (w == 0xffff) {
				ar >> dwLength;
			} else if (w == 0xffff) {
				dwLength = (DWORD)-1;
			} else {
				dwLength = w;
			}
		} else {
			dwLength = b;
		}
		return dwLength;
	}

	template < class Archive, typename INDEX_T, typename SIZE_T >
	friend Archive& operator >> (Archive& ar, TString<TCHAR, INDEX_T, SIZE_T>& str) {
		str.Empty();
		if (sizeof(TCHAR) == sizeof(char)) {
		} else if (sizeof(TCHAR) == sizeof(wchar_t)) {
		} else {
			ASSERT(FALSE);
			return ar;
		}

		size32_t nSizeSourceChar = sizeof(char);
		size32_t nSizeTargetChar = sizeof(TCHAR);

		size32_t dwLength = str.GetTStringLengthFromArchive(ar);
		if (dwLength == (size32_t)-1) {	// UNICODE
			nSizeSourceChar = sizeof(wchar_t);
			dwLength = str.GetTStringLengthFromArchive(ar);
		}

		if (dwLength == (size32_t)-1) {
			AfxThrowArchiveException(CArchiveException::genericException);
			return ar;
		}

		if (dwLength) {
			if (nSizeSourceChar == nSizeTargetChar) {
				if (ar.Read(str.GetBufferSetLength(dwLength), dwLength*nSizeTargetChar) != dwLength*nSizeTargetChar)
					AfxThrowArchiveException(CArchiveException::endOfFile);
			} else {
				size32_t nSizeSource = dwLength*nSizeSourceChar;
				BYTE* pszSource = new BYTE[nSizeSource+nSizeSourceChar];
				memset(pszSource, 0, nSizeSource+nSizeSourceChar);
				if (ar.Read(pszSource, nSizeSource) != nSizeSource)
					AfxThrowArchiveException(CArchiveException::endOfFile);
				if (nSizeSourceChar < nSizeTargetChar) {
					sizeXX_t nCharCount = 0;
					nCharCount = ConvAnsiToUnicode((LPCSTR)pszSource, NULL, nCharCount);
					if (nCharCount > 0)
						ConvAnsiToUnicode((LPCSTR)pszSource, (LPWSTR)str.GetBufferSetLength(nCharCount), nCharCount);
				} else {
					sizeXX_t nCharCount = 0;
					nCharCount = ConvUnicodeToAnsi((LPCWSTR)pszSource, NULL, nCharCount);
					if (nCharCount > 0)
						ConvUnicodeToAnsi((LPCWSTR)pszSource, (LPSTR)str.GetBufferSetLength(nCharCount), nCharCount);
				}
			}
		}

		return ar;
	}


};


WARNING_POP()


template < typename TCHAR >
TString<TCHAR> TFormat(const TCHAR* pszFormat, ...);

#if !defined(_MFC_VER) && !defined(TCHAR)
	typedef char TCHAR;
#endif

typedef TString<TCHAR>		DString;
typedef TString<char>		DStringA;
typedef TString<wchar_t>	DStringW;

//extern "C" {
//WARNING_PUSH_AND_DISABLE(4273)
//		int vsprintf(char *buffer, const char *format, va_list argptr);
//		int vswprintf(wchar_t *buffer, sizeXX_t _Count, const wchar_t *format, va_list argptr);
//WARNING_POP()
//}
inline int _vsprintf(char *buffer, sizeXX_t _Count, const char *format, va_list argptr) {
	return vsprintf_s(buffer, _Count, format, argptr);
}
inline int _vsprintf(wchar_t *buffer, sizeXX_t _Count, const wchar_t *format, va_list argptr) {
	return vswprintf(buffer, _Count, format, argptr);
}

//#ifndef _FORCE_USE_CODEPAGE_949
//#	define _FORCE_USE_CODEPAGE_949 FALSE
//#endif
//
//AFX_EXT_API_MISC BOOL UnicodeToUTF8		(LPCWSTR pwszSource,	LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bPrintable = FALSE);
//AFX_EXT_API_MISC BOOL UnicodeToAnsi949	(LPCWSTR pwszSource,	LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bPrintable = FALSE);
//AFX_EXT_API_MISC BOOL Ansi949ToUnicode	(LPCSTR pszSource,		LPWSTR pwszOut,	sizeXX_t& nCharCount/*wchar_t count including NULL terminater*/,	BOOL bPrintable = FALSE);
//AFX_EXT_API_MISC BOOL Ansi949ToUTF8		(LPCSTR pszSource,		LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bPrintable = FALSE);
//AFX_EXT_API_MISC BOOL UTF8ToUnicode		(LPCSTR pszSource,		LPWSTR pwszOut,	sizeXX_t& nCharCount/*wchar_t count including NULL terminater*/,	BOOL bPrintable = FALSE);
//AFX_EXT_API_MISC BOOL UTF8ToAnsi949		(LPCSTR pszSource,		LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bPrintable = FALSE);
//
//AFX_EXT_API_MISC BOOL ConvUnicodeToUTF8		(LPCWSTR pwszSource,	LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvUnicodeToAnsi		(LPCWSTR pwszSource,	LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvAnsiToUnicode		(LPCSTR pszSource,		LPWSTR pwszOut,	sizeXX_t& nCharCount/*wchar_t count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvAnsiToUTF8		(LPCSTR pszSource,		LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvUTF8ToUnicode		(LPCSTR pszSource,		LPWSTR pwszOut,	sizeXX_t& nCharCount/*wchar_t count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvUTF8ToAnsi		(LPCSTR pszSource,		LPSTR pszOut,	sizeXX_t& nCharCount/*char count including NULL terminater*/,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//
//AFX_EXT_API_MISC BOOL ConvUnicodeToUTF8		(LPCWSTR pwszSource,	TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvUnicodeToAnsi		(LPCWSTR pwszSource,	TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvAnsiToUnicode		(LPCSTR pszSource,		TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvAnsiToUTF8		(LPCSTR pszSource,		TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvUTF8ToUnicode		(LPCSTR pszSource,		TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvUTF8ToAnsi		(LPCSTR pszSource,		TBuffer<BYTE>& buffer,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//
//AFX_EXT_API_MISC BOOL ConvUnicodeToUTF8		(LPCWSTR pwszSource,	CStringA& strA,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvUnicodeToAnsi		(LPCWSTR pwszSource,	CStringA& strA,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvAnsiToUnicode		(LPCSTR pszSource,		CStringW& strW,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvAnsiToUTF8		(LPCSTR pszSource,		CStringA& strA,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvUTF8ToUnicode		(LPCSTR pszSource,		CStringW& strW,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//AFX_EXT_API_MISC BOOL ConvUTF8ToAnsi		(LPCSTR pszSource,		CStringA& strA,	BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949);
//
//inline CStringA CONV_U8(LPCSTR pszSource,  BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949) { CStringA strU8; ConvAnsiToUTF8(pszSource, strU8);    return std::move(strU8); }		// move constructor 없지만..
//inline CStringA CONV_U8(LPCWSTR pszSource, BOOL bForceUseCodepage949 = _FORCE_USE_CODEPAGE_949) { CStringA strU8; ConvUnicodeToUTF8(pszSource, strU8); return std::move(strU8); }
//
//AFX_EXT_API_MISC BOOL IsUTF8String(const char sz[], int* pOutputBufferCount = NULL, BOOL* pbIsMSBSet = NULL);
//AFX_EXT_API_MISC BOOL Conv8BitToUnicode(LPCSTR pszSource, CStringW& strW);	// UTF8 or MBCS to


//#undef FORCE_USE_CODEPAGE_949

#include "TString.hpp"

#endif	// __TSTRING_H__
