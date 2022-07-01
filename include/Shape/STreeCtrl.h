
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSTreeCtrl window

#include "AFX_EXT_SHAPE.h"

#include <afxcmn.h>

class CShapeObject;

class AFX_EXT_CLASS_SHAPE CSTreeCtrl : public CTreeCtrl {
protected:
	CWnd* m_pWndOwner;
	BOOL m_bEditable;
// Construction
public:
	CSTreeCtrl(CWnd* pWndOwner = NULL);

	void SetOwner(CWnd* pWndOWner) { m_pWndOwner = pWndOWner; }
	BOOL IsEditable() const { return m_bEditable; }
	void SetEditable(BOOL bEditable) { m_bEditable = bEditable; }

	void FillObject(const TRefList<CShapeCollection>& collections, HTREEITEM hItemParent = TVI_ROOT);
protected:
	HTREEITEM FillObject(const CShapeObject& object, HTREEITEM hItemParent, HTREEITEM hItemPrev);

public:
	HTREEITEM GetItemByData(const void* pObject, HTREEITEM hItemParent, BOOL bTraverseChild = FALSE) const;
	HTREEITEM GetItem(int iIndex, HTREEITEM hItemParent) const;
	HTREEITEM GetItemPrev(int iIndex, HTREEITEM hItemParent) const;
	int GetItemIndex(HTREEITEM hItem, HTREEITEM hItemParent) const;
	CShapeObject* GetShapeObject(HTREEITEM hItem);

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

public:
	BOOL DeleteItem(HTREEITEM hItem);
	void ClearSelection();
	DWORD m_dwDragStart;
	
// Implementation
public:
	virtual ~CSTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnCopyClone();
	//afx_msg void OnMove();
	afx_msg void OnCopy();
	afx_msg void OnCut();
	afx_msg void OnPaste();

	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	CShapeObject* m_pObjectCopy;
public:
	CShapeObject* GetClipboardObject() { return m_pObjectCopy; }
	enum eNOTIFY_CODE {
		NC_SETFOCUS = NM_LAST-2000U,
		NC_SELECT_OBJECT,
		NC_COPY_OBJECT,
		NC_CUT_OBJECT,
		NC_PASTE_OBJECT,
		NC_DELETE_OBJECT,
	};
	struct NOTIFY_DATA {
		NMHDR hdr;
		CShapeObject* pObject;
		CShapeObject* pObject2;
	};
	LRESULT NotifyEvent(eNOTIFY_CODE evtCode, CShapeObject* pObject);
};
