#pragma once

#include "AFX_EXT_FRAME_GRABBER.h"

// CCameraLayout

#include "TList.hpp"
#include "misc/Profile.h"

//-----------------------------------------------------------------------------
//
class AFX_EXT_CLASS_FRAME_GRABBER CCameraSettingItem {
public:
	BOOL m_bPane;
	CString m_strName;
	CString m_strValue;

public:
	CCameraSettingItem(BOOL bPane = TRUE, LPCTSTR pszName = NULL, LPCTSTR pszValue = NULL) : m_bPane(bPane), m_strName(pszName), m_strValue(pszValue) {
	}
	CCameraSettingItem(const CCameraSettingItem& B) { *this = B; }
	CCameraSettingItem& operator = (const CCameraSettingItem& B) {
		if (this != &B) { m_bPane = B.m_bPane; m_strName = B.m_strName; m_strValue = B.m_strValue; }
		return *this;
	}

	BOOL operator == (LPCTSTR pszName) const { return m_strName.CompareNoCase(pszName) == 0; }

public:
	template <class Archive>
	friend Archive& operator << (Archive& ar, const CCameraSettingItem& B) {
		ar << (int)1;	// version
		ar << B.m_bPane;
		ar << B.m_strName;
		ar << B.m_strValue;
		return ar;
	}
	template <class Archive>
	friend Archive& operator >> (Archive& ar, CCameraSettingItem& B) {
		int nVer = 0;
		ar >> nVer;
		switch (nVer) {
		case 1 :
			ar >> B.m_bPane;
			ar >> B.m_strName;
			ar >> B.m_strValue;
			break;
		default:
			// Error;
			break;
		}
		return ar;
	}
};

//-----------------------------------------------------------------------------
//
class AFX_EXT_CLASS_FRAME_GRABBER CCameraLayoutItem {

public:
	BOOL m_bPane;
	CString m_strName;
	struct {
		int m_nWidth;
		int m_nHeight;
		BOOL m_bOverlapped;
		BOOL m_bTabbedCtrl;
		BOOL m_bSplitVertically;
		TList<CCameraLayoutItem> m_child;
	};	// Pane
	struct {
		CString m_strCameraType;
		CString m_strUnitName;
		CProfileSection m_settings;
	};	// Camera


public:
	CCameraLayoutItem() {
		Init();
	}
	//CCameraLayoutItem(const CCameraLayoutItem& B) { *this = B; }
	//CCameraLayoutItem& operator = (const CCameraLayoutItem& B) {
	//	if (this == &B) {
	//		return *this;
	//	}
	//	m_bPane = B.m_bPane;
	//	m_strName = B.m_strName;

	//	m_nWidth = B.m_nWidth;
	//	m_nHeight = B.m_nHeight;
	//	m_bOverlapped = B.m_bOverlapped;
	//	m_bTabbedCtrl = B.m_bTabbedCtrl;
	//	m_bSplitVertically = B.m_bSplitVertically;
	//	m_child = B.m_child;

	//	m_strCameraType = B.m_strCameraType;
	//	m_strUnitName = B.m_strUnitName;
	//	m_settings = B.m_settings;
	//	return *this;
	//}

	BOOL IsPane() const { return m_bPane; }
	LPCTSTR GetName() const { return m_strName; }
	void SetName(LPCTSTR pszName) { m_strName = pszName; }

	BOOL Init() {
		m_bPane = TRUE;
		m_strName = _T("...");

		m_nWidth = 0;
		m_nHeight = 0;
		m_bOverlapped = FALSE;
		m_bTabbedCtrl = FALSE;
		m_bSplitVertically = FALSE;
		m_child.DeleteAll();

		m_strCameraType.Empty();
		m_strUnitName.Empty();
		m_settings.Init();

		return TRUE;
	}

	BOOL SetPane(LPCTSTR pszName) {
		m_strName = pszName;
		m_bPane = TRUE;
		m_child.DeleteAll();
		return TRUE;
	}
	TList<CCameraLayoutItem>& GetChildPane() { return m_child; }
	const TList<CCameraLayoutItem>& GetChildPane() const { return m_child; }
	CCameraLayoutItem* GetChildPane(int iIndex) {
		if (!m_bPane)
			return NULL;
		if ((iIndex < 0) || (iIndex >= m_child.N()))
			return NULL;
		return &m_child[iIndex];
	}
	const CCameraLayoutItem* GetChildPane(int iIndex) const {
		if (!m_bPane)
			return NULL;
		if ((iIndex < 0) || (iIndex >= m_child.N()))
			return NULL;
		return &m_child[iIndex];
	}

	LPCTSTR GetSettingValue(LPCTSTR pszName, LPCTSTR pszDefaultValue = NULL) const {
		return m_settings.GetItemValue(pszName, pszDefaultValue);
	}
	BOOL SetSettingValue(LPCTSTR pszName, LPCTSTR pszValue) {
		return m_settings.SetItemValue(pszName, pszValue);
	}

public:
	BOOL Load(LPCTSTR pszPath);
	BOOL Store(LPCTSTR pszPath) const;

	BOOL Get(const CProfileSection& section);
	BOOL Set(CProfileSection& section);

	template <class Archive>
	friend Archive& operator << (Archive& ar, const CCameraLayoutItem& B) {
		ar << (int)2;	// version
		ar << B.m_bPane;
		ar << B.m_strName;

		ar << B.m_nWidth;
		ar << B.m_nHeight;
		ar << B.m_bOverlapped;
		ar << B.m_bTabbedCtrl;
		ar << B.m_bSplitVertically;
		ar << B.m_child;

		ar << B.m_strCameraType;
		ar << B.m_strUnitName;
		ar << B.m_settings;

		return ar;
	}
	template <class Archive>
	friend Archive& operator >> (Archive& ar, CCameraLayoutItem& B) {
		int nVer = 0;
		ar >> nVer;
		switch (nVer) {
		case 1 :
			{
				int eCLT;
				ar >> eCLT;
				B.m_bPane = (eCLT==2);
			}
			ar >> B.m_strName;

			ar >> B.m_nWidth;
			ar >> B.m_nHeight;
			ar >> B.m_bOverlapped;
			ar >> B.m_bTabbedCtrl;
			ar >> B.m_bSplitVertically;
			ar >> B.m_child;

			ar >> B.m_strCameraType;
			ar >> B.m_strUnitName;
			{
				TList<CCameraSettingItem> settings;
				ar >> settings;
				CProfileSection& section = B.m_settings;
				for (int i = 0; i < settings.N(); i++)
					section.SetItemValue(settings[i].m_strName, settings[i].m_strValue);
			}
			break;

		case 2 :
			ar >> B.m_bPane;
			ar >> B.m_strName;

			ar >> B.m_nWidth;
			ar >> B.m_nHeight;
			ar >> B.m_bOverlapped;
			ar >> B.m_bTabbedCtrl;
			ar >> B.m_bSplitVertically;
			ar >> B.m_child;

			ar >> B.m_strCameraType;
			ar >> B.m_strUnitName;
			ar >> B.m_settings;
			break;

		default:
			// Error;
			break;
		}
		return ar;
	}

public:
	static void GetCameraList(const CCameraLayoutItem& layout, CStrings& strs);

};
