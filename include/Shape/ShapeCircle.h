#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeObject.h"

#pragma pack(push, 8)


class AFX_EXT_CLASS_SHAPE CShapeCircle : public CShapeObject {
public:
	TRefPointer<CPoint2d> m_rpt;
	CPoint2d& m_ptCenter;
	double m_dRadius;
	double m_dTLength;

public:
	CShapeCircle(COLORREF cr = CR_DEFAULT) : CShapeObject(S_CIRCLE, cr), m_rpt(new CPoint2d), m_ptCenter(*m_rpt), m_dRadius(0), m_dTLength(M_PI*2) {
		//m_ptCenter.x = 0;
		//m_ptCenter.y = 0;
	}
protected:
	CShapeCircle(eSHAPE eShape, COLORREF cr = CR_DEFAULT) : CShapeObject(eShape, cr), m_rpt(new CPoint2d), m_ptCenter(*m_rpt), m_dRadius(0), m_dTLength(M_PI*2) {
		//m_ptCenter.x = 0;
		//m_ptCenter.y = 0;
	}
public:
	CShapeCircle(const CShapeCircle& B) : CShapeObject(S_CIRCLE, B.m_cr), m_rpt(new CPoint2d), m_ptCenter(*m_rpt) {
		*this = B;
	}
	DECLARE_NEWCLONE(CShapeCircle);
	DECLARE_DYNAMIC_CREATE(CShapeCircle, S_CIRCLE);

	CShapeCircle& operator = (const CShapeCircle& B) { return (CShapeCircle&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeCircle* pB = dynamic_cast<const CShapeCircle*>(&B);
		if (!pB)
			return *this;

		m_ptCenter	= pB->m_ptCenter;
		m_dRadius	= pB->m_dRadius;
		m_dTLength	= pB->m_dTLength;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		if (ar.IsStoring()) {
			ar << m_ptCenter << m_dRadius << m_dTLength;
		} else {
			ar >> m_ptCenter >> m_dRadius >> m_dTLength;
		}
		return ar;
	}

	BOOL operator == (const CShapeObject& B) const { return Compare(B); }
	virtual CString Print() const {
		return Format(_T(" (%.3f, %.3f) r(%.3f)"), m_ptCenter.x, m_ptCenter.y, m_dRadius );
	}

public:
	virtual CPoint2d GetCenterPoint() const { return m_ptCenter; }
	virtual void GetPoints(TList<TLineD>& lines) const;
	virtual BOOL SetFromPoints(const TLineD& line) { return FALSE; }
	virtual BOOL GetStartEndPoint(CPoint2d& pt0, CPoint2d& pt1) const {
		pt0 = pt1 = m_ptCenter + CPoint2d(m_dRadius, 0);
		return TRUE;
	}
	virtual void GetBoundingRect(TRectD& rect, BOOL bResetRect = TRUE) const {
		if (bResetRect) {
			rect.SetRect(m_ptCenter.x-m_dRadius, m_ptCenter.y-m_dRadius, m_ptCenter.x+m_dRadius, m_ptCenter.y+m_dRadius);
		} else {
			CShapeObject::GetBoundingRect(rect, CPoint2d(m_ptCenter.x-m_dRadius, m_ptCenter.y-m_dRadius), FALSE);
			CShapeObject::GetBoundingRect(rect, CPoint2d(m_ptCenter.x+m_dRadius, m_ptCenter.y+m_dRadius), FALSE);
		}
	}

	virtual BOOL Compare(const CShapeObject& B) const {
		if (!__super::Compare(B))
			return FALSE;
		const CShapeCircle& _B = (const CShapeCircle&)B;
		return (m_ptCenter == _B.m_ptCenter) && (m_dRadius == _B.m_dRadius) && (m_dTLength == _B.m_dTLength);
	}

public:
	virtual BOOL AddLaserOffset(double dThickness) {
		double dShift = 0;
		if (m_dTLength >= 0)
			dShift =  dThickness;
		else
			dShift = -dThickness;
		m_dRadius += dShift;
		return TRUE;
	}
	// Transform Operations
	virtual void Revert() { m_dTLength = -m_dTLength; }
	virtual void AddOffset(const CPoint2d& ptOffset) { m_ptCenter += ptOffset; }
	virtual void FlipX(const CPoint2d& ptCenter) {
		CCoordTrans ct(1.0, cv::Matx22d::eye(), ptCenter, ptCenter);
		ct.FlipMX();
		m_ptCenter = ct(m_ptCenter);
		m_dTLength = -m_dTLength;
	}
	virtual void FlipY(const CPoint2d& ptCenter) {
		CCoordTrans ct(1.0, cv::Matx22d::eye(), ptCenter, ptCenter);
		ct.FlipMY();
		m_ptCenter = ct(m_ptCenter);
		m_dTLength = -m_dTLength;
	}
	virtual void Rotate(rad_t dTheta, const CPoint2d& ptCenter) {
		CCoordTrans ct;
		ct.SetR(1.0, dTheta, ptCenter, ptCenter);
		m_ptCenter = ct(m_ptCenter);
	}
	virtual void Resize(double dScale, const CPoint2d& ptCenter) {
		CCoordTrans ct(dScale, cv::Matx22d::eye(), ptCenter, ptCenter);
		m_ptCenter = ct(m_ptCenter);
		m_dRadius *= dScale;
	}
	virtual void Resize(double dScaleX, double dScaleY, const CPoint2d& ptCenter) {
		CCoordTrans ct(1, cv::Matx22d(dScaleX, 0, 0, dScaleY), ptCenter, ptCenter);
		m_ptCenter = ct(m_ptCenter);
		double dRatio = sqrt(dScaleX*dScaleX+dScaleY*dScaleY)/sqrt(2.0);
		m_dRadius *= dRatio;
	}
	virtual BOOL Deflate(double dSizeX, double dSizeY) {
		double dRadius1 = m_dRadius-dSizeX/2;
		double dRadius2 = m_dRadius-dSizeY/2;
		if ( (dRadius1 < 0) || (dRadius2 < 0) )
			return FALSE;
		Resize(dRadius1/m_dRadius, dRadius2/m_dRadius, m_ptCenter);
		return FALSE;
	}
	virtual BOOL ClipByRect(const CRect2d& rect, TRefList<CShapeObject>& objects);	// by 이범호
	virtual void ChangeDirection(eSORT_DIRECTION eDirection, const CPoint2d& ptCenter = CPoint2d()) {
		switch (eDirection) {
		case SD_CCW :
			if (m_dTLength < 0)
				Revert();
			break;
		case SD_CW :
			if (m_dTLength > 0)
				Revert();
			break;
		}
	}

public:
	virtual BOOL Draw(ISCanvas& canvas) const;

public:
	static BOOL FitCircle(const TLineD& pts, CShapeCircle& circle);	// 직선이 아닌 점들의 집합을 Circle 로 맞춤.
};

#pragma pack(pop)
