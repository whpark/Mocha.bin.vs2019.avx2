#pragma once

#include <AFXPropertyGridCtrl.h>
#include "misc/Profile.h"
#include "FGCamera.h"
#include "FGComm.h"
#include "Coordinate.hpp"
#include "xMathUtil/MatHelper.h"

#ifdef FG_EXPORTS
#define FG_API __declspec(dllexport)
#else
#define FG_API __declspec(dllimport)
#endif

#pragma pack(push, 8)

class CFrameGrabber;
class IFrameGrabberInterface;
class CFrameGrabberImageLock;

//-----------------------------------------------------------------------------
// IFrameGrabberInterface
//
class IFrameGrabberInterface : public CSimpleLogInterface {
	friend class CFrameGrabber;
	friend class CFrameGrabberImageLock;
public:
	typedef BOOL (*FuncNewImage)(void* pParam);
protected:
	CCriticalSection m_csImgBGR;
	DWORD m_dwTickImageArrived;
	cv::Mat m_img;
	cv::Size m_size;
	FuncNewImage m_funcNewImage;
	void* m_pParam;
	TRefPointer<IFGCamera> m_rFGCamera;
	TRefPointer<IFGComm> m_rComm;

public:
	IFrameGrabberInterface() : m_size(0, 0), m_funcNewImage(NULL), m_pParam(NULL), m_dwTickImageArrived(0) { }
	virtual ~IFrameGrabberInterface() {}

public:
	virtual void SetCallBack(FuncNewImage funcNewImage, void* pParam) { m_funcNewImage = funcNewImage; m_pParam = pParam; }
	virtual BOOL InitFrameGrabber(const CProfileSection& settings, CString& strMessage) = NULL;
	virtual BOOL CloseFrameGrabber() = NULL;
	virtual BOOL StartCapture() = NULL;
	virtual BOOL StopCapture() = NULL;

	// Helper
protected:
	struct T_CAMERA_PROPERTY {
		LPCTSTR pszName;
		BOOL bAllowEdit;
		LPCTSTR pszOption;
		LPCTSTR pszDescription;
	};
	static void GetCameraPropertiesTBL(TList<CMFCPropertyGridProperty>& properties, const T_CAMERA_PROPERTY tbl[], int nItem) {
		if (!tbl)
			return;
		for (int i = 0; i < nItem; i++) {
			CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(tbl[i].pszName, _T(""), tbl[i].pszDescription);
			CStrings strsOption;
			SplitString(tbl[i].pszOption, _T(';'), strsOption);
			for (int iOption = 0; iOption < strsOption.N(); iOption++) {
				pProp->AddOption(strsOption[iOption]);
			}
			pProp->AllowEdit(tbl[i].bAllowEdit);
			properties.Push(pProp);
		}
	}
	static void GetCameraPropertiesComm(TList<CMFCPropertyGridProperty>& properties, const CStrings& strsCamera) {
		static T_CAMERA_PROPERTY tbl[] = {
			{ _T("Comm.Camera"),				FALSE, NULL,													_T("Comm - Camera Type") },
			{ _T("Comm.Port"),					 TRUE, NULL,													_T("Comm - Port") },
			{ _T("Comm.Baud"),					 TRUE, _T("9600;14400;19200;38400;56000;57600;115200;256000;"),	_T("Comm - Speed") },
			{ _T("Comm.ByteSize"),				 TRUE, _T("6;7;8"),												_T("Comm - ByteSize") },
			{ _T("Comm.Parity"),				FALSE, _T("0-NoParity;1-ODDPARITY;2-EVENPARITY"),				_T("Comm - Parity") },
			{ _T("Comm.StopBits"),				FALSE, _T("0-ONESTOPBIT;1-ONE5STOPBITS;2-TWOSTOPBITS"),			_T("Comm - Stop Bits") },
			{ _T("Comm.FlowControl"),			FALSE, _T("0-NoControl;1-X On/Off;2-H/W"),						_T("Comm - FlowControl") },
		};
		GetCameraPropertiesTBL(properties, tbl, countof(tbl));
		for (int i = 0; i < properties.N(); i++) {
			CMFCPropertyGridProperty& prop = properties[i];
			if (tszcmp(prop.GetName(), _T("Comm.Camera")) != 0)
				continue;
			prop.AddOption(_T("-none-"));
			for (int i = 0; i < strsCamera.N(); i++)
				prop.AddOption(strsCamera[i]);
		}
	}

	BOOL ToMat(cv::Mat& img, cv::Mat& imgBuffer, const void* pImage, cv::Size size, int nBufferPitch, int nBitPerPixel, int nChannel, int eColorCoversionCode = -1) {
		using namespace cv;

		try {
			if (!pImage || !size.width || !size.height || (nBufferPitch == 0) || (nBitPerPixel == 0) || (nChannel == 0) )
				return FALSE;

			// Image Size
			int cx = ADJUST_DWORD_ALIGN(size.width);	// for speed up
			int cy = size.height;

			// Image Depth
			int eDepth = CV_8U;
			int nBytePerPixel = 1;
			int nBitPerDot = nBitPerPixel / nChannel;
			if (nBitPerDot > 32) {
				return FALSE;
			} else if (nBitPerDot > 16) {
				eDepth = CV_32S;
				nBytePerPixel = sizeof(int32_t) * nChannel;
			} else if (nBitPerDot > 8) {
				eDepth = CV_16U;
				nBytePerPixel = sizeof(int16_t) * nChannel;
			} else {
				eDepth = CV_8U;
				nBytePerPixel = sizeof(int8_t) * nChannel;
			}
			if (nBufferPitch < nBytePerPixel*size.width)
				return FALSE;

			// Image Channel
			if ( (nChannel < 1) || (nChannel > 4) )
				return FALSE;
			int eType = CV_MAKETYPE(eDepth, nChannel);

			// Set Image (only for BGR)
			if ( (imgBuffer.type() != eType) || (imgBuffer.rows != cy) || (imgBuffer.cols != cx) ) {
				imgBuffer = Mat::zeros(cy, cx, eType);
			}

			// Copy
			if (nBufferPitch == imgBuffer.step) {
				memcpy(imgBuffer.ptr(0), pImage, nBufferPitch*imgBuffer.rows);
			} else {
				const BYTE* pos = NULL;
				pos = (const BYTE*)pImage;
				const size_t nSize = _min((size_t)imgBuffer.step, (size_t)nBufferPitch);
				for (int y = 0; y < img.rows; y++, pos += nBufferPitch) {
					memcpy(imgBuffer.ptr(y), pos, nSize);
				}
			}

			if ( (eColorCoversionCode >= 0) && (eColorCoversionCode < cv::COLOR_COLORCVT_MAX) )
				ConvertColor(imgBuffer, img, eColorCoversionCode);
			else
				img = imgBuffer;

		} catch (...) {
			return FALSE;
		}

		return TRUE;
	}

//-----------------------------------------------------------------------------
public:
	virtual BOOL DoGeneralCommand(LPCTSTR pszCommand, const std::vector<BYTE>& param, std::vector<BYTE>& result) { return TRUE; }
	virtual BOOL DoGeneralCommandConst(LPCTSTR pszCommand, const std::vector<BYTE>& param, std::vector<BYTE>& result) const { return TRUE; }

};

//-----------------------------------------------------------------------------

typedef void (*FuncGetFrameGrabberName)(char* pszName, int nBufferSize);
typedef IFrameGrabberInterface* (*FuncGetFrameGrabber)(void);
typedef void (*FuncGetFrameGrabberProperties)(TList<CMFCPropertyGridProperty>& properties, const CStrings& strsCamera);


#define EXPORT_FG_API(szFGName, classFGName)\
extern "C" {\
	FG_API void GetFrameGrabberName(char* pszName, int nBufferSize) { tszncpy(pszName, szFGName, nBufferSize); }\
	FG_API IFrameGrabberInterface* GetFrameGrabber(void) { return new classFGName; }\
	FG_API void GetFrameGrabberProperties(TList<CMFCPropertyGridProperty>& properties, const CStrings& strsCamera) { classFGName::GetFrameGrabberProperties(properties, strsCamera); }\
};\


#pragma pack(pop)
