#pragma once

#include "AFX_EXT_SCANNER.h"

#include <array>
#include <mutex>

#include "Scanner/Scanner.h"
#include "misc/Profile.h"
#include "Coordinate.hpp"
#include "Shape/Shape.h"
#include "Shape/SCanvas.h"
#include "misc/SimpleLog.h"
#include "xMathUtil/MeshTable.h"
#include "MIP/MIP.h"

#include "Scanner/Scanner.h"

namespace SCANNER_SAMPLE {



	class CScannerException;
	class CScannerSetting;
	class CScannerController;

	//==================================================================================================
	class AFX_EXT_CLASS_SCANNER CScannerException : public __ExceptionBase__ {
	public:
		CScannerException(const char* pszPosition, LPCSTR pszMessage) : __ExceptionBase__(pszPosition, pszMessage) {}
	};


	//==================================================================================================
	class CScannerSetting {
	public:
		CString m_str1to1CalibrationPath;
		CString m_strsCalibrationPath[2];
		CString m_strProgramPath;
		double m_dScale[2];
		std::array<double, 4> m_mat;

		int		m_eLaserType;
		int		m_nStandbyPeriod;
		int		m_nPulseWidth;
		int		m_eTimingBase;
		int		m_nHalfOfTheOutputPeriod;
		int		m_nPulseWidth1;
		int		m_nPulseWidth2;
		int		m_nJumpDelay;
		int		m_nMarkDelay;
		int		m_nPolygonDelay;
		int		m_nLaserOnDelay;
		int		m_nLaserOffDelay;
		double	m_dJumpSpeed;			// mm / sec
		double	m_dMarkSpeed;			// mm / sec
		int		m_iDotDelay;
		double m_dEncoderRate;
		BOOL	m_bFlyMode;
		int		m_iTrackingError;

		struct {
			BOOL	bUse;				// 2014.10.16 Add by PJC

			//set_sky_writing_para : Timelag, LaserOnShift, Nprev, Npost
			double	dTimelag;
			int		iLaserOnShift;
			double	dNSkyWritingPrev;
			double	dNSkyWritingPost;
			int		iSkyWritingMode;
			double	dSkyWritinglimit;

			void Init() {
				bUse = FALSE;
				dTimelag = 0;
				iLaserOnShift = 0;
				dNSkyWritingPrev = 0;
				dNSkyWritingPost = 0;
				iSkyWritingMode = 0;
				dSkyWritinglimit = 0;
			}
		} m_skywriting;

	public:

		CScannerSetting() {
			Init();
		}

		void Init() {
			m_str1to1CalibrationPath.Empty();
			m_strsCalibrationPath[0].Empty();
			m_strsCalibrationPath[1].Empty();
			m_strProgramPath.Empty();

			m_dScale[0] = m_dScale[1] = 1000;
			m_mat = std::array<double, 4>{ 1, 0, 0, 1};

			m_eLaserType = 0;
			m_nStandbyPeriod = 0;
			m_nPulseWidth = 0;
			m_eTimingBase = 0;
			m_nHalfOfTheOutputPeriod = 0;
			m_nPulseWidth1 = 0;
			m_nPulseWidth2 = 0;
			m_nJumpDelay = 0;
			m_nMarkDelay = 0;
			m_nPolygonDelay = 0;
			m_nLaserOnDelay = 0;
			m_nLaserOffDelay = 0;
			m_dJumpSpeed = 0;
			m_dMarkSpeed = 0;
			m_iDotDelay = 100;
			m_dEncoderRate = 0;
			m_bFlyMode = FALSE;

			m_skywriting.Init();
		}

		BOOL SyncData(CProfile& profile, BOOL bStore);

	};


	class AFX_EXT_CLASS_SCANNER CScannerController : public ISCanvas, public CScannerSetting, public CSimpleLogInterface {
	public:
		enum {
			RTC_BUFFER_HALF = 4000,
			RTC_USE_LIST_1 = 1,
			RTC_USE_LIST_2,
		};

		BOOL						m_bInited = FALSE;

		int							m_iCalibration = 0;

		DWORD						m_dwStart = 0;
		DWORD						m_dwTimeout = 0;
		DWORD						m_dwTimeoutLeft = 0;
		//double						m_dPower;
		//int							m_iCurrentList;	// 1base
		//int							m_nCommand;

		//std::map<int, double>		m_dAnalogOut;

		//BOOL m_bListFirst = TRUE;

		double m_dMinMarkingSpeed = 1;

		struct {
			long eMode;
			double dAmplitudeTransversal, dAmplitudeLongitudinal;
			double dFrequency;
		} m_wobbel;

		CCoordTrans					m_ctAlign;

	protected:
		IScanner*					m_pScanner = nullptr;

	public:
		CScannerController(IScanner* pScanner = nullptr);
		virtual ~CScannerController(void);

		void SetScanner(IScanner* pScanner) { m_pScanner = pScanner; }
		IScanner* GetScanner() { return m_pScanner; }

		// Canvas
		virtual void MoveToImpl(const CPoint2d& pt) { ShapeAddJumpPoint(pt); };
		virtual void LineToImpl(const CPoint2d& pt) { ShapeAddMarkPoint(pt); };
		virtual void Dot(const CPoint2d& pt, DWORD dwDuration = 0 /* in usec */) { ShapeAddDot(pt, dwDuration); }

		//std::mutex m_mtxScannerImage;
		//bool m_bUpdateImageScanner = false;
		//cv::Mat m_imgScanner0;
		//cv::Mat m_imgScanner;	// Temp.

	public:
		//virtual double				GetAnalogPinOutSet(int iPort) { return m_dAnalogOut[iPort]; };

		virtual BOOL				Init(CProfileSection& section);
		void						UpdateTimeout();

	public:
		//virtual BOOL				SetWobbel(double dAmplitude, int iFrequency) = NULL;
		//virtual BOOL				SetWobbel(double dAmplitudeTransversal, double dAmplitudeLongitudinal, double dFrequency) = NULL;
		//virtual BOOL				SetWobbel(double dAmplitudeTransversal, double dAmplitudeLongitudinal, double dFrequency, long eMmode) = NULL;
		//virtual BOOL				GetWobbel(double dAmplitudeTransversal, double& dAmplitudeLongitudinal, double& dFrequency, long& eMode) = NULL;

		virtual void				SelectCalibration(int iCalibration);
		virtual BOOL				ResetCalibration(int iCalibration);
		virtual BOOL				LoadCalibrationFile(int iCalibration, LPCTSTR pszPath = NULL);
		virtual BOOL				LoadProgramFile(LPCTSTR pszPath = NULL);
		//virtual BOOL				GetStatus();
		virtual BOOL				IsScannerError(); //GetErors

		//// Double buffer function
		//virtual BOOL				DrawExecute();
		//virtual BOOL				DrawEndExecute();
		//virtual BOOL				DrawListStatus(int iList);
		//virtual BOOL				CurrentListCheck(int iList);

	public:
		virtual BOOL				IsEnd();

		virtual BOOL				ExecuteSetting(int nCorrectNo=0);
		virtual BOOL				ShapeAddJumpPoint(const CPoint2d& pt);
		virtual BOOL				ShapeAddMarkPoint(const CPoint2d& pt);
		virtual BOOL				ShapeAddDot(const CPoint2d& pt, DWORD dwDuration /* in usec */);
		//virtual BOOL				SetAutoMirroCorrection();
		//virtual BOOL				StartAutoMirroCorrection();
		//virtual BOOL 				LaserFire(int iOnOff, double dScannerVoltage);
		virtual void				Arc(const CPoint2d& ptCenter, double dRadius, double dT0, double dTLength);
		virtual void				Ellipse(const CPoint2d& ptCenter, double dRadius1, double dRadius2, double dTFirstAxis, double dT0, double dTLength);
		virtual BOOL				SetMarkingSpeed(double dSpeed);
		virtual BOOL				SetJumpingSpeed(double dSpeed);

		virtual void GotoXY(const CPoint2d& pt);

	public:
		struct T_SCANNER_PARAM {
			CScannerController* pThis {};
			TRefListIter<CShapeObject>* pIterObjects {};
			TRefList<CShapeObject> objects;

			//// Add By LBH
			//double dDefaultMarkSpeed = 0.0;
			//double dDefaultPower = 0.0; // in %
			////
		};
		BOOL						DrawStart(BOOL bCopyObject, const TRefListIter<CShapeObject>& objects, BOOL bWait = FALSE, DWORD dwTimeout = INFINITE);
		BOOL						DrawStop(BOOL bEmergencySTop = FALSE, DWORD dwTimeout = 0);

	protected:
		CEvent						m_evtStop;
		CEvent						m_evtStopped;
		BOOL						m_bPaused;
		HANDLE						m_hDrawWorker;
		static DWORD WINAPI			DrawWorkerT(LPVOID pParam);
		BOOL						DrawWorker(const TRefListIter<CShapeObject>& objects);

	public:
		virtual BOOL OnDrawStart();
		virtual BOOL OnDrawEnd();
		virtual BOOL OnDrawWait();
		virtual BOOL OnDrawStop();

		virtual BOOL IsDrawEnd();	// RTC Only

		bool _IsDrawThreadRunning() const { return (m_hDrawWorker ? TRUE : FALSE); };

	public:
		struct {
			std::mutex mtx;
			LARGE_INTEGER liFreq;
			LARGE_INTEGER li0;
			LARGE_INTEGER liLast;
			LARGE_INTEGER liLastMaterial;
			TPoint2<long> pt0;
			double dX0 = 0;
			double dX = 0;
			long iEncoder = 0;
			long iEncoderOld = 0;
			long iEncoderInt = 0;
			double dPosition = 0;
		} m_encoder;
	public:
		void InitEncoder();
		void UpdateEncoder(double dProcessWidth);
		BOOL ResetEncoder();

	protected:
		CEvent m_evtStopEncoderReader;
		HANDLE m_hEncoderReader = nullptr;
		static DWORD WINAPI EncoderReaderT(LPVOID pParam);
		double m_dProcessingWidth = 0;
		DWORD EncoderReader();

		//CEvent m_evtStopXYReader;
		//HANDLE m_hXYReader = nullptr;
		//static DWORD WINAPI XYReaderT(LPVOID pParam);
		//DWORD XYReader();

	public:
		//void ResetXYPosition() {
		//	lock_guard<mutex> lock(m_mtxScannerImage);
		//	if (!m_imgScanner0.empty())
		//		m_imgScanner0 = 0;
		//	if (!m_imgScanner.empty())
		//		m_imgScanner = 0;
		//}
	};


}
