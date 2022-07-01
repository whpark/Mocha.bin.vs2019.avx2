#pragma once

#include "Scanner/ScannerController.h"

class CScannerCtrl;
class CScannerControllerRTC3 : public CScannerController {
private:
	volatile static long s_nRTC3Loaded;
public:
	CString m_str1to1CalibrationPath;

public:
	CScannerControllerRTC3(void);
	virtual ~CScannerControllerRTC3(void);

public:
	virtual BOOL Init();
	virtual BOOL OpenDevice();
	virtual BOOL CloseDevice();

public:
	virtual void SetBoardIndex(unsigned short usRTCBoardNo = 0);
	virtual BOOL ResetCalibration(int iCorrectNo);
	virtual BOOL LoadCalibrationFile(int iCorrectNo, LPCTSTR pszPath = NULL);
	virtual BOOL LoadProgramFile(LPCTSTR pszPath = NULL);
	virtual BOOL SelectRTC(UINT iCard = NULL);
	virtual BOOL SetWobbelDisable();
	virtual BOOL GetStatus() {return IsDrawEnd();};
	virtual BOOL IsScannerError() { return FALSE; }; //GetErors
	
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

	virtual BOOL SetAnalogPinOut(int iValue/*0 to 1024*/);
	virtual BOOL SetMetrix(double m00,double m01,double m10,double m11) {return TRUE;};
	virtual BOOL SetAutoMirroCorrection(){ return TRUE;};
	virtual BOOL StartAutoMirroCorrection() { return TRUE;};

	// Double buffer function
	virtual BOOL	DrawExecute();
	virtual BOOL	DrawEndExecute();
	virtual BOOL	DrawListStatus(int iList);
	virtual BOOL	CurrentListCheck(int iList);

	//virtual BOOL SetListIndex(int iList = 1);
public:
	virtual BOOL ExecuteSetting(int nCorrectNo=0);
	virtual BOOL LaserFire(int iOnOff = 0);
	//BOOL Wait(DWORD dwTimeout);
protected:
	virtual void MoveToImpl(const TPointD& pt) { ShapeAddJumpPoint(pt); };
	virtual void LineToImpl(const TPointD& pt) { ShapeAddMarkPoint(pt); };
	virtual void Arc(const TPointD& ptCenter, double dRadius, double dT0, double dTLength);
	virtual void Dot(const TPointD& pt, DWORD dwDuration = 0 /* in usec */) {
		ShapeAddDot(pt, dwDuration);
	}
	
};
