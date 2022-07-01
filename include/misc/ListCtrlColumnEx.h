///////////////////////////////////////////////////////////////////////////////
//
// CListCtrlColumnEx by 김명식.
//
//
//

#pragma once
#include <list>
#include <vector>

#include "AFX_EXT_MISC.h"
#include "ListColumnData.h"

#pragma pack(push, 8)

class AFX_EXT_API_MISC CListCtrlColumnEx : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlColumnEx)

protected:
	typedef miscListCtrlColumn::LISTCTRLEX_HEADER			T_LISTCTRL_DATA;
	typedef miscListCtrlColumn::T_PROGRESS_DATA				T_PROGRESS_DATA;
	typedef miscListCtrlColumn::T_CR_TEXT_BK				T_CR_TEXT_BK;

	typedef miscListCtrlColumn::CColumnComboBox				CColumnComboBox;
	typedef miscListCtrlColumn::CColumnDateBox				CColumnDateBox;
	typedef miscListCtrlColumn::CColumnEditBox				CColumnEditBox;
	typedef miscListCtrlColumn::CColumnSpinBox				CColumnSpinBox;

	typedef std::vector<T_LISTCTRL_DATA>::iterator			ITERATOR_DATA;
	typedef std::vector<int>::iterator						ITERATOR_CHECK;
	typedef std::vector<CString>::iterator					ITERATOR_STRS;
	typedef std::vector<T_PROGRESS_DATA>::iterator			ITERATOR_PROGRESS;
	typedef std::vector<T_CR_TEXT_BK>::iterator				ITERATOR_COLOR;
	typedef std::vector<BOOL>::iterator						ITERATOR_BUTTON;
	typedef std::vector<BOOL>::iterator						ITERATOR_ACTIVE;

	typedef std::vector<T_LISTCTRL_DATA>::const_iterator	CONST_ITERATOR_DATA;
	typedef std::vector<int>::const_iterator				CONST_ITERATOR_CHECK;
	typedef std::vector<CString>::const_iterator			CONST_ITERATOR_STRS;
	typedef std::vector<T_PROGRESS_DATA>::const_iterator	CONST_ITERATOR_PROGRESS;
	typedef std::vector<T_CR_TEXT_BK>::const_iterator		CONST_ITERATOR_COLOR;
	typedef std::vector<BOOL>::const_iterator				CONST_ITERATOR_BUTTON;
	typedef std::vector<BOOL>::const_iterator				CONST_ITERATOR_ACTIVE;

public:
	CListCtrlColumnEx();
	virtual ~CListCtrlColumnEx() {
		ITERATOR_DATA itDataEnd = m_arrData.end();
		for (ITERATOR_DATA itDataPos = m_arrData.begin(); itDataPos != itDataEnd; ++itDataPos) {
			std::vector<CString>			comboTemp;
			std::vector<int>				checkTemp;
			std::vector<T_PROGRESS_DATA>	progressTemp;
			std::vector<T_CR_TEXT_BK>		crTextBkTemp;
			std::vector<BOOL>				buttonTemp;
			std::vector<BOOL>				activeTemp;
	
			comboTemp.swap(itDataPos->vtComboText);
			checkTemp.swap(itDataPos->vtCheckData);
			progressTemp.swap(itDataPos->vtProgressData);
			crTextBkTemp.swap(itDataPos->vtCrTextBk);
			buttonTemp.swap(itDataPos->vtButtonDown);
			activeTemp.swap(itDataPos->vtActiveControl);
		}

		std::vector<T_LISTCTRL_DATA> vtTemp;
		m_arrData.swap(vtTemp);
	}

public:
	BOOL SetColumnEdit(
			int iSubItem,
			int iMaxLength = -1,
			DWORD dwEditStyle = ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE);

	BOOL SetColumnEditNumber(
			int iSubItem,
			double dMinNumber = 0.,
			double dMaxNumber = 0.,
			int iMaxLength = -1,
			DWORD dwEditStyle = ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE);

	BOOL SetColumnEditDouble(
			int iSubItem,
			int	iPoint = 3,
			double dMinNumber = 0.,
			double dMaxNumber = 0.,
			int iMaxLength = -1,
			DWORD dwEditStyle = ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE);

	BOOL SetColumnCombo(
			int iSubItem,
			const std::vector<CString>& strs,
			BOOL bShowDropDown = TRUE);

	BOOL SetColumnDate(
			int iSubItem,
			LPCTSTR lpDateFormat = _T("yyyy-MM-dd HH:mm:ss"),
			BOOL bTimeUpdate = TRUE,
			DWORD dwDateStyle = WS_VISIBLE|WS_CHILD|WS_TABSTOP|DTS_SHORTDATEFORMAT);

	BOOL SetColumnSpin(
			int iSubItem,
			int iMinNumber,
			int iMaxNumber,
			int iMaxLength = -1,
			DWORD dwEditStyle = ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_LEFT|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE);

	BOOL SetColumnCheck(
			int iSubItem,
			LPCTSTR lpCheckText = _T(""),
			int iCheckState = 0);

	BOOL SetColumnCheck(
			int iSubItem,
			LPCTSTR lpCheckText,
			LPCTSTR lpUnCheckText,
			int iCheckState = 0);

	BOOL SetColumnProgress(
			int iSubItem,
			LPCTSTR lpProgressFormat = _T("%d %%"),
			BOOL bShowProgressText = TRUE);

	BOOL SetColumnButton(
			int iSubItem,
			LPCTSTR lpCaption = _T(""),
			COLORREF crButton = RGB(120, 200, 200));

	BOOL				ActiveControls();
	BOOL				ActiveControl(int iItem, int iSubItem);
	BOOL				ActiveItemControl(int iItem);
	BOOL				ActiveSubItemControl(int iSubItem);

	BOOL				InactiveControls();
	BOOL				InactiveControl(int iItem, int iSubItem);
	BOOL				InactiveItemControl(int iItem);
	BOOL				InactiveSubItemControl(int iSubItem);

	void				SetAlignText(int iSubItem, eListCtrlExTextAlign alignment = eLCETA_CENTER);
	void				SetRowHeight(int nRowHeight = 20);
	void				SetMark(int iItem, BOOL bAdd = FALSE, BOOL bFocus = TRUE);
	void				SetColorSelectItem(COLORREF crSelectItem) { m_crSelectItem = crSelectItem; }
	void				SetColorCheckMark(COLORREF crCheckMark) { m_crCheckMark = crCheckMark; }
	void				SetColorProgressBar(COLORREF crProgressBar) { m_crProgressBar = crProgressBar; }
	void				SetColorProgressLine(COLORREF crProgressLine) { m_crProgressLine = crProgressLine; }
	void				SetShowRight(BOOL bShowRight = TRUE) { m_bShowRight = bShowRight; }
	
	BOOL				SetTextColor(int iItem, int iSubItem, COLORREF crText = ::GetSysColor(COLOR_WINDOWTEXT));
	BOOL				SetBKColor(int iItem, int iSubItem, COLORREF crBk = ::GetSysColor(COLOR_WINDOW));
	BOOL				SetItemTextColor(int iItem, COLORREF crText = ::GetSysColor(COLOR_WINDOWTEXT));
	BOOL				SetItemBKColor(int iItem, COLORREF crBk = ::GetSysColor(COLOR_WINDOW));
	BOOL				SetSubItemTextColor(int iSubItem, COLORREF crText = ::GetSysColor(COLOR_WINDOWTEXT));
	BOOL				SetSubItemBKColor(int iSubItem, COLORREF crBk = ::GetSysColor(COLOR_WINDOW));

	int					GetColumns() const { return GetHeaderCtrl()->GetItemCount(); }

	// Combo
	int					GetComboCurSel(int iItem, int iSubItem) const;
	int					SetComboCurSel(int iItem, int iSubItem, int iSelect);
	int					GetComboItemCount(int iSubItem) const;
	CString				GetComboLBText(int iSubItem, int iIndex) const;
	int					GetComboLBTextLen(int iSubItem, int iIndex) const;

	// Check
	int					GetCheck(int iItem, int iSubItem) const;
	int					SetCheck(int iItem, int iSubItem, int iCheck = 1);

	// Date
	const COleDateTime*	GetDate(int iItem, int iSubItem) const;
	COleDateTime*		GetDate(int iItem, int iSubItem);
	BOOL				SetDate(int iItem, int iSubItem, COleDateTime odtTime = COleDateTime::GetCurrentTime());

	// Progress
	int					GetPercentage(int iItem, int iSubItem) const;
	BOOL				SetPercentage(int iItem, int iSubItem, int iPercent);
	BOOL				SetPercentage(int iItem, int iSubItem, int iPercent, LPCTSTR lpProgressFormat);

	int					CompareString(int iItem, int iSubItem, const CString& str) const;
	int					CompareString(int iItem, int iSubItem, const std::vector<CString>& strs) const;

	int					InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = 0, int nWidth = -1, int nSubItem = -1);
	int					InsertColumn(int nCol, const LVCOLUMN* pColumn);
	BOOL				DeleteColumn(int nCol);
	BOOL				DeleteAllColumns();

	int					InsertItem(int nItem, LPCTSTR lpszItem);
	int					InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
	int					InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam);
	int					InsertItem(const LVITEM* pItem);

	int					InsertItem(int nItem, LPCTSTR lpszItem, COLORREF crText, COLORREF crBk);
	int					InsertItem(int nItem, LPCTSTR lpszItem, int nImage, COLORREF crText, COLORREF crBk);
	int					InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam, COLORREF crText, COLORREF crBk);
	int					InsertItem(const LVITEM* pItem, COLORREF crText, COLORREF crBk);

	BOOL				DeleteItem(int nItem);
	BOOL				DeleteAllItems();
	
	BOOL				ControlClose();
	BOOL				GetSubItemRectEx(int iItem, int iSubItem, int nArea, CRect& ref) const;

	double				GetItemDouble(int iItem, int iSubItem) const;
	int					GetItemInt(int iItem, int iSubItem) const;

	BOOL				SetItemInt(int nItem, int nSubItem, int nValue);
	BOOL				SetItemDouble(int nItem, int nSubItem, double dValue, int iPoint = 3);
	
	BOOL				SetItemValue(int iItem, int iSubItem, int iValue);
	BOOL				SetItemValue(int iItem, int iSubItem, double dValue, int iPoint = 3);
	BOOL				SetItemValue(int iItem, int iSubItem, LPCTSTR pszValue);

	BOOL				LoadFile(LPCTSTR lpFilePath, LPCTSTR lpDelimiter, std::vector<CString>& strs);
	BOOL				SaveFile(LPCTSTR lpFilePath, LPCTSTR lpDelimiter) const;

protected:
	int								m_iItem;
	int								m_iSubItem;
	int								m_iDataIndex;
	BOOL							m_bShowRight;

	CColumnEditBox					m_editBox;
	CColumnComboBox					m_comboBox;
	CColumnDateBox					m_dateBox;

	std::vector<T_LISTCTRL_DATA>	m_arrData;

	COLORREF						m_crSelectItem;
	COLORREF						m_crCheckMark;
	COLORREF						m_crProgressBar;
	COLORREF						m_crProgressLine;

protected:
	BOOL							StartControl(int iItem, int iSubItem, BOOL bSelect = FALSE);
	int								HitTestEx(CPoint pt, int &iSubItem) const;

	BOOL							OnShowEdit(int iItem, int iSubItem, CRect& rectControl, int iHPos);
	BOOL							OnShowCombo(int iItem, int iSubItem, CRect& rectControl, int iHPos);
	BOOL							OnShowDate(int iItem, int iSubItem, CRect& rectControl, int iHPos);
	BOOL							OnSetCheck(int iItem, int iSubItem, int iHPos);
	BOOL							OnButtonDown(int iItem, int iSubItem, int iHPos);

	void							OnDrawCheck(int iItem, CDC* pDC, CRect& rect, T_LISTCTRL_DATA* pData, const DWORD& dwAlign);
	void							OnDrawProgress(int iItem, CDC* pDC, CRect& rect, T_LISTCTRL_DATA* pData, const DWORD& dwAlign);
	void							OnDrawButton(int iItem, CDC* pDC, CRect& rect, T_LISTCTRL_DATA* pData, const DWORD& dwAlign);

	BOOL							KeyEvent(WPARAM wParam);
	BOOL							BeginLabelMessage(int iItem, int iSubItem);
	BOOL							EndLabelMessage();

	void							InsertDrawItem(int iItem, COLORREF crText = ::GetSysColor(COLOR_WINDOWTEXT), COLORREF crBk = ::GetSysColor(COLOR_WINDOW));
	void							DeleteAllDrawItem();

	CString							strToken(LPCTSTR pszSrc, LPCTSTR pszDelimiter, int& iStart);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnEditEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComboEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDateEnd(WPARAM wParam, LPARAM lParam);

};

#pragma pack(pop)
