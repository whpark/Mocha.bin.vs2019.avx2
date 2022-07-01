#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeDot.h"

#pragma pack(push, 8)

class AFX_EXT_CLASS_SHAPE CShapeVertex : public CShapeDot {
public:
	BOOL m_bLaserOn;
public:
	CShapeVertex(COLORREF cr = CR_DEFAULT) : CShapeDot(S_VERTEX, cr) {
		m_bLaserOn = FALSE;
	}
	CShapeVertex(const CShapeVertex& B) : CShapeDot(S_VERTEX, B.m_cr) {
		*this = B;
	}
	virtual ~CShapeVertex() {
	}
	DECLARE_NEWCLONE(CShapeVertex);
	DECLARE_DYNAMIC_CREATE(CShapeVertex, S_VERTEX);

	CShapeVertex& operator = (const CShapeVertex& B) { return (CShapeVertex&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeVertex* pB = dynamic_cast<const CShapeVertex*>(&B);
		if (!pB)
			return *this;
		m_bLaserOn = pB->m_bLaserOn;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		if (ar.IsStoring()) {
			ar << m_bLaserOn;
		} else {
			ar >> m_bLaserOn;
		}
		return ar;
	}

	BOOL operator == (const CShapeObject& B) const { return Compare(B); }
	virtual CString Print() const { return Format(_T("(%.3f, %.3f), Laser%s"), m_pt.x, m_pt.y, m_bLaserOn ? _T("On") : _T("Off") ); }

public:
	virtual BOOL Compare(const CShapeObject& B) const {
		if (!__super::Compare(B))
			return FALSE;
		const CShapeVertex& _B = (const CShapeVertex&)B;
		return CompareBoolean(m_bLaserOn, _B.m_bLaserOn);
	}

public:
	virtual BOOL Draw(ISCanvas& canvas) const;
};


#pragma pack(pop)
