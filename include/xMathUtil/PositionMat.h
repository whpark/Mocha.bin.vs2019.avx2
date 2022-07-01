﻿#pragma once

#include "AFX_EXT_XMU.h"

#include "Coordinate.hpp"

#define EXT_POINTS		_T(".points")
#define EXT_CTB			_T(".ctb")
#define EXT_CTB5		_T(".ct5")
#define EXT_TSC			_T(".tsc")
#define EXT_SUPER		_T(".super")
#define EXT_TARGET		_T(".target")

#define CTB_RANGE		BIT(16)
#define CTB5_RANGE		BIT(20)
#define TSC_RANGE		BIT(20)
#define CTB_POINTS		(BIT(6)+1)
#define CTB5_POINTS		(BIT(8)+1)
#define TSC_POINTS		(BIT(6)+1)

#pragma pack(push, 8)

//=============================================================================
// CPositionMat
//
class AFX_EXT_CLASS_XMU CPositionMat {
public:

public:
	class __declspec(align(4)) __declspec(novtable) CCTB5Header {
	public:
		enum eCOORD_SYSTEM {
			CS_UNKNOWN = -1, CS_STANDARD, CS_10MM, CS_12MM, CS_14MM, CS_SK1020, CS_POWER33, CS_POWER50, CS_POWER70, CS_POWER95, CS_SPECIAL,
			CS_7MM, CS_HS7, CS_PALM_SCAN, CS_HURRY_20, CS_HURRY_25, CS_HURRY_30, 
		};
	protected:
		BOOL m_b3Dimension;
		UINT32 m_iBitPerMM;						// (uint32) Bit/mm. This parameter generally a integer-multiple of 16. (when only converted from CTB)
		UINT32 m_iWorkingDistance;				// (float)  mm, (up to 3 decimal places)
		UINT32 m_iStretchX, m_iStretchY;		// Image field change with Z
												//   for 3D vector commands this results in a Z-coordinate-dependent modification of the image field

		UINT32 m_iZCorrectionOffset;			// (float) Offset Part [ +- 26 bit ]
		UINT32 m_iZCorrectionLinear;			// (float) Linear Part [ +- 11 bit ]
		UINT32 m_iZCorrectionSquare;			// (float) Square Part [ +-  4 bit ]
		DWORD m_dwFileNo;						// for ab initio calculations: generated by SCANLAB;
												// for file conversion: derived from the file name of the converted file 
												//     (digit sequence directly after the firtst underscore)

		UINT32 m_eCorrection;					// (m_eCorrection / 10) == 0 : Correction w/o F-Theta Objective.
												//                         1 : Correction w/  F-Theta Objective.
												// (m_eCorrection % 10) == 0 : no distortion
												//                         1 : f progression
												//                         2 : height table

		UINT32 m_eSource;						// (m_eSource / 10000) == 1 : Original file (as originally calculated)
												//                        2 : converted from ctb
												//                        3 : reconstructed from txt
												// (m_eSource % 10000) : Version number of the program used to create the correction file

		UINT32 m_eInverseTable;					// (m_eInverseTable & 0x01) : valid inverse table is present
												// (m_eInverseTable & 0x02) == 0 : inverse table caculated ab initio
												//                             1 : inverse table numerically caculated
		
		UINT32 m_iCalibrationGradient;			// (float) [Grad], Mechanical angle deflection in [+- deg] for 96% of maximum deflection
												// (기울기 dy/dx)

		eCOORD_SYSTEM m_eCoordSystem;			// (uint32)	// eCOORD_SYSTEM, CS_UNKNOWN, CS_STANDARD ...

		UINT32 m_iProtectionGlass;				// (float) Protective glass: Thickness [mm] (2 decimal places), Refrection index (3 decmial places)
												// (m_dParameter / 1000000) : Protective Glass Thickness in mm (max 2 decmial places)
												// (m_dParameter % 10000)    : refraction index (max 3 decimal places)
												// ex,  3,521,450 -> 3.52(t), refraction index 1.450.

		BOOL m_bFieldSizeClipped;				// 

	public:
		CCTB5Header() { Init(); }
		~CCTB5Header() {}
	public:
		void Init()											{ memset(this, 0, sizeof(*this)); }

		BOOL Is3Dimension() const							{ return m_b3Dimension; }
		void Set3Dimension(BOOL b3Dimension = TRUE)			{ m_b3Dimension = b3Dimension; }

		UINT32 GetBitPerMM() const							{ return m_iBitPerMM; }
		void SetBitPerMM(UINT32 iBitPerMM) 					{ m_iBitPerMM = iBitPerMM; }

		float GetWorkingDistance() const					{ return m_iWorkingDistance/20.f; }
		void SetWorkingDistance(float dWorkingDistance)		{ m_iWorkingDistance = _round(dWorkingDistance/20.f); }

		float GetStretchX() const							{ return m_iStretchX/1.f; }
		float GetStretchY() const							{ return m_iStretchY/1.f; }
		void SetStretchX(float dStretchX) 					{ m_iStretchX = _round_uint(dStretchX); }
		void SetStretchY(float dStretchY) 					{ m_iStretchY = _round_uint(dStretchY); }

		float GetZCorrectionOffset() const					{ return m_iZCorrectionOffset/1.f; }
		float GetZCorrectionLinear() const					{ return m_iZCorrectionLinear/1.f; }
		float GetZCorrectionSquare() const					{ return m_iZCorrectionSquare/1.f; }
		void SetZCorrectionOffset(float dZCorrectionOffset) { m_iZCorrectionOffset = _round_uint(dZCorrectionOffset); }
		void SetZCorrectionLinear(float dZCorrectionLinear) { m_iZCorrectionLinear = _round_uint(dZCorrectionLinear); }
		void SetZCorrectionSquare(float dZCorrectionSquare) { m_iZCorrectionSquare = _round_uint(dZCorrectionSquare); }

		DWORD GetFileNo() const								{ return m_dwFileNo; }
		void SetFileNo(DWORD dwFileNo) 						{ m_dwFileNo = dwFileNo; }

		BOOL IsCorrectionWithFTheta() const					{ return (m_eCorrection / 10); }
		int GetCorrectionType() const						{ return m_eCorrection % 10; }
		void SetCorrectionWithFTheta(BOOL bWithFTheta) 		{ m_eCorrection = (bWithFTheta*10) + (m_eCorrection%10); }
		void SetCorrectionType(int eType) 					{ m_eCorrection = (m_eCorrection/10)*10 + (eType%10); }

		BOOL IsSourceOriginal() const						{ return m_eSource / 10000 == 1; }
		BOOL IsSourceCTB() const							{ return m_eSource / 10000 == 2; }
		BOOL IsSourceText() const							{ return m_eSource / 10000 == 3; }
		int GetVersion() const								{ return m_eSource % 10000; }
		void SetSourceType(int eSourceType)					{ if ((eSourceType >= 1) && (eSourceType <= 3)) m_eSource = (eSourceType*10000) + (m_eSource%10000); }
		void SetVersion(int eVersion) 						{ m_eSource = (m_eSource/10000)*10000 + (eVersion%10000); }

		BOOL IsInverseTableExists() const					{ return m_eInverseTable & BIT(0); }
		BOOL IsInverseTableNumericallyCalculated() const	{ return m_eInverseTable & BIT(1); }
		void SetInverseTableExists(BOOL bExists)			{ m_eInverseTable = ((~BIT(0)) & m_eInverseTable) | (bExists ? 1 : 0); }
		void SetInverseTableNumericallyCalculated(BOOL bNumericallyCalculateded)
															{ m_eInverseTable = ((~BIT(1)) & m_eInverseTable) | (bNumericallyCalculateded ? BIT(1) : 0); }

		float GetCalibrationGradient() const				{ return m_iCalibrationGradient/1000.f; }	// in GRAD (기울기 dy/dx)
		void SetCalibrationGradient(float dCalibrationGradient)
															{ m_iCalibrationGradient = _round_uint(dCalibrationGradient*1000.f); }

		eCOORD_SYSTEM GetCoordSystem() const				{ return m_eCoordSystem; }
		void SetCoordSystem(eCOORD_SYSTEM eCS) 				{ m_eCoordSystem = eCS; }

		float GetProtectiveGlassThickness() const			{ return (m_iProtectionGlass/10000)/100.f; }
		float GetProtectiveGlassRefractionIndex() const		{ return (m_iProtectionGlass%10000)/1000.f; }

		void SetProtectiveGlassThickness(float dThickness)	{ m_iProtectionGlass = ((int)(dThickness * 100))*10000 + (m_iProtectionGlass%10000); }
		void SetProtectiveGlassRefractionIndex(float dRefractionIndex)
															{ m_iProtectionGlass = (m_iProtectionGlass/10000)*10000 + ((int)(dRefractionIndex*1000))%10000; }

		BOOL IsFieldSizeClipped() const						{ return m_bFieldSizeClipped; }
		void SetFieldSizeClipped(BOOL bClipped) 			{ m_bFieldSizeClipped = bClipped; }
	};
	CCTB5Header m_headerCTB5;
	CSize2d m_ptRange;
	CSize2d m_sizeAvailable;
	cv::Mat m_mpt;
	cv::Mat m_mptInv;

	CPoint2d m_ptsAlign[4];
	cv::Mat m_mpt2;

	CPoint2d m_scale;	// for 'target' only. (HANS)

	struct {
		axis::eDIRECTION eDX, eDY;
	} m_axis;


protected:
	mutable CString m_strPathControlPoint;

public:
	CPositionMat(void);
	~CPositionMat(void);
	CPositionMat(const CPositionMat& B) { *this = B; }
	CPositionMat& operator = (const CPositionMat& B) {
		if (this != &B) {
			m_ptRange = B.m_ptRange;
			m_sizeAvailable = B.m_sizeAvailable;
			m_headerCTB5 = B.m_headerCTB5;
			B.m_mpt.copyTo(m_mpt);
			B.m_mptInv.copyTo(m_mptInv);
			m_strPathControlPoint = B.m_strPathControlPoint;
			m_axis = B.m_axis;
		}
		return *this;
	}

public:
	LPCTSTR GetPathControlPoint() const { return m_strPathControlPoint; }

	BOOL Init();
	BOOL Load(LPCTSTR pszPathControlPoint = NULL);
	BOOL Save(LPCTSTR pszPathControlPoint = NULL) const;

	BOOL LoadPoints(LPCTSTR pszPathControlPoint);
	BOOL SavePoints(LPCTSTR pszPathControlPoint) const;
	BOOL LoadMultiPoints(LPCTSTR pszPathControlPoint);
	BOOL SaveMultiPoints(LPCTSTR pszPathControlPoint) const;
	BOOL LoadCTB(LPCTSTR pszPathControlPoint);
	BOOL SaveCTB(LPCTSTR pszPathControlPoint) const;
	BOOL LoadCTB5(LPCTSTR pszPathControlPoint);
	BOOL SaveCTB5(LPCTSTR pszPathControlPoint) const;
	BOOL LoadTSC(LPCTSTR pszPathControlPoint);
	BOOL SaveTSC(LPCTSTR pszPathControlPoint) const;
	BOOL LoadSuper(LPCTSTR pszPathControlPoint);
	BOOL SaveSuper(LPCTSTR pszPathControlPoint) const;
	BOOL LoadTarget(LPCTSTR pszPathControlPoint);
	BOOL SaveTarget(LPCTSTR pszPathControlPoint) const;

	double CalcSkew();
	BOOL GetBoundingRect(TRectD& rectBounding);

	BOOL MoveToCenter();
	BOOL Rotate90CCW();
	BOOL Rotate90CW();
	BOOL Rotate(rad_t dTheta);
	BOOL ExchangeXY();
	BOOL Transpose();
	BOOL NegX();
	BOOL NegY();
	BOOL FlipX();
	BOOL FlipY();
	BOOL FlipXY();

	BOOL AdjustCoordSystem(axis::eDIRECTION eDX, axis::eDIRECTION eDY);
};

#pragma pack(pop)