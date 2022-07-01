#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeCircle.h"

#pragma pack(push, 8)

template<typename T>T __MOD__(T x, T y) {
	if (0. == y)	return x;
    double m= x - y * floor(x/y);
	if (y>0) {
		if (m>=y)	return 0;
		if (m<0) {
			if (y+m == y)	return 0;
			else			return y+m;
		}
	}
    else {
		if (m<=y)		return 0;
		if (m>0) {
			if (y+m == y)	return 0  ; // just in case...
            else			return y+m;
        }
    }
    return m;
}

class AFX_EXT_CLASS_SHAPE CShapeArc : public CShapeCircle {
public:
	double m_dT0;

public:
	CShapeArc(COLORREF cr = CR_DEFAULT) : CShapeCircle(S_ARC, cr) {
		m_dT0 = 0.0;
	}
protected:
	CShapeArc(eSHAPE eShape, COLORREF cr = CR_DEFAULT) : CShapeCircle(eShape, cr) {
		m_dT0 = 0.0;
	}
public:
	CShapeArc(const CShapeArc& B) : CShapeCircle(S_ARC, B.m_cr) {
		*this = B;
	}
	DECLARE_NEWCLONE(CShapeArc);
	DECLARE_DYNAMIC_CREATE(CShapeArc, S_ARC);

	CShapeArc& operator = (const CShapeArc& B) { return (CShapeArc&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);

		const CShapeArc* pB = dynamic_cast<const CShapeArc*>(&B);
		if (!pB)
			return *this;

		m_dT0 = pB->m_dT0;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		if (ar.IsStoring()) {
			ar << m_dT0;
			// Don't : ar << m_dExt;
		} else {
			ar >> m_dT0;
			// Don't : ar >> m_dExt;
		}
		return ar;
	}

	BOOL operator == (const CShapeObject& B) const { return Compare(B); }
	virtual CString Print() const {
		return Format(_T(" (%.3f, %.3f) r(%.3f), t(%.1f -> %.1f deg)"), m_ptCenter.x, m_ptCenter.y, m_dRadius, m_dT0 * 180/M_PI, (m_dT0+m_dTLength) * 180/M_PI);
	}

public:
	// Angle
	double GetStartAngle() const { return m_dT0; }
	double GetEndAngle() const { return m_dT0+m_dTLength; }
	double SetStartAngle(double dT = 0.0) { return m_dT0 = dT; }
	double SetEndAngle(double dT = 0.0) { m_dTLength = m_dT0 + dT; return m_dT0+m_dTLength; }
	void SwapStartEnd() { m_dT0 = m_dT0 + m_dTLength; m_dTLength = -m_dTLength; }
	void SetStartEndAngle(double dT0, double dT1, int eDirection = 1)	// eDirection : 0 : as is, 1 : CCW, -1 : CW
	{
		if (eDirection == 0) {
			m_dT0 = dT0;
			m_dTLength = dT1 - dT0;
		} else if (eDirection > 0) {
			m_dT0 = AdjustAngle(dT0);
			m_dTLength = AdjustAngle(dT1 - dT0);
		} else if (eDirection < 0) {
			m_dT0 = AdjustAngle(dT0);
			m_dTLength = -AdjustAngle(dT1 - dT0);
		}
	}
	CPoint2d GetStartPoint() const {	return m_ptCenter + CPoint2d(cos(m_dT0) * m_dRadius, sin(m_dT0)* m_dRadius); }
	CPoint2d GetEndPoint() const   { return m_ptCenter + CPoint2d(cos(m_dT0+m_dTLength) * m_dRadius, sin(m_dT0+m_dTLength)* m_dRadius); }
	double GetArcLength() const { return m_dRadius * fabs(m_dTLength); }
	void ExtendArcLength(double dLength) {
		//if (m_dRadius <= dLength)
		if (m_dRadius <= 0.0)
			return;
		double dT = 0;
		if (m_dTLength >= 0)
			dT = dLength/m_dRadius;
		else
			dT = -dLength/m_dRadius;
		m_dT0 = AdjustAngle(m_dT0 - dT/2);
		m_dTLength += dT;
	}

	virtual CPoint2d GetCenterPoint() const { return CShapeObject::GetCenterPoint(); }
	virtual void GetPoints(TList<TLineD>& lines) const;
	virtual BOOL SetFromPoints(const TLineD& line) { return FALSE; }
	virtual BOOL GetStartEndPoint(CPoint2d& pt0, CPoint2d& pt1) const {
		CCoordTrans ct;
		double c = cos(m_dT0) * m_dRadius;
		double s = sin(m_dT0) * m_dRadius;
		pt0 = CPoint2d(c, s) + m_ptCenter;
		c = cos(m_dT0 + m_dTLength) * m_dRadius;
		s = sin(m_dT0 + m_dTLength) * m_dRadius;
		pt1 = CPoint2d(c, s) + m_ptCenter;
		return TRUE;
	}
	virtual void GetBoundingRect(TRectD& rect, BOOL bResetRect = TRUE) const;
	static  void GetBoundingRect(TRectD& rect, const CPoint2d& ptCenter, double dRadius, double dT0, double dTLength);

	virtual BOOL Compare(const CShapeObject& B) const {
		if (!__super::Compare(B))
			return FALSE;
		const CShapeArc& _B = (const CShapeArc&)B;
		return (m_dT0 == _B.m_dT0);
	}

	virtual BOOL Draw(ISCanvas& canvas) const;

public:
	// Transform Operations
	virtual void Revert() { double dT1 = AdjustAngle(m_dT0+m_dTLength); __super::Revert(); m_dT0 = dT1; }
	virtual void FlipX(const CPoint2d& ptCenter) {
		__super::FlipX(ptCenter);
		m_dT0 = AdjustAngle(M_PI-m_dT0);
	}
	virtual void FlipY(const CPoint2d& ptCenter) {
		__super::FlipY(ptCenter);
		m_dT0 = AdjustAngle(-m_dT0);
	}
	virtual void Rotate(rad_t dTheta, const CPoint2d& ptCenter) {
		__super::Rotate(dTheta, ptCenter);
		m_dT0 = AdjustAngle(m_dT0+dTheta);
	}
	//virtual void Resize(double dScale, const CPoint2d& ptCenter);
	//virtual void Resize(double dScaleX, double dScaleY, const CPoint2d& ptCenter);
	//virtual BOOL Deflate(double dSizeX, double dSizeY);
	virtual BOOL ClipByRect(const CRect2d& rect, TRefList<CShapeObject>& objects);

	BOOL IsAngleIncluded(double dT) const {
		dT = AdjustAngle(dT);

		if (fabs(m_dTLength) >= M_PI*2)
			return TRUE;

		double dT0 = m_dT0;
		double dT1 = m_dT0 + m_dTLength;
		if (m_dTLength >= 0) {
			if (dT < dT0)
				dT += M_PI*2;
			return (dT >= dT0) && (dT <= dT1);
		} else {
			if (dT > dT0)
				dT -= M_PI*2;
			return (dT <= dT0) && (dT >= dT1);
		}

		ASSERT(FALSE);
		return FALSE;
	}

	BOOL FitArc(const TLineD& pts) { return FitArc(pts, *this); }
	BOOL FitArc(const CPoint2d& ptCenter, const CPoint2d& pt0, const CPoint2d& pt1, int eDirection = 0, BOOL bFitRadius = TRUE) {
		return FitArc(ptCenter, pt0, pt1, eDirection, bFitRadius, *this);
	}

public:
	BOOL FindIntersection(const CPoint2d& pt0, const CPoint2d& pt1, CPoint2d pts[], BOOL bAll = TRUE) const;
	BOOL FindIntersection(const CShapeLine& line, CPoint2d pts[]) const;
	BOOL FindIntersection(const CShapeArc& arc, CPoint2d pts[]) const;

	//<<===
	// by whom?
	CRect2d		CircleBoundingRect() const;
	double		LineAngleR(const CPoint2d& pt0, const CPoint2d& pt1, double dRoundToAngleR) const;
	CPoint2d	NearestPointOnLine(const CPoint2d& ptCenter, const CPoint2d& pt0, const CPoint2d& pt1, double dRoundToAngleR= 0.) const;
	CPoint2d	NearestPointOnLine(const CPoint2d& ptCenter, const CPoint2d& pt0, const CPoint2d& pt1, BOOL bBeyondSegment=FALSE) const;
	BOOL		CircleIntersectWithLine(const CPoint2d& ptCenter, const double& dRadius, const CPoint2d& pt0, const CPoint2d& pt1) const;
	BOOL		ObjectInRectangle(const CPoint2d& pt0, const CPoint2d& pt1) const;
	//>>===
public:
	static double AdjustAngle(double dTheta);
	static BOOL FitArc(const TLineD& pts, CShapeArc& arc);		// 점이 아닌 직선들을 Arc 로 맞춤. pt0 가 시작점, pt1 이 끝점이 됨.
	static BOOL FitArc(const CPoint2d& ptCenter, const CPoint2d& pt0, const CPoint2d& pt1, int eDirection, BOOL bFitRadius, CShapeArc& arc);
												// eDirection :  0 : auto(assume angle is smaller than 180')
												//               1 : positive angle
												//              -1 : negative angle

};


#pragma pack(pop)

