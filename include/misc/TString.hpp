//////////////////////////////////////////////////////////////////////////////
// Primitives

#ifndef ASSERT
#include <assert.h>
#define ASSERT assert
#endif

WARNING_PUSH_AND_DISABLE(4018 4244)

inline const char* GetSpaceString(const char* ) { return " \t\r\n"; }
inline const wchar_t* GetSpaceString(const wchar_t* ) { return L" \t\r\n"; }

template < typename TCHAR > sizeXX_t tszlen(const TCHAR* psz) {
	if (!psz) return 0;
	const TCHAR* pos = psz;
	for (;*pos; pos++)
		;
	return pos - psz;
}
template < typename TCHAR > TCHAR* tszcpy(TCHAR* pszDest, const TCHAR* pszSrc) {
	if (!pszDest)
		return NULL;
	if (!pszSrc) {
		pszDest[0] = 0;
		return pszDest;
	}

	TCHAR* pos = pszDest;
	while (*pszSrc) { *pos++ = *pszSrc++; };
	*pos = NULL;
	return pszDest;
}
template < typename TCHAR > TCHAR* tszncpy(TCHAR* pszDest, const TCHAR* pszSrc, sizeXX_t nLen) {
	if ( !pszDest || (nLen <= 0) )
		return NULL;
	if (!pszSrc) {
		pszDest[0] = 0;
		return pszDest;
	}
	TCHAR* pos = pszDest;
	while ((nLen-- > 1) && *pszSrc) { *pos++ = *pszSrc++; }
	*pos = NULL;
	return pszDest;
}

template < typename TCHAR > TCHAR* tszcat(TCHAR* pszDest, const TCHAR* pszSrc) {
	if (!pszDest || !pszSrc)
		return NULL;
	TCHAR* pos = pszDest;
	for (;*pos; pos++)
		;
	do { *pos++ = *pszSrc++; } while (*pszSrc);
	*pos = NULL;
	return pszDest;
}
template < typename TCHAR > int tszcmp(const TCHAR* pszA, const TCHAR* pszB) {
	if (!pszA && !pszB)
		return 0;
	if (!pszA || !pszB) {
		if (pszA) return -(int)pszA[0];
		if (pszB) return (int)pszB[0];
		return 0;
	}

	for (; ; pszA++, pszB++) {
		if (*pszA != *pszB)
			return pszA[0] - pszB[0];
		if (!*pszA || !*pszB)
			return 0;
	}
	return 0;
}
template < typename TCHAR > int tszncmp(const TCHAR* pszA, const TCHAR* pszB, sizeXX_t nLen) {
	if (!pszA && !pszB)
		return 0;
	if (!pszA || !pszB) {
		if (pszA) return -(int)pszA[0];
		if (pszB) return (int)pszB[0];
		return 0;
	}

	for (; nLen-- > 0; pszA++, pszB++) {
		if (*pszA != *pszB)
			return pszA[0] - pszB[0];
		if (!*pszA || !*pszB)
			return 0;
	}
	return 0;
}
template < typename TCHAR > int tszicmp(const TCHAR* pszA, const TCHAR* pszB) {
	if (!pszA && !pszB)
		return 0;
	if (!pszA || !pszB) {
		if (pszA) return -(int)pszA[0];
		if (pszB) return (int)pszB[0];
		return 0;
	}

	for (; ; pszA++, pszB++) {
		int c1 = __tolower(*pszA);
		int c2 = __tolower(*pszB);
		if (c1 != c2)
			return c1 - c2;
		if (!c1 || !c2)
			return 0;
	}
	return 0;
}
template < typename TCHAR > int tsznicmp(const TCHAR* pszA, const TCHAR* pszB, sizeXX_t nLen) {
	if (!pszA && !pszB)
		return 0;
	if (!pszA || !pszB) {
		if (pszA) return -(int)pszA[0];
		if (pszB) return (int)pszB[0];
		return 0;
	}

	for (; nLen-- > 0; pszA++, pszB++) {
		int c1 = __tolower(*pszA);
		int c2 = __tolower(*pszB);
		if (c1 != c2)
			return c1 - c2;
		if (!c1 || !c2)
			return 0;
	}
	return 0;
}

template < typename TCHAR > int CompareNumberedStringT(const TCHAR* pszA, const TCHAR* pszB, BOOL bIgnoreCase = FALSE) {
	const TCHAR* p0 = pszA;
	const TCHAR* p1 = pszB;
	for (; *p0 && *p1; p0++, p1++) {
		TCHAR c0 = *p0;
		TCHAR c1 = *p1;
		if (__isdigit(c0) && __isdigit(c1)) {	// for numbers

			// skip '0'
			while (c0 == '0')
				c0 = *(++p0);
			while (c1 == '0')
				c1 = *(++p1);

			// Count Digit Length
			int nDigit0, nDigit1;
			for (nDigit0 = 0; __isdigit(p0[nDigit0]); nDigit0++)
				;
			for (nDigit1 = 0; __isdigit(p1[nDigit1]); nDigit1++)
				;

			// Compare
			if (nDigit0 == nDigit1) {
				if (nDigit0 == 0) {
					continue;
				}
				for (; (nDigit0 > 0) && *p0 && *p1; p0++, p1++, nDigit0--) {
					if (*p0 == *p1)
						continue;
					return *p0 - *p1;
				}
				p0--;
				p1--;
			} else
				return nDigit0 - nDigit1;
		} else {
			if (bIgnoreCase) { c0 = (TCHAR)__tolower(c0); c1 = (TCHAR)__tolower(c1); }
			if (c0 == c1)
				continue;
			return c0 - c1;
		}
	}

	if (bIgnoreCase)
		return tszicmp(p0, p1);
	return tszcmp(p0, p1);
}

template < typename TCHAR > int tdszcmp(const TCHAR* pszA, const TCHAR* pszB) { return CompareNumberedStringT<TCHAR>(pszA, pszB, FALSE); }
template < typename TCHAR > int tdszicmp(const TCHAR* pszA, const TCHAR* pszB) { return CompareNumberedStringT<TCHAR>(pszA, pszB, TRUE); }

template < typename TCHAR > TCHAR* tszupr(TCHAR* psz) {
	if (!psz)
		return NULL;
	for (TCHAR* pos = psz; *pos; pos++)
		if ( (*pos >= 'a') && (*pos <= 'z') )
			*pos = _toupper(*pos);
	return psz;
}
template < typename TCHAR > TCHAR* tszlwr(TCHAR* psz) {
	if (!psz)
		return NULL;
	for (TCHAR* pos = psz; *pos; pos++)
		if ( (*pos >= 'A') && (*pos <= 'Z') )
			*pos = _tolower(*pos);
	return psz;
}
template < typename TCHAR > TCHAR* tszrev(TCHAR* psz) {
	if (!psz)
		return NULL;
	TCHAR* head = psz;
	TCHAR* tail = psz + tszlen(psz)-1;
	while (head < tail) {
		TCHAR c = *head;
		*head = *tail;
		*tail = c;
		head++, tail--;
	}
	return psz;
}
template < typename TCHAR > TCHAR* tszsearch(const TCHAR* psz, int c) {
	if (!psz)
		return NULL;
	for (const TCHAR* pos = psz; *pos; pos++)
		if (*pos == c) return (TCHAR*)pos;
	return NULL;
}
template < typename TCHAR > TCHAR* tszsearch(const TCHAR* psz, const TCHAR* pszSearch) {
	if (!psz || !pszSearch)
		return NULL;
	sizeXX_t nLen = tszlen(psz);
	sizeXX_t nLenSearch = tszlen(pszSearch);
	if (nLen < nLenSearch)
		return NULL;
	if (!nLen || !nLenSearch)
		return NULL;
	sizeXX_t n = nLen - nLenSearch;
	for (intXX_t i = 0; i <= n; i++) {
		if (tszncmp(psz+i, pszSearch, nLenSearch) == 0)
			return (TCHAR*)psz+i;
	}
	return NULL;
}
template < typename TCHAR, typename T > T ttsztoi(const TCHAR* psz) {
	// skip white space
	while (__isspace(*psz)) psz++;

	// Check +/-
	BOOL bMinus = FALSE;
	if (*psz == '-')
		psz++, bMinus = TRUE;
	else if (*psz == '+')
		psz++;

	// skip white space
	while (__isspace(*psz)) psz++;

	intXX_t value = 0;
	while (*psz) {
		TCHAR c = *psz;
		if ( (c >= '0') && (c <= '9') ) {
			value *= 10;
			value += c-'0';
		} else
			break;
		psz++;
	}
	if (bMinus)
		value = -value;
	return value;
}
template < typename TCHAR, typename T > T ttsztoi16(const TCHAR* psz) {
	// skip white space
	while (__isspace(*psz)) psz++;

	// Check +/-
	BOOL bMinus = FALSE;
	if (*psz == '-')
		psz++, bMinus = TRUE;
	else if (*psz == '+')
		psz++;

	// skip white space
	while (__isspace(*psz)) psz++;

	intXX_t value = 0;
	while (*psz) {
		TCHAR c = *psz;
		if ( (c >= '0') && (c <= '9') ) {
			value <<= 4;
			value += c-'0';
		} else if ( (c >= 'a') && (c <= 'f') ) {
			value <<= 4;
			value += c-'a'+10;
		} else if ( (c >= 'A') && (c <= 'F') ) {
			value <<= 4;
			value += c-'A'+10;
		} else
			break;
		psz++;
	}
	return value;
}
template < typename TCHAR > intXX_t tsztoi(const TCHAR* psz)		{ return ttsztoi<TCHAR, intXX_t>(psz); }
template < typename TCHAR > intXX_t tsztoi16(const TCHAR* psz)	{ return ttsztoi16<TCHAR, intXX_t>(psz); }

template < typename TCHAR > int tszrmchar(TCHAR* psz, int chRemove) {	// Remove Charactor from str. returns str length
	if (!psz || !chRemove)
		return 0;
	TCHAR* pszHeader = psz;
	do {
		if (*psz == chRemove) {
			TCHAR* p = psz+1;
			do {
				if (*p == chRemove)
					p++;
				else
					if (!(*psz++ = *p++))
						break;
			} while(TRUE);
			break;
		}
	} while (*psz++);
	return psz - pszHeader - 1;
}

//////////////////////////////////////////////////////////////////////////////
// TString Member Function

//============================================================================
// Buffer Manage
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TCHAR* TString<TCHAR, INDEX_T, SIZE_T>::Attach(TCHAR* psz, SIZE_T nBufferSize) {
	TCHAR* pszDataOld = m_pszData;
	m_nBufferSize = nBufferSize;
	m_pszData = psz;
	m_nLen = tszlen(psz);
	return pszDataOld;
}
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TCHAR* TString<TCHAR, INDEX_T, SIZE_T>::Detach() {
	TCHAR* pszDataOld = m_pszData;
	m_nBufferSize = 0;
	m_pszData = NULL;
	m_nLen = 0;
	return pszDataOld;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
BOOL TString<TCHAR, INDEX_T, SIZE_T>::Init(SIZE_T nMinLen) {
	if (m_pszData && m_nBufferSize) {
		delete [] m_pszData;
		m_pszData = NULL;
		m_nBufferSize = 0;
		m_nLen = 0;
	}

	SIZE_T nBufferSize;
	if ( (nMinLen == 0) || (nMinLen == (SIZE_T)-1) )
		nMinLen = 31;

	if (nMinLen > 0)
		nBufferSize = nMinLen + 1;
	else
		nBufferSize = 0;

	if (nBufferSize < 0)
		return FALSE;
	else if (nBufferSize == 0) {
		;
	} else if (nBufferSize <= 32) {
		nBufferSize = 32;
	} else if (nBufferSize <= 64) {
		nBufferSize = 64;
	} else if (nBufferSize <= 128) {
		nBufferSize = 128;
	} else if (nBufferSize <= 256) {
		nBufferSize = 256;
	} else
		nBufferSize += 32 - (nBufferSize % 32);	// 32 bytes align. (additional 32 bytes (max))

	m_nBufferSize = nBufferSize;
	if (m_nBufferSize > 0) {
		m_pszData = new TCHAR [m_nBufferSize];
		if (!m_pszData) {
			ASSERT(FALSE);
			m_nBufferSize = 0;
			return FALSE;
		}
		memset(m_pszData, 0, sizeof(TCHAR)*m_nBufferSize);
	}

	return TRUE;
}
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TCHAR* TString<TCHAR, INDEX_T, SIZE_T>::PrepareBuffer(SIZE_T nNewLen, BOOL bKeepOriginal) {
	if (bKeepOriginal && (nNewLen < m_nLen))
		nNewLen = m_nLen;

	if (!nNewLen) {
		if (m_pszData && m_nBufferSize)
			m_pszData[0] = NULL;
		return NULL;
	}

	if (nNewLen < m_nBufferSize) {
		m_pszData[nNewLen] = 0;
		return m_pszData;
	}

	TCHAR* buf = NULL;
	SIZE_T nLen = m_nLen;
	if (bKeepOriginal && nLen) {			// Backup
		buf = new TCHAR[nLen];
		memmove(buf, m_pszData, nLen*sizeof(TCHAR));
	}

	Init(nNewLen);

	if (bKeepOriginal && nLen && buf) {		// Restore
		memmove(m_pszData, buf, nLen*sizeof(TCHAR));
		//m_pszData[nLen] = 0; // don't need. (Init 에서 NULL 로 세팅함)
		delete [] buf;
		m_nLen = nLen;
	}

	return m_pszData;
}


//============================================================================
// Assign Operator
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T>& TString<TCHAR, INDEX_T, SIZE_T>::operator = (const TString<char>& B) {
	if ((void*)this == (void*)&B)
		return *this;
	if (sizeof(TCHAR) == sizeof(char)) {
		Init(B.GetLength());
		if ((const char*)B && B.GetLength())
			memmove(m_pszData, (const char*)B, sizeof(char)*B.GetLength());
		m_nLen = B.GetLength();
	} else {
		*this = (const char*)B;
	}

	return *this;
}
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T>& TString<TCHAR, INDEX_T, SIZE_T>::operator = (const TString<wchar_t>& B) {
	if ((void*)this == (void*)&B)
		return *this;
	if (sizeof(TCHAR) == sizeof(wchar_t)) {
		Init(B.GetLength());
		if ((const wchar_t*)B && B.GetLength())
			memmove(m_pszData, (const wchar_t*)B, sizeof(wchar_t)*B.GetLength());
		m_nLen = B.GetLength();
	} else {
		*this = (const wchar_t*)B;
	}
	return *this;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T>& TString<TCHAR, INDEX_T, SIZE_T>::operator = (const TCHAR ch) {
	if (ch) {
		Init(1);
		m_pszData[0] = ch;
		m_nLen = 1;
	} else
		Init();
	return *this;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T>& TString<TCHAR, INDEX_T, SIZE_T>::operator = (const char* psz) {
	if (sizeof(TCHAR) == sizeof(char)) {
		SIZE_T nLen = tszlen(psz);
		if (nLen) {
			Init(nLen);
			memmove(m_pszData, psz, sizeof(TCHAR)*nLen);
			m_nLen = nLen;
		}
		else
			Init();
	} else {
		sizeXX_t nChar = 0;
		ConvAnsiToUnicode(psz, NULL, nChar);
		if (nChar) {
			ConvAnsiToUnicode(psz, (LPWSTR)GetBuffer(nChar), nChar);
			ReleaseBuffer();
		} else
			Empty();
	}
	return *this;
}
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T>& TString<TCHAR, INDEX_T, SIZE_T>::operator = (const wchar_t* psz) {
	if (sizeof(TCHAR) == sizeof(wchar_t)) {
		SIZE_T nLen = tszlen(psz);
		if (nLen) {
			Init(nLen);
			memmove(m_pszData, psz, sizeof(TCHAR)*nLen);
			m_nLen = nLen;
		}
		else
			Init();
	} else {
		sizeXX_t nChar = 0;
		ConvUnicodeToAnsi(psz, NULL, nChar);
		if (nChar) {
			ConvUnicodeToAnsi(psz, (LPSTR)GetBuffer(nChar), nChar);
			ReleaseBuffer();
		} else
			Empty();
	}
	return *this;
}

//============================================================================
// Operators
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T>& TString<TCHAR, INDEX_T, SIZE_T>::operator += (const TString<TCHAR, INDEX_T, SIZE_T>& B) {
	if (this == &B) {
		TString<TCHAR, INDEX_T, SIZE_T> strBuf = *this;
		Init(m_nLen*2);
		memmove(m_pszData, strBuf, strBuf.GetLength()*sizeof(TCHAR));
		memmove(m_pszData+(strBuf.GetLength()*sizeof(TCHAR)), strBuf, strBuf.GetLength()*sizeof(TCHAR));
	} else {
		if (B.m_nLen) {
			PrepareBuffer(m_nLen + B.m_nLen, TRUE);
			memmove(m_pszData+m_nLen, B.m_pszData, sizeof(TCHAR)*B.m_nLen);
			m_nLen += B.m_nLen;
		}
	}
	return *this;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T>& TString<TCHAR, INDEX_T, SIZE_T>::operator += (const TCHAR ch) {
	if (!ch)
		return *this;
	PrepareBuffer(m_nLen+1, TRUE);
	m_pszData[m_nLen++] = ch;
	return *this;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T>& TString<TCHAR, INDEX_T, SIZE_T>::operator += (const TCHAR* psz) {
	SIZE_T nLen = tszlen(psz);
	if (!nLen)
		return *this;
	PrepareBuffer(m_nLen+nLen, TRUE);
	memmove(m_pszData+m_nLen, psz, sizeof(TCHAR)*nLen);
	m_nLen += nLen;
	return *this;
}


//============================================================================
// SubString
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T> TString<TCHAR, INDEX_T, SIZE_T>::Mid(INDEX_T nFirst, SIZE_T nCount) const {
	TString<TCHAR, INDEX_T, SIZE_T> str;
	if (nCount == 0)
		return str;

	if (nFirst >= m_nLen)
		return str;

	if (nCount == (SIZE_T)-1)
		nCount = m_nLen - nFirst;

	ASSERT( (nFirst >= 0) && (nFirst + nCount <= m_nLen) );
	if (nFirst + nCount > m_nLen)
		return str;

	str.Init(nCount);
	memmove(str.m_pszData, m_pszData+nFirst, sizeof(TCHAR)*nCount);
	str.m_nLen = nCount;

	return str;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T> TString<TCHAR, INDEX_T, SIZE_T>::Left(SIZE_T nCount) const {
	TString<TCHAR, INDEX_T, SIZE_T> str;

	if (nCount <= 0)
		return str;
	if (nCount == (SIZE_T)-1)
		nCount = m_nLen;

	str.Init(nCount);
	str.m_nLen = _min(nCount, m_nLen);
	if (str.m_nLen)
		memmove(str.m_pszData, m_pszData, sizeof(TCHAR)*str.m_nLen);

	return str;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T> TString<TCHAR, INDEX_T, SIZE_T>::Right(SIZE_T nCount) const {
	TString<TCHAR, INDEX_T, SIZE_T> str;

	if (nCount <= 0)
		return str;
	if (nCount == (SIZE_T)-1)
		nCount = m_nLen;

	str.Init(nCount);
	str.m_nLen = _min(nCount, m_nLen);
	if (str.m_nLen)
		memmove(str.m_pszData, m_pszData+m_nLen-str.m_nLen, sizeof(TCHAR)*str.m_nLen);

	return str;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T> TString<TCHAR, INDEX_T, SIZE_T>::SpanIncluding(const TCHAR* pszCharSet) const {
	TString<TCHAR, INDEX_T, SIZE_T> str;

	if (!tszlen(pszCharSet))
		return str;

	SIZE_T nCount = 0;
	for (INDEX_T i = 0; i < m_nLen; i++) {
		for (const TCHAR* pos = pszCharSet; *pos; pos++) {
			if (m_pszData[i] != *pos)
				continue;
			nCount++;
			break;
		}
	}

	if (!nCount)
		return str;

	if (nCount == m_nLen)
		return *this;

	str.Init(nCount);

	for (INDEX_T i = 0; i < m_nLen; i++) {
		for (const TCHAR* pos = pszCharSet; *pos; pos++) {
			if (m_pszData[i] != *pos)
				continue;
			str.m_pszData[str.m_nLen++] = *pos;
			break;
		}
	}
	return str;
}
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T> TString<TCHAR, INDEX_T, SIZE_T>::SpanExcluding(const TCHAR* pszCharSet) const {
	if (!tszlen(pszCharSet))
		return *this;

	SIZE_T nCount = m_nLen;
	for (INDEX_T i = 0; i < m_nLen; i++) {
		const TCHAR* pos = NULL;
		for (pos = pszCharSet; pos && *pos; pos++) {
			if (m_pszData[i] == *pos)
				break;
		}
		if (pos && *pos)
			nCount--;
	}

	TString<TCHAR, INDEX_T, SIZE_T> str;
	if (!nCount)
		return str;
	if (nCount == m_nLen)
		return str;

	str.Init(nCount);

	for (INDEX_T i = 0; i < m_nLen; i++) {
		const TCHAR* pos = NULL;
		for (pos = pszCharSet; pos && *pos; pos++) {
			if (m_pszData[i] == *pos)
				break;
		}
		if (!*pos)
			str.m_pszData[str.m_nLen++] += m_pszData[i];
	}
	return str;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
void TString<TCHAR, INDEX_T, SIZE_T>::TrimRight(TCHAR chTarget) {
	INDEX_T i = 0;
	for (i = m_nLen - 1; i >= 0; i--) {
		if (m_pszData[i] != chTarget)
			break;
	}
	if (i != m_nLen - 1) {
		m_nLen = i+1;
		m_pszData[m_nLen] = 0;
	}
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
void TString<TCHAR, INDEX_T, SIZE_T>::TrimRight(const TCHAR* pszTargets) {
	if (!pszTargets)
		pszTargets = GetSpaceString(pszTargets);

	if (m_nLen <= 0)
		return;

	for (INDEX_T iEnd = m_nLen - 1; iEnd >= 0; iEnd--) {
		if (tszsearch(pszTargets, m_pszData[iEnd]))
			continue;
		m_nLen = iEnd+1;
		m_pszData[m_nLen] = 0;
		return;
	}
	m_nLen = 0;
	m_pszData[m_nLen] = 0;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
void TString<TCHAR, INDEX_T, SIZE_T>::TrimLeft(TCHAR chTarget) {
	INDEX_T iStart = 0;
	for (iStart = 0; iStart < m_nLen; iStart++) {
		if (m_pszData[iStart] != chTarget)
			break;
	}
	if (!iStart)
		return;

	m_nLen -= iStart;
	TCHAR* dst = m_pszData;
	TCHAR* pos = m_pszData + iStart;
	for (INDEX_T i = 0; i < m_nLen; i++)
		*dst++ = *pos++;
	m_pszData[m_nLen] = 0;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
void TString<TCHAR, INDEX_T, SIZE_T>::TrimLeft(const TCHAR* pszTargets) {
	if (!pszTargets)
		pszTargets = GetSpaceString(pszTargets);

	INDEX_T iStart = 0;
	for (iStart = 0; iStart < m_nLen; iStart++) {
		if (!tszsearch(pszTargets, m_pszData[iStart]))
			break;
	}
	if (!iStart)
		return;

	m_nLen -= iStart;
	TCHAR* dst = m_pszData;
	TCHAR* pos = m_pszData + iStart;
	for (INDEX_T i = 0; i < m_nLen; i++)
		*dst++ = *pos++;
	m_pszData[m_nLen] = 0;
}

//============================================================================
// String Manipulation
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
SIZE_T TString<TCHAR, INDEX_T, SIZE_T>::Replace(TCHAR chOld, TCHAR chNew) {
	ASSERT(chOld && chNew);
	SIZE_T nReplaced = 0;
	for (INDEX_T i = 0; i < m_nLen; i++) {
		if (m_pszData[i] == chOld) {
			m_pszData[i] = chNew;
			nReplaced++;
		}
	}
	return nReplaced;
}
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
SIZE_T TString<TCHAR, INDEX_T, SIZE_T>::Replace(const TCHAR* pszOld, const TCHAR* pszNew) {
	ASSERT(pszOld && pszNew);
	SIZE_T nLenOld = tszlen(pszOld);
	SIZE_T nLenNew = tszlen(pszNew);
	SIZE_T nReplaced = 0;
	for (INDEX_T i = 0; i < m_nLen; i++) {
		if (tszncmp(m_pszData+i, pszOld, nLenOld) == 0) {
			i += nLenOld;
			i--;
			nReplaced++;
		}
	}
	if (!nReplaced)
		return 0;
	SIZE_T nLen = m_nLen + (nLenNew - nLenOld) * nReplaced;
	TCHAR* buf = Detach();
	Init(nLen);
	TCHAR* pos = m_pszData;
	for (TCHAR* pOriginal = buf; *pOriginal; ) {
		if (tszncmp(pOriginal, pszOld, nLenOld) == 0) {
			if (nLenNew)
				memmove(pos, pszNew, nLenNew*sizeof(TCHAR));
			pos += nLenNew;
			pOriginal += nLenOld;
		} else {
			*pos++ = *pOriginal++;
		}
	}
	m_nLen = nLen;

	delete [] buf;

	return nReplaced;
}
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
SIZE_T TString<TCHAR, INDEX_T, SIZE_T>::Remove(TCHAR chRemove) {
	TCHAR* pos1 = m_pszData;
	TCHAR* pos2 = m_pszData;
	SIZE_T nReplaced = 0;
	while (*pos2) {
		if (*pos2 == chRemove) {
			*pos2++;
			nReplaced++;
		} else {
			if (nReplaced)
				*pos1++ = *pos2++;
		}
	}
	return nReplaced;
}
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
SIZE_T TString<TCHAR, INDEX_T, SIZE_T>::Insert(INDEX_T index, TCHAR ch) {
	ASSERT(index <= m_nLen);
	if ( (index < 0) || (index >= m_nLen) )
		return m_nLen;

	PrepareBuffer(m_nLen+1, TRUE);	// m_nLen 이 0일 경우 PrepareBuffer에서 1이 됨.
	for (INDEX_T i = m_nLen-1; i >= index; i--) {
		m_pszData[i+1] = m_pszData[i];
	}
	m_pszData[index] = ch;
	m_nLen++;
	return m_nLen;
}
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
SIZE_T TString<TCHAR, INDEX_T, SIZE_T>::Insert(INDEX_T index, const TCHAR* psz) {
	ASSERT(index <= m_nLen);
	if ( (index < 0) || (index >= m_nLen) )
		return m_nLen;

	SIZE_T nLenAdd = tszlen(psz);

	PrepareBuffer(m_nLen+nLenAdd, TRUE);
	for (INDEX_T i = m_nLen-1; i >= index; i--) {
		m_pszData[i+nLenAdd] = m_pszData[i];
	}
	if (nLenAdd)
		memmove(m_pszData+index, psz, nLenAdd*sizeof(TCHAR));
	m_nLen += nLenAdd;
	return m_nLen;
}
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
INDEX_T TString<TCHAR, INDEX_T, SIZE_T>::Delete(INDEX_T index, SIZE_T nCount) {
	ASSERT(index < m_nLen);
	if ( (index < 0) || (index >= m_nLen) )
		return -1;

	if (index + nCount > m_nLen)
		nCount = m_nLen - index;

	m_nLen -= nCount;
	for (INDEX_T i = index; i <= m_nLen; i++)			// including NULL terminating character
		m_pszData[i] = m_pszData[i+nCount];
	return m_nLen;
}

//============================================================================
// Searching

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
INDEX_T TString<TCHAR, INDEX_T, SIZE_T>::Find(TCHAR ch, INDEX_T nStart) const {
	TCHAR* pLast = m_pszData+m_nLen;
	for (TCHAR* pos = m_pszData + nStart; pos < pLast; pos++) {
		if (*pos == ch)
			return pos - m_pszData;
	}
	return -1;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
INDEX_T TString<TCHAR, INDEX_T, SIZE_T>::Find(const TCHAR* psz, INDEX_T nStart) const {
	SIZE_T nLen = tszlen(psz);
	ASSERT(nLen);
	if (nLen <= 0)
		return -1;
	TCHAR* pLast = m_pszData+m_nLen-nLen;
	for (TCHAR* pos = m_pszData + nStart; pos < pLast; pos++) {
		if (tszncmp(pos, psz, nLen) == 0)
			return pos - m_pszData;
	}
	return -1;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
INDEX_T TString<TCHAR, INDEX_T, SIZE_T>::ReverseFind(TCHAR ch, INDEX_T nStart) const {
	ASSERT(m_nLen > nStart);
	if (m_nLen <= nStart)
		return -1;
	if (nStart < 0)
		nStart = m_nLen-1;
	for (TCHAR* pos = m_pszData + nStart; pos >= m_pszData; pos--) {
		if (*pos == ch)
			return pos - m_pszData;
	}
	return -1;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
INDEX_T TString<TCHAR, INDEX_T, SIZE_T>::FindOneOf(const TCHAR* pszCharSet) const {
	for (TCHAR* pos = m_pszData; *pos; pos++) {
		if (tszsearch(pszCharSet, *pos) == 0)
			return pos - m_pszData;
	}
	return -1;
}

//============================================================================
// Format
template < typename TCHAR, typename INDEX_T, typename SIZE_T >
void TString<TCHAR, INDEX_T, SIZE_T>::Format(const TCHAR* pszFormat, ...) {
	va_list arglist;
	va_start(arglist, pszFormat);

	FormatV(pszFormat, arglist);

	va_end(arglist);
	return;
}

template < typename TCHAR, typename INDEX_T, typename SIZE_T >
void TString<TCHAR, INDEX_T, SIZE_T>::FormatV(const TCHAR* pszFormat, va_list argList) {
	TCHAR buf[4096];	// "2048" could be problem. ... but I have not enough time...

	_vsprintf(buf, countof(buf), pszFormat, argList);

	*this = buf;
}


template < typename TCHAR, typename INDEX_T, typename SIZE_T >
TString<TCHAR, INDEX_T, SIZE_T> TFormat(const TCHAR* pszFormat, ...) {
	va_list arglist;
	va_start(arglist, pszFormat);

	TString<TCHAR, INDEX_T, SIZE_T> str;
	str.FormatV(pszFormat, arglist);

	va_end(arglist);
	return str;
}

//============================================================================
// String
//


WARNING_POP()
