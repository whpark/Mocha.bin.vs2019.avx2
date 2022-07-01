#pragma once

#include "AFX_EXT_FRAME_GRABBER.h"

// CCameraPaneWnd

#include "CameraLayout.h"
#include "misc/PositionBar.h"

class CCameraPaneWnd;
class CPositionBar;
class CMFCTabCtrl;
class ICameraView;

class AFX_EXT_CLASS_FRAME_GRABBER CCameraPaneWnd : public CWnd {
	DECLARE_DYNAMIC(CCameraPaneWnd)
	enum eCHILD_CONTROL_ID { CCI_TAB = 10000, CCI_VIEW_1 = 10100, CCI_POSITION_BAR_1 = 10200, };

public:
	CCameraPaneWnd();
	virtual ~CCameraPaneWnd();

public:
	BOOL InitPane(const CCameraLayoutItem* pSet = NULL);
	BOOL ActivateChildPane(CWnd* pChildViewOrPane);
	ICameraView* GetCameraView(LPCSTR pszUnitName = NULL);
	//ICameraView* GetActiveCameraView();

protected:
	CCameraLayoutItem m_layout;

	ICameraView* m_pView;
	TList<CCameraPaneWnd> m_panes;
	TList<CPositionBar> m_positionBars;
	CMFCTabCtrl* m_pTabCtrl;

	void OnPositionBar(int iIndex);
	void AdjustLayout(int iIndex = -1);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};
