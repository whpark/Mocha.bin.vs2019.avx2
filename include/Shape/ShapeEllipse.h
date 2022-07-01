#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeArc.h"

#pragma pack(push, 8)

class AFX_EXT_CLASS_SHAPE CShapeEllipse : public CShapeArc {
public:
	double& m_dRadius1;
	double m_dRadius2;
	double m_dTFirstAxis;	// in radian. X 축의 시작 각도.

public:
	CShapeEllipse(COLORREF cr = CR_DEFAULT) : CShapeArc(S_ELLIPSE, cr), m_dRadius1(m_dRadius) {
		m_dRadius2 = m_dRadius1;
		m_dTFirstAxis = 0;
	}
protected:
	CShapeEllipse(eSHAPE eShape, COLORREF cr = CR_DEFAULT) : CShapeArc(eShape, cr), m_dRadius1(m_dRadius) {
		m_dRadius2 = m_dRadius1;
		m_dTFirstAxis = 0;
	}
public:
	CShapeEllipse(const CShapeEllipse& B) : CShapeArc(S_ELLIPSE, B.m_cr), m_dRadius1(m_dRadius) {
		*this = B;
	}
	DECLARE_NEWCLONE(CShapeEllipse);
	DECLARE_DYNAMIC_CREATE(CShapeEllipse, S_ELLIPSE);

	CShapeEllipse& operator = (const CShapeEllipse& B) { return (CShapeEllipse&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeEllipse* pB = dynamic_cast<const CShapeEllipse*>(&B);
		if (!pB)
			return *this;
		//m_dRadius1	= pB->m_dRadius1;
		m_dRadius2		= pB->m_dRadius2;
		m_dTFirstAxis	= pB->m_dTFirstAxis;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		if (ar.IsStoring()) {
			//ar << m_dRadius1;	// CShapeArc 에서 처리 함.
			ar << m_dRadius2;
			ar << m_dTFirstAxis;
		} else {
			//ar >> m_dRadius1;	// CShapeArc 에서 처리 함.
			ar >> m_dRadius2;
			ar >> m_dTFirstAxis;
		}
		return ar;
	}

	BOOL operator == (const CShapeObject& B) const { return Compare(B); }
	virtual CString Print() const {
		return Format(_T(" (%.3f, %.3f) r(%.3f, %.3f), t(%.1f -> %.1f deg), Axis(%.1f deg)"),
			m_ptCenter.x, m_ptCenter.y, m_dRadius1, m_dRadius2, m_dT0 * 180/M_PI, (m_dT0+m_dTLength) * 180/M_PI, m_dTFirstAxis);
	}

public:
	virtual void GetPoints(TList<TLineD>& lines) const;
	virtual BOOL SetFromPoints(const TLineD& line) { return FALSE; }
	virtual BOOL GetStartEndPoint(CPoint2d& pt0, CPoint2d& pt1) const {
		CCoordTrans ct;
		ct.SetR(1.0, rad_t(m_dTFirstAxis), CPoint2d(), m_ptCenter);
		double c = cos(m_dT0);
		double s = sin(m_dT0);
		pt0 = ct(CPoint2d(c * m_dRadius1, s * m_dRadius2));
		c = cos(m_dT0 + m_dTLength);
		s = sin(m_dT0 + m_dTLength);
		pt1 = ct(CPoint2d(c * m_dRadius1, s * m_dRadius2));
		return TRUE;
	}
	virtual void GetBoundingRect(TRectD& rect, BOOL bResetRect = TRUE) const;
	static  void GetBoundingRect(TRectD& rect, const CPoint2d& ptCenter, double dRadius1, double dRadius2, double dTFirstAxis, double dT0, double dTLength);

	virtual BOOL Compare(const CShapeObject& B) const {
		if (!__super::Compare(B))
			return FALSE;
		const CShapeEllipse& _B = (const CShapeEllipse&)B;
		return /*(m_dRadius1 == _B.m_dRadius1) && */(m_dRadius2 == _B.m_dRadius2) && (m_dTFirstAxis == _B.m_dTFirstAxis);
	}

public:
	virtual BOOL AddLaserOffset(double dThickness) {
		double dShift = 0;
		if (m_dTLength >= 0)
			dShift =  dThickness;
		else
			dShift = -dThickness;
		m_dRadius1 += dShift;
		m_dRadius2 += dShift;
		return TRUE;
	}
	// Transform Operations
	//virtual void Revert();
	//virtual void AddOffset(const CPoint2d& ptOffset) { m_ptCenter += ptOffset; }
	virtual void FlipX(const CPoint2d& ptCenter) {
		__super::FlipX(ptCenter);
		m_dTFirstAxis = AdjustAngle(M_PI - m_dTFirstAxis);
	}
	virtual void FlipY(const CPoint2d& ptCenter) {
		__super::FlipY(ptCenter);
		m_dTFirstAxis = AdjustAngle(-m_dTFirstAxis);
	}
	virtual void Rotate(rad_t dTheta, const CPoint2d& ptCenter) {
		__super::Rotate(dTheta, ptCenter);
		m_dTFirstAxis = AdjustAngle(m_dTFirstAxis+dTheta);
	}
	virtual void Resize(double dScale, const CPoint2d& ptCenter) {
		__super::Resize(dScale, ptCenter);
		//m_dRadius1 *= dScale;
		m_dRadius2 *= dScale;
	}
	virtual void Resize(double dScaleX, double dScaleY, const CPoint2d& ptCenter) {
		__super::Resize(dScaleX, dScaleY, ptCenter);
		double dRatio = sqrt(dScaleX*dScaleX+dScaleY*dScaleY)/sqrt(2.0);
		//m_dRadius1 *= dRatio;
		m_dRadius2 *= dRatio;
	}
	virtual BOOL Deflate(double dSizeX, double dSizeY) {
		TRectD rect;
		TLineD pts;
		CCoordTrans ct;
		ct.SetR(1.0, rad_t(m_dTFirstAxis), CPoint2d(), m_ptCenter);
		pts.push_back(ct.Trans(CPoint2d( m_dRadius1,  m_dRadius2)));
		pts.push_back(ct.Trans(CPoint2d( m_dRadius1, -m_dRadius2)));
		pts.push_back(ct.Trans(CPoint2d(-m_dRadius1,  m_dRadius2)));
		pts.push_back(ct.Trans(CPoint2d(-m_dRadius1, -m_dRadius2)));
		CShapeObject::GetBoundingRect(rect, pts, TRUE);
		if (rect.IsRectEmpty())
			return FALSE;
		double dW = rect.Width(), dH = rect.Height();
		CPoint2d ptCenter = rect.CenterPoint();
		rect.DeflateRect(dSizeX/2, dSizeY/2);
		if (rect.IsRectEmpty())
			return FALSE;
		double dScaleX = rect.Width()/dW;
		double dScaleY = rect.Height()/dH;
		Resize(dScaleX, dScaleY, ptCenter);
		return TRUE;
	}
	virtual BOOL ClipByRect(const CRect2d& rect, TRefList<CShapeObject>& objects);

public:
	virtual BOOL Draw(ISCanvas& canvas) const;
};


#pragma pack(pop)
