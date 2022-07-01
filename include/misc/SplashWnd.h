#pragma once

#include "AFX_EXT_MISC.h"
#include <ATLImage.h>
// CSplashWnd

class CSplashThread;

class AFX_EXT_CLASS_MISC CSplashWnd : public CWnd {
friend class CSplashThread;
	DECLARE_DYNAMIC(CSplashWnd)

	enum eTIMER { T_CLOSE = 1035, };

public:
	HINSTANCE m_hInstanceMain;
	int m_idImage;
	CString m_strPathImage;

protected:
	CSplashWnd();
public:
	virtual ~CSplashWnd();

public:
	BOOL Close(DWORD dwDelay = 0);
	static CSplashWnd* CreateSplashWnd(CWinApp& app, LPCTSTR pszPathImage = NULL, int idImage = 0, BOOL bTopMost = TRUE,
		DWORD dwStyleAdd = 0, DWORD dwExStyleAdd = 0, DWORD dwStyleRemove = 0, DWORD dwExStyleRemove = 0);
	static BOOL CloseSplashWnd(CWnd* pWnd);

protected:
	CImage m_img;
	CWnd** m_ppMainWnd;

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
