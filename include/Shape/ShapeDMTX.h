#pragma once

#include "ShapeObject.h"

#include "libDMTX/dmtx.h"

#include "xMathUtil/xMathUtil.h"

#pragma pack(push, 8)

BOOL CompareMat(const cv::Mat& matA, const cv::Mat& matB);

class AFX_EXT_CLASS_SHAPE CShapeDMTX : public CShapeObject {
public:
	TRefPointer<CPoint2d> m_rpt;
	CPoint2d& m_pt;				// Center
	CPoint2d m_ptOffsetX;		// Width of one-bit-box
	CPoint2d m_ptOffsetY;		// Height of one-bit-box
	double m_dFillRate;
	BOOL m_bAsDot;
	double m_dLinePitch;
	cv::Mat m_mat;

public:
	CShapeDMTX(COLORREF cr = CR_DEFAULT) : CShapeObject(S_DMTX, cr), m_rpt(new CPoint2d), m_pt(*m_rpt) {
		m_bAsDot = FALSE;
		m_ptOffsetX.x = 0.6;
		m_ptOffsetX.y = 0.0;
		m_ptOffsetY.x = 0.0;
		m_ptOffsetY.y = 0.6;
		m_dFillRate = 0.5/0.6;
		m_dLinePitch = 0.03;
	}
	CShapeDMTX(const CShapeDMTX& B) : CShapeObject(S_DMTX, B.m_cr), m_rpt(new CPoint2d), m_pt(*m_rpt) {
		*this = B;
	}
	virtual ~CShapeDMTX() {
	}
	DECLARE_NEWCLONE(CShapeDMTX);
	DECLARE_DYNAMIC_CREATE(CShapeDMTX, S_DMTX);

	CShapeDMTX& operator = (const CShapeDMTX& B) { return (CShapeDMTX&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeDMTX* pB = dynamic_cast<const CShapeDMTX*>(&B);
		if (!pB)
			return *this;
		m_pt			= pB->m_pt;
		m_ptOffsetX		= pB->m_ptOffsetX;
		m_ptOffsetY		= pB->m_ptOffsetY;
		pB->m_mat.copyTo(m_mat);
		m_bAsDot		= pB->m_bAsDot;
		m_dFillRate		= pB->m_dFillRate;
		m_dLinePitch	= pB->m_dLinePitch;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		if (ar.IsStoring()) {
			ar << m_pt;
			ar << m_ptOffsetX;
			ar << m_ptOffsetY;
			ar << m_dFillRate;
			ar << m_bAsDot;
			ar << m_dLinePitch;
			StoreMat(ar, m_mat);
		} else {
			ar >> m_pt;
			ar >> m_ptOffsetX;
			ar >> m_ptOffsetY;
			ar >> m_dFillRate;
			ar >> m_bAsDot;
			ar >> m_dLinePitch;
			LoadMat(ar, m_mat);
		}
		return ar;
	}

public:
	BOOL operator == (const CShapeObject& B) const { return Compare(B); }
	virtual CString Print() const {
		return Format(_T(" (%.3f, %.3f) DMTX"), m_pt.x, m_pt.y);
	}

	// Operations
public:
	virtual CPoint2d GetCenterPoint() const {
		CCoordTrans ct;
		GetCT(ct);
		return ct.Trans(CPoint2d(m_mat.cols/2., m_mat.rows/2.));
	}
	virtual void GetPoints(TList<TLineD>& lines) const {
		// To do : 실제 가공할 라인을 뽑아내야 함.
		lines.Push(new TLineD);
		TLineD& pts = lines();
		CCoordTrans ct;
		GetCT(ct);
		pts.push_back(ct.Trans<CPoint2d>(0.0,          0.0));
		pts.push_back(ct.Trans<CPoint2d>(m_mat.cols+1, 0.0));
		pts.push_back(ct.Trans<CPoint2d>(m_mat.cols+1, m_mat.rows+1));
		pts.push_back(ct.Trans<CPoint2d>(0.0,          m_mat.rows+1));
	}
	virtual void GetBoundingRect(TRectD& rect, BOOL bResetRect = TRUE) const {
		CCoordTrans ct;
		GetCT(ct);
		TLineD pts;
		pts.push_back(ct.Trans<CPoint2d>(0.0,          0.0));
		pts.push_back(ct.Trans<CPoint2d>(m_mat.cols+1, 0.0));
		pts.push_back(ct.Trans<CPoint2d>(m_mat.cols+1, m_mat.rows+1));
		pts.push_back(ct.Trans<CPoint2d>(0.0,          m_mat.rows+1));
		CShapeObject::GetBoundingRect(rect, pts, bResetRect);
	}
	virtual BOOL GetStartEndPoint(CPoint2d& pt0, CPoint2d& pt1) const {
		CCoordTrans ct;
		GetCT(ct);
		pt0 = ct(CPoint2d(0.0, 0.0));
		pt1 = ct(CPoint2d(m_mat.cols+1, m_mat.rows+1));
		return TRUE;
	}
	virtual BOOL Compare(const CShapeObject& B) const {
		if (!CShapeObject::Compare(B))
			return FALSE;
		CShapeDMTX& _B = (CShapeDMTX&)B;
		return (m_pt			== _B.m_pt)
			&& (m_ptOffsetX		== _B.m_ptOffsetX)
			&& (m_ptOffsetY		== _B.m_ptOffsetY)
			&& (m_dFillRate		== _B.m_dFillRate)
			&& CompareBoolean(m_bAsDot, _B.m_bAsDot)
			&& (m_bAsDot || (m_dLinePitch == _B.m_dLinePitch))
			&& CompareMat(m_mat, _B.m_mat)
			;
	}

	virtual BOOL AddLaserOffset(double dThickness) { return TRUE; }

	virtual void Revert()											{  }
	virtual void AddOffset(const CPoint2d& ptOffset)					{ m_pt += ptOffset; }
	virtual void FlipX(const CPoint2d& ptCenter)						{ m_pt.x = -(m_pt.x-ptCenter.x) + ptCenter.x; m_ptOffsetX.x = -m_ptOffsetX.x; m_ptOffsetY.x = -m_ptOffsetY.x; }
	virtual void FlipY(const CPoint2d& ptCenter)						{ m_pt.y = -(m_pt.y-ptCenter.y) + ptCenter.y; m_ptOffsetX.y = -m_ptOffsetX.y; m_ptOffsetY.y = -m_ptOffsetY.y; }
	virtual void Rotate(rad_t dTheta, const CPoint2d& ptCenter) {
		CCoordTrans ct;
		ct.SetR(1.0, dTheta);
		m_ptOffsetX = ct.Trans(m_ptOffsetX);
		m_ptOffsetY = ct.Trans(m_ptOffsetY);

		ct.SetShift(ptCenter);
		ct.SetOffset(ptCenter);
		m_pt = ct.Trans(m_pt);
	}
	virtual void Resize(double dScale, const CPoint2d& ptCenter) {
		CCoordTrans ct;
		ct.Set(dScale);
		m_ptOffsetX = ct.Trans(m_ptOffsetX);
		m_ptOffsetY = ct.Trans(m_ptOffsetY);

		ct.SetShift(ptCenter);
		ct.SetOffset(ptCenter);
		m_pt = ct.Trans(m_pt);
	}
	virtual void Resize(double dScaleX, double dScaleY, const CPoint2d& ptCenter) {
		CCoordTrans ct;
		ct.Set(1.0, dScaleX, 0, 0, dScaleY);
		m_ptOffsetX = ct.Trans(m_ptOffsetX);
		m_ptOffsetY = ct.Trans(m_ptOffsetY);

		ct.SetShift(ptCenter);
		ct.SetOffset(ptCenter);
		m_pt = ct.Trans(m_pt);
	}
	virtual BOOL Deflate(double dSizeX, double dSizeY) {
		TRectD rect;
		GetBoundingRect(rect);
		CPoint2d pt1 = rect.TopLeft();
		CPoint2d pt2 = rect.BottomRight();
		CPoint2d ptC = rect.CenterPoint();
		double dW = rect.Width();
		double dH = rect.Height();

		if ( (dW < dSizeX) || (dH < dSizeY) ) {	// 줄어든 크기가 0 이 될 경우
			return FALSE;
		} else {
			double dScaleX = (dW-dSizeX)/dW;
			double dScaleY = (dH-dSizeY)/dH;
			//m_pt.x *= dScale;
			//m_pt.y *= dScale;
			m_ptOffsetX.x *= dScaleX;
			m_ptOffsetX.y *= dScaleY;
			m_ptOffsetY.x *= dScaleX;
			m_ptOffsetY.y *= dScaleY;
			double dRatio = sqrt(dScaleX*dScaleX+dScaleY*dScaleY)/sqrt(2.0);
			m_dFillRate *= dRatio;
			m_dLinePitch *= dRatio;
		}
		return TRUE;
	}
	virtual BOOL ClipByRect(const CRect2d& rect, TRefList<CShapeObject>& objects) { return FALSE; }
	virtual void ChangeDirection(eSORT_DIRECTION eDirection, const CPoint2d& ptCenter = CPoint2d()) { }

public:
	// Drawing
	virtual BOOL Draw(ISCanvas& canvas) const;

protected:
	CCoordTrans GetCT() const { CCoordTrans ct; GetCT(ct); return ct; }
	void GetCT(CCoordTrans& ct) const {
		CPoint2d ptsSource[3];
		CPoint2d ptsTarget[3];
		ptsSource[0] = CPoint2d(0.0, 0.0);
		ptsSource[1] = CPoint2d(1.0, 0.0);
		ptsSource[2] = CPoint2d(0.0, 1.0);

		ptsTarget[0] = CPoint2d(0.0, 0.0);
		ptsTarget[1] = m_ptOffsetX;
		ptsTarget[2] = m_ptOffsetY;
		ct.SetFrom3Points(ptsSource, ptsTarget, TRUE);

		//CPoint2d ptShift(m_mat.cols/2.0, m_mat.rows/2.0);	// Align : H_Center V_Center
		CPoint2d ptShift(0, 0);								// Align : H_Left V_Bottom
		ct.SetShift(ptShift);
		CPoint2d ptOffset;
		ct.GetOffset(ptShift);
		ct.SetOffset(ptOffset+m_pt);
	}

protected:
	static DmtxEncode* s_pEncoder;
public:
	static BOOL DrawToMat(LPCTSTR pszContent, int sizeIdxRequest, cv::Mat& mat);
	static int GetSizeIndex(int cx, int cy);

};

#pragma pack(pop)
