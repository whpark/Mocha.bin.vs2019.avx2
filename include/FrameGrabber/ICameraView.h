
// CameraView.h : interface of the CCameraView class
//

#pragma once

#include "AFX_EXT_FRAME_GRABBER.h"

//class IStageInterface;
class CProfile;
class CIPClient;
class CFrameGrabber;

#include "MIP/MIP.h"
#include "misc/SimpleLog.h"
#include "misc/Profile.h"
#include "Coordinate.hpp"
#include "xMathUtil/CoordTrans.h"
#include "FrameGrabber/FrameGrabber.h"
#include "Shape/Shape.h"

#define F_MACHINE_X			 "MX"
#define F_MACHINE_Y			 "MY"
#define F_IMAGE_X			 "IX"
#define F_IMAGE_Y			 "IY"
#define F_LASER_MACHINE_X	"LMX"
#define F_LASER_MACHINE_Y	"LMY"
#define F_LASER_IMAGE_X		"LIX"
#define F_LASER_IMAGE_Y		"LIY"

// CCameraView window
class AFX_EXT_CLASS_FRAME_GRABBER ICameraView : public CWnd, public CSimpleLogInterface, public IWNDIPCommandTarget {
public:
	DECLARE_DYNAMIC_CREATE_BASE_INTERFACE(ICameraView, LPCTSTR, CString)

	ICameraView(CIPClient* pIPClient = NULL);
	virtual ~ICameraView() { }

protected:
	TRefPointer<CFrameGrabber> m_rFG;

public:
	virtual BOOL InitCamera(LPCSTR pszUnitName, LPCTSTR pszCameraType, const CProfileSection& settings) = NULL;
	virtual BOOL InitCamera(LPCSTR pszUnitName, TRefPointer<CFrameGrabber>& rFG) = NULL;
	virtual CFrameGrabber* GetFrameGrabber() { return m_rFG; }
	virtual const CFrameGrabber* GetFrameGrabber() const { return m_rFG; }
protected:
	virtual BOOL LoadState(BOOL bReload = FALSE) = NULL;
	virtual BOOL SaveState() = NULL;
	virtual BOOL SyncState(BOOL bStore, CProfileSection& section) = NULL;

public:
	virtual BOOL GetImage(cv::Mat& img, bool bFailIfTimedout = true, DWORD dwWaitForNewImage = 3'000) = NULL;
	virtual BOOL GetNewImage(cv::Mat& img, DWORD dwWaitForNewImage = 3'000) = NULL;

	virtual BOOL SaveImage(LPCTSTR pszFileName, LPCTSTR pszText = NULL, const CPoint2d& ptText = CPoint2d(0, 0), int iFont = cv::FONT_HERSHEY_PLAIN, double dFontScale = 10., int iQuality = 95, CString* pstrPathSaved = nullptr) = NULL;
	virtual BOOL SaveImage(const cv::Rect& rect, LPCTSTR pszFileName, LPCTSTR pszText = NULL, const CPoint2d& ptText = CPoint2d(0, 0), int iFont = cv::FONT_HERSHEY_PLAIN, double dFontScale = 10., int iQuality = 95, CString* pstrPathSaved = nullptr) = NULL;
	virtual BOOL SaveImage(const cv::Mat& imgBGR, LPCTSTR pszFileName, LPCTSTR pszText = NULL, const CPoint2d& ptText = CPoint2d(0, 0), int iFont = cv::FONT_HERSHEY_PLAIN, double dFontScale = 10., int iQuality = 95, CString* pstrPathSaved = nullptr) = NULL;

	virtual BOOL StartVideoCapture(LPCTSTR pszFileName, CString& strPathSaved, int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), BOOL bUseScreenSize = FALSE, double dScale = 1.0) = NULL;
	virtual BOOL StopVideoCapture() = NULL;
	virtual BOOL GetVideoCapturedFrames(int &nFrames) const = NULL;

	virtual void RedrawImage(BOOL bForceIfPaused = TRUE) {}

public:
	virtual BOOL CheckUILockPassword() = NULL;

public:
	typedef enum {
		CS_NONE,
		CS_IMAGE,
		CS_CAMERA = CS_IMAGE,
		CS_MACHINE,
		CS_SCREEN,

		nCS,
	} eCOORD_SYSTEM ;
	typedef enum {
		TLA_TRANSPRENT = BIT(0),
		TLA_TA_H_L = BIT(10),
		TLA_TA_H_R = BIT(11),
		TLA_TA_H_C = BIT(12),
		TLA_TA_V_T = BIT(13),
		TLA_TA_V_B = BIT(14),
		TLA_TA_V_C = BIT(15),
	} eTEXT_LABEL_ATTRIBUTE;
	struct T_LABEL {
		wchar_t szName[32];
		wchar_t szText[128];
		eCOORD_SYSTEM eCS;
		CRect2d rect;
		COLORREF crText, crBkgnd;
		DWORD dwFlag;	// eTEXT_LABEL_ATTRIBUTE
		LOGFONT logFont;
		T_LABEL() : crText(RGB(0, 0, 0)), crBkgnd(RGB(255, 255, 255)), dwFlag(0) {
			ZeroVar(szName);
			ZeroVar(szText);
			eCS = CS_IMAGE;
			static CFont fontDefault;
			if (!fontDefault.m_hObject) {
				fontDefault.CreatePointFont(200, _T("Segoe UI"));
			}
			ZeroVar(logFont);
			fontDefault.GetLogFont(&logFont);
		}
	};

	struct T_OVERLAY {
		CString strName;
		//mutable CCriticalSection csOverlay;
		cv::Mat imgOverlay;
		cv::Mat imgOverlayMask;

		mutable struct {
			cv::Mat imgOverlay;
			cv::Mat imgOverlayMask;
			CBitmap bmpMask;
			cv::Size sizeView;
			cv::Rect rectEffective;
			cv::Size sizeCamera;
		} prep;

		DWORD dwTickStart;
		DWORD dwDuration;

		T_OVERLAY(LPCTSTR pszName = _T("Default"), cv::Mat const& imgOverlay = cv::Mat{}, cv::Mat const& imgOverlayMask = cv::Mat{}, DWORD dwTickDuration = 0);
		BOOL operator == (LPCTSTR pszName) const { return strName == pszName; }

		BOOL SetImage(cv::Mat const& imgOverlay, cv::Mat const& imgOverlayMask, DWORD dwTickDuration);
		BOOL OverlayImage(cv::Mat& imgBGR) const;
		BOOL OverlayImage(CDC& dc, const cv::Size& sizeView, const CRect& rectClient, const cv::Size& sizeCamera, const cv::Rect& rectEffective) const;

		void ClearPrepared() const {
			prep.imgOverlay.release();
			prep.imgOverlayMask.release();
			if (prep.bmpMask.m_hObject)
				prep.bmpMask.DeleteObject();
		}

	};

	struct T_SIMPLE_SHAPE {
		CString strName;
		TRefPointer<CShapeObject> rObject;
		double dLineThickness;
		eCOORD_SYSTEM eCS;
		DWORD dwTickStart;
		DWORD dwDuration;

		T_SIMPLE_SHAPE(LPCTSTR pszName, TRefPointer<CShapeObject> rObject, double dLineThickness = 1.0, eCOORD_SYSTEM eCS = CS_IMAGE, DWORD dwDuration = INFINITE) {
			this->strName = pszName;
			this->dwTickStart = GetTickCount();
			this->dwDuration = dwDuration;
			this->rObject = rObject;
			this->dLineThickness = dLineThickness;
			this->eCS = eCS;
		}

		BOOL operator == (LPCTSTR pszName) const { return strName == pszName; }
	};

	virtual BOOL ShowToolMenu(BOOL bShow = TRUE) = NULL;
	virtual BOOL IsToolMenuVisible() const = NULL; 
	virtual BOOL GetZoomedSize(const cv::Size& sizeReal, cv::Rect& rectEffective, cv::Size& sizeImage, cv::Size& sizeView) const = NULL;

	virtual BOOL SetCTCameraToStage(const CCoordTrans& ct, int iLensNo = -1) = NULL;

	virtual BOOL ShowCross(BOOL bShow, const CPoint2d& ptStage, COLORREF crColor, DWORD dwDuration = 10*1000, UINT fPenStyle = PS_SOLID) = NULL;
	virtual BOOL ShowText(BOOL bShow, const CPoint2d& ptStage = CPoint2d(), LPCTSTR pszText = NULL, COLORREF cr = RGB(0, 0, 0), LOGFONT* pLogFont = NULL, DWORD dwDuration = 10*1000) = NULL;
	virtual BOOL ShowRectangle(BOOL bShow, const CRect2d& ptStage, COLORREF crColor, const int& iThickness, DWORD dwDuration = 10*1000) = NULL;
	virtual BOOL ShowCircle(BOOL bShow, const CRect2d& ptStage, COLORREF crColor, const int& iThickness, DWORD dwDuration = 10*1000) = NULL;
	virtual BOOL AddLabel(const T_LABEL& label) = NULL;
	virtual BOOL DeleteLabel(LPCTSTR pszLabelName) = NULL;
	virtual BOOL AddOverlay(LPCTSTR pszName, cv::Mat imgOverlay, cv::Mat imgOverlayMask, DWORD dwDuration) = NULL;
	virtual BOOL DeleteOverlay(LPCTSTR pszName) = NULL;

	virtual BOOL AddSimpleShape(TRefPointer<T_SIMPLE_SHAPE> rSimpleShape) = NULL;
	virtual BOOL DeleteSimpleShape(LPCTSTR pszName = NULL) = NULL;
	virtual BOOL AddSimpleShapeRect(LPCTSTR pszName, const RECT2D& rect, COLORREF cr = RGB(255, 255, 255), double dLineThickness = 1.0, eCOORD_SYSTEM eCS = CS_IMAGE, DWORD dwDuration = INFINITE) = NULL;
	virtual BOOL AddSimpleShapeCircle(LPCTSTR pszName, const POINT2D& ptCenter, double dRadius, COLORREF cr = RGB(255, 255, 255), double dLineThickness = 1.0, eCOORD_SYSTEM eCS = CS_IMAGE, DWORD dwDuration = INFINITE) = NULL;

	virtual CPoint2d ConvCameraToStage(const CPoint2d& ptCamera, int iLensNo = -1) = NULL;
	virtual CPoint2d ConvStageToCamera(const CPoint2d& ptStage, int iLensNo = -1) = NULL;
	virtual CCoordTrans GetCTCameraToStage(int iLensNo = -1) = NULL;

public:
	virtual BOOL StartMoveDisplayRegion(CPoint point) = NULL;
	virtual BOOL EndMoveDisplayRegion() = NULL;
	virtual BOOL UpdateMenuBar() = NULL;
	virtual BOOL DetachView() = NULL;
	virtual BOOL AttachView() = NULL;

protected:
	//static OSVERSIONINFO s_versionOS;
	static const bool s_bWindows7OrGreater;
	//static DWORD GetOSVersionMajor();
	//static DWORD GetOSVersionMinor();

	DECLARE_IP_AND_INTERFACE()


/*
	------------------------------------------------------------------------------
	// Image Selection/Save
	//
	ON_IP("Image", "Grab", OnIPImageGrab) : 이미지 캡춰.
		var : none.
		varResult :
			"Image" : cv::Mat.


	ON_IP_EXCL("Image", "SelectRegion", ) : 화면에서 마우스로 영역을 선택하도록 함. 도중에 'ESC' 키를 누를 경우 FALSE가 리턴됨. (선택된 영역의 이미지도 받아올 수 있음)
		var : 
			"TwoClick" : BOOL,	TRUE(default) : 첫번째 클릭 -> 시작, 두번째 클릭 -> 끝,
								FALSE : 클릭 & Drag 로 영역 선택
			"HideAttr" : BOOL,	TRUE : 선택 영역 보여주지 않음.
								FALSE(default) : 선택 영역 보여줌

			"RubberBand" : BOOL(FALSE), TRUE : 선택한 영역의 크기/위치를 변경할 수 있게 함.
			"MessageBox" : BOOL(FALSE), TRUE : 메세지 박스 출력
		varResult :
			"MX", "MY" : double (Stage 좌표계), 선택된 영역의 중앙 좌표
			"nPtsMachine" : int, 좌표 갯수.
			"MX0", "MY0" ~ "MX1", "MY1" : double, 선택된 영역

			"IX", "IY" : double (이미지 좌표계), 선택된 영역의 중앙 좌표
			"nPtsImage" : int, 좌표 갯수
			"IX0", "IY0" ~ "IX1", "IY1" : double, (이미지 좌표계)선택된 영역

			"nPtsLaserImage" : int, 좌표 갯수 (항상 1)
			"LIX", "LIY" : double (이미지 좌표계. Laser Center Offset 기준)

			"matImage" : 선택된 영역 이미지. GetChildItemMat() 함수로 이미지를 가져올 수 있음.

	ON_IP("Image", "Load", OnIPImageLoad)
		var :
			"FileName" : string, 없을 경우, CFileDialog 를 사용하여 사용자에게 받아옴.

	ON_IP("Image", "Save", OnIPImageSave)
		var :
			"FileName" : string, 없을 경우, CFileDialog 를 사용하여 사용자에게 받아옴. ( misc.lib 에 있는 GetUniqueFileName() 함수를 사용함 ).
						파일 이름에 '?' 가 있을 경우, 번호로 바꿔서 저장함. ex, "Test???.bmp" --> "Test000.bmp", "Test001.bmp"... 
			"Text" : string, 출력할 텍스트.
			"TextX", "TextY" : double (0.0, 0.0) - Text를 출력할 위치
			"FontFace" : int(cv::FONT_HERSHEY_PLAIN) - Font, BIT(4) : Italic
			    FONT_HERSHEY_SIMPLEX = 0,
				FONT_HERSHEY_PLAIN = 1,
				FONT_HERSHEY_DUPLEX = 2,
				FONT_HERSHEY_COMPLEX = 3,
				FONT_HERSHEY_TRIPLEX = 4,
				FONT_HERSHEY_COMPLEX_SMALL = 5,
				FONT_HERSHEY_SCRIPT_SIMPLEX = 6,
				FONT_HERSHEY_SCRIPT_COMPLEX = 7,
			"FontScale" : double(10.0) 텍스트 크기
			"Quality" : int(95), jpg 저장 품질(~100)
			"MessageBox" : BOOL(FALSE), TRUE 일 경우, 이미지 저장 후, 메세지 박스 출력.

		varResult :
			"PathSaved" : string, 실제 저장된 경로.

	ON_IP("Image", "SaveHardcopy", OnIPImageSaveHardcopy) - ImageSave 와 동일하나, 화면에 출력되는 그대로를 저장 함.
		var :
			"FileName" : string, 없을 경우, CFileDialog 를 사용하여 사용자에게 받아옴. ( misc.lib 에 있는 GetUniqueFileName() 함수를 사용함 )
						파일 이름에 '?' 가 있을 경우, 번호로 바꿔서 저장함. ex, "Test???.bmp" --> "Test000.bmp", "Test001.bmp"... 
			"Text" : string, 출력할 텍스트.
			"TextX", "TextY" : double (0.0, 0.0) - Text를 출력할 위치
			"FontFace" : int(cv::FONT_HERSHEY_PLAIN) - Font, BIT(4) : Italic
			    FONT_HERSHEY_SIMPLEX = 0,
				FONT_HERSHEY_PLAIN = 1,
				FONT_HERSHEY_DUPLEX = 2,
				FONT_HERSHEY_COMPLEX = 3,
				FONT_HERSHEY_TRIPLEX = 4,
				FONT_HERSHEY_COMPLEX_SMALL = 5,
				FONT_HERSHEY_SCRIPT_SIMPLEX = 6,
				FONT_HERSHEY_SCRIPT_COMPLEX = 7,
			"FontScale" : double(10.0) 텍스트 크기
			"Quality" : int(95), jpg 저장 품질(~100)
			"MessageBox" : BOOL(FALSE), TRUE 일 경우, 이미지 저장 후, 메세지 박스 출력.

		varResult :
			"PathSaved" : string, 실제 저장된 경로.

	ON_IP("Image", "SaveRegion", OnIPImageSaveRegion) : 사용자가 화면에서 영역을 선택하게 하고, 선택된 영역을 저장함.
		var :
			iphandler ("Image", "SelectRegion") 에 사용하는 항목 + iphandler("Image", "Save") 에 사용하는 항목.

		varResult :
			"PathSaved" : string, 실제 저장된 경로.

	ON_IP("Image", "StartVideoCapture", OnIPImageStartVideoCapture) : 동영상 저장.
		var :
			"FileName" : string, 없을 경우, CFileDialog 를 사용하여 사용자에게 받아옴. ( misc.lib 에 있는 GetUniqueFileName() 함수를 사용함 ).
						파일 이름에 '?' 가 있을 경우, 번호로 바꿔서 저장함. ex, "Test???.bmp" --> "Test000.bmp", "Test001.bmp"... 
			"FourCC" : String, 동영상 압축 코덱. 기본 "MJPG" 만 가능.
						(Codec Pack 을 설치하여 DIV4, DIV5, PIM1(MPEG1), PIM2(MPEG2), .. 등을 사용할 수 있음. 인터넷 검색...)
			"UseScreenSize" : BOOL(FALSE) -	TRUE 일 경우, 화면출력된 영상을 저장(이 때, 윈도우 크기를 변경시키면 저장 안됨.)
											FALSE 일 경우, Camera 에서 들어온 이미지를 저장.
			"Scale" : double(1.0) 화면 배율
			"Length" : unsigned int, 동영상 저장할 길이 (in msec)

		varResult :
			"PathSaved" : string, 실제 저장할 경로.


	------------------------------------------------------------------------------
	// Image
	//
	ON_IP_EXCL("Calibrate", "ScreenToMachine", OnIPCalibrateScreenToMachine) :
		Screen <-> Stage 좌표 Calibration UI를 화면에 띄워줌.

	ON_IP("Measure", "ShowCameraDelayDlg")	: Camera Delay 측정 창을 화면에 띄움.
		
	ON_IP_EXCL("Calibrate", "LaserCenterOffset", OnIPCalibrateLaserCenterOffset) :
		Laser Center Offset 영역을 선택하는 UI 시작.
		
	ON_IP_EXCL("Calibrate", "ResetLaserCenterOffset", OnIPCalibrateResetLaserCenterOffset) :
		Laser Center Offset 초기화

	ON_IP_EXCL("Calibrate", "GetLaserCenterOffset", OnIPCalibrateGetLaserCenterOffset) :
		Laser Center Offset 값을 받아옴.
		varResult :
			"Offset" : CPoint2d, 현재 Laser Center Offset 값.
			"Offset0" : 0번 Lens의 Laser Center Offset 값.
			...
			"Offset9" : 9번 Lens의 Laser Center Offset 값.

	ON_IP_EXCL("Calibrate", "SetLaserCenterOffset", OnIPCalibrateGetLaserCenterOffset) :
		Laser Center Offset 값을 세팅.
		var :
			"LensNo" : int (-1). (Optional) LensNo.
			"Offset" : CPoint2d, Laser Center Offset 값.

	------------------------------------------------------------------------------
	// 좌표값 변환
	// M : Machine, C : Camera Image, S : Screen
	ON_IP("Conv", "M2C", OnIPConvM2C)
	ON_IP("Conv", "C2M", OnIPConvC2M)
	ON_IP("Conv", "M2S", OnIPConvM2S)
	ON_IP("Conv", "S2M", OnIPConvS2M)
	ON_IP("Conv", "C2S", OnIPConvC2S)
	ON_IP("Conv", "S2C", OnIPConvS2C)
		var :
			"LensNo" : int, 계산할 Lens 번호.
			"X" : double, X 값
			"Y" : duoble, Y 값
		varResult :
			"X" : double, 변환 된 X 값
			"Y" : double, 변환 된 Y 값

	ON_IP("Conv", "GetCT", OnIPConvGetCT)
		var :
			"LensNo" : int, 계산할 Lens 번호.
		varResult :
			"C2M"
			"M2C"
			"S2M"
			"M2S"
			"C2S"
			"S2C" : CCoordTrans

	ON_IP("Conv", "SetCT", OnIPConvSetCT) : Camera to Stage 좌표 변환 세팅
		var :
			"LensNo" : int(-1), Lens 번호.
			"C2M" : CCoordTrans, (Camera 좌표계를 Stage 좌표계로 변환)

	ON_IP("Conv", "GetLaserOffset", OnIPConvGetLaserOffset)
		var :
			"MX" : double, X 값
			"MY" : double, Y 값
		varResult :
			"IX" : double, Image 상의 X 값.
			"IY" : double, Image 상의 Y 값.

	// M : Machine, P : Pulse
	ON_IP("Conv", "SlitP2M", OnIPConvSlitP2M) : Pulse 값을 Machine 좌표계로 변환.
		var :
			"PulseX", "PulseY" : double, Pulse X/Y 값
		varResult :
			"Width", "Height" : double, 가로/세로 (Stage 좌표계)

	ON_IP("Conv", "SlitM2P", OnIPConvSlitM2P) : Machine 단위 값을 Pulse 로 변환.
		var :
			"Width", "Height" : double, 가로/세로 (Stage 좌표계)
		varResult :
			"PulseX", "PulseY" : double, Pulse X/Y 값


	------------------------------------------------------------------------------
	// Camera
	//
	ON_IP("Camera", "Setting", OnIPCameraSetting) : 카메라 설정 윈도우를 화면에 띄움
		var :
			"Command" : CStringA - Camera Command.
		varResult :
			"Result" : (Binary) 리턴 값


	------------------------------------------------------------------------------
	// View
	//
	ON_IP("View", "ToolMenu", OnIPViewToolMenu) : View Tool Menu.
		var :
			"View" : BOOL(TRUE)

	ON_IP("View", "Activate", OnIPViewActivate) :
		var :
			"Activate" : BOOL(TRUE), TRUE 일 경우 Show Camera Window, & Focus 할당. 화면에 표시. FALSE 일 경우 해제.

	ON_IP("View", "DisplaySubRegion", OnIPViewDisplayRegion) : 화면 확대시 전체 영역중 어느 영역이 Display 되는지 화면에 Display Region 표시.
		var :
			"View" : BOOL(TRUE)

	ON_IP("View", "CrossMark", OnIPViewCrossMark) : + 마크 (Laser Center Offset)  화면에 출력.
		var :
			"View" : BOOL(현재 상태)
			"Color" : uint32_t(현재 색상)	RGB Color.
			"Width" : uint32_t(현재 크기 x) Width
			"Height" : uint32_t(현재 크기 y) Height


	ON_IP("View", "Grid", OnIPViewGrid) : N/A

	ON_IP("View", "Slit", OnIPViewSlit) : Slit Region 을 화면에 출력.
		var :
			"View" : BOOL(TRUE)

	ON_IP("View", "FocusValue", OnIPViewFocusValue) : Focus Value
		var :
			"View" : BOOL(TRUE)

	ON_IP("View", "SelectedRegionSize", OnIPViewSelectedRegionSize) : 마우스로 선택한 영역의 크기를 화면에도 출력
		var :
			"View" : BOOL(TRUE)

	ON_IP("View", "Measure", OnIPViewMeasure) : 마우스로 선택한 길이를 화면에 출력. 
		var :
			"View" : BOOL(TRUE)

	ON_IP("View", "BoostImage", OnIPViewBoostImage) : 
		var :
			"View" : BOOL(TRUE)

	ON_IP("View", "Zoom", OnIPViewZoom) : Zoom 선택
		var :
			"Zoom" : int(Z_FIT_TO_SCREEEN)

	ON_IP("View", "ShowText", OnIPViewShowText) : 화면에 text 출력
		var :
			"View" : BOOL(TRUE) On/Off 
			"Color" : COLORREF, Text Color
			"MX", "MY" : Text 출력 위치 (Text Align : H-Center, V-Center)
			"Font" : LOGFONT logFont. (var.SetChildItemUserType 으로 세팅하면 됨)
			"Text" : String.
			"Duration" : 화면에 떠 있는 시간. 0 일 경우, 마우스 클릭시까지.

	ON_IP("View", "Popup", OnIPViewPopup) : 윈도우창을 메인윈도우에서 분리 (토글)
		var :
			"Popup" : BOOL(TRUE) TRUE : 분리, FALSE : 다시 메인 윈도우에 붙임

	ON_IP("View", "Circle", OnIPViewCircle) : 화면에 Circle을 그림
		var :
			"View" : BOOL(TRUE), On/Off
			"Color" : COLORREF, Color
			"Left", "Right", "Top", "Bottom" : double, Machine 좌표(Rect)
			"Thickness" : int, 그리는 선의 두께
			"Duration" : 화면에 떠 있는 시간, 0 또는 INFINITE 일 경우, 마우스 클릭시까지.

	================ 사용 하지 마세요... ("Draw", "Rectangle") 사용 하세요 ================ ON_IP("View", "Rectangle", OnIPViewRectangle) : 화면에 Rectangle을 그림
		var :
			"View" : BOOL(TRUE), On/Off
			"Color" : COLORREF, Color
			"Left", "Right", "Top", "Bottom" : double, Machine 좌표(Rect)
			"Thickness" : int, 그리는 선의 두께
			"Duration" : 화면에 떠 있는 시간, 0 또는 INFINITE 일 경우, 마우스 클릭시까지.

			
			
			
			
			
	ON_IP("View", "SetUILockPassword", OnIPViewUILockPassword) : UI Lock Password (Camera Calibration 화면만... 저장할 때 Password 확인 함)
		var :
			"Password" : String.
	ON_IP("View", "GetUILockPassword", OnIPViewUILockPassword) :
		varResult :
			"Password" : String.


*/

/* Draw Function

	ON_IP("Draw", "Init", OnIPDrawInit) : Draw 함수 초기화

	ON_IP("Draw", "Start", OnIPDrawStart) : CShapeObject 시작.
		var :
			"Mode" : int(1) Draw Mode. 1 : 하나만 그림, 2 : 여러개 그리기
			"Shape" : int(CShapeObject::S_LINE). CShapeObject::S_LINE, CShapeObject::S_POLY_LINE 두개만 지원.
			"LaserThickness" : double(0.030) : Laser 두께를 점선으로 그림. (0일 경우 그리지 않음)
			"Color" : uint32_t(RGB(255, 0, 0)) : Object 색상.

	ON_IP("Draw", "GetObject", OnIPDrawGetObject) : CShapeObject 가지고 옴.
		var : 없음
		varResult :
			"CShapeGroup" - 자세한 내용은 src/Vision/DrawTestDlg.cpp - CDrawTestDlg::OnGetObject() 함수 참조.

	ON_IP("Draw", "Overlay", OnIPDrawOverlay) //20150730 JSK : Show Overlay Image on display.
		var :
			"Draw" (or "Overlay") : BOOL (TRUE), On/Off,
			"Name" : String("Default"), 이름.
			"OverlayImage" : Mat, CV_8UC3, Display Image 와 동일한 사이즈
			"OverlayImageMask" : Mat, CV_8UC1 Mask, Display Image 와 동일한 사이즈
			"Duration" : DWORD(0), 지속시간(in msec). 0일 경우 계속 표시.

			예제, 설명 : (JSK)
				Mat imgOverlay = Mat(imgOrg.size(), CV_8UC3, cv::Scalar(0,0,0));
				Mat imgOverlayMask = Mat(imgOrg.size(), CV_8UC1, cv::Scalar(0));

				// 오버레이로 표시될 부분 설정
				cv::circle(imgOverlay, Point(imgOverlay.cols / 2, imgOverlay.rows / 2), 100, cv::Scalar(0,255,0), 3);
				putText(imgOverlay,(LPCSTR)FormatA("TEST1") , Point(50, 200), 0, 5, cv::Scalar(0,0,255), 5);
				putText(imgOverlay,(LPCSTR)FormatA("TEST2") , Point(50, 400), 0, 5, cv::Scalar(255,0,0), 5);
	
				// 오버레이로 표시될 부분의 마스크 영역
				for(int y=0; y<imgOverlay.rows; y++){
					for(int x=0; x<imgOverlay.cols; x++){
						if(imgOverlay.at<cv::Vec3b>(y,x)[0] !=0 || imgOverlay.at<cv::Vec3b>(y,x)[1] !=0 || imgOverlay.at<cv::Vec3b>(y,x)[2] !=0){
							imgOverlayMask.at<UCHAR>(y,x) = 255;
						}
					}
				} 

				CIPVariable varFP, varResultFP;
				varFP.SetChildItemMat("OverlayImage", imgOverlay);
				varFP.SetChildItemMat("OverlayImageMask", imgOverlayMask);
				varFP.SetChildItem("Draw", 1);
				varFP.SetChildItem("Name", _T("Name"));
				if (!SendIPCommand("Vision", NULL, "Draw", "DrawOverlay", varFP, varResultFP))
				{

				}

	ON_IP("Draw", "Label", OnIPDrawLabel) : 화면에 text 출력. (Font, TextColor, BkgndColor, ... ICameraView::T_LABEL 참조)
		var :
			"Label" : ICameraView::T_LABEL label,
					  label.szText 가 "" (empty) 일 경우, 삭제.
					  label.szName 이 "" (empty) 일 경우, 전체 Label 삭제.

		ex.)
			ICameraView::T_LABEL label;
			wcscpy(label.szName, L"name");
			wcscpy(label.szText, L"CameraName");
			CIPVar var;
			var.SetChildItemUserType("Label", &label);
			PostIPCommand("Vision", NULL, "View", "Label", var);

	ON_IP("Draw", "Rectangle", OnIPViewRectangle) : 화면에 Rectangle을 그림 (테두리만)
		var :
			"Draw" : BOOL (TRUE), On/Off,
			"Name" : String("Default"), 이름.
			"CoordSystem" : eCOORD_SYSTEM (ICameraView 참조), 0 : Image 좌표, 1 : Machine 좌표, 2 : Screen 좌표
			"Color" : COLORREF, Color
			"Thickness" : double, 그리는 선의 두께 (in pixel size)
			"Duration" : 화면에 떠 있는 시간, 0 일 경우, 마우스 클릭시까지.

			"Rect" : CRect2d - rectangle 좌표(Rect)

	ON_IP("Draw", "Circle", OnIPViewRectangle) : 화면에 Circle 을 그림 (테두리만)
		var :
			"Draw" : BOOL (TRUE), On/Off,
			"Name" : String("Default"), 이름.
			"CoordSystem" : eCOORD_SYSTEM (ICameraView 참조), 0 : Image 좌표, 1 : Machine 좌표, 2 : Screen 좌표
			"Color" : COLORREF, Color
			"Thickness" : double, 그리는 선의 두께 (in pixel size)
			"Duration" : 화면에 떠 있는 시간, 0 일 경우, 마우스 클릭시까지.

			"Center" : CPoint2d - 중심 좌표
			"Radius" : double(0.0) - 반지름

	ON_IP("Draw", "Cross", OnIPViewRectangle) : 화면에 Cross 을 그림 (테두리만)
		var :
			"Draw" : BOOL (TRUE), On/Off,
			"Name" : String("Default"), 이름.
			"CoordSystem" : eCOORD_SYSTEM (ICameraView 참조), 0 : Image 좌표, 1 : Machine 좌표, 2 : Screen 좌표
			"Color" : COLORREF, Color
			"Thickness" : double, 그리는 선의 두께 (in pixel size)
			"Duration" : 화면에 떠 있는 시간, 0 일 경우, 마우스 클릭시까지.

			"Center" : CPoint2d - 중심 좌표
			"Size" : double(0.0) - 크기

*/



/* Image Processing
	ON_IP("Image", "FindPattern") : Pattern Matching.

		var :
			"Image"				: Mat, Source Image. 없을 경우, FrameGrabber로부터 가지고 옴.
			"Pattern", 또는 "PathPattern" 둘 중의 하나는 입력해야 함.
			"Pattern"			: patternImage. (AddChildItemMat() 함수로 입력)
			"PathPattern"		: string, Pattern Image 경로(File).

			"Method"			: int(TM_CCOEFF_NORMED), Pattern Matching
			"CenterOffset"		: CPoint2d(0, 0), : Center 위치에서 Offset 만큼 이동한 위치를 더해서 리턴.
			"Rate"				: double, 0.0~1.0,
			"ShowRate"			: BOOL(FALSE) TRUE 일 경우, 화면에 결과값 출력
			"ShowTact"			: BOOL(FALSE) TRUE 일 경우, 화면에 소요시간 출력
			"ScaleDown"			: double(1.0) - 이미지의 크기를 비율 만큼 줄여서 Pattern 을 찾음. 찾은 뒤, 근처에서 원본 이미지로 다시 검색 (ex, 0.5, 0.25 ...) ScaleDown/ScaleUp 둘 다 동시 사용 가능.
			"ScaleUp"			: double(1.0) - Subpixeling. 일단 원본 이미지에서 찾은 후, 스케일을 키워 다시 찾음. ScaleDown/ScaleUp 둘 다 동시 사용 가능.
			"ScaleUpMethod"		: int(cv::INTER_LINEAR) - Scale Up Method.
			"GetImage"			: BOOL(FALSE) TRUE 일 경우, 찾은 이미지를 "ImageFound" 에 넣어서 보냄.
			"ROI"				: CRect2d, Region Of Interest ( in Pixel Size )

			"ManualRetry"		: BOOL(FALSE) TRUE 일 경우, 실패시 사용자로부터 영역을 선택하여 받아옴

			"ColorFound"		: uint32_t(RGB(0, 0, 255)) - 십자 마크 색깔 (성공시)
			"ColorNotFound"		: uint32_t(RGB(255, 0, 0)) - 십자 마크 색깔 (점수가 낮거나, 실패시. 가장 근접한 위치에 표시)
			"DisplayDuration"	: uint32_t(10*1000) in msec - 십자 마크 화면 유지 시간. 0 / -1 입력시 계속 출력.
			"PenStyle"			: uint32_t(PS_SOLID) - 십자 마크 선 형태

		varResult :
			"MX", "MY" : double (Stage 좌표계), 선택된 영역의 중앙 좌표
			"nPtsMachine" : int, 좌표 갯수.
			"MX0", "MY0" ~ "MX1", "MY1" : double, 선택된 영역

			"IX", "IY" : double (이미지 좌표계), 선택된 영역의 중앙 좌표
			"nPtsImage" : int, 좌표 갯수
			"IX0", "IY0" ~ "IX1", "IY1" : double, (이미지 좌표계)선택된 영역

			"nPtsLaserImage" : int, 좌표 갯수 (항상 1)
			"LIX", "LIY" : double (이미지 좌표계. Laser Center Offset 기준)

			"RateFound" : double() Pattern Matching 결과 값
			"ImageFound" : Mat. 이미지를 찾은 경우 찾은 이미지를 리턴.


	ON_IP("Image", "FindEdge") : Edge Finder.
		var :
			"Threshold" : double(20.0) Edge 구분 Theshold.
			"OpenOperationSize" : int(0) erode, dilate Kernel Size. 3 이상일 경우 실행 됨.
			"LineFittingMethod" : int(cv::DIST_HUBER) Line Fitting Method.
			"ShowRate" : BOOL(FALSE), 찾았을 경우, 결과 Rate 값을 화면에 10초동안 표시함.
		varResult :
			"Direction" : int, 0 : ->, 1 : <-, 2 : downward, 3 : upward
			"MX", "MY" : double (Stage 좌표계), 선택된 영역의 중앙 좌표
			"nPtsMachine" : int, 좌표 갯수.
			"MX0", "MY0" ~ "MX1", "MY1" : double, 선택된 영역

			"IX", "IY" : double (이미지 좌표계), 선택된 영역의 중앙 좌표
			"nPtsImage" : int, 좌표 갯수
			"IX0", "IY0" ~ "IX1", "IY1" : double, (이미지 좌표계)선택된 영역

			"nPtsLaserImage" : int, 좌표 갯수 (항상 1)
			"LIX", "LIY" : double (이미지 좌표계. Laser Center Offset 기준)

			"Rate" : double 결과 Rate 값



	ON_IP("Image", "FindCorner") : Edge Corner 모서리 찾기
		var :
			"MinThreshold" : double(60.0)
			"MaxThreshold" : double(120.0)
			"StepThreshold" : double(10.0)
			"MinArea" : double(10000.0)

		varResult :
			"MX", "MY" : double (Stage 좌표계), 선택된 영역의 중앙 좌표
			"nPtsMachine" : int, 좌표 갯수.
			"MX0", "MY0" ~ "MX1", "MY1" : double, 선택된 영역

			"IX", "IY" : double (이미지 좌표계), 선택된 영역의 중앙 좌표
			"nPtsImage" : int, 좌표 갯수
			"IX0", "IY0" ~ "IX1", "IY1" : double, (이미지 좌표계)선택된 영역

			"nPtsLaserImage" : int, 좌표 갯수 (항상 1)
			"LIX", "LIY" : double (이미지 좌표계. Laser Center Offset 기준)

	ON_IP("Image", "FindControlLine") : Scanner Calibration 용 Cross Mark Search.
		var :
			"LineSize" : int(41), 십자마크 크기 (pixel 단위)
			"LineThickness" : 십자마크 선폭 (pixel 단위)
			"Side" : int(BIT(0)|BIT(1)|BIT(2)|BIT(3)), 십자마크 선 형태. (오른쪽 가지부터 반시계방향으로 index)
					BIT(0) : '╺' 오른쪽 가지 있음.	
					BIT(1) : '╹' 윗쪽 가지 있음.  
					BIT(2) : '╸' 왼쪽 가지 있음.  
					BIT(3) : '╻' 아랫쪽 가지 있음.
					ex, 
						'┗' : BIT(0)|BIT(1) -> 003 (8진수)
						'┏' : BIT(0)|BIT(3) -> 011
						'┛' : BIT(1)|BIT(2) -> 006
						'┓' : BIT(2)|BIT(3) -> 014
						'┣' : BIT(0)|BIT(1)|BIT(3) -> 013
						'┫' : BIT(1)|BIT(2)|BIT(3) -> 016
						'┳' : BIT(0)|BIT(2)|BIT(3) -> 015
						'┻' : BIT(0)|BIT(1)|BIT(2) -> 007
						'╋' : BIT(0)|BIT(1)|BIT(2)|BIT(3) -> 017

			"BlackBkgnd" : BOOL(FALSE)
					FALSE(default) : 배경이 밝고, 레이저 가공라인이 어두울 경우.
					TRUE : 배경색깔이 어둡고, 레이저 가공라인이 밝을 경우.

		varResult :
			"MX", "MY" : double (Stage 좌표계), 선택된 영역의 중앙 좌표
			"nPtsMachine" : int, 좌표 갯수.
			"MX0", "MY0" ~ "MX1", "MY1" : double, 선택된 영역

			"IX", "IY" : double (이미지 좌표계), 선택된 영역의 중앙 좌표
			"nPtsImage" : int, 좌표 갯수
			"IX0", "IY0" ~ "IX1", "IY1" : double, (이미지 좌표계)선택된 영역

			"nPtsLaserImage" : int, 좌표 갯수 (항상 1)
			"LIX", "LIY" : double (이미지 좌표계. Laser Center Offset 기준)


	ON_IP("Image", "FindCrossLineCenter") : Scanner Calibration 용 Cross Mark Search.
		var :
			"Threshold" : double(90.0) Binaryzing threshold
			"MinArea" : double(10000.0) : 이미지 레이블링시 최소의 면적 (계산용)
			"Ratio" : double(0.7) x, y축의 이미지 평균값 분리 후 라인 피팅시 사용되는 점의 비율
			"LineCount" : int(200) x, y축의 이미지 평균값 분리 개수 (라인 피팅시 사용)
			"LineThickness" : int(300) Line의 최대 두께

			"BlackBkgnd" : BOOL(FALSE)
					FALSE(default) : 배경이 밝고, 레이저 가공라인이 어두울 경우.
					TRUE : 배경색깔이 어둡고, 레이저 가공라인이 밝을 경우.

		varResult :
			"MX", "MY" : double (Stage 좌표계), 선택된 영역의 중앙 좌표
			"nPtsMachine" : int, 좌표 갯수.
			"MX0", "MY0" ~ "MX1", "MY1" : double, 선택된 영역

			"IX", "IY" : double (이미지 좌표계), 선택된 영역의 중앙 좌표
			"nPtsImage" : int, 좌표 갯수
			"IX0", "IY0" ~ "IX1", "IY1" : double, (이미지 좌표계)선택된 영역

			"nPtsLaserImage" : int, 좌표 갯수 (항상 1)
			"LIX", "LIY" : double (이미지 좌표계. Laser Center Offset 기준)


	ON_IP("Image", "FindCrossLineCenter201702") : Scanner Calibration 용 Cross Mark Search.	( by 장성규 )
		var :
			"Threshold"			: double(90.0) Binaryzing threshold
			"MinArea"			: double(10000.0) : 이미지 레이블링시 최소의 면적 (계산용)
			"LineCount"			: int(200) x, y축의 이미지 평균값 분리 개수 (라인 피팅시 사용)
			"Outlier"			: int(1)     - 라인피팅 참조 거리 - 선택 영역 내의 라인만 결과값에 영향을 줌 (튀는 값 제거)
			"LineFittingCount"	: int(5)     - 라인피팅 갯수 (만약 5이면 가로 5개, 세로 5개 총 25개의 포인트 검출 후 중심 점에서 가장 가까운 포인트 반환)
			"Detail"			: BOOL(TRUE) - Detail 사용
			"iDetailLevel"		: int(20)    - 확대 배율

			"BlackBkgnd"		: BOOL(FALSE)
					FALSE(default) : 배경이 밝고, 레이저 가공라인이 어두울 경우.
					TRUE : 배경색깔이 어둡고, 레이저 가공라인이 밝을 경우.

		varResult :
			"MX", "MY" : double (Stage 좌표계), 선택된 영역의 중앙 좌표
			"nPtsMachine" : int, 좌표 갯수.
			"MX0", "MY0" ~ "MX1", "MY1" : double, 선택된 영역

			"IX", "IY" : double (이미지 좌표계), 선택된 영역의 중앙 좌표
			"nPtsImage" : int, 좌표 갯수
			"IX0", "IY0" ~ "IX1", "IY1" : double, (이미지 좌표계)선택된 영역

			"nPtsLaserImage" : int, 좌표 갯수 (항상 1)
			"LIX", "LIY" : double (이미지 좌표계. Laser Center Offset 기준)


	ON_IP("Image", "FindLineCenter", OnIPImageFindLineCenter) : 오브젝트 엣지의 중심점 찾기
		var :
			"SearchAreaTopBottom"	: int(BOTTOM)  오브젝트가 위에 있는 경우 TOP, 아래에 있는경우 BOTTOM
			"SearchAreaLeftRight"	: int(LEFT)    좌표상의 위치 - LEFT, CENTER, RIGHT / LEFT경우 오브젝트는 오른쪽에 보여지고 RIGHT경우 오브젝트는 왼쪽에 보여짐
			"Threshold"				: int(128)
			"OffsetPixel"			: int(0)       결과 좌표의 Offset 만큼 Pixel이 이동 ( 오브젝트가 TOP 일경우 아래쪽으로 OFFSET 적용, BOTTOM일 경우 위쪽으로 OFFSET 적용)
			"MatchingRate"			: double(0.1)  매칭 확룔이 파라미터의 값보다 작게 될 경우 에러 리턴

		varResult :
			"ErrorCode" : 1 : 매칭율 에러, 2 : 서칭 에러

			"MX", "MY" : double (Stage 좌표계), 선택된 영역의 중앙 좌표
			"nPtsMachine" : int, 좌표 갯수.
			"MX0", "MY0" ~ "MX1", "MY1" : double, 선택된 영역

			"IX", "IY" : double (이미지 좌표계), 선택된 영역의 중앙 좌표
			"nPtsImage" : int, 좌표 갯수
			"IX0", "IY0" ~ "IX1", "IY1" : double, (이미지 좌표계)선택된 영역

			"nPtsLaserImage" : int, 좌표 갯수 (항상 1)
			"LIX", "LIY" : double (이미지 좌표계. Laser Center Offset 기준)


	ON_IP("Image", "FindCrossLight") : LCD/LED로 Cross Line 을 점등시킨 후, 점등된 Cross Line 의 중앙점을 찾는 함수
		var :
			"MaxThreshold" : int(200)	영상의 최대 밝기 - 밝으면 200정도, 어두우면 50정도
			"MinThreshold" : int(10)	영상의 최소 밝기 - 10으로 두면 됨
			"Accuracy"     : int(1)		정밀도 - 숫자가 작을수록 정밀도가 높음 (1 - 10)
			"MinRefPoint"  : int(4)		설정된 참조 포인트 개수 이하가 되면 에러
			"MaxRefPoint"  : int(25)	설정된 참조 포인트 개수 이상이 되면 에러

		varResult :
			"ErrorCode" : 1:입력영상이 칼라영상이 아님, 2:참조 포인트 개수 에러, 3:인식실패, 4:인식률 저조

			"MX", "MY" : double (Stage 좌표계), 선택된 영역의 중앙 좌표
			"nPtsMachine" : int, 좌표 갯수.
			"MX0", "MY0" ~ "MX1", "MY1" : double, 선택된 영역

			"IX", "IY" : double (이미지 좌표계), 선택된 영역의 중앙 좌표
			"nPtsImage" : int, 좌표 갯수
			"IX0", "IY0" ~ "IX1", "IY1" : double, (이미지 좌표계)선택된 영역

			"nPtsLaserImage" : int, 좌표 갯수 (항상 1)
			"LIX", "LIY" : double (이미지 좌표계. Laser Center Offset 기준)

			"DistPixelX" : double
			"DistPixelY" : double


	ON_IP("Image", "FindSimpleObject", OnIPImageFindSimpleObject) : 백색배경의 검정색 Object 찾기
		var :
			"MinArea"	: double(0) 오브젝트 최소 면적.
			"MaxArea"	: double(0) 오브젝트 최대 면적.
			"Threshold"	: double(120)

		varResult :
			"MX", "MY" : double (Stage 좌표계), 무게중심 좌표
			"Outline" : list. Child : CPoint2d

	ON_IP("Image", "FindCrossLine", OnIPImageFindCrossLine) : (in Test) 두 직선의 교점 찾기. 라인이 많을 경우, 가운데 라인 사용. (가로선/세로선만 가능함. 그 이외에는 오차 크게 발생)
		var :
			"Threshold"		: double(120.0)
			"LineThickness"	: double(50)
			"MinLineLength"	: double(500)
			"MinAngle"		: double(80)
			"BlackBkgnd"	: BOOL(FALSE)
					FALSE(default) : 배경이 밝고, 레이저 가공라인이 어두울 경우.
					TRUE : 배경색깔이 어둡고, 레이저 가공라인이 밝을 경우.
			"Scale"			: double(1.0)

		varResult :
			"MX", "MY" : double (Stage 좌표계)
			
	// BOE
	ON_IP("Image", "FindAlignmark", OnIPImageFindAlignmark)
	ON_IP("Image", "FindHoleCrack", OnIPImageFindHoleCrack)
	//RTR
	ON_IP("Image", "FindR2REdge", OnIpImageFindR2REdge)
	//UTI
	ON_IP("Image", "FindUTIAlignment", OnIpImageFindUTIAlignment)
	//KCC
	ON_IP("Image", "FindKCCAlignment", OnIpImageFindKCCAlignment)

	------------------------------------------------------------------------------
	// FrameGrabber Information
	//

	----- AVAL -----
		"AVAL.Board"				: $(BoardName),$(BoardIndex). ex) "APX-3324,0"
		"AVAL.INIFile"				: Path to ini File. ex)
		"AVAL.Channel"				: Channel(Connector) Index, option: "1", "2", ...
		"AVAL.BayerFormat"			: Bayer Color Format, option: "none", "Bayer-GB", "Bayer-BG", "Bayer-RG", "Bayer-GR"
		"AVAL.BayerByteShift"		: Bayer Color Shift (AVAL은 Bayer Format 시, 실제메모리 시작 주소가 달라짐.) ex) "0", "1", "2"
		"AVAL.PoCL"					: Power Over Camera Link. option: "USE", "N/A"
		"AVAL.PoCLTimeout"			: PoCL timoeut : 500ms 이상 줘야 함. 샘플에는 3초로 되어 있음. ex) "0", "500", "1000", "3000"
		"Comm.Camera"				: Camera Type. option: "Sony", "CIS" (대소문자 주의)
		"Comm.Port"					: 포트이름 필요없음.
		"Comm.Baud"					: Camera 마다 설정 다름.
		"Comm.ByteSize"				: Camera 마다 설정 다름. Option : "6", "7", "8"
		"Comm.Parity"				: Camera 마다 설정 다름. Option : "0-NoParity" "1-ODDPARITY" "2-EVENPARITY"
		"Comm.StopBits"				: Camera 마다 설정 다름. Option : "0-ONESTOPBIT" "1-ONE5STOPBITS" "2-TWOSTOPBITS"
		"Comm.FlowControl"			: Camera 마다 설정 다름. Option : "0-NoControl" "1-X On/Off" "2-H/W"

	
	----- Euresys -----
		"Euresys.CAMFile"			: Cam File Path
		"Euresys.BoardIndex"		: Board Index, Option : "0" "1" "2" "3" "4"
		"Euresys.BoardTopology"		: Option : "-1 none" "24.MC_BoardTopology_MONO" "25.MC_BoardTopology_DUO" "29.MC_BoardTopology_UNIFORM"
		"Euresys.BoardConnector"	: Option : "M" "N" "X" "Y" "Z" "A" "B" "C" "D"
		"Euresys.BayerFormat"		: Option : "none" "Bayer-GB" "Bayer-BG" "Bayer-RG" "Bayer-GR"), _T("Bayer Color Format") },
		"Comm.Camera"				: Camera Type. option: "Sony", "CIS" (대소문자 주의)
		"Comm.Port"					: 포트를 생성해서 사용. 제어판에서 COM 포트 열어보고, 없는 이름 사용하면 됨. ex, 제어판에 COM1~4 까지 있으면, COM5 사용 가능.
		"Comm.Baud"					: Camera 마다 설정 다름.
		"Comm.ByteSize"				: Camera 마다 설정 다름. Option : "6", "7", "8"
		"Comm.Parity"				: Camera 마다 설정 다름. Option : "0-NoParity" "1-ODDPARITY" "2-EVENPARITY"
		"Comm.StopBits"				: Camera 마다 설정 다름. Option : "0-ONESTOPBIT" "1-ONE5STOPBITS" "2-TWOSTOPBITS"
		"Comm.FlowControl"			: Camera 마다 설정 다름. Option : "0-NoControl" "1-X On/Off" "2-H/W"

	----- GigE.Baumer -----
		"GigE.ForceSelect,			: Camera MAC Address 리스트. (일반적으로) 최초 한 번만 선택해 주면 됨.
		"GigE.IP"					: Camera IP
		"GigE.Subnet"				: Camera SubnetMask
		"GigE.Gateway"				: Camera Gateway (설정 안해도 됨)
		
	----- GigE.Baumer -----
		"GigEBaumer.CameraID"		: Camera ID : Camera Name
		"GigEBaumer.IP				: Camera IP ex) "192.168.100.3"


	----- Neptune -----
		"Neptune.CameraIndex"		: Camera Index, Option :"0" "1" "2" "3" "4" "5" "6" "7" "8" "9" "10"...



	----- Matrox -----
		"Matrox.DCFPath"			: DCF File Path
		"Matrox.SystemDescriptor"	: Option : 
										"M_DEFAULT"
										"M_SYSTEM_HOST"
										"M_SYSTEM_CORONA_II"
										"M_SYSTEM_METEOR_II"
										"M_SYSTEM_METEOR_II_DIG"
										"M_SYSTEM_METEOR_II_CL"
										"M_SYSTEM_METEOR_II_1394"
										"M_SYSTEM_1394"
										"M_SYSTEM_ODYSSEY"
										"M_SYSTEM_ODYSSEY_HOST"
										"M_SYSTEM_HELIOS"
										"M_SYSTEM_IRIS"
										"M_SYSTEM_CRONOSPLUS"
										"M_SYSTEM_MORPHIS"
										"M_SYSTEM_SOLIOS"
										"M_SYSTEM_MTXAUXILIARY"
										"M_SYSTEM_VIO"
										"M_SYSTEM_NEXIS"
										"M_SYSTEM_GPU"
										"M_SYSTEM_GIGE_VISION"
										"M_SYSTEM_MORPHISQXT"
										"M_SYSTEM_SOLIOS_GIGE"
										"M_SYSTEM_MORPHIS_EVO"
										"M_SYSTEM_IRIS_GT"
										"M_SYSTEM_RADIENT"
										"M_SYSTEM_ORION_HD"
		"Matrox.SystemIndex"		: System(Board) Index, Option : "0" "1" "2" "3" "4" ...
		"Matrox.DeviceNo"			: Device(Camera) Index, Option : "0" "1" "2" "3" "4" ...
		"Matrox.BayerFormat"		: Bayer Color Format, Option : "none", "Bayer-GB", "Bayer-BG", "Bayer-RG", "Bayer-GR"
		"Comm.Camera"				: Camera Type. option: "Sony", "CIS" (대소문자 주의)
		"Comm.Port"					: 제어판에 Board/Camera 와 연결된 COM 포트 나와 있음. 동일한 포트 사용해야 함.
		"Comm.Baud"					: Camera 마다 설정 다름.
		"Comm.ByteSize"				: Camera 마다 설정 다름. Option : "6", "7", "8"
		"Comm.Parity"				: Camera 마다 설정 다름. Option : "0-NoParity" "1-ODDPARITY" "2-EVENPARITY"
		"Comm.StopBits"				: Camera 마다 설정 다름. Option : "0-ONESTOPBIT" "1-ONE5STOPBITS" "2-TWOSTOPBITS"
		"Comm.FlowControl"			: Camera 마다 설정 다름. Option : "0-NoControl" "1-X On/Off" "2-H/W"

*/
};
