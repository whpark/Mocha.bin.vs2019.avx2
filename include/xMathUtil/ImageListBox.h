#pragma once

#include "AFX_EXT_XMU.h"
#include "Coordinate.hpp"

#include "qtl/qtl.hpp"

#include "misc/Profile.h"

#pragma pack(push, 8)


//=================================================================================================================================
//
class CImageListBoxItem {
public:
	CString strPath;
	double dMatchingRatePass = 0.8, dMatchingRateMin = 0.5;
	CPoint2d ptCenterOffset;
	CString strCookie;
	bool bDrawCenterOffset{true};

public:
	CImageListBoxItem() = default;
	CImageListBoxItem(const CImageListBoxItem& B) = default;
	CImageListBoxItem(CImageListBoxItem&& B) = default;
	CImageListBoxItem(LPCTSTR pszPath, const CPoint2d& ptCenterOffset = CPoint2d(), double dMatchingRatePass = 0.8, double dMatchingRateMin = 0.5, LPCTSTR pszCookie = nullptr) {
		//this->img = img;
		this->strPath = pszPath;
		this->ptCenterOffset = ptCenterOffset;
		this->dMatchingRatePass = dMatchingRatePass;
		this->dMatchingRateMin  = dMatchingRateMin;
		if (pszCookie)
			this->strCookie = pszCookie;
	}
	CImageListBoxItem& operator = (const CImageListBoxItem& B) = default;


	bool operator == (const CImageListBoxItem& B) const {
		return 1
			&& (strPath.CompareNoCase(B.strPath) == 0)
			&& (dMatchingRatePass == B.dMatchingRatePass)
			&& (dMatchingRateMin  == B.dMatchingRateMin)
			&& (ptCenterOffset == B.ptCenterOffset)
			&& (strCookie == B.strCookie)
			;
	}

	bool SyncData(bool bStore, CProfileSection& section) {
		section.SyncItemValue(bStore, _T("Path"), strPath);
		section.SyncItemValue(bStore, _T("MatchingRatePass"), dMatchingRatePass, _T("%g"));
		section.SyncItemValue(bStore, _T("MatchingRateMin"),  dMatchingRateMin, _T("%g"));
		section.SyncItemValue(bStore, _T("CenterOffset"), ptCenterOffset, _T("%g"));
		section.SyncItemValue(bStore, _T("Cooky"), strCookie);
		return true;
	}

public:
	mutable cv::Mat img;	// cache
	const cv::Mat& GetImage() const { return img; }

	bool LoadImage() const {
		try {
			if (!strPath.IsEmpty())
				img = cv::imread(CONV_A(strPath));
		} catch (...) {
		}
		return false;
	}
	bool LoadImage(LPCTSTR pszPath) {
		try {
			std::filesystem::path path(pszPath);
			if (!path.empty())
				img = cv::imread(path.string());
			strPath = pszPath;
			return true;
		} catch (...) {
		}
		return false;
	}
};

// CImageListBox

//---------------------------------------------------------------------------------------------------------------------------------
//
class AFX_EXT_CLASS_XMU CImageListBox : public CListBox {
friend class CImageListCtrl;
	DECLARE_DYNAMIC(CImageListBox)
protected:
	std::vector<CImageListBoxItem> m_items;

public:
	int m_eResizingMethod = cv::INTER_LINEAR;

	CImageListBox();
	virtual ~CImageListBox();

	int AddImage(const CImageListBoxItem& item);
	int InsertImage(int iIndex, const CImageListBoxItem& item);
	int DeleteImage(int iIndex);

	void ResetContent();

	void Reserve(int nCount) { m_items.reserve(nCount); }

	bool Sync(bool bStore, std::vector<CImageListBoxItem>& items);

protected:
	// manipulating listbox items
	using CListBox::AddString;
	using CListBox::InsertString;
	using CListBox::DeleteItem;
	using CListBox::DeleteString;
	using CListBox::Dir;

	// selection helpers
	using CListBox::FindString;
	using CListBox::SelectString;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

#pragma pack(pop)
