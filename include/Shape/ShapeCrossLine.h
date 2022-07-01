#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeComplex.h"

#pragma pack(push, 8)

class AFX_EXT_CLASS_SHAPE CShapeCrossLine : public CShapeComplex {
public:
	TRefPointer<CPoint2d> m_rpt;
	CPoint2d& m_pt;

	double m_dWidth;
	double m_dHeight;
	double m_dThickness;
	double m_dLineT;
	BOOL m_bOutLineBox;
	double m_dWidthBox;
	double m_dHeightBox;
	double m_dThicknessBox;

public:
	CShapeCrossLine(COLORREF cr = CR_DEFAULT) : CShapeComplex(S_CROSS_LINE, cr), m_rpt(new CPoint2d), m_pt(*m_rpt) {
		m_pt.x = 0;
		m_pt.y = 0;

		m_dWidth = 2.0;
		m_dHeight = 2.0;
		m_dThickness = 0.6;
		m_dLineT = 0.01;
		m_bOutLineBox = TRUE;
		m_dWidthBox = 3;
		m_dHeightBox = 3;
		m_dThicknessBox = 0.2;
	}
	CShapeCrossLine(const CShapeCrossLine& B) : CShapeComplex(S_CROSS_LINE, B.m_cr), m_rpt(new CPoint2d), m_pt(*m_rpt) {
		*this = B;
	}
	virtual ~CShapeCrossLine() {
	}
	DECLARE_NEWCLONE(CShapeCrossLine);
	DECLARE_DYNAMIC_CREATE(CShapeCrossLine, S_CROSS_LINE);

	CShapeCrossLine& operator = (const CShapeCrossLine& B) { return (CShapeCrossLine&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeCrossLine* pB = dynamic_cast<const CShapeCrossLine*>(&B);
		if (!pB)
			return *this;
		m_pt			= pB->m_pt;
		m_dWidth		= pB->m_dWidth;
		m_dHeight		= pB->m_dHeight;
		m_dThickness	= pB->m_dThickness;
		m_dLineT		= pB->m_dLineT;
		m_bOutLineBox	= pB->m_bOutLineBox;
		m_dWidthBox		= pB->m_dWidthBox;
		m_dHeightBox	= pB->m_dHeightBox;
		m_dThicknessBox	= pB->m_dThicknessBox;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		if (ar.IsStoring()) {
			ar << m_pt;
			ar << m_dWidth;
			ar << m_dHeight;
			ar << m_dThickness;
			ar << m_dLineT;
			ar << m_bOutLineBox;
			ar << m_dWidthBox;
			ar << m_dHeightBox;
			ar << m_dThicknessBox;
		} else {
			ar >> m_pt;
			ar >> m_dWidth;
			ar >> m_dHeight;
			ar >> m_dThickness;
			ar >> m_dLineT;
			ar >> m_bOutLineBox;
			ar >> m_dWidthBox;
			ar >> m_dHeightBox;
			ar >> m_dThicknessBox;
		}
		return ar;
	}

	BOOL operator == (const CShapeComplex& B) const { return Compare(B); }
	virtual CString Print() const {
		return Format(_T(" (%.3f, %.3f)"), m_pt.x, m_pt.y);
	}

	virtual BOOL Compare(const CShapeObject& B) const {
		if (!__super::Compare(B))
			return FALSE;
		const CShapeCrossLine& _B = (const CShapeCrossLine&)B;
		return (m_pt == _B.m_pt)
			&& (m_dWidth			== _B.m_dWidth)
			&& (m_dHeight			== _B.m_dHeight)
			&& (m_dThickness		== _B.m_dThickness)
			&& (m_dLineT			== _B.m_dLineT)
			&& (m_bOutLineBox		== _B.m_bOutLineBox)
			&& (m_dWidthBox			== _B.m_dWidthBox)
			&& (m_dHeightBox		== _B.m_dHeightBox)
			&& (m_dThicknessBox		== _B.m_dThicknessBox)
			;
	}

	virtual BOOL AddLaserOffset(double dThickness) { return TRUE; }

public:
	virtual BOOL Make();

};

#pragma pack(pop)
