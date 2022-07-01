#pragma once

#include "AFX_EXT_XMU.h"
#include "Coordinate.hpp"

#include "qtl/qtl.hpp"
#include "for_each_member.h"

#pragma pack(push, 8)

//-----------------------------------------------------------------------------
// interface CoordTrans;
class ICoordTrans {
public:
	// Constructors
	ICoordTrans() { }
	virtual ~ICoordTrans() { }

	DECLARE_NEWCLONE_BASE_INTERFACE(ICoordTrans);

	CPoint2d operator () (const CPoint2d& pt) const { return Trans(pt); }
	CPoint3d operator () (const CPoint3d& pt) const { return Trans(pt); }

	//-------------------------------------------------------------------------
	// Operation
	//
	virtual CPoint2d Trans(const CPoint2d& pt) const = NULL;
	virtual CPoint2d TransI(const CPoint2d& pt) const = NULL;
	virtual CPoint3d Trans(const CPoint3d& pt) const = NULL;
	virtual CPoint3d TransI(const CPoint3d& pt) const = NULL;

	virtual double TransLength(double dLength) const = NULL;
	virtual double TransLengthI(double dLength) const = NULL;

	virtual bool IsRightHanded() const = NULL;
};


//-----------------------------------------------------------------------------
class AFX_EXT_CLASS_XMU CCoordTransChain : public ICoordTrans {
protected:
	qtl::TPtrDeque<ICoordTrans> m_chain;	// 마지막 Back() CT부터 Front() 까지 Transform() 적용.
public:
	// Constructors
	CCoordTransChain() { }
	virtual ~CCoordTransChain() { }
	CCoordTransChain(const CCoordTransChain& B) = default;
	CCoordTransChain& operator = (const CCoordTransChain& B) = default;

	CCoordTransChain& operator *= (const CCoordTransChain& B)	{ for (const auto& ct : m_chain) PushChainBack(ct.NewClone()); return *this; }
	CCoordTransChain& operator *= (const ICoordTrans& B)		{ PushChainBack(B.NewClone()); return *this; }
	CCoordTransChain operator * (const CCoordTransChain& B) {
		CCoordTransChain ct;
		m_chain.AddTo(ct.m_chain);
		B.m_chain.AddTo(ct.m_chain);
		return std::move(ct);
	}

	void PushChainFront(ICoordTrans* pCT)	{ m_chain.PushFront(pCT); }
	void PushChainBack(ICoordTrans* pCT)	{ m_chain.Push(pCT); }
	void ClearChain() { m_chain.DeleteAll(); }

	DECLARE_NEWCLONE(CCoordTransChain);

	//-------------------------------------------------------------------------
	// Operation
	//
	virtual CPoint2d Trans(const CPoint2d& pt) const;
	virtual CPoint2d TransI(const CPoint2d& pt) const;
	virtual CPoint3d Trans(const CPoint3d& pt) const;
	virtual CPoint3d TransI(const CPoint3d& pt) const;

	virtual double TransLength(double dLength) const;
	virtual double TransLengthI(double dLength) const;
	virtual bool IsRightHanded() const;

};


//-----------------------------------------------------------------------------
//class CCoordTrans2d;
class AFX_EXT_CLASS_XMU CCoordTrans2d : public ICoordTrans {
public:
	double m_dScale;
	cv::Matx22d m_mat;
	CPoint2d m_ptShift;
	CPoint2d m_ptOffset;

public:
	// Constructors
	CCoordTrans2d(double dScale = 1.0, const cv::Matx22d& m = cv::Matx22d::eye(), const CPoint2d& ptShift = CPoint2d(), const CPoint2d& ptOffset = CPoint2d()) :
		m_dScale(dScale),
		m_mat(m),
		m_ptShift(ptShift),
		m_ptOffset(ptOffset)
	{
	}

	CCoordTrans2d(const CCoordTrans2d& B) = default;
	CCoordTrans2d& operator = (const CCoordTrans2d& B) = default;

	DECLARE_NEWCLONE(CCoordTrans2d);

	//bool operator == (const CCoordTrans2d& B) const { return (m_dScale == B.m_dScale) && (m_mat == B.m_mat) && (m_ptShift == B.m_ptShift) && (m_ptOffset == B.m_ptOffset); }
	//bool operator != (const CCoordTrans2d& B) const { return !(*this == B); }
	//-------------------------------------------------------------------------
	bool GetInverse(CCoordTrans2d& ctI) const;
	CCoordTrans2d GetInverse() const;

	//-------------------------------------------------------------------------
	// Setting
	bool Set(double dScale, const cv::Matx22d& m, const CPoint2d& ptShift, const CPoint2d& ptOffset);
	bool SetR(double dScale, angle_rad_t dTheta, const CPoint2d& ptShift = CPoint2d(), const CPoint2d& ptOffset = CPoint2d());

	bool SetFrom2Points(const CPoint2d ptsSource[], const CPoint2d ptsTarget[], bool bCalcScale = true, double dMinDeterminant = 0.0);
	bool SetFrom3Points(const CPoint2d ptsSource[], const CPoint2d ptsTarget[], bool bCalcScale = true, double dMinDeterminant = 0.0);

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

	void GetShift(double& dShiftX, double& dShiftY) const		{ dShiftX = m_ptShift.x; dShiftY = m_ptShift.y; }
	const CPoint2d& GetShift() const							{ return m_ptShift; }
	CPoint2d& GetShift()										{ return m_ptShift; }
	void SetShift(double dShiftX, double dShiftY)				{ m_ptShift.x = dShiftX; m_ptShift.y = dShiftY; }
	void SetShift(const CPoint2d& ptShift)						{ m_ptShift = ptShift; }

	void GetOffset(double& dOffsetX, double& dOffsetY) const	{ dOffsetX = m_ptOffset.x; dOffsetY = m_ptOffset.y; }
	const CPoint2d& GetOffset() const							{ return m_ptOffset; }
	CPoint2d& GetOffset()										{ return m_ptOffset; }
	void SetOffset(double dOffsetX, double dOffsetY)			{ m_ptOffset.x = dOffsetX; m_ptOffset.y = dOffsetY; }
	void SetOffset(const CPoint2d& ptOffset)					{ m_ptOffset = ptOffset; }

	void RotateM(angle_rad_t dTheta);
	void Rotate(angle_rad_t dTheta, const CPoint2d& ptCenter);
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
	virtual CPoint2d Trans(const CPoint2d& pt) const {
		double x = pt.x - m_ptShift.x;
		double y = pt.y - m_ptShift.y;
		CPoint2d ptT;
		ptT = m_dScale * (m_mat * (pt-m_ptShift)) + m_ptOffset;
		return ptT;
	}
	virtual CPoint3d Trans(const CPoint3d& pt) const {
		CPoint2d ptT(Trans((CPoint2d&)pt));
		return CPoint3d(ptT.x, ptT.y, pt.z);
	}
	virtual CPoint2d TransI(const CPoint2d& pt) const {
		return GetInverse().Trans(pt);
	}
	virtual CPoint3d TransI(const CPoint3d& pt) const {
		CPoint2d ptT(GetInverse().Trans(pt));
		return CPoint3d(ptT.x, ptT.y, pt.z);
	}

	virtual double TransLength(double dLength) const {
		CPoint2d pt0 = Trans(CPoint2d(0, 0));
		CPoint2d pt1 = Trans(CPoint2d(1, 1));
		double dScale = pt0.Distance(pt1) / sqrt(2.0);
		return dScale * dLength;
	}
	virtual double TransLengthI(double dLength) const {
		CPoint2d pt0 = Trans(CPoint2d(0, 0));
		CPoint2d pt1 = Trans(CPoint2d(1, 1));
		double dScale = sqrt(2.0) / pt0.Distance(pt1);
		return dScale * dLength;
	}
	virtual bool IsRightHanded() const;

	CCoordTrans2d& operator *= (const CCoordTrans2d& B);
	CCoordTrans2d operator * (const CCoordTrans2d& B) const;

//
public:
	template <class Archive> Archive& StoreTo(Archive& ar) const {
		ar << CStringA("CoordTrans2d");
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
		} else {
			__throw_exception__("Serialization FAILED");
		}
		return ar;
	}
	template <class Archive>
	friend Archive& operator << (Archive& ar, const CCoordTrans2d& B) {
		return B.StoreTo(ar);
	}
	template <class Archive>
	friend Archive& operator >> (Archive& ar, CCoordTrans2d& B) {
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
	FEM_BEGIN_TBL(CCoordTrans2d)
		FEM_ADD_MEMBER(m_dScale)
		FEM_ADD_MEMBER(m_mat)
		FEM_ADD_MEMBER(m_ptShift)
		FEM_ADD_MEMBER(m_ptOffset)
	FEM_END_TBL()

};


//=============================================================================
class AFX_EXT_CLASS_XMU CCoordTransNL : public CCoordTrans2d {
public:
	double m_mat02 = 0, m_mat12 = 0;
public:
	// Constructors
	CCoordTransNL(double dScale = 1.0, const cv::Matx23d& m = cv::Matx23d::eye(), const CPoint2d& ptShift = CPoint2d(), const CPoint2d& ptOffset = CPoint2d()) :
		CCoordTrans2d(dScale, cv::Matx22d(m(0, 0), m(0, 1), m(1, 0), m(1, 1)), ptShift, ptOffset)
	{
	}

	CCoordTransNL(const CCoordTransNL& B) = default;
	CCoordTransNL(const CCoordTrans2d& B) : CCoordTrans2d(B), m_mat02(0), m_mat12(0) {}

	DECLARE_NEWCLONE(CCoordTransNL);

	CCoordTransNL& operator = (const CCoordTransNL& B) = default;
	CCoordTransNL& operator = (const CCoordTrans2d& B) { (CCoordTrans2d&)*this = B; m_mat02 = 0; m_mat12 = 0; return *this; }

	//bool operator == (const CCoordTransNL& B) const { return ((CCoordTrans2d&)*this == (CCoordTrans2d&)B) && (m_mat02 == B.m_mat02) && (m_mat12 == B.m_mat12); }
	//bool operator != (const CCoordTransNL& B) const { return !(*this == B); }

	//-------------------------------------------------------------------------
protected:
	bool GetInverse(CCoordTransNL& ctI) const {
		if ( (m_mat02 == 0.0) && (m_mat12 == 0.0) )
			return __super::GetInverse(ctI);
		throw std::exception("GetInverse() NOT Supported");
	}
	CCoordTransNL GetInverse() const {
		if ( (m_mat02 == 0.0) && (m_mat12 == 0.0) )
			return __super::GetInverse();
		throw std::exception("GetInverse() NOT Supported");
	}

public:
	//-------------------------------------------------------------------------
	// Setting
	bool Set(double dScale, const cv::Matx22d& m, const CPoint2d& ptShift, const CPoint2d& ptOffset);
	bool Set(double dScale, const cv::Matx23d& m, const CPoint2d& ptShift, const CPoint2d& ptOffset);

	bool SetFrom2Points(const CPoint2d ptsSource[], const CPoint2d ptsTarget[], bool bCalcScale = true, double dMinDeterminant = 0.0) {
		m_mat02 = m_mat12 = 0;
		return __super::SetFrom2Points(ptsSource, ptsTarget, bCalcScale, dMinDeterminant);
	}
	bool SetFrom3Points(const CPoint2d ptsSource[], const CPoint2d ptsTarget[], bool bCalcScale = true, double dMinDeterminant = 0.0) {
		m_mat02 = m_mat12 = 0;
		return __super::SetFrom3Points(ptsSource, ptsTarget, bCalcScale, dMinDeterminant);
	}
	bool SetFrom4Points(const CPoint2d ptsSource[4], const CPoint2d ptsTarget[4], bool bCalcScale = true, double dMinDeterminant = 0.0);

	void SetMatrix(double m00, double m01, double m02, double m10, double m11, double m12) {
		m_mat(0, 0) = m00; m_mat(0, 1) = m01; m_mat02 = m02;
		m_mat(1, 0) = m10; m_mat(1, 1) = m11; m_mat12 = m12;
	}
	void SetMatrix(const cv::Matx23d& m) {
		m_mat(0, 0) = m(0, 0); m_mat(0, 1) = m(0, 1); m_mat02 = m(0, 2);
		m_mat(1, 0) = m(1, 0); m_mat(1, 1) = m(1, 1); m_mat12 = m(1, 2);
	}
	void GetMatrix(double& m00, double& m01, double& m02, double& m10, double& m11, double& m12) const {
		m00 = m_mat(0, 0); m01 = m_mat(0, 1); m02 = m_mat02;
		m10 = m_mat(1, 0); m11 = m_mat(1, 1); m12 = m_mat12;
	}
	void GetMatrixXY(double& m02, double& m12) const { m02 = m_mat02; m12 = m_mat12; }
	void SetMatrixXY(double m02, double m12) { m_mat02 = m02; m_mat12 = m12; }

	void NegMX();
	void NegMY();
	void FlipMX();
	void FlipMY();
	void FlipX(double x = 0);
	void FlipY(double y = 0);
	void FlipXY(double x = 0, double y = 0);
	void FlipXY(const CPoint2d& pt) { FlipXY(pt.x, pt.y); }

	//-------------------------------------------------------------------------
	// Operation
	//
	virtual CPoint2d Trans(const CPoint2d& pt) const {
		CPoint2d p = pt-m_ptShift;
		CPoint2d ptT;
		ptT = m_dScale * (m_mat * p + CPoint2d(m_mat02, m_mat12)*p.x*p.y) + m_ptOffset;
		return ptT;
	}
	virtual CPoint3d Trans(const CPoint3d& pt) const {
		CPoint2d ptT(Trans((CPoint2d&)pt));
		return CPoint3d(ptT.x, ptT.y, pt.z);
	}
	virtual CPoint2d TransI(const CPoint2d& pt) const {
		ASSERT(false);
		return GetInverse().Trans(pt);
	}
	virtual CPoint3d TransI(const CPoint3d& pt) const {
		ASSERT(false);
		CPoint2d ptT(GetInverse().Trans(pt));
		return CPoint3d(ptT.x, ptT.y, pt.z);
	}

	friend CCoordTransNL operator * (const CCoordTrans2d& A, const CCoordTransNL& B);

	//
public:
	template <class Archive> Archive& StoreTo(Archive& ar) const {
		ar << CStringA("CoordTransNL");
		ar << m_dScale
			<< m_mat(0, 0) << m_mat(0, 1) << m_mat02 << m_mat(1, 0) << m_mat(1, 1) << m_mat12
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
			m_mat02 = m_mat12 = 0;
		} else if ( (str == "CoordTrans2.0") || (str == "CoordTransNL") ) {
			ar >> m_dScale
				>> m_mat(0, 0) >> m_mat(0, 1) >> m_mat02 >> m_mat(1, 0) >> m_mat(1, 1) >> m_mat12
				>> m_ptShift
				>> m_ptOffset
				;
		}
		return ar;
	}
	template <class Archive>
	friend Archive& operator << (Archive& ar, const CCoordTransNL& B) {
		return B.StoreTo(ar);
	}
	template <class Archive>
	friend Archive& operator >> (Archive& ar, CCoordTransNL& B) {
		return B.LoadFrom(ar);
	}

	template <class CProfileSection> 
	bool SyncData(bool bStore, CProfileSection& section) {
		__super::SyncData(bStore, section);
		section.SyncItemValue(bStore, _T("m02"), m_mat02);
		section.SyncItemValue(bStore, _T("m12"), m_mat12);
		return true;
	}

public:
	FEM_BEGIN_TBL_C(CCoordTransNL, CCoordTrans2d)
		FEM_ADD_MEMBER(m_mat02)
		FEM_ADD_MEMBER(m_mat12)
	FEM_END_TBL()
};


//=============================================================================

class AFX_EXT_CLASS_XMU CCoordTrans3d : public ICoordTrans {
public:
	typedef cv::Matx33d mat_t;
public:
	double m_dScale;
	mat_t m_mat;
	CPoint3d m_ptShift;
	CPoint3d m_ptOffset;

public:
	// Constructors
	CCoordTrans3d(
		double dScale = 1.0,
		const mat_t& mat = mat_t::eye(),		// I (eigen matrix)
		const CPoint3d& ptShift = CPoint3d(0, 0, 0),
		const CPoint3d& ptOffset = CPoint3d(0, 0, 0)
		)
	{
		m_dScale = dScale;
		m_mat = mat;
		m_ptShift = ptShift;
		m_ptOffset = ptOffset;
	}
	CCoordTrans3d(const CCoordTrans3d& B) = default;
	CCoordTrans3d& operator = (const CCoordTrans3d& B) = default;
	//{
	//	if (this != &B) {
	//		m_dScale		= B.m_dScale;
	//		m_mat			= B.m_mat;
	//		m_ptShift		= B.m_ptShift;
	//		m_ptOffset		= B.m_ptOffset;
	//	}
	//	return *this;
	//}

	DECLARE_NEWCLONE(CCoordTrans3d);

	//bool operator == (const CCoordTrans3d& B) const { return (m_dScale == B.m_dScale) && (m_mat == B.m_mat) && (m_ptShift == B.m_ptShift) && (m_ptOffset == B.m_ptOffset); }
	//bool operator != (const CCoordTrans3d& B) const { return !(*this == B); }

	bool IsSame(const CCoordTrans3d& B) const {
		return (*this)(CPoint3d(0, 0, 0)) == B.Trans(CPoint3d(0, 0, 0))
			&& (*this)(CPoint3d(1, 0, 0)) == B.Trans(CPoint3d(1, 0, 0))
			&& (*this)(CPoint3d(0, 1, 0)) == B.Trans(CPoint3d(0, 1, 0))
			&& (*this)(CPoint3d(0, 0, 1)) == B.Trans(CPoint3d(0, 0, 1))
			;
	}

	//static bool IsValid(const cv::Mat& mat);
	//bool IsValid() const { return IsValid(m_mat); }

	//-------------------------------------------------------------------------
	bool GetInverse(CCoordTrans3d& ctI) const;
	CCoordTrans3d GetInverse() const;

	//-------------------------------------------------------------------------
	// Setting
	bool Set(double dScale = 1.0,
		const mat_t& mat = mat_t::eye(),		// I (eigen matrix)
		const CPoint3d& ptShift = CPoint3d(0, 0, 0),
		const CPoint3d& ptOffset = CPoint3d(0, 0, 0));
	bool SetRXAxis(double dScale = 1.0,
		angle_rad_t dT = 0.0_rad,
		const CPoint3d& ptShift = CPoint3d(0, 0, 0),
		const CPoint3d& ptOffset = CPoint3d(0, 0, 0));
	bool SetRYAxis(double dScale = 1.0,
		angle_rad_t dT = 0.0_rad,
		const CPoint3d& ptShift = CPoint3d(0, 0, 0),
		const CPoint3d& ptOffset = CPoint3d(0, 0, 0));
	bool SetRZAxis(double dScale = 1.0,
		angle_rad_t dT = 0.0_rad,
		const CPoint3d& ptShift = CPoint3d(0, 0, 0),
		const CPoint3d& ptOffset = CPoint3d(0, 0, 0));
	bool SetRXYZ(double dScale = 1.0,
		angle_rad_t dTx = 0.0_rad, angle_rad_t dTy = 0.0_rad, angle_rad_t dTz = 0.0_rad,
		const CPoint3d& ptShift = CPoint3d(0, 0, 0),
		const CPoint3d& ptOffset = CPoint3d(0, 0, 0));

	bool SetFrom4Points(const CPoint3d pts0[4], const CPoint3d pts1[4], bool bCalcScale = true, double dMinDeterminant = 0.0);
	bool SetFrom3Points(const CPoint3d pts0[3], const CPoint3d pts1[3], bool bCalcScale = true, double dMinDeterminant = 0.0);
	//bool SetFrom2Points(const CPoint3d pts0[2], const CPoint3d pts1[2], bool bCalcScale = true, double dMinDeterminant = 0.0);

	void SetScale(double dScale) { m_dScale = dScale; }
	double GetScale() const { return m_dScale; }
	void SetMatrix(double m[9]) {
		m_mat = mat_t(m);
	}
	void SetMatrix(const mat_t& mat) { m_mat = mat; }
	void SetMatrix(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22) {
		double m[] = { m00, m01, m02, m10, m11, m12, m20, m21, m22 };
		SetMatrix(m);
	}
	void SetMatrixRotateXAxis(angle_rad_t dT) {
		double c = cos(dT), s = sin(dT);
		SetMatrix(1, 0, 0, 0, c, -s, 0, s, c);
	}
	void SetMatrixRotateYAxis(angle_rad_t dT) {
		double c = cos(dT), s = sin(dT);
		SetMatrix(c, 0, s, 0, 1, 0, -s, 0, c);
	}
	void SetMatrixRotateZAxis(angle_rad_t dT) {
		double c = cos(dT), s = sin(dT);
		SetMatrix(c, -s, 0, s, c, 0, 0, 0, 1);
	}
	void SetMatrixRotateXYZ(angle_rad_t dTx, angle_rad_t dTy, angle_rad_t dTz) { m_mat = GetRMatX(dTx) * GetRMatY(dTy) * GetRMatZ(dTz); }
	void SetMatrixRotateXZY(angle_rad_t dTx, angle_rad_t dTy, angle_rad_t dTz) { m_mat = GetRMatX(dTx) * GetRMatZ(dTz) * GetRMatY(dTy); }
	void SetMatrixRotateYXZ(angle_rad_t dTx, angle_rad_t dTy, angle_rad_t dTz) { m_mat = GetRMatY(dTy) * GetRMatX(dTx) * GetRMatY(dTz); }
	void SetMatrixRotateYZX(angle_rad_t dTx, angle_rad_t dTy, angle_rad_t dTz) { m_mat = GetRMatY(dTy) * GetRMatZ(dTz) * GetRMatX(dTx); }
	void SetMatrixRotateZXY(angle_rad_t dTx, angle_rad_t dTy, angle_rad_t dTz) { m_mat = GetRMatZ(dTz) * GetRMatX(dTx) * GetRMatY(dTy); }
	void SetMatrixRotateZYX(angle_rad_t dTx, angle_rad_t dTy, angle_rad_t dTz) { m_mat = GetRMatZ(dTz) * GetRMatY(dTy) * GetRMatX(dTx); }
	mat_t& GetMatrix() { return m_mat; }
	const mat_t& GetMatrix() const { return m_mat; }
	void GetMatrix(double m[mat_t::cols * mat_t::rows]) const {
		//ASSERT((m_mat.cols >= 3) && (m_mat.rows >= 3) && (m_mat.depth() == CV_64F));
		//if ( (m_mat.cols < 3) && (m_mat.rows < 3) && (m_mat.depth() != CV_64F) )
		//	return;
		m[0] = m_mat(0, 0); m[1] = m_mat(0, 1); m[2] = m_mat(0, 2);
		m[3] = m_mat(1, 0); m[4] = m_mat(1, 1); m[5] = m_mat(1, 2);
		m[6] = m_mat(2, 0); m[7] = m_mat(2, 1); m[8] = m_mat(2, 2);
	}
	void GetMatrix(double& m00, double& m01, double& m02, double& m10, double& m11, double& m12, double& m20, double& m21, double& m22) const {
		//ASSERT((m_mat.cols >= 3) && (m_mat.rows >= 3) && (m_mat.depth() == CV_64F));
		//if ( (m_mat.cols < 3) && (m_mat.rows < 3) && (m_mat.depth() != CV_64F) )
		//	return;
		m00 = m_mat(0, 0); m01 = m_mat(0, 1); m02 = m_mat(0, 2);
		m10 = m_mat(1, 0); m11 = m_mat(1, 1); m12 = m_mat(1, 2);
		m20 = m_mat(2, 0); m21 = m_mat(2, 1); m22 = m_mat(2, 2);
	}

	void SetShift(double dShiftX, double dShiftY, double dShiftZ) {
		m_ptShift.SetPoint(dShiftX, dShiftY, dShiftZ);
	}
	template < class TPOINT >
	void SetShift(const TPOINT& ptShift) {
		m_ptShift.SetPoint(ptShift.x, ptShift.y, ptShift.z);
	}
	void GetShift(double& dShiftX, double& dShiftY, double& dShiftZ) const {
		dShiftX = m_ptShift.x; dShiftY = m_ptShift.y; dShiftZ = m_ptShift.z;
	}
	template < class TPOINT >
	void GetShift(TPOINT& ptShift) const {
		ptShift.x = m_ptShift.x; ptShift.y = m_ptShift.y; ptShift.z = m_ptShift.z;
	}
	const CPoint3d& GetShift() const { return m_ptShift; }
	CPoint3d& GetShift() { return m_ptShift; }

	void SetOffset(double dOffsetX, double dOffsetY, double dOffsetZ) {
		m_ptOffset.SetPoint(dOffsetX, dOffsetY, dOffsetZ);
	}
	template < class TPOINT >
	void SetOffset(const TPOINT& ptOffset) {
		m_ptOffset.SetPoint(ptOffset.x, ptOffset.y, ptOffset.z);
	}
	void GetOffset(double& dOffsetX, double& dOffsetY, double& dOffsetZ) const {
		dOffsetX = m_ptOffset.x; dOffsetY = m_ptOffset.y; dOffsetZ = m_ptOffset.z;
	}
	template < class TPOINT >
	void GetOffset(TPOINT& ptOffset) const {
		ptOffset.x = m_ptOffset.x; ptOffset.y = m_ptOffset.y; ptOffset.z = m_ptOffset.z;
	}
	const CPoint3d& GetOffset() const { return m_ptOffset; }
	CPoint3d& GetOffset() { return m_ptOffset; }

	void RotateMX(angle_rad_t dTx);
	void RotateMY(angle_rad_t dTy);
	void RotateMZ(angle_rad_t dTz);
	void FlipMX();
	void FlipMY();
	void FlipMZ();
	void FlipX(double x = 0);
	void FlipY(double y = 0);
	void FlipZ(double z = 0);
	void FlipXY(double x = 0, double y = 0);
	void FlipYZ(double y = 0, double z = 0);
	void FlipZX(double z = 0, double x = 0);
	void FlipXYZ(double x = 0, double y = 0, double z = 0);
	void FlipXYZ(const CPoint3d& ptCenter);

	//-------------------------------------------------------------------------
	// Operation
	//

		// ptTarget = (dScale + alpha) * m * ( ptSource - ptShift ) + ptOffset
	void Trans(double x0, double y0, double z0, double& x1, double& y1, double& z1) const;

	virtual CPoint2d Trans(const CPoint2d& pt) const { return Trans(CPoint3d(pt.x, pt.y, 0)); }
	virtual CPoint2d TransI(const CPoint2d& pt) const { return TransI(CPoint3d(pt.x, pt.y, 0)); }
	virtual CPoint3d Trans(const CPoint3d& pt) const {
		CPoint3d ptT;
		ptT = m_dScale * (m_mat * (pt - m_ptShift)) + m_ptOffset;
		return ptT;
	}
	virtual CPoint3d TransI(const CPoint3d& pt) const {
		return GetInverse().Trans(pt);
	}

	virtual double TransLength(double dLength) const {
		CPoint3d pt0 = Trans(CPoint3d(0, 0, 0));
		CPoint3d pt1 = Trans(CPoint3d(1, 1, 1));
		double dScale = pt0.Distance(pt1) / sqrt(3.0);
		return dScale * dLength;
	}
	virtual double TransLengthI(double dLength) const {
		CPoint3d pt0 = Trans(CPoint3d(0, 0, 0));
		CPoint3d pt1 = Trans(CPoint3d(1, 1, 1));
		double dScale = sqrt(3.0) / pt0.Distance(pt1);
		return dScale * dLength;
	}
	bool IsRightHanded() const;


	CPoint3d Project(const CPoint3d& pt, const CPoint3d& alpha = CPoint3d(0, 0, 0)) const;

	CCoordTrans3d& operator *= (const CCoordTrans3d& B);
	CCoordTrans3d operator * (const CCoordTrans3d& B) const;

//
public:
	template <class Archive> Archive& StoreTo(Archive& ar) const {
		ar << CStringA("CoordTrans3d1.1");
		ar << m_dScale;
		double m[9];
		GetMatrix(m);
		ar << m[0] << m[1] << m[2] << m[3] << m[4] << m[5] << m[6] << m[7] << m[8];
		ar << m_ptShift;
		ar << m_ptOffset;
		return ar;
	}
	template <class Archive> Archive& LoadFrom(Archive& ar) {
		CStringA str;
		ar >> str;
		if (str != "CoordTrans3d1.1")
			return ar;
		ar >> m_dScale;
		double m[9];
		ar >> m[0] >> m[1] >> m[2] >> m[3] >> m[4] >> m[5] >> m[6] >> m[7] >> m[8];
		SetMatrix(m);
		ar >> m_ptShift;
		ar >> m_ptOffset;
		return ar;
	}
	template <class Archive>
	friend Archive& operator << (Archive& ar, const CCoordTrans3d& B) {
		return B.StoreTo(ar);
	}
	template <class Archive>
	friend Archive& operator >> (Archive& ar, CCoordTrans3d& B) {
		return B.LoadFrom(ar);
	}

	template <class CProfileSection> 
	bool SyncData(bool bStore, CProfileSection& section) {
		section.SyncItemValue(bStore, _T("Scale"), m_dScale);
		double m[9] = {
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};
		if (bStore)
			GetMatrix(m);
		section.SyncItemValue(bStore, _T("m00"), m[0]);
		section.SyncItemValue(bStore, _T("m01"), m[1]);
		section.SyncItemValue(bStore, _T("m02"), m[2]);
		section.SyncItemValue(bStore, _T("m10"), m[3]);
		section.SyncItemValue(bStore, _T("m11"), m[4]);
		section.SyncItemValue(bStore, _T("m12"), m[5]);
		section.SyncItemValue(bStore, _T("m20"), m[6]);
		section.SyncItemValue(bStore, _T("m21"), m[7]);
		section.SyncItemValue(bStore, _T("m22"), m[8]);
		if (!bStore)
			SetMatrix(m);
		section.SyncItemValue(bStore, _T("Shfit"), m_ptShift);
		section.SyncItemValue(bStore, _T("Offset"), m_ptOffset);
		return true;
	}

public:
	static mat_t GetRMatX(angle_rad_t dT);
	static mat_t GetRMatY(angle_rad_t dT);
	static mat_t GetRMatZ(angle_rad_t dT);
	static bool MakeProjection(CCoordTrans3d& ct, const CPoint3d& vecPlaneNorm, const CPoint3d& ptPlaneBase = CPoint3d());
	static bool MakeProjectionOntoXY(CCoordTrans3d& ct, const CPoint3d& vecPlaneX, const CPoint3d& vecPlaneY, const CPoint3d& ptPlaneBase = CPoint3d());

public:
	FEM_BEGIN_TBL(CCoordTrans3d)
		FEM_ADD_MEMBER(m_dScale)
		FEM_ADD_MEMBER(m_mat)
		FEM_ADD_MEMBER(m_ptShift)
		FEM_ADD_MEMBER(m_ptOffset)
	FEM_END_TBL()
};



template < class CIPVar >
bool SyncIPVarCT(bool bStore, CIPVar& varCT, CCoordTrans2d& ct) {
	if (bStore) {
		varCT.SetChildItemT("scale",	ct.m_dScale);
		varCT.SetChildItemT("m00",		ct.m_mat(0, 0));
		varCT.SetChildItemT("m01",		ct.m_mat(0, 1));
		varCT.SetChildItemT("m10",		ct.m_mat(1, 0));
		varCT.SetChildItemT("m11",		ct.m_mat(1, 1));
		varCT.SetChildItemT("Shift",	ct.m_ptShift);
		varCT.SetChildItemT("Offset",	ct.m_ptOffset);
	} else {
		varCT.GetChildItemT("scale",	ct.m_dScale);
		varCT.GetChildItemT("m00",		ct.m_mat(0, 0));
		varCT.GetChildItemT("m01",		ct.m_mat(0, 1));
		varCT.GetChildItemT("m10",		ct.m_mat(1, 0));
		varCT.GetChildItemT("m11",		ct.m_mat(1, 1));
		varCT.GetChildItemT("Shift",	ct.m_ptShift);
		varCT.GetChildItemT("Offset",	ct.m_ptOffset);
	}
	return true;
}

template < class CIPVar >
bool SyncIPVarCT(bool bStore, CIPVar& varCT, CCoordTransNL& ct) {
	SyncIPVarCT(bStore, varCT, (CCoordTrans2d&)ct);
	if (bStore) {
		varCT.SetChildItemT("m02",		ct.m_mat02);
		varCT.SetChildItemT("m12",		ct.m_mat12);
	} else {
		varCT.GetChildItemT("m02",		ct.m_mat02);
		varCT.GetChildItemT("m12",		ct.m_mat12);
	}
	return true;
}


#pragma pack(pop)
