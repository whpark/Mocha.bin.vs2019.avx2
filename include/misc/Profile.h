#pragma once

#include "AFX_EXT_MISC.h"
#include "xUtil.h"

#pragma pack(push, 8)

class CProfileSection;
class CProfileItem;
class CProfile;

class AFX_EXT_CLASS_MISC CProfileItem {
friend class CProfileSection;
friend class CProfile;
protected:
	CString m_strOriginal;
	CString m_strName;
	CString m_strValue;
public:

	CProfileItem(LPCTSTR szName = NULL, LPCTSTR szValue = NULL) {
		m_strName = szName;
		m_strName.TrimLeft(); m_strName.TrimRight();
		m_strValue = szValue;
	}
	CProfileItem(const CProfileItem& B) {
		*this = B;
	}
	CProfileItem& operator = (const CProfileItem& B) {
		if (this != &B) {
			m_strOriginal = B.m_strOriginal;
			m_strName = B.m_strName;
			m_strValue = B.m_strValue;
		}
		return *this;
	}

	void Init() { m_strOriginal.Empty(); m_strName.Empty(); m_strValue.Empty(); }
	BOOL SetFromString(LPCTSTR szText, TCHAR cDelimeter, BOOL bUseEscapeCharacters);
	BOOL SetToString(CString& strLine, TCHAR cDelimeter, BOOL bUseEscapeCharacters, LPCTSTR pszFilter) const;

	BOOL operator == (LPCSTR szName) const {
		CString strName(szName);
		return m_strName.CompareNoCase(strName) == 0;
	}
	BOOL operator == (LPCWSTR szName) const {
		CString strName(szName);
		return m_strName.CompareNoCase(strName) == 0;
	}
	bool operator == (const CProfileItem& B) const {
		return (m_strName.CompareNoCase(B.m_strName) == 0) && (m_strValue == B.m_strValue);
	}
	bool operator != (const CProfileItem& B) const { return !(*this == B); }

	LPCTSTR GetName() const { return m_strName; }
	LPCTSTR GetValue() const { return m_strValue; }
	LPCTSTR GetOriginalLine() const { return m_strOriginal; }

	int32_t		GetValueI32() const ;
	uint32_t	GetValueU32() const ;
	int64_t		GetValueI64() const ;
	uint64_t	GetValueU64() const ;
	double		GetValueDouble() const ;
	template < typename TCHAR, int nBufferCount >
	BOOL		GetValueString(TCHAR (&szValue)[nBufferCount]) const { return GetValueStringImpl<char, CStringA>(szValue, nBufferCount); }
	BOOL		GetValueString(char* szValue, int nBufferCount/* COUNT (not size) including NULL terminator.*/) const ;
	BOOL		GetValueString(wchar_t* szValue, int nBufferCount/* COUNT (not size) including NULL terminator. */) const ;
	BOOL		GetValueBoolean() const ;

public:
	BOOL SetValue(int32_t nValue, LPCTSTR pszFormat = NULL);
	BOOL SetValue(uint32_t nValue, LPCTSTR pszFormat = NULL);
	BOOL SetValue(int64_t nValue, LPCTSTR pszFormat = NULL);
	BOOL SetValue(uint64_t nValue, LPCTSTR pszFormat = NULL);
	BOOL SetValue(double dValue, LPCTSTR pszFormat = NULL);
	BOOL SetValue(LPCSTR szValue, LPCSTR pszFormat = NULL);
	BOOL SetValue(LPCWSTR szValue, LPCWSTR pszFormat = NULL);
	BOOL SetValue(bool bValue, LPCWSTR pszFormat = NULL);

	// Binary In/Out
public:
	template <class Archive>
	friend Archive& operator << (Archive& ar, const CProfileItem& B) {
		//ar << B.m_strOriginal;
#ifdef _UNICODE
		ar << B.m_strName;
		ar << B.m_strValue;
#else
		ar << CStringW(B.m_strName);
		ar << CStringW(B.m_strValue);
#endif
		return ar;
	}
	template <class Archive>
	friend Archive& operator >> (Archive& ar, CProfileItem& B) {
		//ar >> B.m_strOriginal;
		B.m_strOriginal.Empty();
		B.m_strName.Empty();
		B.m_strValue.Empty();

#ifdef _UNICODE
		ar >> B.m_strName;
		ar >> B.m_strValue;
#else
		CStringW str;
		ar >> str; B.m_strName = str;
		ar >> str; B.m_strValue = str;
#endif
		return ar;
	}

protected:
	template < typename TCHAR, typename STRING >
	BOOL GetValueStringImpl(TCHAR* szValue, int nBufferCount) const {
		if (!szValue || nBufferCount <= 0)
			return FALSE;
		STRING strValue;
		strValue = STRING(m_strValue);

		tszncpy(szValue, (const TCHAR*)strValue, nBufferCount);

		return TRUE;
	}
};

class AFX_EXT_CLASS_MISC CProfileSection {
friend class CProfile;
protected:
	CString m_strName;
	TList<CProfileSection> m_subSections;
	TList<CProfileItem> m_items;
public:
	CProfileSection(LPCTSTR szName = NULL) {
		m_strName = szName;
		m_strName.TrimLeft(); m_strName.TrimRight();
		m_subSections.SetInterlockMode();
		m_items.SetInterlockMode();
	}
	CProfileSection(const CProfileSection& B) { *this = B; }
	CProfileSection& operator = (const CProfileSection& B) {
		if (this == &B) return *this;
		m_strName = B.m_strName;
		m_subSections = B.m_subSections;
		m_items = B.m_items;
		return *this;
	}
	~CProfileSection() {
	}

	void Init(BOOL bInitName = TRUE) { if (bInitName) m_strName.Empty(); m_subSections.DeleteAll(); m_items.DeleteAll(); }

	bool operator == (LPCTSTR szName) const {
		CString strName(szName);
		strName.TrimLeft(); strName.TrimRight();
		return m_strName.CompareNoCase(strName) == 0;
	}
	bool operator == (const CProfileSection& B) const {
		if ( (m_strName.CompareNoCase(B.m_strName) != 0)
			|| (m_subSections.size() != B.m_subSections.size())
			|| (m_items.size() != B.m_items.size())
			)
			return false;

		for (const auto& item : m_items) {
			const auto* pItemB = B.GetItemP(item.m_strName);
			if (!pItemB || (item != *pItemB) )
				return false;
		}

		for (const auto& section : m_subSections) {
			bool bFound = false;
			for (int i = 0; i < B.GetSubSections().size(); i++) {
				const auto& sectionB = B.GetSubSections()[i];
				if (section.m_strName.CompareNoCase(sectionB.m_strName) != 0)
					continue;
				bFound = true;
				if (section != sectionB)
					return false;
			}
			if (!bFound)
				return false;
		}

		return true;
	}
	bool operator != (const CProfileSection& B) const { return !(*this == B); }

	// Get & Set Profile Values
	int32_t		GetItemValue(LPCTSTR szItem, int32_t nDefault = 0) const ;
	uint32_t	GetItemValue(LPCTSTR szItem, uint32_t nDefault = 0) const ;
	int64_t		GetItemValue(LPCTSTR szItem, int64_t nDefault = 0) const ;
	uint64_t	GetItemValue(LPCTSTR szItem, uint64_t nDefault = 0) const ;
	double		GetItemValue(LPCTSTR szItem, double dDefault = 0.0) const ;
	LPCTSTR		GetItemValue(LPCTSTR szItem, LPCTSTR szDefault = NULL) const ;
	template < typename TCHAR, int nBufferCount >
	BOOL		GetItemValueString(LPCTSTR szItem, TCHAR (&szValue)[nBufferCount], const TCHAR* szDefaultValue = NULL) const {
		return GetItemValueString(szItem, szValue, nBufferCount, szDefaultValue);
	}
	BOOL		GetItemValueString(LPCTSTR szItem, char* szValue, int nBufferCount/* COUNT (not size) including NULL terminator.*/, const char* szDefaultValue = NULL) const {
		return GetItemValueStringImpl<char, CStringA>(szItem, szValue, nBufferCount, szDefaultValue);
	}
	BOOL		GetItemValueString(LPCTSTR szItem, wchar_t* szValue, int nBufferCount/* COUNT (not size) including NULL terminator. */, const wchar_t* szDefaultValue = NULL) const {
		return GetItemValueStringImpl<wchar_t, CStringW>(szItem, szValue, nBufferCount, szDefaultValue);
	}
	BOOL		GetItemValueBoolean(LPCTSTR szItem, BOOL bDefault = FALSE) const ;
	bool		GetItemValue(LPCTSTR szItem, bool bDefault = false) const { return GetItemValueBoolean(szItem, bDefault) ? true : false; }
	template < class T_COORD, class = T_COORD::coord_t >
	T_COORD	GetItemValue(LPCTSTR szItem, const T_COORD& coordDefault, LPCTSTR pszFormat = NULL) const {
		T_COORD coord;
		if (Text2Coord(coord, GetItemValue(szItem, (LPCTSTR)NULL)))
			return coord;
		return coordDefault;
	}

public:
	BOOL		SetItemValue(LPCTSTR szItem, int32_t nValue, LPCTSTR pszFormat = NULL);
	BOOL		SetItemValue(LPCTSTR szItem, uint32_t nValue, LPCTSTR pszFormat = NULL);
	BOOL		SetItemValue(LPCTSTR szItem, int64_t nValue, LPCTSTR pszFormat = NULL);
	BOOL		SetItemValue(LPCTSTR szItem, uint64_t nValue, LPCTSTR pszFormat = NULL);
	BOOL		SetItemValue(LPCTSTR szItem, double dValue, LPCTSTR pszFormat = NULL);
	BOOL		SetItemValue(LPCTSTR szItem, LPCSTR szValue, LPCSTR pszFormat = NULL);
	BOOL		SetItemValue(LPCTSTR szItem, LPCWSTR szValue, LPCWSTR pszFormat = NULL);
	BOOL		SetItemValueString(LPCTSTR szItem, LPCSTR szValue, LPCSTR pszFormat = NULL)		{ return SetItemValue(szItem, szValue, pszFormat); }
	BOOL		SetItemValueString(LPCTSTR szItem, LPCWSTR szValue, LPCWSTR pszFormat = NULL)	{ return SetItemValue(szItem, szValue, pszFormat); }
	BOOL		SetItemValueBoolean(LPCTSTR szItem, BOOL bValue, LPCTSTR pszFormat = NULL);
	BOOL		SetItemValue(LPCTSTR szItem, bool bValue, LPCTSTR pszFormat = NULL)				{ return SetItemValueBoolean(szItem, bValue ? TRUE : FALSE, pszFormat); }
	template < class T_COORD, class = T_COORD::coord_t >
	BOOL		SetItemValue(LPCTSTR szItem, const T_COORD& coord, LPCTSTR pszFormatEach = NULL) { CString str; Coord2Text(coord, str, pszFormatEach); return SetItemValue(szItem, str); }

	// Is Profile Value?
	BOOL		IsItemValue(LPCTSTR szItem, int32_t nValue, BOOL bTrueIfNULL = FALSE) const ;
	BOOL		IsItemValue(LPCTSTR szItem, uint32_t nValue, BOOL bTrueIfNULL = FALSE) const ;
	BOOL		IsItemValue(LPCTSTR szItem, int64_t nValue, BOOL bTrueIfNULL = FALSE) const ;
	BOOL		IsItemValue(LPCTSTR szItem, uint64_t nValue, BOOL bTrueIfNULL = FALSE) const ;
	BOOL		IsItemValue(LPCTSTR szItem, double dValue, BOOL bTrueIfNULL = FALSE) const ;
	BOOL		IsItemValue(LPCTSTR szItem, LPCSTR szValue, BOOL bTrueIfNULL = FALSE) const ;
	BOOL		IsItemValue(LPCTSTR szItem, LPCWSTR szValue, BOOL bTrueIfNULL = FALSE) const ;
	BOOL		IsItemValueString(LPCTSTR szItem, LPCSTR szValue, BOOL bTrueIfNULL = FALSE) const	{ return IsItemValue(szItem, szValue, bTrueIfNULL); }
	BOOL		IsItemValueString(LPCTSTR szItem, LPCWSTR szValue, BOOL bTrueIfNULL = FALSE) const	{ return IsItemValue(szItem, szValue, bTrueIfNULL); }
	BOOL		IsItemValueBoolean(LPCTSTR szItem, BOOL bValue, BOOL bTrueIfNULL = FALSE) const ;
	BOOL		IsItemValue(LPCTSTR szItem, bool bValue, BOOL bTrueIfNULL = FALSE) const			{ return IsItemValueBoolean(szItem, bValue, bTrueIfNULL); }
	template < class T_COORD, class = T_COORD::coord_t >
	BOOL		IsItemValue(LPCTSTR szItem, const T_COORD& coord, BOOL bTrueIfNULL = FALSE) const {
		LPCTSTR szResult = GetItemValue(szItem, (LPCTSTR)NULL);
		if (bTrueIfNULL && !szResult)
			return TRUE;
		return GetItemValue(szItem, coord) == coord;
	}

WARNING_PUSH_AND_DISABLE(4100)
	void SyncItemValueBoolean(BOOL bStore, LPCTSTR szItem, BOOL& bValue, LPCTSTR pszForamt = NULL) {
		if (bStore) SetItemValueBoolean(szItem, bValue);
		else bValue = GetItemValueBoolean(szItem, bValue);
	}
WARNING_POP()

#define TProfileSectionSyncFunction(T) \
	void SyncItemValue(BOOL bStore, LPCTSTR szItem, T& value, LPCTSTR pszFormat = NULL) {\
		if (bStore) SetItemValue(szItem, value, pszFormat);\
		else value = GetItemValue(szItem, value);\
	}\

	TProfileSectionSyncFunction(int32_t);
	TProfileSectionSyncFunction(uint32_t);
	TProfileSectionSyncFunction(int64_t);
	TProfileSectionSyncFunction(uint64_t);
WARNING_PUSH_AND_DISABLE(4244)
	TProfileSectionSyncFunction(float);
WARNING_POP()
	TProfileSectionSyncFunction(double);
	TProfileSectionSyncFunction(CString);
	template < class T_COORD, class = T_COORD::coord_t >
	TProfileSectionSyncFunction(T_COORD);
	TProfileSectionSyncFunction(bool);

	template < typename TCHAR, int nBufferCount >
	void SyncItemValue(BOOL bStore, LPCTSTR szItem, TCHAR (&szValue)[nBufferCount], const TCHAR* pszFormat = NULL) {
		SyncItemValue(bStore, szItem, szValue, nBufferCount, pszFormat);
	}
	template < typename TCHAR >
	void SyncItemValue(BOOL bStore, LPCTSTR szItem, TCHAR* szValue, int nBufferCount, const TCHAR* pszFormat = NULL) {
		if (bStore) SetItemValueString(szItem, szValue, pszFormat);
		else GetItemValueString(szItem, szValue, nBufferCount, pszFormat);
	}
#undef TProfileSectionSyncFunction

public:
	LPCTSTR GetName() const { return m_strName; }
	TList<CProfileItem>& GetItems() { return m_items; }
	const TList<CProfileItem>& GetItems() const { return m_items; }
	CProfileItem& GetItem(int iItem) { return m_items[iItem]; }
	const CProfileItem& GetItem(int iItem) const { return m_items[iItem]; }
	const CProfileItem* GetItemP(LPCSTR szItem) const { return m_items.SearchByValue(szItem); }
	const CProfileItem* GetItemP(LPCWSTR szItem) const { return m_items.SearchByValue(szItem); }
	CProfileItem* GetItemP(LPCSTR szItem) { return m_items.SearchByValue(szItem); }
	CProfileItem* GetItemP(LPCWSTR szItem) { return m_items.SearchByValue(szItem); }

	BOOL DeleteSection(LPCTSTR szSection) {
		CString strSection(szSection);
		strSection.TrimLeft(); strSection.TrimRight();
		return m_subSections.DeleteByValue(szSection);
	}
	BOOL DeleteItem(LPCTSTR szItem) {
		CString strItem(szItem);
		strItem.TrimLeft(); strItem.TrimRight();
		return m_items.DeleteByValue(strItem);
	}

	TList<CProfileSection>& GetSubSections() { return m_subSections; }
	const TList<CProfileSection>& GetSubSections() const { return m_subSections; }

	CProfileSection& GetSubSection(LPCTSTR szName) {
		CString strName(szName);
		strName.TrimLeft(); strName.TrimRight();
		intXX_t iIndex = m_subSections.FindByValue(strName);
		if (iIndex < 0) {
			m_subSections.Push(new CProfileSection(strName));
			return m_subSections();
		}
		return m_subSections[iIndex];
	}
	const CProfileSection& GetSubSection(LPCTSTR szName) const {
		CString strName(szName);
		strName.TrimLeft(); strName.TrimRight();
		intXX_t iIndex = m_subSections.FindByValue(strName);
		if (iIndex < 0)
			__throw_exception__("No Sub Section...");
		return m_subSections[iIndex];
	}

	// Binary In/Out
public:
	template <class Archive>
	friend Archive& operator << (Archive& ar, const CProfileSection& B) {
		ar << (int32_t)0x12340001;	// version
#ifdef _UNICODE
		ar << B.m_strName;
#else
		ar << CStringW(B.m_strName);
#endif
		ar << B.m_items;
		ar << B.m_subSections;
		return ar;
	}
	template <class Archive>
	friend Archive& operator >> (Archive& ar, CProfileSection& B) {
		B.m_strName.Empty();
		B.m_items.DeleteAll();
		B.m_subSections.DeleteAll();

		int32_t nVer = 0;
		ar >> nVer;
		switch (nVer) {
		case 0x12340001 :
#ifdef _UNICODE
			ar >> B.m_strName;
#else
			{
				CStringW str;
				ar >> str; B.m_strName = str;
			}
#endif
			ar >> B.m_items;
			ar >> B.m_subSections;
			break;
		default:
			// Error;
			break;
		}
		return ar;
	}

	template < typename Item >
	bool SyncContainer(bool bStore, std::vector<Item>& items) {
		size_t nItem = 0;
		if (bStore) {
			// Delete Sub-Sections starting the same name
			DeleteSubSectionStartingWith(Format(_T("%s_"), m_strName));

			SetItemValue(_T("nItem"), items.size());
			nItem = items.size();
		} else {
			items.clear();
			nItem = GetItemValue(_T("nItem"), nItem);
			if (!nItem)
				return true;
			items.assign(nItem, Item());
		}
		for (int iItem = 0; iItem < nItem; iItem++) {
			auto& item = items[iItem];
			auto& sectionItem = GetSubSection(Format(_T("%s_%d"), m_strName, iItem));
			item.SyncData(bStore, sectionItem);
		}
		return true;
	}
	template < typename Item, int nItem >
	bool SyncContainer(bool bStore, std::array<Item, nItem>& items) {
		if (bStore) {
			DeleteSubSectionStartingWith(Format(_T("%s_"), m_strName));
		}

		for (int iItem = 0; iItem < items.size(); iItem++) {
			auto& item = items[iItem];
			auto& sectionItem = GetSubSection(Format(_T("%s_%d"), m_strName, iItem));
			item.SyncData(bStore, sectionItem);
		}
		return true;
	}
	template < class Container >
	bool SyncContainer(bool bStore, Container& items) {
		if (bStore) {

			DeleteSubSectionStartingWith(Format(_T("%s_"), m_strName));

			SetItemValue(_T("nItem"), items.size());
		} else {
			items.DeleteAll();
			size_t nItem = 0;
			nItem = GetItemValue(_T("nItem"), nItem);
			if (!nItem)
				return true;
			for (int i = 0; i < nItem; i++) items.Push(new typename Container::type_t);
		}
		for (int iItem = 0; iItem < items.size(); iItem++) {
			auto& item = items[iItem];
			auto& sectionItem = GetSubSection(Format(_T("%s%d"), m_strName, iItem));
			item.SyncData(bStore, sectionItem);
		}
		return true;
	}

protected:
	CProfileItem* Find_or_AddNewItem(LPCTSTR szItem);
	template < typename TCHAR, typename STRING >
	BOOL GetItemValueStringImpl(LPCTSTR szItem, TCHAR* szValue, int nBufferCount, const TCHAR* szDefaultValue = NULL) const {
		if (!szValue || nBufferCount <= 0)
			return FALSE;
		CString strItem(szItem);
		strItem.TrimLeft(); strItem.TrimRight();
		intXX_t iItem = -1;
		STRING strValue;
		const CProfileItem* pItem = NULL;
		if (!strItem.IsEmpty() && ((pItem = m_items.SearchByValue(strItem)) != NULL) ) {
			strValue = STRING(pItem->m_strValue);
		} else {
			strValue = STRING(szDefaultValue);
		}

		tszncpy(szValue, (const TCHAR*)strValue, nBufferCount);

		return TRUE;
	}

	void DeleteSubSectionStartingWith(LPCTSTR pszName) {
		auto nLen = _tcslen(pszName);
		for (int i = 0; i < m_subSections.size(); i++) {
			if (_tcsncmp(pszName, m_subSections[i].m_strName, nLen) == 0) {
				m_subSections.Delete(i);
				i--;
			}
		}
	}
};

class AFX_EXT_CLASS_MISC CProfile {
protected:
	CArchiveU::eENCODING m_eEncoding;
protected:
	TCHAR m_cDelimeter;
	TCHAR m_cComment;
	TCHAR m_cSectionOpen;
	TCHAR m_cSectionClose;
	TCHAR m_cSubSectionOpen;
	TCHAR m_cSubSectionClose;
	TList<CProfileSection> m_sections;
	CString m_strPathProfile;
	BOOL m_bUseEscapeCharacters;

private:
	CString m_strPassword;
public:
	BOOL IsEncrypted() const { return !m_strPassword.IsEmpty(); }

public:
	CProfile(LPCTSTR szPathProfile = NULL, CArchiveU::eENCODING eEncoding = CArchiveU::E_AUTO, LPCTSTR pszPassword = NULL)
		: m_strPassword(pszPassword)
	{
		Init(szPathProfile, eEncoding, pszPassword);
		m_sections.SetInterlockMode();
	}
	CProfile(const CProfile& B) { *this = B; }
	CProfile& operator = (const CProfile& B) {
		if (this == &B) return *this;
		m_eEncoding				= B.m_eEncoding;
		m_cDelimeter			= B.m_cDelimeter;
		m_cComment				= B.m_cComment;
		m_cSectionOpen			= B.m_cSectionOpen;
		m_cSectionClose			= B.m_cSectionClose;
		m_cSubSectionOpen		= B.m_cSubSectionOpen;
		m_cSubSectionClose		= B.m_cSubSectionClose;
		m_sections				= B.m_sections;
		m_strPathProfile		= B.m_strPathProfile;
		m_bUseEscapeCharacters	= B.m_bUseEscapeCharacters;
		m_strPassword			= B.m_strPassword;
		return *this;
	}

	~CProfile(void) {
	}

	void Init(LPCTSTR szPathProfile = NULL, CArchiveU::eENCODING eEncoding = CArchiveU::E_AUTO, LPCTSTR pszPassword = NULL) {
		m_eEncoding = eEncoding;
		m_cDelimeter = _T('=');
		m_cComment = _T(';');
		m_cSectionOpen = _T('[');
		m_cSectionClose = _T(']');
		m_cSubSectionOpen = _T('{');
		m_cSubSectionClose = _T('}');
		m_strPathProfile = szPathProfile;
		m_bUseEscapeCharacters = TRUE;
		m_sections.DeleteAll();
		if (!m_strPathProfile.IsEmpty())
			Load();
	}

	BOOL Load(LPCTSTR szPathProfile = NULL, CArchiveU::eENCODING eEncoding = CArchiveU::E_AUTO);
	BOOL Save(LPCTSTR szPathProfile = NULL, CArchiveU::eENCODING eEncoding = CArchiveU::E_AUTO);
protected:
	BOOL SaveSection(CArchiveU& ar, const CProfileSection& section, int nDepth);

public:
	LPCTSTR GetProfilePath() const { return m_strPathProfile; }

	void SetUseEscapeCharacters(BOOL bUseEscapeCharacters = TRUE) { m_bUseEscapeCharacters = bUseEscapeCharacters; }
	BOOL GetUseEscapeCharacters() { return m_bUseEscapeCharacters; }

	// Get & Set Profile Values
#define TProfileFunctionGet(T) \
	T GetProfileValue(LPCTSTR szSection, LPCTSTR szItem, T defaultValue = NULL) const { \
		CString strSection(szSection);\
		strSection.TrimLeft(); strSection.TrimRight();\
		if (strSection.IsEmpty())\
			return defaultValue;\
		const CProfileSection& section = GetSection(strSection);\
		return section.GetItemValue(szItem, defaultValue);\
	}
#define TProfileFunctionSet(T) \
	BOOL SetProfileValue(LPCTSTR szSection, LPCTSTR szItem, T value, LPCTSTR pszFormat = NULL) {\
		CString strSection(szSection);\
		strSection.TrimLeft(); strSection.TrimRight();\
		if (strSection.IsEmpty())\
			return FALSE;\
		CProfileSection& section = GetSection(strSection);\
		CString strItem(szItem);\
		strItem.TrimLeft(); strItem.TrimRight();\
		if (strItem.IsEmpty())\
			return FALSE;\
		return section.SetItemValue(strItem, value, pszFormat);\
	}
#define TProfileFunctionIs(T) \
	BOOL IsProfileValue(LPCTSTR szSection, LPCTSTR szItem, T value, BOOL bTrueIfNULL = FALSE) const {\
		CString strSection(szSection);\
		strSection.TrimLeft(); strSection.TrimRight();\
		if (strSection.IsEmpty())\
			return FALSE;\
		const CProfileSection& section = GetSection(strSection);\
		return section.IsItemValue(szItem, value, bTrueIfNULL);\
	}

#define TProfileSyncFunction(T) \
	void SyncProfileValue(BOOL bStore, LPCTSTR szSection, LPCTSTR szItem, T& value, LPCTSTR pszFormat = NULL) {\
		if (bStore) SetProfileValue(szSection, szItem, value, pszFormat);\
		else value = GetProfileValue(szSection, szItem, value);\
	}\

	TProfileFunctionGet(int32_t)
	TProfileFunctionGet(uint32_t)
	TProfileFunctionGet(int64_t)
	TProfileFunctionGet(uint64_t)
	TProfileFunctionGet(double)
	TProfileFunctionGet(LPCTSTR)

	TProfileFunctionSet(int32_t)
	TProfileFunctionSet(uint32_t)
	TProfileFunctionSet(int64_t)
	TProfileFunctionSet(uint64_t)
	TProfileFunctionSet(double)
	TProfileFunctionSet(LPCTSTR)

	TProfileFunctionIs(int32_t)
	TProfileFunctionIs(uint32_t)
	TProfileFunctionIs(int64_t)
	TProfileFunctionIs(uint64_t)
	TProfileFunctionIs(double)
	TProfileFunctionIs(LPCTSTR)

	template < typename TCHAR, int nBufferCount >
	BOOL GetProfileValueString(LPCTSTR szSection, LPCTSTR szItem, TCHAR (&szValue)[nBufferCount], const TCHAR* szDefaultValue = NULL) const {
		return GetProfileValueString(szSection, szItem, szValue, nBufferCount, szDefaultValue);
	}
	BOOL GetProfileValueString(LPCTSTR szSection, LPCTSTR szItem, char* szValue, int nBufferCount, const char* szDefaultValue = NULL) const {
		return GetProfileValueStringImpl<char, CStringA>(szSection, szItem, szValue, nBufferCount, szDefaultValue);
	}
	BOOL GetProfileValueString(LPCTSTR szSection, LPCTSTR szItem, wchar_t* szValue, int nBufferCount, const wchar_t* szDefaultValue = NULL) const {
		return GetProfileValueStringImpl<wchar_t, CStringW>(szSection, szItem, szValue, nBufferCount, szDefaultValue);
	}

	BOOL SetProfileValueString(LPCTSTR szSection, LPCTSTR szItem, const char* szValue, const char* pszFormat = NULL) {
		return SetProfileValue(szSection, szItem, CString(szValue), CString(pszFormat));
	}
	BOOL SetProfileValueString(LPCTSTR szSection, LPCTSTR szItem, const wchar_t* szValue, const wchar_t* pszFormat = NULL) {
		return SetProfileValue(szSection, szItem, CString(szValue), CString(pszFormat));
	}
	BOOL IsProfileValueString(LPCTSTR szSection, LPCTSTR szItem, const char* szValue, BOOL bTrueIfNULL = FALSE) const {
		return IsProfileValue(szSection, szItem, CString(szValue), bTrueIfNULL);
	}
	BOOL IsProfileValueString(LPCTSTR szSection, LPCTSTR szItem, const wchar_t* szValue, BOOL bTrueIfNULL = FALSE) const {
		return IsProfileValue(szSection, szItem, CString(szValue), bTrueIfNULL);
	}

	template < typename TCHAR, int nBufferCount >
	void SyncProfileValue(BOOL bStore, LPCTSTR szSection, LPCTSTR szItem, TCHAR (&szValue)[nBufferCount], const TCHAR* pszFormat = NULL) {
		return SyncProfileValue(bStore, szSection, szItem, szValue, nBufferCount, pszFormat);
	}
	template < typename TCHAR >
	void SyncProfileValue(BOOL bStore, LPCTSTR szSection, LPCTSTR szItem, TCHAR* szValue, int nBufferCount, const TCHAR* pszFormat = NULL) {
		if (bStore) SetProfileValueString(szSection, szItem, szValue, pszFormat);
		else GetProfileValueString(szSection, szItem, szValue, nBufferCount);
	}

	TProfileSyncFunction(int32_t)
	TProfileSyncFunction(uint32_t)
	TProfileSyncFunction(int64_t)
	TProfileSyncFunction(uint64_t)
WARNING_PUSH_AND_DISABLE(4244)
	TProfileSyncFunction(float)
WARNING_POP()
	TProfileSyncFunction(double)
	TProfileSyncFunction(CString)


	template < class T_COORD, class = T_COORD::coord_t >
	TProfileFunctionGet(T_COORD)
	template < class T_COORD, class = T_COORD::coord_t >
	TProfileFunctionSet(T_COORD)
	template < class T_COORD, class = T_COORD::coord_t >
	TProfileFunctionIs(T_COORD)
	template < class T_COORD, class = T_COORD::coord_t >
	TProfileSyncFunction(T_COORD)

#undef TProfileFunction
#undef TProfileSyncFunction

	// Get & Set Bolean Profile Value
	BOOL GetProfileValueBoolean(LPCTSTR szSection, LPCTSTR szItem, BOOL bDefault = FALSE) const {
		CString strSection(szSection);
		strSection.TrimLeft(); strSection.TrimRight();
		if (strSection.IsEmpty())
			return bDefault;
		const CProfileSection& section = GetSection(strSection);
		return section.GetItemValueBoolean(szItem, bDefault);
	}
	BOOL SetProfileValueBoolean(LPCTSTR szSection, LPCTSTR szItem, BOOL bValue) {
		CString strSection(szSection);
		strSection.TrimLeft(); strSection.TrimRight();
		if (strSection.IsEmpty())
			return FALSE;
		CProfileSection& section = GetSection(strSection);
		return section.SetItemValueBoolean(szItem, bValue);
	}
	void SyncProfileValueBoolean(BOOL bStore, LPCTSTR szSection, LPCTSTR szItem, BOOL& bValue) {
		if (bStore) SetProfileValueBoolean(szSection, szItem, bValue);
		else bValue = GetProfileValueBoolean(szSection, szItem, bValue);
	}
	bool GetProfileValue(LPCTSTR szSection, LPCTSTR szItem, bool bDefault) const {
		return GetProfileValueBoolean(szSection, szItem, bDefault) ? true : false;
	}
	BOOL SetProfileValue(LPCTSTR szSection, LPCTSTR szItem, bool bValue) {
		return SetProfileValueBoolean(szSection, szItem, bValue);
	}
	void SyncProfileValue(BOOL bStore, LPCTSTR szSection, LPCTSTR szItem, bool& bValue) {
		if (bStore) SetProfileValue(szSection, szItem, bValue);
		else bValue = GetProfileValue(szSection, szItem, bValue);
	}


public:

	WARNING_PUSH_AND_DISABLE(4244)
	CProfileSection& GetSection(LPCTSTR szSection) {
		CString strSection(szSection);
		strSection.TrimLeft(); strSection.TrimRight();
		intXX_t iSection = m_sections.FindByValue(strSection);
		if (iSection < 0) {
			iSection = m_sections.Push(new CProfileSection(strSection));
		}
		return m_sections[iSection];
	}
	const CProfileSection& GetSection(LPCTSTR szSection) const {
		CString strSection(szSection);
		strSection.TrimLeft(); strSection.TrimRight();
		intXX_t iSection = m_sections.FindByValue(strSection);
		if (iSection < 0) {
			static const CProfileSection sectionNULL;
			return sectionNULL;
		}
		return m_sections[iSection];
	}
	BOOL DeleteSection(LPCTSTR szSection) {
		CString strSection(szSection);
		strSection.TrimLeft(); strSection.TrimRight();
		intXX_t iSection = m_sections.FindByValue(strSection);
		if (iSection < 0)
			return FALSE;
		m_sections.Delete(iSection);
		return TRUE;
	}
	WARNING_POP()

	TList<CProfileSection>& GetSections() { return m_sections; };
	CProfileSection& GetSection(int iSection) { return m_sections[iSection]; };
	const CProfileSection& GetSection(int iSection) const { return m_sections[iSection]; };
	CArchiveU::eENCODING GetFileEncoding() { return m_eEncoding; }

protected:
	template < typename TCHAR, typename STRING >
	BOOL GetProfileValueStringImpl(LPCTSTR szSection, LPCTSTR szItem, TCHAR* szValue, int nBufferCount, const TCHAR* szDefaultValue = NULL) const {
		if (!szValue || (nBufferCount <= 0) )
			return FALSE;
		CString strValue = GetProfileValue(szSection, szItem, CString(szDefaultValue));
		tszncpy(szValue, (const TCHAR*)STRING(strValue), nBufferCount);
		return TRUE;
	}

};

#pragma pack(pop)
