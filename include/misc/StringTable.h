#pragma once

#include "AFX_EXT_MISC.h"

#include "xUtil.h"

#pragma pack(push, 8)	// default align. (8 bytes)

class DStringItem;
class DStringTable;

//=============================================================================
// String Item
//
#define MAX_LANGUAGE 3
//template<int nMaxLanguage>
class DStringItem {
	friend class DStringTable;
protected:
	DString m_strID;
	DString m_strValues[MAX_LANGUAGE];

public:
	DStringItem() {}
	DStringItem(LPCSTR pszID) : m_strID(pszID) {
	}
	DStringItem(LPCWSTR pszID) : m_strID(pszID) {
	}
	DStringItem(const DStringItem& B) { *this = B; }
	DStringItem& operator = (const DStringItem& B);

	BOOL SetString(TList<DStringA>& strs);
	BOOL SetString(TList<DStringW>& strs);

	BOOL operator == (LPCSTR pszID) const;
	BOOL operator == (LPCWSTR pszID) const;

	const DString& GetString(int iLanguage);
	
};

//=============================================================================
// String Table
//
class AFX_EXT_CLASS_MISC DStringTable {
protected:
	TList<DStringItem> m_table;
	int m_iCurrentLanguage;

public:
	DStringTable(int iDefaultLanguage = 0) : m_iCurrentLanguage(iDefaultLanguage) {
	}
	DStringTable(LPCSTR pszFile, int iDefaultLanguage = 0) : m_iCurrentLanguage(iDefaultLanguage) {
		LoadStringTable(pszFile);
	}
	DStringTable(LPCWSTR pszFile, int iDefaultLanguage = 0) : m_iCurrentLanguage(iDefaultLanguage) {
		LoadStringTable(pszFile);
	}
	~DStringTable(void) {
	}

	// Select & Get Language
	int GetCurrentLanguage() const { return m_iCurrentLanguage; }
	int SetCurrentLanguage(int iLanguage) {
		int iOldLanguage = m_iCurrentLanguage;
		m_iCurrentLanguage = iLanguage;
		return iOldLanguage;
	}
	int GetLanguageCount() { DStringItem item; return countof(item.m_strValues); }

	// table file
public:
	BOOL LoadStringTable(LPCSTR pszFile);
	BOOL LoadStringTable(LPCWSTR pszFile);

	// String Item
public:
	DString  GetString(LPCSTR   pszID, int iLanguage = -1) const;
	DString  GetString(LPCWSTR  pszID, int iLanguage = -1) const;
	DString  GetString(intXX_t    iID,   int iLanguage = -1) const;
	CString  GetCString(LPCSTR  pszID, int iLanguage = -1) const;
	CString  GetCString(LPCWSTR pszID, int iLanguage = -1) const;
	CString  GetCString(intXX_t   iID,   int iLanguage = -1) const;
	DStringA GetAString(LPCSTR  pszID, int iLanguage = -1) const;
	DStringA GetAString(LPCWSTR pszID, int iLanguage = -1) const;
	DStringA GetAString(intXX_t   iID,   int iLanguage = -1) const;
	DStringW GetWString(LPCSTR  pszID, int iLanguage = -1) const;
	DStringW GetWString(LPCWSTR pszID, int iLanguage = -1) const;
	DStringW GetWString(intXX_t   iID,   int iLanguage = -1) const;

	LPCTSTR operator[] (intXX_t iID) const;
	LPCTSTR operator[] (LPCSTR pszID) const;
	LPCTSTR operator[] (LPCWSTR pszID) const;
	LPCTSTR operator() (intXX_t iID, int iLanguage = -1) const;
	LPCTSTR operator() (LPCSTR pszID, int iLanguage = -1) const;
	LPCTSTR operator() (LPCWSTR pszID, int iLanguage = -1) const;
protected:

public:
	static BOOL MakeStringTableHeader(LPCTSTR pszTextFile);
};

#pragma pack(pop)
