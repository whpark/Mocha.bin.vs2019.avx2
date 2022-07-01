#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeLine.h"
#include "misc/ArchiveU.h"

#pragma pack(push, 8)
WARNING_PUSH_AND_DISABLE(4018)

class AFX_EXT_CLASS_SHAPE CShapePolyLine : public CShapeLine {
public:
	std::vector<double> m_dBulges;
	BOOL m_bLoop;

protected:
	CShapePolyLine(eSHAPE eShape, COLORREF cr = CR_DEFAULT) : CShapeLine(S_SPLINE, cr) {
		m_bLoop = TRUE;
	}

public:
	CShapePolyLine(COLORREF cr = CR_DEFAULT) : CShapeLine(S_POLY_LINE, cr) {
		m_bLoop = TRUE;
	}
	CShapePolyLine(const CShapePolyLine& B) : CShapeLine(S_POLY_LINE, B.m_cr) {
		*this = B;
	}

	CShapePolyLine& operator = (const CShapeLine& B) {
		(CShapeLine&)*this = B;
		m_dBulges.clear();
		for (int i = 0; i < m_pts.size(); i++)
			m_dBulges.push_back(0.0);
		return *this;
	}
	CShapePolyLine& operator = (const CShapePolyLine& B) { return (CShapePolyLine&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapePolyLine* pB = dynamic_cast<const CShapePolyLine*>(&B);
		if (!pB)
			return *this;

		m_dBulges.clear();
		m_dBulges	= pB->m_dBulges;
		m_bLoop		= pB->m_bLoop;

		return *this;
	}
	DECLARE_NEWCLONE(CShapePolyLine);
	DECLARE_DYNAMIC_CREATE(CShapePolyLine, S_POLY_LINE);

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		if (ar.IsStoring()) {
			ar << m_dBulges;
			ar << m_bLoop;
		} else {
			ar >> m_dBulges;
			ar >> m_bLoop;
		}
		return ar;
	}

	BOOL operator == (const CShapeObject& B) const { return Compare(B); }
	virtual CString Print() const {
		CString str;
		for (int i = 0; i < m_pts.size(); i++)
			str += Format(_T(" (%.3f, %.3f) b(%.3f)"), m_pts[i].x, m_pts[i].y, m_dBulges.size() > i ? m_dBulges[i] : 0.0);
		return str;
	}

public:
	virtual void GetPoints(TList<TLineD>& lines) const;
	virtual void GetBoundingRect(TRectD& rect, BOOL bResetRect = TRUE) const;
	virtual void AddPoint(const CPoint2d& pt) { AddPoint(pt, 0.0); }
	virtual void AddPoint(const CPoint2d& pt, double dBulge) {
		m_pts.push_back(pt);
		while (m_dBulges.size() < m_pts.size()-1)
			m_dBulges.push_back(0.0);
		m_dBulges.push_back(dBulge);
	}

public:
	virtual BOOL GetStartEndPoint(CPoint2d& pt0, CPoint2d& pt1) const {
		if (!m_pts.size())
			return FALSE;
		if (m_bLoop) {
			pt0 = pt1 = m_pts.front();
		} else {
			pt0 = m_pts.front();
			pt1 = m_pts.back();
		}
		return TRUE;
	}
	virtual BOOL Compare(const CShapeObject& B) const {
		if (!__super::Compare(B))
			return FALSE;
		const CShapePolyLine& _B = (const CShapePolyLine&)B;
		if (m_dBulges.size() != _B.m_dBulges.size())
			return FALSE;
		for (int i = 0; i < m_pts.size(); i++) {
			if (m_pts[i] != _B.m_pts[i])
				return FALSE;
		}
		return TRUE;
	}

	virtual BOOL AddLaserOffset(double dThickness);
	virtual void Revert();
	virtual void FlipX(const CPoint2d& ptCenter)						{ __super::FlipX(ptCenter); for (int i = 0; i < m_dBulges.size(); i++) m_dBulges[i] = -m_dBulges[i]; }
	virtual void FlipY(const CPoint2d& ptCenter)						{ __super::FlipY(ptCenter); for (int i = 0; i < m_dBulges.size(); i++) m_dBulges[i] = -m_dBulges[i]; }

	virtual BOOL ClipByRect(const CRect2d& rect, TRefList<CShapeObject>& objects);
	virtual void ChangeDirection(eSORT_DIRECTION eDirection, const CPoint2d& ptCenter = CPoint2d());

	virtual BOOL Draw(ISCanvas& canvas) const;

public:
	TRefPointer<CShapeObject> ApproxToArcLine(double dMinArcLength = 0.0, double dMaxTolerance = 0.010) const;
	static BOOL ApproxToArcLine(const TLineD& line, TRefList<CShapeObject>& objects, double dMinArcLength = 0.0, double dMaxTolerance = 0.010);
	static BOOL GetArcFromBulge(double dBulge, const CPoint2d& pt0, const CPoint2d& pt1, CShapeArc& arc);
	BOOL Split(TRefList<CShapeObject>& objects) const;
};


WARNING_POP()
#pragma pack(pop)
