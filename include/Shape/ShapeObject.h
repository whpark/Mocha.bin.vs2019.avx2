#pragma once

#include "AFX_EXT_SHAPE.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "TList.hpp"
#include "xMathUtil/CoordTrans.h"
#include "misc/Profile.h"

class ISCanvas;

#pragma pack(push, 8)

#ifndef _CPPRTTI
#error Please, Complie with /GR option. (Project Property -> C/C++ -> Language -> Enable Runtime Type Information)
#endif

enum eSHAPE_HATCHING {
	SH_NONE,
	SH_L2R = BIT(1),	// Left to Right
	SH_T2B = BIT(2),	// Top to Bottom
	SH_LT2RB = BIT(3),	// LeftTop to RightBottom
	SH_LB2RT = BIT(4),	// LeftBottom to RightTop
};

class CShapeObject;
class CShapeGroup;
class CShapeCollection;
class CShapeDot;
class CShapeVertex;
class CShapeLine;
class CShapePolyLine;
class CShapeSpline;
class CShapeCircle;
class CShapeArc;
class CShapeEllipse;
class CShapeComplex;
class CShapeText;
class CShapeDMTX;
class CShapeCrossLine;

typedef std::vector<CPoint2d> TLineD;
typedef std::vector<CPoint2i> TLineI;

//=============================================================================
// CShapeObject

class AFX_EXT_CLASS_SHAPE CShapeObject {
public:
	enum eSHAPE { S_NONE, S_GROUP, S_COLLECTION, S_DOT, S_VERTEX, S_LINE, S_POLY_LINE, S_SPLINE, S_CIRCLE, S_ARC, S_ELLIPSE, S_COMPLEX, S_TEXT, S_DMTX, S_CROSS_LINE, nSHAPE };
						// S_GROUP : 단순 집합. 
						// S_COLLECTION : DXF Layer. Laser Recipe 를 가지고 있음.

	enum eCR { CR_DEFAULT = RGB(255, 255, 255), };

	// Shape Type. ( or RTTI ? )
private:
	eSHAPE m_eShape;
protected:
	COLORREF m_cr;

	DWORD m_eHatching;
	double m_dHatching;
public:
	// Temporary
	void* m_pCookie;
	CString m_strCookie;

	// Constructor
private:
	CShapeObject() {
		m_cr = 0;
		m_eHatching = SH_NONE;
		m_dHatching = 0.0;
		m_pCookie = nullptr;
	}
public:
	CShapeObject(COLORREF cr) : m_eShape(S_NONE), m_cr(cr) {
		m_eHatching = SH_NONE;
		m_dHatching = 0.0;
		m_pCookie = nullptr;
	}
protected:
	CShapeObject(eSHAPE eShape, COLORREF cr = CR_DEFAULT) : m_eShape(eShape), m_cr(cr) {
		m_eHatching = SH_NONE;
		m_dHatching = 0.0;
		m_pCookie = nullptr;
	}
public:
	CShapeObject(const CShapeObject& B) : m_eShape(S_NONE) {
		*this = B;
	}
	virtual ~CShapeObject(void) {
	}

public:
	DECLARE_NEWCLONE_BASE_INTERFACE(CShapeObject)
	DECLARE_DYNAMIC_CREATE_BASE_INTERFACE(CShapeObject, eSHAPE, int)
	DECLARE_DYNAMIC_SERIALIZE_BASE(CShapeObject)

public:
	// default assign operator
	CShapeObject& operator = (const CShapeObject& B) { return CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		m_eShape = B.m_eShape;
		m_cr = B.m_cr;
		m_eHatching = B.m_eHatching;
		m_dHatching = B.m_dHatching;
		m_pCookie = B.m_pCookie;
		m_strCookie = B.m_strCookie;
		return *this;
	}
	virtual CShapeObject& CopyAttrFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		m_cr = B.m_cr;
		m_eHatching = B.m_eHatching;
		m_dHatching = B.m_dHatching;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		if (ar.IsStoring()) {
			ar << m_cr;
			ar << m_eHatching;
			ar << m_dHatching;
			//ar << m_strCookie;
		} else {
			ar >> m_cr;
			ar >> m_eHatching;
			ar >> m_dHatching;
			//ar >> m_strCookie;
		}
		return ar;
	}

public:
	BOOL operator == (const CShapeObject& B) const { return Compare(B); }

	// Attributes
	static LPCTSTR GetShapeName(eSHAPE eShape);
	virtual LPCTSTR GetShapeName() const;
	eSHAPE GetShape() const { return m_eShape; }
	COLORREF GetColor() const { return m_cr; }
	virtual COLORREF SetColor(COLORREF cr);

	virtual DWORD GetHatching() const { return m_eHatching; }
	virtual double GetHatchingDensity() const { return m_dHatching; }
	virtual void SetHatching(DWORD eHatching = SH_NONE, double dHatching = 0.0) { m_eHatching = eHatching; m_dHatching = dHatching; }

	virtual CString Print() const = NULL;

public:
	// Operations
	virtual CPoint2d GetCenterPoint() const { TRectD rect; GetBoundingRect(rect); return rect.CenterPoint(); };
private:
	void GetPointsConst(TList<TLineD>& lines) const { GetPoints(lines); }	// Helper
public:
	virtual void GetPoints(TList<TLineD>& lines) const = NULL;						// Bounding Rect계산용. Add Points incrementally. DO NOT RESET pts (DO NOT CALL pts.DeleteAll())
	virtual void GetPoints(TList<TLineD>& lines) { GetPointsConst(lines) ; }			// Bounding Rect계산용. Add Points incrementally. DO NOT RESET pts (DO NOT CALL pts.DeleteAll())
	virtual BOOL SetFromPoints(const TLineD& line) { return FALSE; }
	virtual void GetBoundingRect(TRectD& rect, BOOL bResetRect = TRUE) const;
	virtual BOOL GetStartEndPoint(CPoint2d& pt0, CPoint2d& pt1) const = NULL;
	virtual BOOL Compare(const CShapeObject& B) const;

	virtual BOOL AddLaserOffset(double dThickness) = NULL;

	// Transform Operations
	virtual void Revert() = NULL;														// 가공 순서 Reverse
	virtual void AddOffset(const CPoint2d& ptOffset) = NULL;								// Object Center 이동.
	virtual void FlipX(const CPoint2d& ptCenter) = NULL;									// Object Center 중심으로 Y축 반전. (X값 변경)
	virtual void FlipY(const CPoint2d& ptCenter) = NULL;									// Object Center 중심으로 X축 반전. (Y값 변경)
	virtual void Rotate(rad_t dTheta, const CPoint2d& ptCenter) = NULL;					// Object Center 중심으로 회전
	virtual void Resize(double dScale, const CPoint2d& ptCenter) = NULL;					// Object Center 중심으로 크기 변환
	virtual void Resize(double dScaleX, double dScaleY, const CPoint2d& ptCenter) = NULL;// Object Center 중심으로 크기 변환
	virtual BOOL Deflate(double dSizeX, double dSizeY) = NULL;							// 줄어드는 양. (원 Size가 10일경우, 2을 넣으면 8가 됨. 양쪽으로 2 씩 줄일 경우, *2 해서, 4를 넣어야 함)
	virtual BOOL ClipByRect(const CRect2d& rect, TRefList<CShapeObject>& objects) = NULL;	// by 이범호
	enum eSORT_DIRECTION {
		SD_AS_IS, SD_ANY,
		SD_RIGHT, SD_LEFT, SD_UP, SD_DOWN,
		SD_RIGHT_UP, SD_RIGHT_DOWN, SD_LEFT_UP, SD_LEFT_DOWN,
		SD_UP_RIGHT, SD_UP_LEFT, SD_DOWN_RIGHT, SD_DOWN_LEFT,
		SD_CCW, SD_CW,
		nSD
	};
	virtual void ChangeDirection(eSORT_DIRECTION eDirection, const CPoint2d& ptCenter = CPoint2d()) = NULL;
	static LPCTSTR GetDirectionName(eSORT_DIRECTION eDirection);

public:
	// Drawing
	virtual BOOL Draw(ISCanvas& canvas) const = NULL;

public:
	// Hepler
	static BOOL GetBoundingRect(TRectD& rect, const CPoint2d& pt, BOOL bResetRect = TRUE);
	static BOOL GetBoundingRect(TRectD& rect, const TLineD& pts, BOOL bResetRect = TRUE);
	static BOOL GetBoundingRect(TRectD& rect, const TList<TLineD>& lines, BOOL bResetRect = TRUE);
	static BOOL GetBoundingRect(TRectD& rect, const TRectD& rectObject, BOOL bResetRect = TRUE);

	static BOOL MergeLines(TRefList<CShapeObject>& objects, double dMaxSizeX = 0.0, double dMaxSizeY = 0.0);	// 시작/끝점이 겹치는 line object 를 하나로 합침
};

typedef CShapeObject* PShapeObject;

//-----------------------------------------------------------------------------

AFX_EXT_API_SHAPE int CompareObjectTolerance_PXPY(const CShapeObject& ob1, const CShapeObject& ob2, void* pdToleranceLineDistance);
AFX_EXT_API_SHAPE int CompareObjectTolerance_PXNY(const CShapeObject& ob1, const CShapeObject& ob2, void* pdToleranceLineDistance);
AFX_EXT_API_SHAPE int CompareObjectTolerance_NXPY(const CShapeObject& ob1, const CShapeObject& ob2, void* pdToleranceLineDistance);
AFX_EXT_API_SHAPE int CompareObjectTolerance_NXNY(const CShapeObject& ob1, const CShapeObject& ob2, void* pdToleranceLineDistance);
AFX_EXT_API_SHAPE int CompareObjectTolerance_PYPX(const CShapeObject& ob1, const CShapeObject& ob2, void* pdToleranceLineDistance);
AFX_EXT_API_SHAPE int CompareObjectTolerance_PYNX(const CShapeObject& ob1, const CShapeObject& ob2, void* pdToleranceLineDistance);
AFX_EXT_API_SHAPE int CompareObjectTolerance_NYPX(const CShapeObject& ob1, const CShapeObject& ob2, void* pdToleranceLineDistance);
AFX_EXT_API_SHAPE int CompareObjectTolerance_NYNX(const CShapeObject& ob1, const CShapeObject& ob2, void* pdToleranceLineDistance);

AFX_EXT_API_SHAPE int CompareObjectX(const CShapeObject& ob1, const CShapeObject& ob2);
AFX_EXT_API_SHAPE int CompareObjectY(const CShapeObject& ob1, const CShapeObject& ob2);
AFX_EXT_API_SHAPE int CompareObjectXD(const CShapeObject& ob1, const CShapeObject& ob2, void* pdToleranceLineDistance);
AFX_EXT_API_SHAPE int CompareObjectYD(const CShapeObject& ob1, const CShapeObject& ob2, void* pdToleranceLineDistance);
AFX_EXT_API_SHAPE int CompareArcX(const CShapeObject& arc1, const CShapeObject& arc2);
AFX_EXT_API_SHAPE int CompareArcY(const CShapeObject& arc1, const CShapeObject& arc2);
AFX_EXT_API_SHAPE int CompareLineX(const CShapeObject& line1, const CShapeObject& line2);
AFX_EXT_API_SHAPE int CompareLineY(const CShapeObject& line1, const CShapeObject& line2);
AFX_EXT_API_SHAPE int CompareLineXD(const CShapeObject& line1, const CShapeObject& line2, void* pdToleranceLineDistance);
AFX_EXT_API_SHAPE int CompareLineYD(const CShapeObject& line1, const CShapeObject& line2, void* pdToleranceLineDistance);

//-----------------------------------------------------------------------------
BOOL AFX_EXT_API_SHAPE AddLaserOffsetToLine(const TLineD& pts, TLineD& ptsResult, double dThickness, BOOL bLoop);

#pragma pack(pop)
