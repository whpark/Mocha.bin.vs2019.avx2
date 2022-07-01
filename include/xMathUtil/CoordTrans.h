#pragma once

#include "AFX_EXT_XMU.h"
#include "Coordinate.hpp"
#include "CoordTransEx.h"

#pragma pack(push, 8)

//-----------------------------------------------------------------------------
//class CCoordTrans;

class AFX_EXT_CLASS_XMU CCoordTrans : public ICoordTrans {
public:
	//typedef struct {
	//	double dScale;
	//	double m00, m01, m02, m10, m11, m12;
	//	double dShiftX, dShiftY;
	//	double dOffsetX, dOffsetY;
	//} T_MAT_TRANS;

	//union {
	//	T_MAT_TRANS m_matTrans;
	//	struct {
	//		double m_dScale;
	//		cv::Matx22d m_mat;
	//		CPoint2d m_ptShift;
	//		CPoint2d m_ptOffset;
	//	};
	//};
	double m_dScale;
	cv::Matx22d m_mat;
	CPoint2d m_ptShift;
	CPoint2d m_ptOffset;

public:
	// Constructors
	CCoordTrans(double dScale = 1.0, const cv::Matx22d& m = cv::Matx22d::eye(), const CPoint2d& ptShift = CPoint2d(), const CPoint2d& ptOffset = CPoint2d()) :
		m_dScale(dScale),
		m_mat(m),
		m_ptShift(ptShift),
		m_ptOffset(ptOffset)
	{
	}
	CCoordTrans(double dScale, double m00, double m01, double m10, double m11, double dShiftX, double dShiftY, double dOffsetX, double dOffsetY) :
		m_dScale(dScale),
		m_mat(m00, m01, m10, m11),
		m_ptShift(dShiftX, dShiftY),
		m_ptOffset(dOffsetX, dOffsetY)
	{
	}

	DECLARE_NEWCLONE(CCoordTrans);

	virtual CPoint2d Trans(const CPoint2d& pt) const	{ CPoint2d ptT; Trans(pt.x, pt.y, ptT.x, ptT.y); return ptT; }
	virtual CPoint3d Trans(const CPoint3d& pt) const	{ CPoint3d ptT; Trans(pt.x, pt.y, ptT.x, ptT.y); ptT.z = pt.z; return ptT; }
	virtual CPoint2d TransI(const CPoint2d& pt) const	{ return GetInverse().Trans(pt); }
	virtual CPoint3d TransI(const CPoint3d& pt) const	{ return GetInverse().Trans(pt); }

	CCoordTrans(const CCoordTrans& B) = default;
	CCoordTrans& operator = (const CCoordTrans& B) = default;
	bool operator == (const CCoordTrans& B) const { return (m_dScale == B.m_dScale) && (m_mat == B.m_mat) && (m_ptShift == B.m_ptShift) && (m_ptOffset == B.m_ptOffset); }
	bool operator != (const CCoordTrans& B) const { return !(*this == B); }
	//-------------------------------------------------------------------------
	bool GetInverse(CCoordTrans& ctI) const;
	CCoordTrans GetInverse() const;

	//-------------------------------------------------------------------------
	// Setting
	bool Set(double dScale = 1.0, double m00 = 1.0, double m01 = 0.0, double m10 = 0.0, double m11 = 1.0, double dShiftX = 0.0, double dShiftY = 0.0, double dOffsetX = 0.0, double dOffsetY = 0.0);
	bool SetR(double dScale, angle_rad_t dTheta, const CPoint2d& ptShift = CPoint2d(), const CPoint2d& ptOffset = CPoint2d());
	bool Set(double dScale, const cv::Matx22d& m, const CPoint2d& ptShift, const CPoint2d& ptOffset);

	bool SetFrom2Points(const double xs[2], const double ys[2], const double xd[2], const double yd[2], bool bCalcScale = true, double dMinDeterminant = 0.0);
	bool SetFrom3Points(const double xs[3], const double ys[3], const double xd[3], const double yd[3], bool bCalcScale = true, double dMinDeterminant = 0.0);

	template < class TPOINT >
	bool SetFrom2Points(const TPOINT ptSource[2], const TPOINT ptTarget[2], bool bCalcScale = true, double dMinDeterminant = 0.0) {
		double xs[2], ys[2], xd[2], yd[2];
		for (int i = 0; i < 2; i++) {
			xs[i] = ptSource[i].x; ys[i] = ptSource[i].y; xd[i] = ptTarget[i].x; yd[i] = ptTarget[i].y;
		}
		return SetFrom2Points(xs, ys, xd, yd, bCalcScale, dMinDeterminant);
	}
	template < class TPOINT, typename INDEX_T, typename SIZE_T, template < class TPOINT, typename INDEX_T, typename SIZE_T > class TLIST >
	bool SetFrom2Points(const TLIST<TPOINT, INDEX_T, SIZE_T>& ptSource, const TLIST<TPOINT, INDEX_T, SIZE_T>& ptTarget, bool bCalcScale = true, double dMinDeterminant = 0.0) {
		double xs[2], ys[2], xd[2], yd[2];
		if ( (ptSource.N() < 2) || (ptTarget.N() < 2) )
			return false;
		for (int i = 0; i < 2; i++) {
			xs[i] = ptSource[i].x; ys[i] = ptSource[i].y; xd[i] = ptTarget[i].x; yd[i] = ptTarget[i].y;
		}
		return SetFrom2Points(xs, ys, xd, yd, bCalcScale, dMinDeterminant);
	}
	template < class TPOINT >
	bool SetFrom3Points(const TPOINT ptSource[3], const TPOINT ptTarget[3], bool bCalcScale = true, double dMinDeterminant = 0.0) {
		double xs[3], ys[3], xd[3], yd[3];
		for (int i = 0; i < 3; i++) {
			xs[i] = ptSource[i].x; ys[i] = ptSource[i].y; xd[i] = ptTarget[i].x; yd[i] = ptTarget[i].y;
		}
		return SetFrom3Points(xs, ys, xd, yd, bCalcScale, dMinDeterminant);
	}
	template < class TCONTAINER >
	bool SetFromPoints(const TCONTAINER& ptSource, const TCONTAINER& ptTarget, bool bCalcScale = true, double dMinDeterminant = 0.0) {
		double xs[4], ys[4], xd[4], yd[4];
		int nPoint = (int)_min(countof(xs), _min(ptSource.size(), ptTarget.size()));
		if (nPoint < 2)
			return false;
		for (int i = 0; i < nPoint; i++) {
			xs[i] = ptSource[i].x; ys[i] = ptSource[i].y; xd[i] = ptTarget[i].x; yd[i] = ptTarget[i].y;
		}
		if (nPoint <= 2)
			return SetFrom2Points(xs, ys, xd, yd, bCalcScale, dMinDeterminant);
		else if (nPoint <= 3)
			return SetFrom3Points(xs, ys, xd, yd, bCalcScale, dMinDeterminant);
		return false;
	}

	void SetScale(double dScale) { m_dScale = dScale; }
	double GetScale() const { return m_dScale; }
	void SetMatrix(double m00, double m01, double m10, double m11) {
		m_mat = cv::Matx22d(m00, m01, m10, m11);
	}
	void SetMatrix(const cv::Matx22d& m) {
		m_mat = m;
	}
	void SetMatrix(angle_rad_t dTheta) {
		double c, s;
		c = cos(dTheta);
		s = sin(dTheta);
		SetMatrix(c, -s, s, c);
	}
	void GetMatrix(double& m00, double& m01, double& m10, double& m11) const { m00 = m_mat(0, 0); m01 = m_mat(0, 1); m10 = m_mat(1, 0); m11 = m_mat(1, 1); }

	void GetShift(double& dShiftX, double& dShiftY) const {
		dShiftX = m_ptShift.x; dShiftY = m_ptShift.y;
	}
	template < class T, template <typename> class TPOINT >
	void GetShift(TPOINT<T>& ptShift) const {
		ptShift.x = m_ptShift.x; ptShift.y = m_ptShift.y;
	}
	const CPoint2d& GetShift() const { return m_ptShift; }
	CPoint2d& GetShift() { return m_ptShift; }
	void SetShift(double dShiftX, double dShiftY) {
		m_ptShift.x = dShiftX; m_ptShift.y = dShiftY;
	}
	template < class TPOINT >
	void SetShift(const TPOINT& ptShift) {
		m_ptShift.x = ptShift.x; m_ptShift.y = ptShift.y;
	}

	void GetOffset(double& dOffsetX, double& dOffsetY) const {
		dOffsetX = m_ptOffset.x; dOffsetY = m_ptOffset.y;
	}
	template < class TPOINT >
	void GetOffset(TPOINT& ptOffset) const {
		ptOffset.x = m_ptOffset.x; ptOffset.y = m_ptOffset.y;
	}
	const CPoint2d& GetOffset() const { return m_ptOffset; }
	CPoint2d& GetOffset() { return m_ptOffset; }
	void SetOffset(double dOffsetX, double dOffsetY) {
		m_ptOffset.x = dOffsetX; m_ptOffset.y = dOffsetY;
	}
	template < class TPOINT >
	void SetOffset(const TPOINT& ptOffset) {
		m_ptOffset.x = ptOffset.x; m_ptOffset.y = ptOffset.y;
	}

	void RotateM(angle_rad_t dTheta);
	void Rotate(angle_rad_t dTheta, double x, double y);
	template < class TPOINT >
	void Rotate(angle_rad_t dTheta, const TPOINT& pt) { Rotate(dTheta, pt.x, pt.y); }
	void NegMX();
	void NegMY();
	void FlipMX();
	void FlipMY();
	void FlipX(double x = 0);
	void FlipY(double y = 0);
	void FlipXY(double x = 0, double y = 0);
	template < class TPOINT >
	void FlipXY(const TPOINT pt) { FlipXY(pt.x, pt.y); }

	//-------------------------------------------------------------------------
	// Operation
	//
	virtual void Trans(double x0, double y0, double& x1, double& y1) const {
		double x = x0 - m_ptShift.x;
		double y = y0 - m_ptShift.y;
		x1 = m_dScale * (m_mat(0, 0)*x + m_mat(0, 1)*y) + m_ptOffset.x;
		y1 = m_dScale * (m_mat(1, 0)*x + m_mat(1, 1)*y) + m_ptOffset.y;
	}
	virtual void Trans(double x0, double y0, int& x1, int& y1) const {
		double x = x0 - m_ptShift.x;
		double y = y0 - m_ptShift.y;
		x1 = _round(m_dScale * (m_mat(0, 0)*x + m_mat(0, 1)*y) + m_ptOffset.x);
		y1 = _round(m_dScale * (m_mat(1, 0)*x + m_mat(1, 1)*y) + m_ptOffset.y);
	}
	virtual void Trans(double x0, double y0, long& x1, long& y1) const {
		double x = x0 - m_ptShift.x;
		double y = y0 - m_ptShift.y;
		x1 = _round(m_dScale * (m_mat(0, 0)*x + m_mat(0, 1)*y) + m_ptOffset.x);
		y1 = _round(m_dScale * (m_mat(1, 0)*x + m_mat(1, 1)*y) + m_ptOffset.y);
	}

	template < class TPOINT >
	TPOINT Trans(double x, double y) const {
		double x2, y2;
		Trans(x, y, x2, y2);
		return TPOINT(x2, y2);
	}
	template < class TPOINT >
	TPOINT Trans(const TPOINT& pt) const {
		TPOINT ptResult(pt);		// X, Y 이외에 Z 축이 있을경우, 보호하기 위해서...
		Trans(pt.x, pt.y, ptResult.x, ptResult.y);
		return ptResult;
	}
	template < int nPoint, class TPOINT >
	void TransArr(TPOINT pts[nPoint]) const {
		for (int i = 0; i < nPoint; i++)
			Trans(pts[i].x, pts[i].y, pts[i].x, pts[i].y);
	}
	template < class TPOINT >
	void TransArr(TPOINT* ppts, int nPoint) const {
		if (!ppts)
			return;
		for (int i = 0; i < nPoint; i++)
			Trans(ppts[i].x, ppts[i].y, ppts[i].x, ppts[i].y);
	}
	template < class container >
	void TransContainer(container& pts) const {
		for (int i = 0; i < pts.size(); i++)
			Trans(pts[i].x, pts[i].y, pts[i].x, pts[i].y);
	}
	template < class TPOINT, typename INDEX_T, typename SIZE_T, template < class TPOINT, typename INDEX_T, typename SIZE_T > class TLIST >
	void TransL(const TLIST<TPOINT, INDEX_T, SIZE_T>& pts, TLIST<TPOINT, INDEX_T, SIZE_T>& ptsResult) const {
		ptsResult.CopyFrom(pts);
		TransL<TPOINT>(ptsResult);
	}
	template < class TPOINT, typename INDEX_T, typename SIZE_T, template < class TPOINT, typename INDEX_T, typename SIZE_T > class TLIST >
	void TransL(TLIST<TPOINT, INDEX_T, SIZE_T>& pts) const {
		for (SIZE_T i = 0; i < pts.N(); i++)
			pts[i] = Trans<TPOINT>(pts[i]);
	}
	template < class TPOINT >
	void TransL(const std::vector<TPOINT>& pts, std::vector<TPOINT>& ptsResult) const {
		ptsResult = pts;
		TransL<TPOINT>(ptsResult);
	}
	template < class TPOINT >
	void TransL(std::vector<TPOINT>& pts) const {
		for (SIZE_T i = 0; i < pts.size(); i++)
			pts[i] = Trans<TPOINT>(pts[i]);
	}
	double Trans(double dLength) const;
	double TransLength(double dLength) const { return Trans(dLength); }

	template < class TPOINT >
	TPOINT TransI(double x, double y) const {
		return GetInverse().Trans<TPOINT>(x, y);
	}

	template < class TPOINT >
	TPOINT TransI(const TPOINT& pt) const { return GetInverse().Trans<TPOINT>(pt); }

	template < class TPOINT, typename INDEX_T, typename SIZE_T, template < class TPOINT, typename INDEX_T, typename SIZE_T > class TLIST >
	bool TransIL(const TLIST<TPOINT, INDEX_T, SIZE_T>& pts, TLIST<TPOINT, INDEX_T, SIZE_T>& ptsResult) const {
		CCoordTrans ctI;
		bool bResult = GetInverse(ctI);
		ctI.Trans<TPOINT>(pts, ptsResult);
		return bResult;
	}
	template < class TPOINT, typename INDEX_T, typename SIZE_T, template < class TPOINT, typename INDEX_T, typename SIZE_T > class TLIST >
	void TransIL(TLIST<TPOINT, INDEX_T, SIZE_T>& pts) const {
		CCoordTrans ctI;
		bool bResult = GetInverse(ctI);
		ctI.Trans<TPOINT, INDEX_T, SIZE_T>(pts);
		return bResult;
	}
	template < class TPOINT >
	void TransIL(const std::vector<TPOINT>& pts, std::vector<TPOINT>& ptsResult) const {
		CCoordTrans ctI;
		bool bResult = GetInverse(ctI);
		ctI.TransL(pts, ptsResult);
		return bResult;
	}
	template < class TPOINT >
	void TransIL(std::vector<TPOINT>& pts) const {
		CCoordTrans ctI;
		bool bResult = GetInverse(ctI);
		ctI.TransL(pts);
		return bResult;
	}
	double TransI(double dLength) const;
	double TransLengthI(double dLength) const { return Trans(dLength); }
	virtual bool IsRightHanded() const;

	template < class TPOINT >
	TPOINT operator() (const TPOINT& pt) const { return Trans<TPOINT>(pt.x, pt.y); }
	template < class TPOINT >
	TPOINT operator() (double x, double y) const { return Trans<TPOINT>(x, y); }

	CCoordTrans& operator *= (const CCoordTrans& B);
	CCoordTrans operator * (const CCoordTrans& B) const;

//
public:
	template <class Archive> Archive& StoreTo(Archive& ar) const {
		ar << CStringA("CoordTrans1.0");
		ar << m_dScale
			<< m_mat(0, 0) << m_mat(0, 1) << m_mat(1, 0) << m_mat(1, 1)
			<< m_ptShift
			<< m_ptOffset
			;
		return ar;
	}
	template <class Archive> Archive& LoadFrom(Archive& ar) {
		CStringA str;
		ar >> str;
		if ( (str == "CoordTrans1.0") || (str == "CoordTrans2d") ) {
			ar >> m_dScale
				>> m_mat(0, 0) >> m_mat(0, 1) >> m_mat(1, 0) >> m_mat(1, 1)
				>> m_ptShift
				>> m_ptOffset
				;
		} else if (str == "CoordTrans2.0") {
			double m02dummy = 0, m12dummy = 0;
			ar >> m_dScale
				>> m_mat(0, 0) >> m_mat(0, 1) >> m02dummy >> m_mat(1, 0) >> m_mat(1, 1) >> m12dummy
				>> m_ptShift
				>> m_ptOffset
				;
		}
		return ar;
	}
	template <class Archive>
	friend Archive& operator << (Archive& ar, const CCoordTrans& B) {
		return B.StoreTo(ar);
	}
	template <class Archive>
	friend Archive& operator >> (Archive& ar, CCoordTrans& B) {
		return B.LoadFrom(ar);
	}

	template <class CProfileSection> 
	bool SyncData(bool bStore, CProfileSection& section) {
		section.SyncItemValue(bStore, _T("Scale"), m_dScale);
		section.SyncItemValue(bStore, _T("m00"), m_mat(0, 0));
		section.SyncItemValue(bStore, _T("m01"), m_mat(0, 1));
		section.SyncItemValue(bStore, _T("m10"), m_mat(1, 0));
		section.SyncItemValue(bStore, _T("m11"), m_mat(1, 1));
		section.SyncItemValue(bStore, _T("ShiftX"), m_ptShift.x);
		section.SyncItemValue(bStore, _T("ShiftY"), m_ptShift.y);
		section.SyncItemValue(bStore, _T("OffsetX"), m_ptOffset.x);
		section.SyncItemValue(bStore, _T("OffsetY"), m_ptOffset.y);
		return true;
	}

public:
	// 함수 이름이 너무 길어서 정리.
	//
	BOOL SetTransformMatrix(double dScale = 1.0, double m00 = 1.0, double m01 = 0.0, double m10 = 0.0, double m11 = 1.0, double dShiftX = 0.0, double dShiftY = 0.0, double dOffsetX = 0.0, double dOffsetY = 0.0) {
		return Set(dScale, { m00, m01, m10, m11 } , { dShiftX, dShiftY }, { dOffsetX, dOffsetY });
	}
	BOOL SetTransformMatrixT(double dScale = 1.0, double dTheta = 0.0, double dShiftX = 0.0, double dShiftY = 0.0, double dOffsetX = 0.0, double dOffsetY = 0.0) {
		return SetR(dScale, (rad_t)dTheta, { dShiftX, dShiftY }, { dOffsetX, dOffsetY });
	}

	BOOL SetTransformMatrixFrom2Points(const double xs[2], const double ys[2], const double xd[2], const double yd[2], BOOL bCalcScale = TRUE, double dMinDeterminant = 0.0) {
		return SetFrom2Points(xs, ys, xd, yd, bCalcScale ? true : false, dMinDeterminant);
	}
	BOOL SetTransformMatrixFrom3Points(const double xs[3], const double ys[3], const double xd[3], const double yd[3], BOOL bCalcScale = TRUE, double dMinDeterminant = 0.0) {
		return SetFrom3Points(xs, ys, xd, yd, bCalcScale ? true : false, dMinDeterminant);
	}

	template < class TPOINT >
	BOOL SetTransformMatrixFrom2Points(const TPOINT ptSource[2], const TPOINT ptTarget[2], BOOL bCalcScale = TRUE, double dMinDeterminant = 0.0) {
		double xs[2], ys[2], xd[2], yd[2];
		for (int i = 0; i < 2; i++) {
			xs[i] = ptSource[i].x; ys[i] = ptSource[i].y; xd[i] = ptTarget[i].x; yd[i] = ptTarget[i].y;
		}
		return SetFrom2Points(xs, ys, xd, yd, bCalcScale, dMinDeterminant);
	}
	template < class TPOINT, typename INDEX_T, typename SIZE_T, template < class TPOINT, typename INDEX_T, typename SIZE_T > class TLIST >
	BOOL SetTransformMatrixFrom2Points(const TLIST<TPOINT, INDEX_T, SIZE_T>& ptSource, const TLIST<TPOINT, INDEX_T, SIZE_T>& ptTarget, BOOL bCalcScale = TRUE, double dMinDeterminant = 0.0) {
		double xs[2], ys[2], xd[2], yd[2];
		if ( (ptSource.N() < 2) || (ptTarget.N() < 2) )
			return FALSE;
		for (int i = 0; i < 2; i++) {
			xs[i] = ptSource[i].x; ys[i] = ptSource[i].y; xd[i] = ptTarget[i].x; yd[i] = ptTarget[i].y;
		}
		return SetFrom2Points(xs, ys, xd, yd, bCalcScale, dMinDeterminant);
	}
	template < class TPOINT >
	BOOL SetTransformMatrixFrom3Points(const TPOINT ptSource[3], const TPOINT ptTarget[3], BOOL bCalcScale = TRUE, double dMinDeterminant = 0.0) {
		double xs[3], ys[3], xd[3], yd[3];
		for (int i = 0; i < 3; i++) {
			xs[i] = ptSource[i].x; ys[i] = ptSource[i].y; xd[i] = ptTarget[i].x; yd[i] = ptTarget[i].y;
		}
		return SetFrom3Points(xs, ys, xd, yd, bCalcScale, dMinDeterminant);
	}

};


#pragma pack(pop)
