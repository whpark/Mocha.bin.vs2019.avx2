#pragma once

#include "AFX_EXT_MESSAGE_TOOL_WND.h"

#include "MIP/MIP.h"

#include "TRefPointer.hpp"
#include "MessageTool.hpp"
//#include "misc/EditBox.h"

#include <afxmt.h>
#pragma warning (disable : 4244)
#define TIMER_ANIMATION	9090

class CMessageThread;
class CMessageWnd;
class CMessageWnd : public CFrameWnd {
	friend class CMessageThread;
	DECLARE_DYNCREATE(CMessageWnd)	

	enum {
		eS_AREA,
		eS_MESSAGE,
		eS_INPUT,
		eS_END,
	} eS;

	enum {
		eC_NONE = -1,
		eC_BUTTON_1,
		eC_BUTTON_2,
		eC_BUTTON_3,
		eC_BUTTON_4,

		eC_EDIT_INPUT,
		eC_END,
	} eC;

public:
	CMessageWnd(CWnd* pwndParent=NULL);							 // protected constructor used by dynamic creation
	virtual					~CMessageWnd();

public:
	static TRefPointer<CIPClient> s_rIPClient;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:
	CEdit m_Edit;
	CWnd* m_pwndParent;
	int m_iSelect, m_iEdit;
	CRect m_rectS[eS_END], m_rectC[eC_END];
	CString m_strButton[eC_BUTTON_4 - eC_BUTTON_1+1];
	CString m_strMessage;
	CStrings m_strsAnimation;
	DWORD m_dwTick;
	int m_iAnimation = 0;

	void ComputeArea();
public:
	T_MESSAGE_DATA	m_tMD;
	PIPCOMMAND m_pCMD = nullptr;
	void SetMessageData(const T_MESSAGE_DATA& tMD);
	void SetCommand(PIPCOMMAND pCMD);
	void DrawText(CDC* pDC, LPRECT rect, LPCTSTR szText, int iFontSize, int iFontWeight, COLORREF crText, DWORD dwAlign = DT_LEFT | DT_VCENTER, LPCTSTR szFont = _T("Segoe UI"));
	void DrawLineRect(CDC* pDC, LPRECT rect, COLORREF crPen, int iTick = 1);
	void DrawTextBk(CDC* pDC, LPRECT rect, LPCTSTR szText, int iFontSize, int iFontWeight, COLORREF crPen, COLORREF crBack, COLORREF crText, DWORD dwAlign = DT_LEFT | DT_VCENTER, LPCTSTR szFont = _T("Segoe UI"), BOOL bPassword = FALSE);
	static CMessageWnd* CreateMessageWnd(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);
};
