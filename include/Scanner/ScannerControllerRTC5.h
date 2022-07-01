#pragma once

#include "Scanner/ScannerController.h"

class CScannerCtrl;

class CScannerControllerRTC5 : public CScannerController {
private:
	volatile static long s_nRTC5Loaded;
public:
	CString m_str1to1CalibrationPath;

public:
	CScannerControllerRTC5(void);
	virtual ~CScannerControllerRTC5(void);

public:
	virtual BOOL Init();
	virtual BOOL OpenDevice();
	virtual BOOL CloseDevice();

public:
	virtual void SetBoardIndex(unsigned short usRTCBoardNo = 0);
	virtual BOOL ResetCalibration(int iCorrectNo);
	virtual BOOL LoadCalibrationFile(int iCorrectNo, LPCTSTR pszPath = NULL);
	virtual BOOL LoadProgramFile(LPCTSTR pszPath = NULL);

	virtual BOOL SelectRTC(UINT iCard);
	virtual BOOL GetStatus() {return IsDrawEnd();};
	virtual BOOL IsScannerError(); //GetErors
public:
	virtual BOOL OnDrawStart();
	virtual BOOL OnDrawEnd();
	virtual BOOL OnDrawWait();
	virtual BOOL OnDrawPause();
	virtual BOOL OnDrawStop();
	virtual BOOL OnDrawContinue();
	virtual BOOL OnDrawBeforeStep(const CShapeObject* pObject=NULL);
	virtual BOOL OnDrawAfterStep(const CShapeObject* pObject=NULL);

	virtual BOOL IsDrawEnd();	// RTC Only

	virtual BOOL ShapeAddJumpPoint(const TPointD& pt);
	virtual BOOL ShapeAddMarkPoint(const TPointD& pt);
	virtual BOOL ShapeAddDot(const TPointD& pt, DWORD dwDuration /* in usec */);
	virtual BOOL SetMarkingSpeed(double dSpeed);
	virtual BOOL SetJumpingSpeed(double dSpeed);
	virtual BOOL SetWobbelDisable();
	

	virtual BOOL SetAnalogPinOut(int iValue/*0 to 1024*/);
	virtual BOOL SetMetrix(double m00,double m01,double m10,double m11);
	virtual BOOL SetAutoMirroCorrection();
	virtual BOOL StartAutoMirroCorrection();
	
	// Double buffer function
	virtual BOOL DrawExecute();
	virtual BOOL DrawEndExecute();
	virtual BOOL DrawListStatus(int iList);
	virtual BOOL CurrentListCheck(int iList);


	//virtual BOOL SetListIndex(int iList = 1);
public:
	virtual BOOL ExecuteSetting(int nCorrectNo=0);
	virtual BOOL LaserFire(int iOnOff = 0);
	
	//BOOL Wait(DWORD dwTimeout);
protected:
	virtual void MoveToImpl(const TPointD& pt) { ShapeAddJumpPoint(pt); };
	virtual void LineToImpl(const TPointD& pt) { ShapeAddMarkPoint(pt); };
	virtual void Arc(const TPointD& ptCenter, double dRadius, double dT0, double dTLength);
	virtual void Ellipse(const TPointD& ptCenter, double dRadius1, double dRadius2, double dTFirstAxis, double dT0, double dTLength);

	virtual void GotoXY(const CPoint2d& pt);
};
