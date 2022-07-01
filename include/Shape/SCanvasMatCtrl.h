#pragma once

#include "SCanvas.h"
#include "xMathUtil/MatView.h"

#pragma pack(push, 8)

// CSCanvasMatCtrl

class AFX_EXT_CLASS_SHAPE CSCanvasMatCtrl : public CWnd, public CSCanvasMat {
	DECLARE_DYNAMIC(CSCanvasMatCtrl)

public:
	CSCanvasMatCtrl();
	virtual ~CSCanvasMatCtrl();

public:
	typedef enum { MF_NONE, MF_DRAG_IMAGE, MF_SELECT_OBJECT, MF_SELECT_REGION, MF_ZOOM_IN, MF_ZOOM_OUT, } eMOUSE_FUNCTION;
	enum eNOTIFY_CODE {
		NC_SETFOCUS = NM_LAST-2000U,
		NC_KILLFOCUS,
		NC_SEL_REGION,
		NC_SELECT_OBJECT,
	};
	struct NOTIFY_DATA {
		NMHDR hdr;
		CRect2d rect;
		TRefPointer<CShapeObject> rObject;
	};
	typedef enum { BM_TRANSPARENT, BM_ALPHABLEND, } eBLIT_METHOD;

	enum { eIDStart = 1010, eIDZoomLable = eIDStart, eIDZoom, eIDZoomUp, eIDZoomDown, eIDInfo, eIDViewPositionBox, eIDEND};

	double m_dMaxZoom;
	double m_dMinZoom;

protected:
	// Toolbar
	BOOL m_bShowTool;
	CRect m_rectTool;
	//CStatic m_staticZoom;
	CComboBox m_comboZoom;
	//CRefCounter m_rcUpdateScroll;
	CMFCButton m_btnZoom[2];
	CStatic m_staticInfo;
	CButton m_btnViewPositionBox;

	// Data
	CRect2d m_rectBound;
	TRefList<CShapeCollection> m_collections;
	TRefPointer<CShapeObject> m_rObjectSelected;

	// Attributes
	struct T_ATTRIBUTE {
		CString strName;
		cv::Mat img, imgView;
		eBLIT_METHOD bm;
		union {
			struct {
				COLORREF crTransparent;
			};
			struct {
				BLENDFUNCTION blend;
			};
		};
		int operator == (LPCTSTR pszName) const { return strName == pszName; }
	};
	TList<T_ATTRIBUTE> m_imgsAttribute;

	enum eMOUSE_MODE { MM_NONE, MM_ONECLICK, MM_TWOCLICK };
	struct {
		BOOL bDragMode;

		CPoint2d ptImgLast;
		CPoint2i ptScrollLast;
		CPoint2i ptLast;
		CPoint2i ptCurrent;
	} m_mouse;

	struct T_CROSS_MARK {
		CString strName;
		CString strLabel;
		BOOL bShow;
		CPoint2d pt;
		CSize2i size;
		int nPenStyle;
		int iThick;
		COLORREF cr;
		int operator == (LPCTSTR pszName) const { return strName == pszName; }
	};
	struct T_RECT_REGION {
		CString strName;
		CString strLabel;
		BOOL bShow;
		CRect2d rect;
		int nPenStyle;
		int iThick;
		COLORREF cr;
		int operator == (LPCTSTR pszName) const { return strName == pszName; }
	};
	struct {
		TList<T_CROSS_MARK> crosses;
		TList<T_RECT_REGION> rects;
	} m_display;

public:
	struct T_AXES {
		bool bShow = false;
		COLORREF crAxes { 0x00'90'120'120 };
		COLORREF crText { 0x00'90'90'70 };
		CSize2i sizeInPixel { 30, 30 };
	};
protected:
	T_AXES m_axes;

	// operations
public:
	BOOL InitView(double dZoom = -1);
	BOOL SetShape(const TRefList<CShapeCollection>& collections, double dZoom = -1, BOOL bCopyShape = FALSE);

	BOOL SetCenter(const CPoint2d& pt = CPoint2d(-1, -1));
	double GetZoom() { double dZoom = -1; CheckAndGetZoom(dZoom); return dZoom; }
	void SetMouseFunction(eMOUSE_FUNCTION eMFLButton = MF_SELECT_OBJECT, eMOUSE_FUNCTION eMFMButton = MF_DRAG_IMAGE, eMOUSE_FUNCTION eMFRButton = MF_DRAG_IMAGE, eMOUSE_FUNCTION eMFWheelUp = MF_ZOOM_IN, eMOUSE_FUNCTION eMFWheelDown = MF_ZOOM_OUT) {
		m_eMFLButton = eMFLButton; m_eMFMButton = eMFMButton; m_eMFRButton = eMFRButton; m_eMFWheelUp = eMFWheelUp; m_eMFWheelDown = eMFWheelDown;
	}
	void GetMouseFunction(eMOUSE_FUNCTION& eMFLButton, eMOUSE_FUNCTION& eMFMButton, eMOUSE_FUNCTION& eMFRButton, eMOUSE_FUNCTION& eMFWheelUp, eMOUSE_FUNCTION& eMFWheelDown) const {
		eMFLButton = m_eMFLButton; eMFMButton = m_eMFMButton; eMFRButton = m_eMFRButton; eMFWheelUp = m_eMFWheelUp; eMFWheelDown = m_eMFWheelDown;
	}
	void ShowTool(BOOL bShow);
	BOOL IsToolVisible() const { return m_bShowTool; }

	BOOL GetShape(TRefList<CShapeCollection>& collections) const { collections.CopyRefFrom(m_collections); return TRUE; }
	const TRefList<CShapeCollection>& GetShape() const { return m_collections; }
	TRefList<CShapeCollection>& GetShape() { return m_collections; }

	TRefPointer<CShapeObject>& GetSelectedObject() { return m_rObjectSelected; }
	const CShapeObject* GetSelectedObjectConst() const { return m_rObjectSelected; }
	BOOL SelectObject(CShapeObject* pObject);

	BOOL GetCurrentRect(CRect2d& rect) const;
	const cv::Mat& GetImage() const { return m_mat; }

	BOOL AddAttributeLayer(LPCTSTR pszName, const cv::Mat& img, COLORREF crTransparent);
	BOOL AddAttributeLayer(LPCTSTR pszName, const cv::Mat& img, BLENDFUNCTION blend);
	BOOL SetAttributeLayer(LPCTSTR pszName, const cv::Mat& img);
	BOOL DeleteAttributeLayer(LPCTSTR pszName = NULL);

	BOOL ShowCrossMark(LPCTSTR pszName = NULL, BOOL bShow = TRUE);
	int AddCrossMark(LPCTSTR pszName, const CPoint2d& pt, LPCTSTR pszLabel = NULL, int iSizeX = 5, int iSizeY = 5, int nPenStyle = PS_SOLID, int iThick = 3, COLORREF cr = RGB(255, 0, 0));
	BOOL DeleteCrossMark(LPCTSTR pszName = NULL);

	BOOL ShowRectRegion(LPCTSTR pszName = NULL, BOOL bShow = TRUE);
	int AddRectRegion(LPCTSTR pszName, const CRect2d& rect, LPCTSTR pszLabel = NULL, int nPenStyle = PS_DOT, int iThick = 3, COLORREF crPen = RGB(255, 255, 255));
	BOOL DeleteRectRegion(LPCTSTR pszName = NULL);

	void ShowAxes(const T_AXES& axes = {.bShow = true}) {
		m_axes = axes;
		Invalidate();
	}

protected:
	BOOL CheckAndGetZoom(double& dZoom);

	eMOUSE_FUNCTION m_eMFLButton, m_eMFMButton, m_eMFRButton, m_eMFWheelUp, m_eMFWheelDown;
	BOOL DoMouseFunction(eMOUSE_FUNCTION eMF, BOOL bDown, UINT nFlags, CPoint point, short zDelta = 0);
	BOOL SelectRegion(CRect2d& rect);

	LRESULT NotifyEvent(eNOTIFY_CODE evtCode, const CRect2d& rect, TRefPointer<CShapeObject>& rObject);
	LRESULT NotifyEvent(eNOTIFY_CODE evtCode, const CRect2d& rect = CRect2d());

public:
	void GetClientRect(LPRECT lpRect) const;
	void ClientToScreen(LPPOINT lpPoint) const;
	void ClientToScreen(LPRECT lpRect) const;
	void ScreenToClient(LPPOINT lpPoint) const;
	void ScreenToClient(LPRECT lpRect) const;
	CCoordTrans GetCT() const { return m_ct; }
protected:
	BOOL UpdateDisplay(double dZoom = -1);
	BOOL UpdateDisplay(const TRefList<CShapeCollection>& collections, cv::Mat& imgView, const CRect& rectClient, double dZoom);
	BOOL UpdateDisplayImage(const cv::Mat& imgOrg, cv::Mat& imgView, const CRect& rectClient, double dZoom);
	BOOL UpdateScrollBars(double dZoom = -1);
	int OnScroll(int eBar, UINT nSBCode, UINT nPos);

protected:

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnPaint();

	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeZoom();
	afx_msg void OnEditchangeZoom();
	afx_msg void OnZoomUp();
	afx_msg void OnZoomDown();
};

#pragma pack(pop)
