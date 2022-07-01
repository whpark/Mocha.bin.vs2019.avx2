#pragma once

#include "AFX_EXT_FRAME_GRABBER.h"
#include <AFXPropertyGridCtrl.h>

#include "TRefPointer.hpp"

#include "misc/Profile.h"
#include "Coordinate.hpp"

#include "FGCamera.h"

#include "ICameraView.h"
#include "CameraLayout.h"
#include "CameraPaneWnd.h"

class IFrameGrabberInterface;
class AFX_EXT_CLASS_FRAME_GRABBER CFrameGrabber;
class AFX_EXT_CLASS_FRAME_GRABBER CFrameGrabberImageLock;

class CCameraDlg;

//-----------------------------------------------------------------------------
//

class AFX_EXT_CLASS_FRAME_GRABBER CFrameGrabber {
	friend class TRefPointer<CFrameGrabber>;
	friend class CFrameGrabberImageLock;
protected:
	IFrameGrabberInterface* m_pFrameGrabber;
	static BOOL s_bGPUChecked;
	static BOOL s_bUseGPU;
protected:
	CProfileSection m_settings;
	BOOL m_bFlipX;
	BOOL m_bFlipY;
	int m_eRotate;
	struct {
		BOOL bRotateArbitrary;
		double dRotationAngle;	// in deg
		BOOL bClip;
		cv::Mat matMap1, matMap2;
		cv::Rect rcClip;
		//cv::Mat matRotation;
	} m_rotation;
	struct {
		BOOL bClipRect;
		cv::Rect rcClip;
	} m_clip;

	// for Live Image
protected:
	BOOL				m_bLive;
	DWORD				m_dwTickGrabbed;
	DWORD				m_dwTimeoutGrab;
	cv::Mat				m_imgBGRStatic;	// for Static Image
	TRefList<CEvent>	m_evtsNewImage;

	// for Triggered Image
public:
	class T_IMAGE_BUFFER {
	public:
		DWORD dwTick;
		cv::Mat img;
		T_IMAGE_BUFFER() : dwTick(0) {}
		T_IMAGE_BUFFER(cv::Size size, int type) : dwTick(0), img(size, type) {}
	};
protected:
	std::atomic<bool>	m_bSerial;
	int					m_nMaxImage;
	TList<T_IMAGE_BUFFER>	m_queueImages;
	TList<T_IMAGE_BUFFER>	m_queueImageBuffers;

protected:
	CFrameGrabber(IFrameGrabberInterface* pFrameGrabber = NULL);
	CFrameGrabber(const CFrameGrabber& B) {
		ASSERT(FALSE);
	}
	CFrameGrabber& operator = (const CFrameGrabber& B) {
		ASSERT(FALSE);
		return *this;
	}

	virtual ~CFrameGrabber(void);

public:
	CProfileSection& GetSettings() { return m_settings; }
	const CProfileSection& GetSettings() const { return m_settings; }

	const IFrameGrabberInterface* GetFrameGrabberInterface() const { return m_pFrameGrabber; };
	IFrameGrabberInterface* GetFrameGrabberInterface() { return m_pFrameGrabber; };

public:
	static BOOL LoadFrameGrabberDll(LPCTSTR pszFolderDll = _T(".\\FG\\"), LPCTSTR pszNameFilter = NULL);
	static BOOL UnloadFrameGrabberDll();
	static void GetFrameGrabberList(CStrings& strsFrameGrabber);
	static void GetFrameGrabberProperties(LPCTSTR pszFrameGrabber, TList<CMFCPropertyGridProperty>& properties);
	static TRefPointer<CFrameGrabber> GetFrameGrabber(LPCTSTR pszCameraType, const CProfileSection& settings, CString& strMessage);
	static void GetCameraInterfaceList(CStrings& strsCameraInterface);

public:
	static BOOL IsGPUEnabled() { return s_bUseGPU; }
	static BOOL EnableGPU(BOOL bEnable = TRUE);
	cv::Size GetImageSize();
	int GetImageType();

	BOOL SetClipRect(const cv::Rect& rcClip, BOOL bClip = TRUE);
	void GetClipRect(cv::Rect& rcClip, BOOL& bClip) const;

	// new Image Notify
public:
	virtual BOOL GetImage(cv::Mat& img, bool bFailIfTimedout = true, DWORD dwWaitForNewImage = 3'000);	// dwWaitForNewImage : 0 - Don't wait, else - Wait for Duration.
	virtual BOOL GetNewImage(cv::Mat& img, DWORD dwWaitForNewImage = 3'000);	// dwWaitForNewImage : 0 - Don't wait, else - Wait for Duration.
	BOOL SetStaticImage(cv::Mat& img);
	BOOL SetLive(BOOL bLive = TRUE);	// returns TRUE if live.
	BOOL IsLive() const { return m_bLive; }
	BOOL IsFreezed() const;
	void SetGrabTimeout(DWORD dwTimeout = 500) { m_dwTimeoutGrab = dwTimeout; }
	DWORD GetGrabTimeout() const { return m_dwTimeoutGrab; }

	// Serial Grab
	BOOL StartSerialGrab(int nMaxImage);
	BOOL StopSerialGrab();
	BOOL IsSerialGrab() const { return m_bSerial; }
	BOOL ReserveImageBuffer(int nMaxImage);

	class CImageBufferReturner {
	public:
		CFrameGrabber* pFG;
		TRefPointer<CFrameGrabber::T_IMAGE_BUFFER>& rImage;
	public:
		CImageBufferReturner(CFrameGrabber* _pFG, TRefPointer<CFrameGrabber::T_IMAGE_BUFFER>& _rImage) : pFG(_pFG), rImage(_rImage) {}
		~CImageBufferReturner() {
			if (pFG && rImage)
				pFG->ReturnImageBuffer(rImage);
		}
	};
	BOOL PopImageBuffer(TList<T_IMAGE_BUFFER>& images);
	BOOL PopImageBuffer(TRefPointer<T_IMAGE_BUFFER>& rImage);

	BOOL ReturnImageBuffer(TList<T_IMAGE_BUFFER>& images);
	BOOL ReturnImageBuffer(TRefPointer<T_IMAGE_BUFFER>& rImage);

public:
	virtual void InsertWaitingQueue(TRefPointer<CEvent>& rEvent);
	virtual void DeleteWaitingQueue(TRefPointer<CEvent>& rEvent);

protected:
	virtual void PreProcess();
	void NotifyNewImage();
	static BOOL NewImageT(void* pParam);
	BOOL NewImage();

public:
	struct T_FRAME_COUNT {
		DWORD dwTick0 = 0, dwTick1 = 0, dwTick_mid = 0;
		int nFrames = 0, nFrames_mid = 0;
		std::atomic<double> dFramesPerSec;
	};
protected:
	T_FRAME_COUNT m_frameCount;
public:
	const T_FRAME_COUNT& GetFrameCounter() const { return m_frameCount; }

public:
	virtual BOOL InitFrameGrabber(const CProfileSection& settings, CString& strMessage);
	virtual BOOL CloseFrameGrabber();
	virtual BOOL StartCapture();
	virtual BOOL StopCapture();

protected:
	TRefPointer<IFGCamera> m_rFGCamera;
	CCameraDlg* m_pCameraDlg;
public:
	BOOL IsCommPortOpen();
	BOOL OpenCommPort();
	BOOL CloseCommPort();
	BOOL OpenCameraSetting(CWnd* pWndParent = NULL);
	IFGCamera* GetCamera() { return m_rFGCamera; }

};

//-----------------------------------------------------------------------------
//
class AFX_EXT_CLASS_FRAME_GRABBER CFrameGrabberImageLock {
protected:
	IFrameGrabberInterface* m_pFG;
public:
	CFrameGrabberImageLock(CFrameGrabber* pFG);
	CFrameGrabberImageLock(IFrameGrabberInterface* pFG);
	cv::Mat& GetImage();
	~CFrameGrabberImageLock();
};

//=============================================================================
//

//#define F_MACHINE_X			 "MX"
//#define F_MACHINE_Y			 "MY"
//#define F_IMAGE_X			 "IX"
//#define F_IMAGE_Y			 "IY"
//#define F_LASER_MACHINE_X	"LMX"
//#define F_LASER_MACHINE_Y	"LMY"
//#define F_LASER_IMAGE_X		"LIX"
//#define F_LASER_IMAGE_Y		"LIY"

class CIPClient;
class CSimpleLog;
class CProfile;

AFX_EXT_CLASS_FRAME_GRABBER BOOL InitVision(CIPClient* pIPClient, CProfile* pProfile = NULL, CSimpleLog* pLog = NULL, LPCTSTR pszDLLFolder = NULL);
