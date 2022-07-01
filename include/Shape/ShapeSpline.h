#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeLine.h"

#pragma pack(push, 8)
WARNING_PUSH_AND_DISABLE(4018)

#define DEFAULT_CP_TOLERANCE 1e-7      // 0.0000001
#define DEFAULT_FIT_TOLERANCE 1e-10    // 0.0000000001
#define DEFAULT_KNOT_TOLERANCE  1e-7   // 0.0000001

enum eSPLINE_FLAG {
    eSLF_CLOSED   = 0x01,
    eSLF_PERIODIC = 0x02,
    eSLF_RATIONAL = 0x04,
    eSLF_PLANAR   = 0x08,
    eSLF_LINEAR   = 0x10
};

class AFX_EXT_CLASS_SHAPE CShapeSpline : public CShapePolyLine {
public:
	std::vector<double> m_dKnots;
	std::vector<CPoint2d> m_ptsFit;
	std::vector<double> m_dWeights;
	int m_eFlags, m_nDegree;
	double m_dKnotTolerance;
	double m_dFitTolerance;
	double m_dCPTolerance;	// Control Point Tolerance

public:
	CShapeSpline(COLORREF cr = CR_DEFAULT) : CShapePolyLine(S_SPLINE, cr) {
		m_eFlags			= eSLF_RATIONAL;
		m_nDegree			= 3;
		m_dKnotTolerance	= 0.;
		m_dFitTolerance		= 0.;
		m_dCPTolerance		= 0.;
	}
	CShapeSpline(const CShapeSpline& B) : CShapePolyLine(S_SPLINE, B.m_cr) {
		*this = B;
	}
	DECLARE_NEWCLONE(CShapeSpline);
	DECLARE_DYNAMIC_CREATE(CShapeSpline, S_SPLINE);

	CShapeSpline& operator = (const CShapePolyLine& B) {
		(CShapePolyLine&)*this = B;
		m_dKnots.clear();
		for (int i = 0; i < m_pts.size(); i++)
			m_dKnots.push_back(0.0);
		m_ptsFit.clear();
		m_dWeights.clear();
		return *this;
	}
	CShapeSpline& operator = (const CShapeSpline& B) { return (CShapeSpline&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeSpline* pB = dynamic_cast<const CShapeSpline*>(&B);
		if (!pB)
			return *this;

		m_eFlags			= pB->m_eFlags;
		m_nDegree			= pB->m_nDegree;
		m_dKnotTolerance	= pB->m_dKnotTolerance;
		m_dFitTolerance		= pB->m_dFitTolerance;
		m_dCPTolerance		= pB->m_dCPTolerance;
		m_dKnots.clear();
		m_dKnots = pB->m_dKnots;
		m_ptsFit.clear();
		m_ptsFit = pB->m_ptsFit;
		m_dWeights.clear();
		m_dWeights = pB->m_dWeights;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		if (ar.IsStoring()) {
			ar << m_eFlags;
			ar << m_nDegree;
			ar << m_dKnotTolerance;
			ar << m_dFitTolerance;
			ar << m_dCPTolerance;
			ar << m_eFlags;
			ar << m_dKnots;
			ar << m_ptsFit;
			ar << m_dWeights;
		} else {
			m_dKnots.clear();
			m_ptsFit.clear();
			m_dWeights.clear();
			ar >> m_eFlags;
			ar >> m_nDegree;
			ar >> m_dKnotTolerance;
			ar >> m_dFitTolerance;
			ar >> m_dCPTolerance;
			ar >> m_eFlags;
			ar >> m_dKnots;
			ar >> m_ptsFit;
			ar >> m_dWeights;
		}
		return ar;
	}

	BOOL operator == (const CShapeObject& B) const { return Compare(B); }
	virtual CString Print() const {
		CString str;
		for (int i = 0; i < m_pts.size(); i++)
			str += Format(_T("(%.3f, %.3f), "), m_pts[i].x, m_pts[i].y);
		for (int i = 0; i < m_dKnots.size(); i++)
			str += Format(_T("%g, "), m_dKnots[i]);
		return str;
	}

public:
	virtual void AddControlPoint(const CPoint2d& pt) {
		m_pts.push_back(pt);
	}
	virtual void AddKnots(const double& d) { 
		m_dKnots.push_back(d);
	}

public:
	virtual BOOL GetStartEndPoint(CPoint2d& pt0, CPoint2d& pt1) const {
		if (!m_pts.size())
			return FALSE;
		if (m_eFlags==eSLF_CLOSED) {
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
		const CShapeSpline& _B = (const CShapeSpline&)B;
		if (m_dKnots.size() != _B.m_dKnots.size())
			return FALSE;
		for (int i = 0; i < m_pts.size(); i++) {
			if (m_pts[i] != _B.m_pts[i])
				return FALSE;
		}
		return TRUE;
	}

	virtual BOOL AddLaserOffset(double dThickness) {
		return AddLaserOffsetToLine(m_pts, m_pts, dThickness, (m_eFlags==eSLF_CLOSED)?TRUE:FALSE);
	}
	virtual void Revert() {
		CShapePolyLine::Revert();
		ReverseVectorItem(m_dKnots);
	}

	virtual BOOL Draw(ISCanvas& canvas) const;
};


WARNING_POP()
#pragma pack(pop)

