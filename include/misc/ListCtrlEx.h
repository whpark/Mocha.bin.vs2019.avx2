#pragma once

#include "AFX_EXT_MISC.h"
#include "ListColumnData.h"

#pragma pack(push, 8)

#define SCROLL_SIZE			20

//<< KMS
	//#ifndef SAFE_DELETE
	//#	define SAFE_DELETE(p)			{ if(p){ delete p; p=NULL; } }
	//#endif
//>>
	#ifndef SAFE_DELETE_OBJECT
	#	define SAFE_DELETE_OBJECT(p)	{ if(p){ p->DeleteObject(); delete p; p=NULL; } }
	#endif

//<< KMS
	//enum eListCtrlExTextAlign { 
	//	eLCETA_LEFT	= DT_LEFT,
	//	eLCETA_CENTER	= DT_CENTER,
	//	eLCETA_RIGHT	= DT_RIGHT, 
	//};
	//
	//enum eListCtrlExLineType {
	//	eLCELT_NONE	= -1, 
	//	eLCELT_SOLID	= PS_SOLID,
	//	eLCELT_DOT	= PS_DOT, 
	//};
//>>

///////////////////////////////////////////////////////////////////////////////////////////////////
// 2007.01.24.		START	Add By			SeongHyeong, Lee
//class CMathEx
//{
//public:
//	template<class OutType, class InType>
//	static OutType Round(InType Val, int iRound) {
//		char		szBuf[50]	= {0, };
//		char		szFmt[10] = {0, };
//		sprintf( szFmt, "%%.%df", iRound );
//		sprintf( szBuf, szFmt, Val );
//
//		return (OutType)atof(szBuf);
//	}
//};
// 2007.01.24.		END
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
// 2007.05.10.		START	Add By			PWH
// class IListCtrlItemWrapper
class IListCtrlExItemWrapper { // KMS CListCtrlExWnd와 중복 방지
protected:
	void*	m_pCookie;
public:
	IListCtrlExItemWrapper()			{ m_pCookie = NULL; }
	virtual ~IListCtrlExItemWrapper()	{}

	void SetCookie(void* _pCookie)		{ m_pCookie = _pCookie; }
	void* GetCookie()					{ return m_pCookie; }

	virtual BOOL GetSubItemText(int iSubItem, CString& strSubItem) = NULL;
	virtual BOOL SetSubItemText(int iSubItem, LPCTSTR szSubItem) = NULL;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// 2007.01.24.		START	Add By			SeongHyeong, Lee

class AFX_EXT_API_MISC CListCtrlEx  : public CWnd {
	DECLARE_DYNAMIC(CListCtrlEx)

protected:
	//<< KMS
	typedef miscListCtrlColumn::LISTCTRLEX_HEADER			LISTCTRLEX_HEADER;
	typedef miscListCtrlColumn::T_PROGRESS_DATA				T_PROGRESS_DATA;

	typedef miscListCtrlColumn::CColumnComboBox				CColumnComboBox;
	typedef miscListCtrlColumn::CColumnDateBox				CColumnDateBox;
	typedef miscListCtrlColumn::CColumnEditBox				CColumnEditBox;
	typedef miscListCtrlColumn::CColumnSpinBox				CColumnSpinBox;

	typedef std::vector<LISTCTRLEX_HEADER>::iterator		ITERATOR_DATA;
	typedef std::vector<int>::iterator						ITERATOR_CHECK;
	typedef std::vector<CString>::iterator					ITERATOR_STRS;
	typedef std::vector<T_PROGRESS_DATA>::iterator			ITERATOR_PROGRESS;

	typedef std::vector<LISTCTRLEX_HEADER>::const_iterator	CONST_ITERATOR_DATA;
	typedef std::vector<int>::const_iterator				CONST_ITERATOR_CHECK;
	typedef std::vector<CString>::const_iterator			CONST_ITERATOR_STRS;
	typedef std::vector<T_PROGRESS_DATA>::const_iterator	CONST_ITERATOR_PROGRESS;
	//>>

	//<< pWH
protected:
	IListCtrlExItemWrapper* m_pItemWrapper;
public:
	void SetItemWrapper(IListCtrlExItemWrapper* pItemWrapper) {
		m_pItemWrapper = pItemWrapper;
	}
	//>>

public:
	//<< PWH
	//typedef struct _LISTCTRLEX_ITEM_SUB {
	//	char						szDesc[128];
	//	COLORREF					TextColor;
	//	COLORREF					BackgroundColor;
	//} LISTCTRLEX_ITEM_SUB, FAR* LPLISTCTRLEX_ITEM_SUB;
	//>>

	typedef struct _LISTCTRLEX_ITEM {
		//<< PWH
		//void*						pData;
		//std::vector<LISTCTRLEX_ITEM_SUB>	SubItems;
		void*						pCookie;
		DWORD						dwItemState;
		COLORREF					crText;
		COLORREF					crBkgnd;
		//>>
	} LISTCTRLEX_ITEM, FAR* LPLISTCTRLEX_ITEM;

	//<< KMS
		//typedef struct _LISTCTRLEX_HEADER {
		//	TCHAR					szDesc[128];
		//	int						width;
		//	eListCtrlExTextAlign		alignment;
		//	BOOL						hover;
		//	BOOL						bVisible;	// PWH
		//} LISTCTRLEX_HEADER, FAR* LPLISTCTRLEX_HEADER;
	//>>

// Control Information
	typedef struct _LISTCTRLEX_CONTROL_INFO {
		std::vector<LISTCTRLEX_HEADER>	Headers;
		std::vector<LPLISTCTRLEX_ITEM>	Items;
	} LISTCTRLEX_CONTROL_INFO, FAR* LPLISTCTRLEX_CONTROL_INFO;

	CListCtrlEx(CWnd* pwndParent=NULL);
	virtual				~CListCtrlEx();

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);
public:
	virtual BOOL			Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int				OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void			OnDestroy();
	afx_msg void			OnPaint();
	afx_msg BOOL			OnEraseBkgnd(CDC* pDC);
	afx_msg void			OnSize(UINT nType, int cx, int cy);
	
	afx_msg void			OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void			OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void			OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void			OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void			OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL			OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void			OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void			OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void			OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT			OnEditEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT			OnComboEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT			OnDateEnd(WPARAM wParam, LPARAM lParam);
	

//==================================================================================================
// Member Veriable
protected:
	CWnd*						m_pwndParent;
	LPLISTCTRLEX_CONTROL_INFO	m_lpInfo;																// Control information
//	LPLISTCTRLEX_HEADER			m_lpHdrSel;
	eListCtrlExLineType			m_lceltRowLine;
	eListCtrlExLineType			m_lceltColLine;
	CPen*						m_ppenRowLine;
	CPen*						m_ppenColLine;
	BOOL						m_bShowHeader;
	int							m_iHeaderHeight;											// Header height
	int							m_iItemHeight;												// Item row height
	BOOL						m_bDrawingLock;											// Drawing lock flag	
	BOOL						m_bMoveLock;

	POINT						m_ptSelectedHeader;
	LISTCTRLEX_HEADER*			m_lpSelectedHeader;
	int							m_iSelectedHeaderWidth;

	CFont*						m_pfontHeader;												// Header font
	CFont*						m_pfontItem;												// Item font
	int							m_iSelectedItem;											// Item selection
	int							m_iItemLimit;
public:

protected:
	void						fn_ComputeScrollbar();										// Compute the window scroll-bar information
	void						fn_DrawHeaders(CDC* pDC, CRect rect);							// Draw headers
	void						fn_DrawItems(CDC* pDC, CRect rect);							// Draw items
	void						fn_DrawItemBackground(CDC* pDC, CRect rect, LPLISTCTRLEX_ITEM lpItem, int iItemIndex);			// Draw item background
	//<< PWH
	//void						fn_DrawItemText(CDC* pDC, CRect rect, LPLISTCTRLEX_ITEM_SUB lpSub, LPLISTCTRLEX_HEADER lpHeader);	// Draw item text
	void						fn_DrawItemText(CDC* pDC, CRect rect, LISTCTRLEX_HEADER* lpHeader, LPCTSTR szDesc, COLORREF crText, COLORREF crBkgnd, DWORD dwState);	// Draw item text
	//>>
	void						fn_LockDrawing()	{ m_bDrawingLock=TRUE; }
	void						fn_UnlockDrawing()	{ m_bDrawingLock=FALSE; }

	//<< KMS
	std::vector<LPLISTCTRLEX_ITEM>&	fn_GetItems() const;
	std::vector<LISTCTRLEX_HEADER>&	fn_GetHeaders() const;
	//>>
public:

	
	inline void					RecalcLayout()		{ fn_ComputeScrollbar(); }
	void						SetItemRowLineType(eListCtrlExLineType eLineStyle = eLCELT_NONE);
	void						SetItemColLineType(eListCtrlExLineType eLineStyle = eLCELT_NONE);
	void						SetHeaderFont(LOGFONT lf);
	CFont*						GetHeaderFont() const; // KMS add const
	void						SetItemFont(LOGFONT lf);
	CFont*						GetItemFont() const; // KMS add const
	
	void						ShowHeader(BOOL bShow);
	void						SetHeaderHeight(int iHeight)	{ m_iHeaderHeight = iHeight; }
	int							GetHeaderHeight() const			{ return m_iHeaderHeight; } // KMS add const
	void						SetItemRowHeight(int iHeight)	{ m_iItemHeight = iHeight; }
	int							GetItemRowHeight() const		{ return m_iItemHeight; } // KMS add const
	void						SetItemBackgroundColor(int iItem, int iSubItem, COLORREF colorBackground);
	COLORREF					GetItemBackgroundColor(int iItem, int iSubItem) const; // KMS add const
	void						SetItemRowColor(int iItem, COLORREF colorRow);
	void						SetItemTextColor(int iItem, int iSubItem, COLORREF colorText);
	COLORREF					GetItemTextColor(int iItem, int iSubItem) const; // KMS add const

	void						RemoveAllHeaders();
	void						AddHeader(LISTCTRLEX_HEADER Header);
	void						AddHeader(LPCTSTR lpszDesc, int iWidth, eListCtrlExTextAlign eAlign=eLCETA_LEFT, BOOL bVisible = TRUE /*PWH*/);

	//<< PWH
	BOOL						SetColumnVisible(int iColumn, BOOL bVisible = TRUE, BOOL bInvalidate = TRUE);
	BOOL						GetColumnVisible(int iColumn) const; // KMS add const
	BOOL						SetColumnWidth(int iColumn, int nWidth, BOOL bInvalidate = TRUE);
	int							GetColumnWidth(int iColumn) const; // KMS add const
	int							FindHeader(LPCTSTR szDesc) const; // KMS add const
	//>>

	//<< PWH
	//void						AddItem(LPCTSTR lpszDesc, COLORREF colorText=::GetSysColor(COLOR_WINDOWTEXT), void* pData=NULL);
	void						SetItemLimit(int iCount) { m_iItemLimit = iCount; }
	int							GetItemLimit() const			{ return m_iItemLimit;	} // KMS add const
	void						AddItem(void* pCookie, COLORREF colorText=::GetSysColor(COLOR_WINDOWTEXT));
	void						Reserve(int nCount);
	BOOL						SetItem(int iItem, void* pCookie, COLORREF crText=::GetSysColor(COLOR_WINDOWTEXT), COLORREF crBkgnd=::GetSysColor(COLOR_WINDOW), DWORD dwState = 0);
	BOOL						RemoveItem(int iItem);
	//>>
	void						SetItemRowData(int iItem, void* pData);
	void*						GetItemRowData(int iItem) const;  // KMS add const
	void						RemoveAllItems();
	void						SetItemText(int iItem, int iSubItem, LPCTSTR lpszItemText, COLORREF crText=::GetSysColor(COLOR_WINDOWTEXT), COLORREF crBkgnd=::GetSysColor(COLOR_WINDOW), DWORD dwState = 0);
	//<< PWH
	//char*						GetItemText(int iItem, int iSubItem);
	CString						GetItemText(int iItem, int iSubItem) const; // KMS add const
	//>>

	int							GetItemSize() const; // KMS add const
	int							GetSelectedItemIndex() const { return m_iSelectedItem; } // KMS add const
	void						SetSelectItemIndex(int iIndex, BOOL bNotify = FALSE);
	void						MoveNext();
	void						MovePrevieous();
	void						MoveLock(BOOL bLock=FALSE)	{ m_bMoveLock=bLock; }
	BOOL						IsMoveLocked() const { return m_bMoveLock; }  // KMS add const

	//<< PWH
	BOOL						EnsureVisible(int iItem);
	//>>

//<< KMS
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
	
	BOOL SetColumnCombo(
			int iSubItem,
			const std::vector<CString>& strs,
			BOOL bShowoDropDown = TRUE);
	
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

	void			SetColorCheckMark(COLORREF crCheckMark) { m_crCheckMark = crCheckMark; }
	void			SetColorProgressBar(COLORREF crProgressBar) { m_crProgressBar = crProgressBar; }
	void			SetColorProgressLine(COLORREF crProgressLine) { m_crProgressLine = crProgressLine; }

	BOOL			GetSubItemRect(int iItem, int iSubItem, CRect& rectSubItem);
	int				GetHeaderSize() const { if (!m_lpInfo) NULL; return (int)m_lpInfo->Headers.size(); }

	// Combo
	int				GetComboCurSel(int iItem, int iSubItem) const;
	int				SetComboCurSel(int iItem, int iSubItem, int iSelect);
	int				GetComboItemCount(int iSubItem) const;
	CString			GetComboLBText(int iSubItem, int iIndex) const;
	int				GetComboLBTextLen(int iSubItem, int iIndex) const;

	// Date
	COleDateTime	GetDate(int iItem, int iSubItem) const;
	BOOL			SetDate(int iItem, int iSubItem, COleDateTime odtTime = COleDateTime::GetCurrentTime());

	// Check
	int				GetCheck(int iItem, int iSubItem) const;
	int				SetCheck(int iItem, int iSubItem, int iCheck = 1);
	int				ShowHeaderCheck(int iSubItem, int iShow = 1);

	// Progress
	int				GetPercentage(int iItem, int iSubItem) const;
	BOOL			SetPercentage(int iItem, int iSubItem, int iPercent);
	BOOL			SetPercentage(int iItem, int iSubItem, int iPercent, LPCTSTR lpProgressFormat);

	int				CompareString(int iItem, int iSubItem, const CString& str) const;
	int				CompareString(int iItem, int iSubItem, const std::vector<CString>& strs) const;

	BOOL			ControlClose();

	double			GetItemDouble(int iItem, int iSubItem) const;
	int				GetItemInt(int iItem, int iSubItem) const;

	void			SetItemInt(int nItem, int nSubItem, int nValue, COLORREF crText=::GetSysColor(COLOR_WINDOWTEXT), COLORREF crBkgnd=::GetSysColor(COLOR_WINDOW), DWORD dwState = 0);
	void			SetItemDouble(int nItem, int nSubItem, double dValue, int iPoint = 3, COLORREF crText=::GetSysColor(COLOR_WINDOWTEXT), COLORREF crBkgnd=::GetSysColor(COLOR_WINDOW), DWORD dwState = 0);
	
	void			SetItemValue(int iItem, int iSubItem, int iValue, COLORREF crText=::GetSysColor(COLOR_WINDOWTEXT), COLORREF crBkgnd=::GetSysColor(COLOR_WINDOW), DWORD dwState = 0);
	void			SetItemValue(int iItem, int iSubItem, double dValue, COLORREF crText=::GetSysColor(COLOR_WINDOWTEXT), COLORREF crBkgnd=::GetSysColor(COLOR_WINDOW), DWORD dwState = 0);
	void			SetItemValue(int iItem, int iSubItem, LPCTSTR pszValue, COLORREF crText=::GetSysColor(COLOR_WINDOWTEXT), COLORREF crBkgnd=::GetSysColor(COLOR_WINDOW), DWORD dwState = 0);

	void			AddItem(COLORREF colorText=::GetSysColor(COLOR_WINDOWTEXT));

	BOOL			SaveFile(LPCTSTR lpFilePath, LPCTSTR lpDelimiter) const;
	BOOL			LoadFile(LPCTSTR lpFilePath, LPCTSTR lpDelimiter, std::vector<CString>& strs);
	
	template <typename TYPE> BOOL LoadFile(LPCTSTR lpFilePath, LPCTSTR lpDelimiter, std::vector<CString>& strs) {
		if (!m_pItemWrapper)
			ASSERT(m_pItemWrapper);

		std::vector<LISTCTRLEX_HEADER>&Headers	= fn_GetHeaders();
		RemoveAllItems();
		
		CString	str;
		CFile	file;
		if (file.Open(lpFilePath, CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeRead)) {
			int iTemp = 0;
			int iItem = 0;
		
			CArchiveU	arU(&file, CArchiveU::load);
			arU.ReadEncodingSignature();
			while (arU.ReadStringU(str)) {
				if (iTemp == 0) {
					++iTemp;
					continue;
				}
				
				int iStart = 0;
				CString strTok;
				strs.push_back(str);

				AddItem(new TYPE);
				for (int iSubItem = 0; iSubItem < GetHeaderSize(); ++iSubItem) {
					strTok = strToken(str, lpDelimiter, iStart);
					ITERATOR_DATA itData = Headers.begin() + iSubItem;
					if (itData->bCheck)
						SetCheck(iItem, iSubItem, _ttoi(strTok));
					else if (itData->bProgress)
						SetPercentage(iItem, iSubItem, _ttoi(strTok));
					else SetItemText(iItem, iSubItem, strTok);
				}
				++iItem;
			}
		
			arU.Close();
			file.Close();
		}
		else
			return FALSE;
		
		Invalidate(FALSE);
		return TRUE;
	}

protected:
	int								m_iItem;
	int								m_iSubItem;
	int								m_iDataIndex;
	BOOL							m_bStartControl;
	BOOL							m_bCursorArrow;

	CColumnEditBox					m_editBox;
	CColumnComboBox					m_comboBox;
	CColumnDateBox					m_dateBox;

	COLORREF						m_crCheckMark;
	COLORREF						m_crProgressBar;
	COLORREF						m_crProgressLine;

protected:
	BOOL							StartControl(int iItem, int iSubItem);
	int								HitTestEx(CPoint point, int& iSubItem);

	BOOL							OnShowEdit(int iItem, int iSubItem, const ITERATOR_DATA& itData);
	BOOL							OnShowCombo(int iItem, int iSubItem, const ITERATOR_DATA& itData);
	BOOL							OnShowDate(int iItem, int iSubItem, const ITERATOR_DATA& itData);
	BOOL							OnSetCheck(int iItem, int iSubItem, const ITERATOR_DATA& itData);

	void							OnDrawCheck(int iItem, CDC* pDC, CRect rect, LISTCTRLEX_HEADER* pData, const DWORD& dwAlign);
	void							OnDrawProgress(int iItem, CDC* pDC, CRect rect, LISTCTRLEX_HEADER* pPData, const DWORD& dwAlign);

	BOOL							BeginLabelMessage(int iItem, int iSubItem);
	BOOL							EndLabelMessage();

	BOOL							KeyEvent(WPARAM wParam);

	CString							strToken(LPCTSTR pszSrc, LPCTSTR pszDelimiter, int& iStart);
//>>

};
#pragma pack(pop)

// 2007.01.24.		END
///////////////////////////////////////////////////////////////////////////////////////////////////
