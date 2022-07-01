#pragma once

#include "AFX_EXT_MISC.h"

// PositionBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPositionBar window

class AFX_EXT_CLASS_MISC CPositionBar : public CWnd {
// Construction
public:
	enum DIRECTION {Vertical, Horizontal};
	CPositionBar();
	virtual BOOL Init(const CRect& rectBoundary, DIRECTION);
	virtual BOOL Init(int idBoundaryCtrl, DIRECTION);

// Attributes
public:
	void SetBoundary(const CRect& rectBoundary) { m_rectBoundary = rectBoundary; m_idBoundaryCtrl = -1; }
	void SetBoundary(int idBoundaryCtrl) { m_idBoundaryCtrl = idBoundaryCtrl; }

	DIRECTION GetDirection() const { return m_direction; }

	BOOL m_bOverridePaint = FALSE;

// Operations
public:
// Overrides
public:

// Implementation
public:
	virtual ~CPositionBar();

	// Generated message map functions
protected:
	CRect GetBoundaryRect();
	DIRECTION m_direction;
	CRect m_rectBoundary;
	int m_idBoundaryCtrl;
	CPoint m_pt;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
