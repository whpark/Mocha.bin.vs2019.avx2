#pragma once

#include "AFX_EXT_XMU.h"

// CMatView

#include "Coordinate.hpp"
#include "misc/xUtil.h"
#include "xMathUtil/CoordTrans.h"
#include "xMathUtil/CoordTransEx.h"

#include "2dMatArray.h"

#pragma pack(push, 8)

#if 0
	afx_msg void OnSelRegion___(NMHDR* pNotifyStruct, LRESULT* pResult);

	ON_NOTIFY(CMatView::NC_SEL_REGION, IDC_, OnSelRegion___)

	CMatView::NOTIFY_DATA* pData = (CMatView::NOTIFY_DATA*)pNotifyStruct;
#endif


//=============================================================================
//


class CMatViewPatternMatchingDlg;
class CMatViewSettingDlg;
class CProfileSection;
class AFX_EXT_CLASS_XMU CMatView : public CWnd {
	DECLARE_DYNAMIC(CMatView)
public:
	using this_t = CMatView;
	using base_t = CWnd;

public:
	typedef enum { MF_NONE, MF_DRAG_IMAGE, MF_SELECT_REGION, MF_ZOOM_IN, MF_ZOOM_OUT, MF_SCROLL_UP, MF_SCROLL_DOWN, MF_MEASURE } eMOUSE_FUNCTION;
	enum eNOTIFY_CODE {
		NC_SETFOCUS = NM_LAST-2000U,
		NC_KILLFOCUS,
		NC_SEL_REGION,

		// m_bNotifyAllMouseEvent = true 설정한 다음에... ptWindow, ptImage, ptReal 만 valid.
		NC_MOUSE_MOVE,
		NC_LBUTTON_DOWN,
		NC_LBUTTON_UP,
		NC_RBUTTON_DOWN,
		NC_RBUTTON_UP,
		NC_MBUTTON_DOWN,
		NC_MBUTTON_UP,
		NC_KEY_DOWN,
	};
	struct NOTIFY_DATA {
		NMHDR hdr;
		CRect2d rect;
		CRect2d rectReal;
		cv::Rect rcImage;

		CPoint ptWindow;		// 마우스 Window 좌표
		CPoint2d ptImage;		// 현재 마우스 포지션
		CPoint2d ptReal;		// 현재 마우스 포지션
	};
	typedef enum { BM_TRANSPARENT, BM_ALPHABLEND, } eBLIT_METHOD;
	//void OnSelRegionMatView(NMHDR* pNotifyStruct, LRESULT* pResult);
	//ON_NOTIFY(CMatView::NC_SEL_REGION, IDC_MAT_VIEW, OnSelRegionMatView)

public:
	// Setting
	class CSetting {
	public:
		BOOL bShowTool = true;								// Toolbar 화면 표시

		int eScaleUpMethod = cv::INTER_CUBIC;				// 이미지 확대시 Interpolation 방법 설정
		int eScaleDownMethod = cv::INTER_AREA;				// 이미지 축소시 Interpolation 방법 설정
		BOOL b2ClickSelection = true;						// 이미지 영역 선택시, click 회수 설정. true : 2click, false : one-click
		BOOL bClickToMagnifySelectedRect = true;			// 선택 영역 클릭시 확대.
		BOOL bPatternMatching = true;						// Pattern Matching 창을 사용할지 결정. true : Toolbar 에 아이콘 표시
		BOOL bToolTipOnCursor = true;						// 구현 안됨
		BOOL bClearSelectionOnMouseButton = false;			// 마우스 버튼 클릭시 현재 선택된 영역 지우기
		BOOL bMapWindow = true;								// 전체 이미지중에서 현재 화면에 보이는 영역이 어느만큼인지 보여줌.
		BOOL bStatusOnBottom = false;						// Status Info 를 윈도우 아래에 배치
		double dZoomRate = 1.1;								// Zoom In/Out 비율 선택
		double dMouseSpeed = 2.0;							// 마우스 Drag - 이동시 속도
		int iJPGQuality = 95;								// JPG 이미지 저장시 품실
		BOOL bKeyboardNavigation = false;					// Keyboard Scroll (VK_UP/VK_DONW/VK_LEFT/VK_RIGHT)
		int eAutoScrollMethod = 1;							// 0 : pgdn, 1 : space bottom, 2 : space top
		int tSmoothScroll = 0;								// Smooth Scroll Duration in ms
		BOOL bPutPixelValue = true;							// Put Pixel Value

		void SyncProfile(bool bStore, CProfileSection& section);
		void SyncRegistry(bool bStore, LPCTSTR pszSection, CWinApp& app);
	};
	CString m_strRegistrySection;	// Load/Store Settings from/to ...

	const CSetting& GetSettings() const { return *m_setting; }
	void SetSettings(const CSetting& setting) {
		*m_setting = setting;
		UpdateTool();
		ShowTool(m_setting->bShowTool);
	}

	bool m_bNotifyAllMouseEvent = false;
	CSetting& GetSettings() { return *m_setting; }	// Call this function before creating...

protected:
	std::unique_ptr<CSetting> m_setting;	// stack 에 만드니까 에러 남. MSVC Bug?

protected:
	enum { eIDStart = 1010, /*eIDZoomLable = eIDStart, */
		eIDZoom = eIDStart, eIDZoomUp, eIDZoomDown, eIDZoomReset, eIDSave, eIDPatternMatching, eIDSettings, eIDInfo, /*eIDViewPositionBox, */
		eIDEnd};

protected:
	// Toolbar
	CRect m_rectTool;
	CComboBox m_comboZoom;
	std::array<CMFCButton, 3> m_btnZoom;
	CMFCButton m_btnSave;
	CMFCButton m_btnTool;
	CMFCButton m_btnSetting;
	CEdit m_staticInfo;
	bool m_bDraggingProcessed = true;
	//CMFCToolTipCtrl m_ttInfo;		// .....잘 안되네.....
	//CButton m_btnViewPositionBox;
	std::unique_ptr<CMatViewPatternMatchingDlg> m_pDlgPatternMatching;
	// View
	CCoordTrans m_ctI2S;	// Image to Screen
	std::unique_ptr<ICoordTrans> m_rCTI2M;
	BOOL m_bChannels[3];
	C2dMatArray m_imgOrg;
	//cv::Mat m_imgOrg;
	cv::Mat m_imgView;

	// Attributes
	struct T_ATTRIBUTE {
		CString strName;
		cv::Point ptPatch;				// Patch 위치
		CPoint ptTargetOffset;			// Dispatch 위치. Screen 좌표계
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
		BOOL operator == (LPCTSTR pszName) const { return strName.Compare(pszName) == 0; }
	};
	std::deque<T_ATTRIBUTE> m_imgsAttribute;

	//enum eMOUSE_MODE { MM_NONE, MM_ONECLICK, MM_TWOCLICK };
	struct {
		bool bDragMode{};
		bool bSelectionMode{};	// selectionmode, measure ... 하나의 변수로 만들었어야 했는데...... drag 는 분리해야 함. 동시에 실행할 수 있음
		bool bMeasure{};
		CPoint2d ptSelect0;
		CPoint2d ptSelect1;

		CPoint2d ptImgLast;
		CPoint2i ptScrollLast;
		CPoint2i ptLast;
		CPoint2i ptCurrent;
	} m_mouse;

	template < typename T_ATTRIBUTE >
	class TDisplayAttributeContainer : public std::map<CString, T_ATTRIBUTE> {
		using this_t = TDisplayAttributeContainer;
		using base_t = std::map<CString, T_ATTRIBUTE>;

	public:
		HWND& m_hWnd;
		base_t& base() { return (base_t&)*this; }
		base_t const& base() const { return (base_t const&)*this; }

	public:
		size_t ShowAttribute(LPCTSTR pszName, bool bShow = true) {
			size_t n{};
			if (pszName) {
				base()[pszName].bShow = bShow;
				n = 1;
			}
			else {
				n = base().size();
				for (auto& [key, v] : base()) {
					v.bShow = bShow;
				}
			}
			if (m_hWnd)
				::InvalidateRect(m_hWnd, nullptr, false);
			return n;
		}
		bool AddAttribute(LPCTSTR pszName, T_ATTRIBUTE&& value) {
			if (!pszName)
				return false;
			base()[pszName] = std::move(value);
			if (m_hWnd)
				::InvalidateRect(m_hWnd, nullptr, false);
			return true;
		}
		T_ATTRIBUTE& Get(LPCTSTR pszName) {
			return base()[pszName];
		}
		size_t DeleteAttribute(LPCTSTR pszName = nullptr) {
			size_t n{};
			if (pszName) {
				n = base().erase(pszName);
			}
			else {
				n = base().size();
				base().clear();
			}
			if (m_hWnd)
				::InvalidateRect(m_hWnd, nullptr, false);
			return n;
		}
	};

	struct S_CROSS_MARK {
		bool bShow = false;
		CString strLabel;
		CPoint2d pt;
		CSize2i size;
		int nPenStyle = PS_SOLID;
		int iThick = 1;
		COLORREF cr = RGB(255, 255, 255);
		bool operator == (S_CROSS_MARK const&) const = default;
		bool operator != (S_CROSS_MARK const&) const = default;
	};
	struct S_RECT_REGION {
		bool bShow = false;
		CString strLabel;
		CRect2d rect;
		int nPenStyle = PS_SOLID;
		int iThick = 1;
		COLORREF cr = RGB(255, 255, 255);
		bool operator == (S_RECT_REGION const&) const = default;
		bool operator != (S_RECT_REGION const&) const = default;
	};
	struct S_ELLIPSE {
		bool bShow = false;
		CString strLabel;
		cv::RotatedRect rect;
		int nPenStyle = PS_SOLID;
		int iThick = 1;
		COLORREF cr = RGB(255, 255, 255);
		bool operator == (S_ELLIPSE const&) const = default;
		bool operator != (S_ELLIPSE const&) const = default;
	};
public:
	struct {
		HWND& m_hWnd;
		TDisplayAttributeContainer<S_CROSS_MARK> crosses{.m_hWnd = m_hWnd};
		TDisplayAttributeContainer<S_RECT_REGION> rects{.m_hWnd = m_hWnd};
		TDisplayAttributeContainer<S_ELLIPSE> ellipses{.m_hWnd = m_hWnd};
		S_RECT_REGION rectCurrent;
	} m_display;

protected:
	struct {
		CPoint2d pt0, delta;
		std::chrono::steady_clock::time_point t0, t1;
	} m_smooth_scroll;
	static HCURSOR m_hCursorMag;
	static HCURSOR m_hCursorCross;

public:
	CMatView();   // standard constructor
	virtual ~CMatView();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// operations
public:
	BOOL InitView(double dZoom = -1);
	BOOL SetImage(const cv::Mat& img, double dZoom = -1, BOOL bCopyImage = false);
	BOOL SetImageCenter(const CPoint2d& pt = CPoint2d(-1, -1), double dZoom = -1, bool bDelayedUpdate = false);
	double GetZoom() { double dZoom = -1; CheckAndGetZoom(dZoom); return dZoom; }
	void SetZoom(double dZoom);
	void SetMouseFunction(eMOUSE_FUNCTION eMFLButton = MF_DRAG_IMAGE, eMOUSE_FUNCTION eMFMButton = MF_DRAG_IMAGE, eMOUSE_FUNCTION eMFRButton = MF_SELECT_REGION,
		eMOUSE_FUNCTION eMFWheelUp = MF_SCROLL_UP, eMOUSE_FUNCTION eMFWheelDown = MF_SCROLL_DOWN, eMOUSE_FUNCTION eMFWheelUpCtrl = MF_ZOOM_IN, eMOUSE_FUNCTION eMFWheelDownCtrl = MF_ZOOM_OUT)
	{
		m_eMFLButton = eMFLButton; m_eMFMButton = eMFMButton; m_eMFRButton = eMFRButton; m_eMFWheelUp = eMFWheelUp; m_eMFWheelDown = eMFWheelDown; m_eMFWheelUpCtrl = eMFWheelUpCtrl; m_eMFWheelDownCtrl = eMFWheelDownCtrl;
	}
	void GetMouseFunction(eMOUSE_FUNCTION& eMFLButton, eMOUSE_FUNCTION& eMFMButton, eMOUSE_FUNCTION& eMFRButton,
		eMOUSE_FUNCTION& eMFWheelUp, eMOUSE_FUNCTION& eMFWheelDown, eMOUSE_FUNCTION& eMFWheelUpCtrl, eMOUSE_FUNCTION& eMFWheelDownCtrl) const {
		eMFLButton = m_eMFLButton; eMFMButton = m_eMFMButton; eMFRButton = m_eMFRButton;
		eMFWheelUp = m_eMFWheelUp; eMFWheelDown = m_eMFWheelDown;
		eMFWheelUpCtrl = m_eMFWheelUpCtrl; eMFWheelDownCtrl = m_eMFWheelDownCtrl;
	}
	void ShowTool(BOOL bShow);
	BOOL IsToolVisible() const { return m_setting->bShowTool; }
	void UpdateTool();

	BOOL GetImage(cv::Mat& img) { if (m_imgOrg.empty()) return FALSE; cv::Rect rc(0, 0, m_imgOrg.GetWidth(), m_imgOrg.GetHeight()); m_imgOrg.GetROI(rc).copyTo(img); return TRUE; }
	BOOL GetImageView(cv::Mat& img) const { if (m_imgView.empty()) return FALSE; m_imgView.copyTo(img); return TRUE; }
	cv::Mat GetImage() { cv::Mat img; GetImage(img); return std::move(img); }
	BOOL GetCurrentImageRect(CRect2d& rect) const;
	BOOL SetCurrentImageRect(const CRect2d& rect);

	BOOL StartSelectionMode(const CPoint2d& ptImage, bool bMeasure = false);
	BOOL EndSelectionMode();
	BOOL GetSelectedImageRect(CRect2d& rect) const;
	BOOL SetSelectedImageRect(const CRect2d& rect, bool bSelect = true);

	//const cv::Mat& GetImage() const { return m_imgOrg; }
	C2dMatArray& GetMatImage() { return m_imgOrg; }
	const C2dMatArray& GetMatImage() const { return m_imgOrg; }
	const cv::Mat& GetImageView() const { return m_imgView; }

	BOOL AddAttributeLayer(LPCTSTR pszName, const cv::Mat& img, COLORREF crTransparent);
	BOOL AddAttributeLayer(LPCTSTR pszName, const cv::Mat& img, BLENDFUNCTION blend);
	BOOL AddAttributeLayer(LPCTSTR pszName, const cv::Point& ptLT, const cv::Mat& img, COLORREF crTransparent);
	BOOL AddAttributeLayer(LPCTSTR pszName, const cv::Point& ptLT, const cv::Mat& img, BLENDFUNCTION blend);
	BOOL SetAttributeLayer(LPCTSTR pszName, const cv::Mat& img);
	BOOL DeleteAttributeLayer(LPCTSTR pszName = NULL);

	BOOL ShowCrossMark(LPCTSTR pszName = NULL, BOOL bShow = TRUE);
	int AddCrossMark(LPCTSTR pszName, const CPoint2d& pt, LPCTSTR pszLabel = NULL, int iSizeX = 5, int iSizeY = 5, int nPenStyle = PS_SOLID, int iThick = 3, COLORREF cr = RGB(255, 0, 0));
	BOOL DeleteCrossMark(LPCTSTR pszName = NULL);

	BOOL ShowRectRegion(LPCTSTR pszName = NULL, BOOL bShow = TRUE);
	BOOL AddRectRegion(LPCTSTR pszName, const CRect2d& rect, LPCTSTR pszLabel = NULL, int nPenStyle = PS_DOT, int iThick = 3, COLORREF crPen = RGB(255, 255, 255));
	BOOL DeleteRectRegion(LPCTSTR pszName = NULL);

protected:
	BOOL CheckAndGetZoom(double& dZoom);

	eMOUSE_FUNCTION m_eMFLButton{}, m_eMFMButton{}, m_eMFRButton{}, m_eMFWheelUp{}, m_eMFWheelDown{}, m_eMFWheelUpCtrl{}, m_eMFWheelDownCtrl{};
	BOOL DoMouseFunction(eMOUSE_FUNCTION eMF, BOOL bDown, UINT nFlags, CPoint point, short zDelta = 0);
	BOOL SelectRegion(CRect2d& rect);

	LRESULT NotifyEvent(eNOTIFY_CODE evtCode, const CRect2d& rect = CRect2d(), bool bImageRect = false);
	LRESULT NotifyMouseEvent(eNOTIFY_CODE evtCode, CPoint pt);

public:
	void GetClientRect(LPRECT lpRect) const;
	CCoordTrans GetCT() const { return m_ctI2S; }
	void SetCT_I2M(std::unique_ptr<ICoordTrans> rCTI2M) { m_rCTI2M = std::move(rCTI2M); }
	void SetCT_I2M(const CCoordTrans& ctI2M) { m_rCTI2M.reset(new CCoordTrans(ctI2M)); }
	ICoordTrans* GetCT_I2M() { return m_rCTI2M.get(); }
	BOOL UpdateMousePosition(const CPoint& pt);
	BOOL UpdateTrackPosition(LPCRECT lpRect);
	BOOL UpdateToolTip(LPCTSTR psz);
	int GetScaleDownMethod() const { return m_imgOrg.GetScaleDownMethod(); }
	int GetScaleUpMethod() const { return m_imgOrg.GetScaleUpMethod(); }
	void SetResizingMethod(int eScaleDownMethod = cv::INTER_LINEAR, int eScaleUpMethod = cv::INTER_LINEAR, bool bThumbnailInBkgnd = false);

protected:
	bool DrawGrid(CDC& dc, CRect const& rectView);
	bool DrawAttributes(CDC& dc, CRect const& rectView);
	bool DrawMiniMap(CDC& dc, CRect const& rectView);
	bool DrawSelectedRect(CDC& dc, CRect const& rectView);
	BOOL UpdateDisplayImage(double dZoom = -1, bool bDelayedUpdate = false);
	BOOL PrepareDisplayImage(const cv::Mat& imgOrg, cv::Mat& imgView, const cv::Point& ptPatch, CPoint& ptOffset, const CRect& rectClient, double dZoom = -1);
	BOOL PrepareDisplayImage(C2dMatArray& imgSet, cv::Mat& imgView, const CRect& rectClient, double dZoom = -1);
	BOOL UpdateScrollBars(double dZoom = -1);
	int OnScroll(int eBar, UINT nSBCode, UINT nPos);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	CPoint2d m_ptImageCenter;
	bool m_bFocusedBefore = false;
	enum eTIMER { T_UPDATE_DISPLAY_IMAGE = 1045, T_CHECK_FOCUS, T_SMOOTH_SCROLL };
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//afx_msg void OnNcPaint();
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
	afx_msg void OnZoomReset();
	afx_msg void OnSave();
	afx_msg void OnPatternMatching();
	afx_msg void OnSettings();
	//afx_msg void OnSelchangeScaleDownMethod();
	//afx_msg void OnSelchangeScaleUpMethod();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnUpdateDisplayImage(WPARAM wParam, LPARAM lParam);
};


#pragma pack(pop)
