#pragma once

#include "AFX_EXT_SHAPE.h"

#include "TList.hpp"

#include "Coordinate.hpp"
#include "xMathUtil/CoordTrans.h"

//class CShape;

#pragma pack(push, 8)

typedef std::vector<CPoint2d> TLineD;
typedef std::vector<CPoint2i> TLineI;

//-----------------------------------------------------------------------------
// ISCanvas : Interface of Canvas
class AFX_EXT_CLASS_SHAPE ISCanvas {
private:
	CCoordTrans m_ctP;
	CCoordTrans m_ct1;
	CCoordTrans m_ct2;

private:
	mutable CDC m_dc;	// Device Context for Text function 'GetGlyphOutlineW'

protected:
	BOOL m_bSeamless;	// TRUE : MoveTo operates like LineTo
	double m_dErrorTolerance;

protected:
	COLORREF m_cr;
	CCoordTrans m_ct;
	double m_dLaserOffset;
	CPoint2d m_ptLast;

protected:
	cv::Scalar m_crArrow;
	BOOL m_bShowDirection;
	BOOL m_bShowOrder;
	int m_iOrder;
	int m_iArrowSize;
	int m_iArrowThickness;

public:
	int m_nSamplingPointsPerPI;

public:
	ISCanvas() {
		m_dLaserOffset = 0.0;
		m_nSamplingPointsPerPI = 48;
		m_bSeamless = FALSE;
		m_dErrorTolerance = 0.0;
		memset(m_ptLast.val, 0xff, sizeof(m_ptLast.val));	// not zero. set INVALID_VALUE
		m_bShowDirection = FALSE;
		m_bShowOrder = FALSE;
		m_iArrowSize = 5;
		m_iOrder = 0;
		m_iArrowThickness = 1;
	}
	virtual ~ISCanvas() {}
	ISCanvas(const ISCanvas& B) { *this = B; }
	ISCanvas& operator = (const ISCanvas& B) {
		if (this == &B) return *this;
		m_ctP = B.m_ctP;
		m_ct1 = B.m_ct1;
		m_ct2 = B.m_ct2;
		if (m_dc.m_hDC)
			m_dc.DeleteDC();
		m_dc.CreateCompatibleDC(&B.m_dc);
		m_cr = B.m_cr;
		m_ct = B.m_ct;
		m_dLaserOffset = B.m_dLaserOffset;
		m_nSamplingPointsPerPI = B.m_nSamplingPointsPerPI;

		memset(m_ptLast.val, 0xff, sizeof(m_ptLast.val));	// not zero. set INVALID_VALUE

		m_crArrow			= B.m_crArrow;
		m_bShowDirection	= B.m_bShowDirection;
		m_bShowOrder		= B.m_bShowOrder;
		m_iArrowSize		= B.m_iArrowSize;
		m_iOrder			= B.m_iOrder;
		m_iArrowThickness	= B.m_iArrowThickness;

		return *this;
	}

	virtual BOOL CreateCanvas(const TRectD& rectSource, const TRectD& rectTarget, BOOL bKeepAspectRatio = TRUE);
	virtual void PreDraw() {
		m_iOrder = 0;
		memset(m_ptLast.val, 0xff, sizeof(m_ptLast.val));	// not zero. set INVALID_VALUE
	}
	virtual void PostDraw() {
	}

	virtual void SetCoordTrans(const CCoordTrans* pCT1 = NULL, const CCoordTrans* pCT2 = NULL, const CCoordTrans* pCTP = NULL) {
		if (pCT1)
			m_ct1 = *pCT1;
		if (pCT2)
			m_ct2 = *pCT2;
		if (pCTP)
			m_ctP = *pCTP;

		m_ct = m_ctP * m_ct1 * m_ct2;
	}
	void InitCoordTrans() { m_ct.Set(); m_ct1.Set(); m_ct2.Set(), m_ctP.Set(); }

	CCoordTrans& GetCT() { return m_ct; }
	CCoordTrans& GetCT1() { return m_ct1; }
	CCoordTrans& GetCT2() { return m_ct2; }
	CCoordTrans& GetCTP() { return m_ctP; }
	CCoordTrans& GetCT(int iIndex) { if (iIndex == 1) return m_ct1; else if (iIndex == 2) return m_ct2; else if (iIndex == -1) return m_ctP; return m_ct; }

	virtual void SetLaserOffset(double dLaserOffset = 0.0) { m_dLaserOffset = dLaserOffset; }
	double GetLaserOffset() const { return m_dLaserOffset; }

	BOOL SetSeamless(BOOL bSeamless) { BOOL _bSeamless = m_bSeamless; m_bSeamless = bSeamless; return _bSeamless; }
	BOOL ISSeamless() const { return m_bSeamless; }
	double SetErrorTolerance(double dErrorTolerance = 0.001) { double dOld = m_dErrorTolerance; m_dErrorTolerance = dErrorTolerance; return dOld; }
	double GetErrorTolerance() const { return m_dErrorTolerance; }

	void ShowDirection(BOOL bShow = TRUE, BOOL bShowOrder = TRUE, COLORREF crArrow = RGB(255, 0, 0), int iArrowSize = 16, int iArrowThickness = 1) {
		m_bShowDirection = bShow;
		m_bShowOrder = bShowOrder;
		m_crArrow[0] = GetBValue(crArrow);
		m_crArrow[1] = GetGValue(crArrow);
		m_crArrow[2] = GetRValue(crArrow);
		m_iArrowSize = iArrowSize;
		m_iOrder = 0;
		m_iArrowThickness = iArrowThickness;
	}
	BOOL StopDrawingDirection();
	BOOL ResumeDrawingDirection();
	BOOL IsDirectionVisible() const { return m_bShowDirection; }

protected:
	virtual void MoveToImpl(const CPoint2d& pt) = NULL;
	virtual void LineToImpl(const CPoint2d& pt) = NULL;
public:
	virtual void SetColor(COLORREF cr) { m_cr = cr; }

	// Primative
	void MoveTo(const CPoint2d& pt);
	void LineTo(const CPoint2d& pt, BOOL bShowAttrib = FALSE);

	// Extensive
	void MoveTo(double x, double y) { MoveTo(CPoint2d(x, y)); };
	void LineTo(double x, double y) { LineTo(CPoint2d(x, y)); };
	virtual void Dot(const CPoint2d& pt, DWORD dwDuration = 0 /* in usec */);
	virtual void Line(const TLineD& pts, BOOL bLoop = FALSE);
	virtual void PolyLine(const TLineD& pts, const std::vector<double>& dBulges, BOOL bLoop);
	virtual void Spline(const TLineD& pts, const std::vector<double>& dKnots, BOOL bLoop);
	virtual void Arc(const CPoint2d& ptCenter, double dRadius, double dT0, double dTLength);
	virtual void Ellipse(const CPoint2d& ptCenter, double dRadius1, double dRadius2, rad_t dTFirstAxis, rad_t dT0, rad_t dTLength);

	//-----------------------
	// Hatch :
	//
	//	linePolygon : 시작과 끝점이 다를 경우, 첫번째 점과 마지막점을 연결하게 됨. 점의 갯수가 최소한 3개 이상 있어야 함.
	//
	//	eHatching :	SH_NONE
	//				SH_L2R
	//				SH_T2B
	//				SH_LT2RB
	//				SH_LB2RT
	virtual void Hatch(const TList<TLineD>& lines, DWORD eHatching, double dHatching); 

	//-----------------------
	// Text :
	//	pt : 기준 점
	//
	//	dwAlign :	DT_TOP
	//				DT_LEFT
	//				DT_CENTER
	//				DT_RIGHT
	//				DT_VCENTER
	//				DT_BOTTOM
	//
	//	lf : 크기는 무시됨. (기본 72 point(720) 으로 만들어야 함)
	//
	virtual void Text(LPCWSTR pszText, const CPoint2d& pt, DWORD dwAlign /*DT_...*/, double dHeight, double dLineSpacingFactor, const LOGFONT& lf, DWORD eHatching, double dHatching);
};

//-----------------------------------------------------------------------------
// CCanvasLineExtractor
class AFX_EXT_CLASS_SHAPE CSCanvasLineExtractor : public ISCanvas {
public:
	TList<TLineD>& m_lines;
protected:
	CPoint2d m_ptLastCT;

public:
	CSCanvasLineExtractor(TList<TLineD>& lines) : m_lines(lines) {
	}
	virtual ~CSCanvasLineExtractor() {}

public:
	virtual void MoveToImpl(const CPoint2d& pt) {
		m_ptLastCT = m_ct.Trans(pt);
		if (m_lines.N() && !m_lines.Last()->size())
			return;
		m_lines.Push(new TLineD);
	}
	virtual void LineToImpl(const CPoint2d& pt) {
		if (!m_lines.N())
			return;
		TLineD& line = *m_lines.Last();
		if (!line.size())
			line.push_back(m_ptLastCT);
		m_ptLastCT = m_ct.Trans(pt);
		line.push_back(m_ptLastCT);
	}
};

//-----------------------------------------------------------------------------
// CCanvasMat
class AFX_EXT_CLASS_SHAPE CSCanvasMat : public ISCanvas {
public:
	cv::Size m_sizeEffective;
	cv::Mat m_mat;
protected:
	cv::Scalar m_crBGR;
	CPoint2d m_ptLastCT;
	int m_iLineThickness;

public:
	CSCanvasMat() {
		m_iLineThickness = 1;
	}
	CSCanvasMat(const CSCanvasMat& B) { *this = B; }
	virtual ~CSCanvasMat() {}

	CSCanvasMat& operator = (const CSCanvasMat& B) {
		if (this == &B)
			return *this;
		(ISCanvas&)*this = (ISCanvas&)B;
		m_sizeEffective		= B.m_sizeEffective;
		B.m_mat.copyTo(m_mat);
		m_crBGR				= B.m_crBGR;
		m_ptLastCT			= B.m_ptLastCT;
		m_iLineThickness	= B.m_iLineThickness;

		return *this;
	}

	virtual BOOL CreateCanvas(const TRectD& rectSource, const TRectD& rectTarget, BOOL bKeepAspectRatio = TRUE);
	CCoordTrans GetCT(const CRect& rectClient) const {
		CCoordTrans ct(m_ct);
		ct.m_ptOffset.x += (rectClient.Width() - m_sizeEffective.width) /2;
		ct.m_ptOffset.y += (rectClient.Height() - m_sizeEffective.height) /2;
		return ct;
	}

public:
	virtual void SetColor(COLORREF cr);
	virtual void MoveToImpl(const CPoint2d& pt);
	virtual void LineToImpl(const CPoint2d& pt);

	virtual void Dot(const CPoint2d& pt, DWORD dwDuration = 0 /* in usec */);

public:
	int SetLineThickness(int iLineThickness = 1) { int iOldLineThickness = m_iLineThickness; m_iLineThickness = iLineThickness; return iOldLineThickness; }
	int GetLineThickness() const { return m_iLineThickness; }

public:
	static cv::Scalar RGB2Scalar(COLORREF cr);
	static BOOL MatToDC(const cv::Mat& img, const cv::Size& sizeEffective, CDC& dc, const CRect& rect);

};

//-----------------------------------------------------------------------------
//
BOOL AFX_EXT_API_SHAPE AddLaserOffsetToLine(const TLineD& pts, TLineD& ptsResult, double dThickness, BOOL bLoop);

#pragma pack(pop)
