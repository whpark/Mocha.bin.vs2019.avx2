#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeObject.h"

#pragma pack(push, 8)

WARNING_PUSH_AND_DISABLE(4018)

enum eAREA_CODE {	
	eINSIDE = 0,	// 0000
	eLEFT	= 1,	// 0001
	eRIGHT	= 2,	// 0010
	eBOTTOM = 4,	// 0100
	eTOP	= 8,    // 1000
};

class AFX_EXT_CLASS_SHAPE CShapeLine : public CShapeObject {
public:
	TLineD m_pts;

public:
	CShapeLine(COLORREF cr = CR_DEFAULT) : CShapeObject(S_LINE, cr) {
	}
protected:
	CShapeLine(eSHAPE eShape, COLORREF cr = CR_DEFAULT) : CShapeObject(eShape, cr) {
	}
public:
	CShapeLine(const CShapeLine& B) : CShapeObject(S_LINE, B.m_cr) {
		*this = B;
	}
	virtual ~CShapeLine() {
	}
	DECLARE_NEWCLONE(CShapeLine);
	DECLARE_DYNAMIC_CREATE(CShapeLine, S_LINE);

	CShapeLine& operator = (const CShapeLine& B) { return (CShapeLine&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeLine* pB = dynamic_cast<const CShapeLine*>(&B);
		if (!pB)
			return *this;
		m_pts.clear();
		m_pts = pB->m_pts;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		if (ar.IsStoring()) {
			ar << m_pts;
		} else {
			ar >> m_pts;
		}
		return ar;
	}

	BOOL operator == (const CShapeObject& B) const { return Compare(B); }
	virtual CString Print() const {
		CString str;
		//m_pts.First();
		for (size_t i = 0; i < m_pts.size(); i++)
			str += Format(_T(" (%.3f, %.3f)"), m_pts[i].x, m_pts[i].y);
		return str;
	}

public:
	virtual CPoint2d GetCenterPoint() const { TRectD rect; GetBoundingRect(rect); return rect.CenterPoint(); }
	virtual void GetPoints(TList<TLineD>& lines) const { lines.Push(new TLineD(m_pts)); }
	virtual BOOL SetFromPoints(const TLineD& line) { m_pts = line; return m_pts.size() >= 2 ? TRUE : FALSE; }
	virtual void GetBoundingRect(TRectD& rect, BOOL bResetRect = TRUE) const {
		CShapeObject::GetBoundingRect(rect, m_pts, bResetRect);
	}
	virtual void AddPoint(const CPoint2d& pt) { m_pts.push_back(pt); }

	virtual BOOL GetStartEndPoint(CPoint2d& pt0, CPoint2d& pt1) const {
		if (m_pts.size() < 1)
			return FALSE;
		pt0 = m_pts.front();
		pt1 = m_pts.back();
		return TRUE;
	}
	virtual BOOL Compare(const CShapeObject& B) const {
		if (!__super::Compare(B))
			return FALSE;
		const CShapeLine& _B = (const CShapeLine&)B;
		if (m_pts.size() != _B.m_pts.size())
			return FALSE;
		for (int i = 0; i < m_pts.size(); i++) {
			if (m_pts[i] != _B.m_pts[i])
				return FALSE;
		}
		return TRUE;
	}

	virtual BOOL AddLaserOffset(double dThickness)					{ return AddLaserOffsetToLine(m_pts, m_pts, dThickness, FALSE); }

	virtual void Revert() { 
		size_t n = m_pts.size()/2;
		for (size_t i = 0; i < n; i++) {
			size_t iLeft = i;
			size_t iRight = m_pts.size()-i-1;
			auto pt = m_pts[iLeft];
			m_pts[iLeft] = m_pts[iRight];
			m_pts[iRight] = pt;
		}
	}
	virtual void AddOffset(const CPoint2d& ptOffset)					{ /*m_pts.First(); */for (size_t i = 0; i < m_pts.size(); i++) m_pts[i] += ptOffset; }
	virtual void FlipX(const CPoint2d& ptCenter)						{ CCoordTrans ct(1.0, cv::Matx22d(-1, 0, 0, 1), ptCenter, ptCenter); ct.TransL(m_pts); }
	virtual void FlipY(const CPoint2d& ptCenter)						{ CCoordTrans ct(1.0, cv::Matx22d(1, 0, 0, -1), ptCenter, ptCenter); ct.TransL(m_pts); }
	virtual void Rotate(rad_t dTheta, const CPoint2d& ptCenter)			{ CCoordTrans ct; ct.SetR(1.0, dTheta, ptCenter, ptCenter); ct.TransL(m_pts); }
	virtual void Resize(double dScale, const CPoint2d& ptCenter)		{ CCoordTrans ct(dScale, cv::Matx22d::eye(), ptCenter, ptCenter); ct.TransL(m_pts); }
	virtual void Resize(double dScaleX, double dScaleY, const CPoint2d& ptCenter)
																		{ CCoordTrans ct(1, cv::Matx22d(dScaleX, 0, 0, dScaleY), ptCenter, ptCenter); ct.TransL(m_pts); }
	virtual BOOL Deflate(double dSizeX, double dSizeY) {
		TRectD rect;
		GetBoundingRect(rect);
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

	virtual BOOL ClipByRect(const CRect2d& rect, TRefList<CShapeObject>& objects);	// by 이범호
	virtual void ChangeDirection(eSORT_DIRECTION eDirection, const CPoint2d& ptCenter = CPoint2d());

	virtual BOOL Draw(ISCanvas& canvas) const;

public:
	BOOL FindIntersection(const CPoint2d& pt0, const CPoint2d& pt1, CPoint2d& pt) const;
	BOOL FindIntersection(const CShapeLine& line, CPoint2d& pt) const;

public:
	BOOL CohenSutherlandLineClipping(const TRectD& rect, const CPoint2d& pt0, const CPoint2d& pt1, CPoint2d& ptS, CPoint2d& ptE);
};

WARNING_POP()

#pragma pack(pop)
