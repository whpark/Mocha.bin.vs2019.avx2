
#pragma once

#include "AFX_EXT_XMU.h"

#include "Coordinate.hpp"

#include "MatHelper.h"

#include "PositionMat.h"

#pragma pack(push, 8)

// 두 직선의 교점
AFX_EXT_API_XMU bool FindCrossPoint(const CPoint2d& ptA1, const CPoint2d& ptA2, const CPoint2d& ptB1, const CPoint2d& ptB2, CPoint2d& ptCross, bool bInnerPoint = false);
AFX_EXT_API_XMU bool FindCrossPoint(const CPoint3d& ptA1, const CPoint3d& ptA2, const CPoint3d& ptB1, const CPoint3d& ptB2, CPoint3d& ptCross, bool bInnerPoint = false, double dMaxError = 1.e-3);

// Circle 중심점
AFX_EXT_API_XMU BOOL FindCircleCenter(const CPoint2d& pt0, const CPoint2d& pt1, const CPoint2d& pt2, CPoint2d& ptCenter);

// 한 직선과 한 점의 수선의 발
AFX_EXT_API_XMU BOOL FindPerpendicularFoot(const CPoint2d& ptLine1, const CPoint2d& ptLine2, const CPoint2d& pt, CPoint2d& ptPerpendicularFoot);

inline double LineFunction_GetY(const cv::Vec4f& line, double x) { CPoint2d v(line[0], line[1]); CPoint2d p0(line[2], line[3]); return (((((x) - p0.x) * v.y) / v.x) + p0.y); }
inline double LineFunction_GetX(const cv::Vec4f& line, double y) { CPoint2d v(line[0], line[1]); CPoint2d p0(line[2], line[3]); return (((((y) - p0.y) * v.x) / v.y) + p0.x); }

typedef enum : int {
	SCT_NONE, SCT_RTC3, SCT_RTC4, SCT_RTC5, SCT_RTC6, SCT_TSC, SCT_HANS,
} eSCANNER_CONTROLLER_TYPE;

inline eSCANNER_CONTROLLER_TYPE GetScannerControllerType(LPCTSTR pszName) {
	static struct {
		TCHAR const* pszName;
		eSCANNER_CONTROLLER_TYPE eScannerControllerType;
	} pairs[] = {
		{ _T("RTC3"),	SCT_RTC3 },
		{ _T("RTC4"),	SCT_RTC4 },
		{ _T("RTC5"),	SCT_RTC5 },
		{ _T("RTC6"),	SCT_RTC6 },
		{ _T("TSC"),	SCT_TSC },
		{ _T("HANS"),	SCT_HANS },
	};

	if (!pszName)
		return SCT_NONE;

	for (const auto& pair : pairs) {
		if (_tcsicmp(pair.pszName, pszName) == 0)
			return pair.eScannerControllerType;
	}

	return SCT_NONE;
}

// Make CTB
AFX_EXT_API_XMU BOOL MakeCTB(
	CPositionMat& ptCTB,									// [out] result Calibration Matrix
	bool bSingleScale,										// [in] default : true. ScaleX, ScaleY를 같이 계산할지 따로 따로 계산할지 선택
	double& dScaleX,										// [out] Scale X, 
	double& dScaleY,										// [out] Scale Y
	CString& strMessage,									// [out] Result Message

	const CPositionMat& pmSource,							// [in] Source Position
	double dScaleRate = 1.0,								// [in] optional (dScaleX = dScaleX*ScaleRate, dScaleY = dScale*dScaleRate)
	axis::eDIRECTION eDX = axis::D_X, axis::eDIRECTION eDY = axis::D_Y,				// [in] Scanner X/Y Axis -> Machine X/Y Axis
	BOOL bLocallyFit = TRUE, BOOL bLocallyFitOuter = FALSE,	// [in] 
	int nOrder = 4, int nOrderOuter = 4,
	int* pNOrder = NULL, int* pNOrderOuter = NULL,			// [out] optional
	const CPositionMat& pmSourceCTB = CPositionMat(),		// [in] optional, Source Calibration File. if 1to1 used, NOT needed.
	eSCANNER_CONTROLLER_TYPE eScannerControllerType = SCT_RTC5,										// [in] optional, if TRUE output is CTB5.
	BOOL bCTB5CalcInverseTable = TRUE,						// [in] optional, valid if CTB5 is true, 
	LPCTSTR pszDistanceError = _T("Error.txt")				// [in] optional, Reports Distnace Error to the file [pszDistanceError].
	);

#pragma pack(pop)
