#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeObject.h"

#pragma pack(push, 8)

class AFX_EXT_CLASS_SHAPE CShapeDot : public CShapeObject {
public:
	TRefPointer<CPoint2d> m_rpt;
	CPoint2d& m_pt;
	DWORD m_dwDuration;

public:
	CShapeDot(COLORREF cr = CR_DEFAULT) : CShapeObject(S_DOT, cr), m_rpt(new CPoint2d), m_pt(*m_rpt) {
		m_pt.x = 0;
		m_pt.y = 0;
		m_dwDuration = 0;
	}
protected:
	CShapeDot(eSHAPE eShape, COLORREF cr = CR_DEFAULT) : CShapeObject(eShape, cr), m_rpt(new CPoint2d), m_pt(*m_rpt) {
		m_pt.x = 0;
		m_pt.y = 0;
		m_dwDuration = 0;
	}
public:
	CShapeDot(const CPoint2d& pt, COLORREF cr = CR_DEFAULT) : CShapeObject(S_DOT, cr), m_rpt(new CPoint2d), m_pt(*m_rpt) {
		m_pt.x = pt.x;
		m_pt.y = pt.y;
		m_dwDuration = 0;
	}
	CShapeDot(double x, double y, COLORREF cr = CR_DEFAULT) : CShapeObject(S_DOT, cr), m_rpt(new CPoint2d), m_pt(*m_rpt) {
		m_pt.x = x;
		m_pt.y = y;
		m_dwDuration = 0;
	}
	CShapeDot(const CShapeDot& B) : CShapeObject(S_DOT, B.m_cr), m_rpt(new CPoint2d), m_pt(*m_rpt) {
		*this = B;
	}
	virtual ~CShapeDot() {
	}
	DECLARE_NEWCLONE(CShapeDot);
	DECLARE_DYNAMIC_CREATE(CShapeDot, S_DOT);

	CShapeDot& operator = (const CShapeDot& B) { return (CShapeDot&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeDot* pB = dynamic_cast<const CShapeDot*>(&B);
		if (!pB)
			return *this;
		m_pt			= pB->m_pt;
		m_dwDuration	= pB->m_dwDuration;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		if (ar.IsStoring()) {
			ar << m_pt;
			ar << m_dwDuration;
		} else {
			ar >> m_pt;
			ar >> m_dwDuration;
		}
		return ar;
	}

	BOOL operator == (const CShapeObject& B) const { return Compare(B); }

	virtual CString Print() const { return Format(_T("(%.3f, %.3f)"), m_pt.x, m_pt.y); }

public:
	virtual CPoint2d GetCenterPoint() const { return m_pt; }
	virtual void GetPoints(TList<TLineD>& lines) const { lines.Push(new TLineD); lines().push_back(*m_rpt); }
	virtual BOOL GetStartEndPoint(CPoint2d& pt0, CPoint2d& pt1) const { pt0 = pt1 = m_pt; return TRUE; }

	virtual BOOL Compare(const CShapeObject& B) const {
		if (!__super::Compare(B))
			return FALSE;
		const CShapeDot& _B = (const CShapeDot&)B;
		return (m_pt == _B.m_pt) && (m_dwDuration == _B.m_dwDuration);
	}

	virtual BOOL AddLaserOffset(double dThickness)						{ return TRUE; }

	virtual void Revert()												{  }
	virtual void AddOffset(const CPoint2d& ptOffset)					{ m_pt += ptOffset; }
	virtual void FlipX(const CPoint2d& ptCenter)						{ m_pt.x = -(m_pt.x-ptCenter.x) + ptCenter.x; }
	virtual void FlipY(const CPoint2d& ptCenter)						{ m_pt.y = -(m_pt.y-ptCenter.y) + ptCenter.y; }
	virtual void Rotate(rad_t dTheta, const CPoint2d& ptCenter)			{ CCoordTrans ct; ct.SetR(1.0, dTheta, ptCenter, ptCenter); m_pt = ct(m_pt); }
	virtual void Resize(double dScale, const CPoint2d& ptCenter)		{ CCoordTrans ct(dScale, cv::Matx22d::eye(), ptCenter, ptCenter); m_pt = ct(m_pt); }
	virtual void Resize(double dScaleX, double dScaleY, const CPoint2d& ptCenter)
																		{ CCoordTrans ct(1, cv::Matx22d(dScaleX, 0, 0, dScaleY), ptCenter, ptCenter); m_pt = ct(m_pt); }
	virtual BOOL Deflate(double dSizeX, double dSizeY)					{ return TRUE; }
	virtual BOOL ClipByRect(const CRect2d& rect, TRefList<CShapeObject>& objects);
	virtual void ChangeDirection(eSORT_DIRECTION eDirection, const CPoint2d& ptCenter = CPoint2d()) { }

public:
	virtual BOOL Draw(ISCanvas& canvas) const;
};


#pragma pack(pop)
