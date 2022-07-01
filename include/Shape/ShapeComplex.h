#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeObject.h"
#include "misc/xUtil.h"

#pragma pack(push, 8)

class AFX_EXT_CLASS_SHAPE CShapeComplex : public CShapeObject {
public:
	CCoordTrans m_ct;

protected:
	TRectD m_rect;
	TList<TLineD> m_lines;

	// Constructor
public:
	CShapeComplex(COLORREF cr = CR_DEFAULT) : CShapeObject(S_COMPLEX, cr) {
	}
protected:
	CShapeComplex(eSHAPE eShape, COLORREF cr = CR_DEFAULT) : CShapeObject(eShape, cr) {
	}
public:
	CShapeComplex(const CShapeComplex& B) : CShapeObject(S_COMPLEX, B.m_cr) {
		*this = B;
	}
	virtual ~CShapeComplex(void) {
	}
	//DECLARE_NEWCLONE(CShapeComplex);
	//DECLARE_DYNAMIC_CREATE(CShapeComplex, S_COMPLEX);

public:
	// default assign operator
	CShapeComplex& operator = (const CShapeComplex& B) { return (CShapeComplex&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeComplex* pB = dynamic_cast<const CShapeComplex*>(&B);
		if (!pB)
			return *this;
		m_ct	= pB->m_ct;
		m_lines.DeleteAll();
		m_lines.CopyFrom(pB->m_lines);
		m_rect	= pB->m_rect;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		// To do : m_rect, m_lines..... how to?
		if (ar.IsStoring()) {
			ar << m_ct;
		} else {
			ar >> m_ct;
		}
		return ar;
	}

public:
	BOOL operator == (const CShapeComplex& B) const { return Compare(B); }

	BOOL IsEmpty() const { return m_lines.N() ? FALSE : TRUE; }
	virtual BOOL Make() = NULL;

public:
	virtual CString Print() const { return Format(_T(" ShapeComplex"));}

	// Operations
	virtual CPoint2d GetCenterPoint() const { return m_rect.CenterPoint(); };
public:
	virtual void GetPoints(TList<TLineD>& lines) const {
		for (int i = 0; i < m_lines.N(); i++) {
			lines.Push(new TLineD);
			lines() = m_lines[i];
		}
	}
	virtual void GetBoundingRect(TRectD& rect, BOOL bResetRect = TRUE) const {
		TLineD line;
		line.push_back(CPoint2d(m_rect.left,  m_rect.top));
		line.push_back(CPoint2d(m_rect.left,  m_rect.bottom));
		line.push_back(CPoint2d(m_rect.right, m_rect.top));
		line.push_back(CPoint2d(m_rect.right, m_rect.bottom));
		m_ct.TransL(line);
		CShapeObject::GetBoundingRect(rect, line, bResetRect);
	}
	virtual BOOL GetStartEndPoint(CPoint2d& pt0, CPoint2d& pt1) const {
		if (!m_lines.N() || !m_lines[0].size())
			return FALSE;
		pt0 = m_ct(m_lines.First()->front());
		pt1 = m_ct(m_lines.Last()->back());
		return TRUE;
	}
	virtual BOOL Compare(const CShapeObject& B) const {
		if (!CShapeObject::Compare(B))
			return FALSE;
		const CShapeComplex& _B = (const CShapeComplex&)B;
		return (m_ct == _B.m_ct)
			&& (m_rect == _B.m_rect)
			&& (m_lines == _B.m_lines)
			;
	}

	virtual BOOL AddLaserOffset(double dThickness) {
		for (int i = 0; i < m_lines.N(); i++) {
			AddLaserOffsetToLine(m_lines[i], m_lines[i], dThickness, FALSE);
		}
		return TRUE;
	}

	// Transform Operations
	virtual void Revert() {
		for (int i = 0; i < m_lines.N(); i++) {
			ReverseVectorItem(m_lines[i]);
		}
		m_lines.Invert();
	}
	virtual void AddOffset(const CPoint2d& ptOffset) {
		CPoint2d ptOffset0;
		m_ct.GetOffset(ptOffset0);
		m_ct.SetOffset(ptOffset0 + ptOffset);
	}
	virtual void FlipX(const CPoint2d& ptCenter) { m_ct.FlipX(ptCenter.x); }
	virtual void FlipY(const CPoint2d& ptCenter) { m_ct.FlipY(ptCenter.y); }
	virtual void Rotate(rad_t dTheta, const CPoint2d& ptCenter) { m_ct.Rotate(dTheta, ptCenter); }
	virtual void Resize(double dScale, const CPoint2d& ptCenter) { Resize(dScale, dScale, ptCenter); }
	virtual void Resize(double dScaleX, double dScaleY, const CPoint2d& ptCenter) {
		CCoordTrans backup(m_ct);
		m_ct.Set(1, dScaleX, 0, 0, dScaleY);
		m_ct.Trans(m_rect.left, m_rect.top, m_rect.left, m_rect.top);
		m_ct.Trans(m_rect.right, m_rect.bottom, m_rect.right, m_rect.bottom);
		m_ct *= backup;
	}
	virtual BOOL Deflate(double dSizeX, double dSizeY) {
		TRectD rect;
		GetBoundingRect(rect);
		TSizeD size;
		size.cx = rect.Width() - dSizeX;
		size.cy = rect.Height() - dSizeY;
		if (size.cx <= 0)
			size.cx = 0;
		if (size.cy <= 0)
			size.cy = 0;
		double dScaleX = (rect.Width() == 0.0) ? 0.0 : size.cx/rect.Width();
		double dScaleY = (rect.Height() == 0.0) ? 0.0 : size.cy/rect.Height();
		Resize(dScaleX, dScaleY, rect.CenterPoint());
		return TRUE;
	}
	virtual BOOL ClipByRect(const CRect2d& rect, TRefList<CShapeObject>& objects);
	virtual void ChangeDirection(eSORT_DIRECTION eDirection, const CPoint2d& ptCenter = CPoint2d()) {
	}

public:
	// Drawing
	virtual BOOL Draw(ISCanvas& canvas) const;

};


#pragma pack(pop)
