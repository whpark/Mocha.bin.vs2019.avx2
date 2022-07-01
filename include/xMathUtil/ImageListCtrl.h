#pragma once

#include "AFX_EXT_XMU.h"
#include "Coordinate.hpp"

#include "qtl/qtl.hpp"

#include "MatView.h"
#include "ImageListBox.h"


#pragma pack(push, 8)

// CImageListCtrl dialog

class AFX_EXT_CLASS_XMU CImageListCtrl : public CMDialogEx {
	DECLARE_DYNAMIC(CImageListCtrl)

public:
	CImageListCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageListCtrl();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = XMATHUTIL_IDD_IMAGE_LIST, };
#endif

public:
	CImageListBox m_lst;
	CMatView m_view;
	CComboBox m_cookies;

	double m_dDefaultMatchingRatePass = 0.8;
	double m_dDefaultMatchingRateMin = 0.4;

public:
	bool SetCookieList(LPCTSTR pszCookies = _T("1;2;3;4"));

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK() {}
	virtual void OnCancel() {}

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnAddMark();
	afx_msg void OnDeleteMark();
	afx_msg void OnMoveMarkLeft();
	afx_msg void OnMoveMarkRight();

	afx_msg void OnSelchangeList();
	//afx_msg void OnCbCenterOffset();
	bool m_bUseNotify = true;
	afx_msg void OnChangeCenterOffset();
	afx_msg void OnEditchangeCookies();
	afx_msg void OnChangeMatchingRatePass();
	afx_msg void OnChangeMatchingRateMin();
	afx_msg void OnUpdateOffset();
	//afx_msg void OnSelRegionMark(NMHDR* pNotifyStruct, LRESULT* pResult);
};

#pragma pack(pop)
