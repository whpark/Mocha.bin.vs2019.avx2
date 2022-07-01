#pragma once

#include "AFX_EXT_XMU.h"

#include "Coordinate.hpp"
#include "TList.hpp"
#include "CoordTransEx.h"

#pragma pack(push, 8)

//=============================================================================
//class IMeshTable
//
class AFX_EXT_CLASS_XMU IMeshTable : public ICoordTrans {
protected:
	cv::Mat m_matSource;
	cv::Mat m_matTarget;

public:
	IMeshTable() {
	}
	virtual ~IMeshTable() {
	}
	IMeshTable(const IMeshTable& B) { *this = B; }
	IMeshTable& operator = (const IMeshTable& B) {
		if (this == &B) return *this;
		B.m_matSource.copyTo(m_matSource);
		B.m_matTarget.copyTo(m_matTarget);
		return *this;
	}

	// Initialize
public:
	virtual BOOL Clear();
	virtual BOOL IsEmpty() const { return m_matSource.empty() || m_matTarget.empty(); }
	virtual BOOL MakeTable(const cv::Mat& matSource, const cv::Mat& matTarget);

	cv::Mat GetSourceMatrix() const { cv::Mat mat; m_matSource.copyTo(mat); return mat; }
	cv::Mat GetTargetMatrix() const { cv::Mat mat; m_matTarget.copyTo(mat); return mat; }

	// Operation
public:
	virtual BOOL Trans0(const CPoint3d& v1, cv::Vec6d& v2, BOOL bExtrapolation = TRUE) const = NULL;
	virtual BOOL Trans0I(const CPoint3d& v1, cv::Vec6d& v2, BOOL bExtrapolation = TRUE) const = NULL;

public:
	BOOL Trans(double v1, double& v2, BOOL bExtrapolation = TRUE);
	BOOL Trans(double v1, CPoint2d& v2, BOOL bExtrapolation = TRUE);
	BOOL Trans(double v1, CPoint3d& v2, BOOL bExtrapolation = TRUE);
	BOOL Trans(const CPoint2d& pt, double& v2, BOOL bExtrapolation = TRUE);
	BOOL Trans(const CPoint2d& pt, CPoint2d& v2, BOOL bExtrapolation = TRUE);
	BOOL Trans(const CPoint2d& pt, CPoint3d& v2, BOOL bExtrapolation = TRUE);

	BOOL TransI(double v1, double& v2, BOOL bExtrapolation = TRUE);
	BOOL TransI(double v1, CPoint2d& v2, BOOL bExtrapolation = TRUE);
	BOOL TransI(double v1, CPoint3d& v2, BOOL bExtrapolation = TRUE);
	BOOL TransI(const CPoint2d& pt, double& v2, BOOL bExtrapolation = TRUE);
	BOOL TransI(const CPoint2d& pt, CPoint2d& v2, BOOL bExtrapolation = TRUE);
	BOOL TransI(const CPoint2d& pt, CPoint3d& v2, BOOL bExtrapolation = TRUE);

protected:
	BOOL Trans(const CPoint3d& pt, double& v2, BOOL bExtrapolation = TRUE);		// N/A. for future use.
	BOOL Trans(const CPoint3d& pt, CPoint2d& v2, BOOL bExtrapolation = TRUE);	// N/A. for future use.
	BOOL Trans(const CPoint3d& pt, CPoint3d& v2, BOOL bExtrapolation = TRUE);	// N/A. for future use.
	BOOL TransI(const CPoint3d& pt, double& v2, BOOL bExtrapolation = TRUE);	// N/A. for future use.
	BOOL TransI(const CPoint3d& pt, CPoint2d& v2, BOOL bExtrapolation = TRUE);	// N/A. for future use.
	BOOL TransI(const CPoint3d& pt, CPoint3d& v2, BOOL bExtrapolation = TRUE);	// N/A. for future use.

public:
	static const char s_sign[16];
	template < class ARCHIVE >
	friend ARCHIVE& operator << (ARCHIVE& ar, const IMeshTable& B) {
		ar.Write(s_sign, sizeof(s_sign));
		StoreMat(ar, B.m_matSource);
		StoreMat(ar, B.m_matTarget);
		return ar;
	}
	template < class ARCHIVE >
	friend ARCHIVE& operator >> (ARCHIVE& ar, IMeshTable& B) {
		char sign[sizeof(IMeshTable::s_sign)];
		ar.Read(sign, sizeof(sign));
		if (memcmp(s_sign, sign, sizeof(s_sign)) != 0) {
			__throw_exception__("NOT MeshTable");
			return ar;
		}
		LoadMat(ar, B.m_matSource);
		LoadMat(ar, B.m_matTarget);
		return ar;
	}

	// ICoordTrans
public:
	bool m_bExtrapolation = true;
	virtual CPoint2d Trans(const CPoint2d& pt) const {
		cv::Vec6d v2;
		if (Trans0(pt, v2, m_bExtrapolation))
			return CPoint2d(v2[0], v2[1]);
		return pt;
	}
	virtual CPoint3d Trans(const CPoint3d& pt) const {
		cv::Vec6d v2;
		if (Trans0(pt, v2, m_bExtrapolation))
			return CPoint3d(v2[0], v2[1], v2[2]);
		return pt;
	}
	virtual CPoint2d TransI(const CPoint2d& pt) const {
		cv::Vec6d v2;
		if (Trans0I(pt, v2, m_bExtrapolation))
			return CPoint2d(v2[0], v2[1]);
		return pt;
	}
	virtual CPoint3d TransI(const CPoint3d& pt) const {
		cv::Vec6d v2;
		if (Trans0I(pt, v2, m_bExtrapolation))
			return CPoint3d(v2[0], v2[1], v2[2]);
		return pt;
	}
	virtual double TransLength(double dLength) const {
		CPoint2d pt0(0, 0), pt1(dLength, dLength);
		CPoint2d ptA = Trans(pt0);
		CPoint2d ptB = Trans(pt1);
		return ptA.Distance(ptB) / sqrt(2.0);
	}
	virtual double TransLengthI(double dLength) const {
		CPoint2d pt0(0, 0), pt1(1, 1);
		CPoint2d ptA = Trans(pt0);
		CPoint2d ptB = Trans(pt1);
		return sqrt(2.0) / ptA.Distance(ptB) * dLength;
	}

protected:
	virtual bool IsRightHanded() const { ASSERT(FALSE); return true; }	// N/A

};

//=============================================================================
//class CMeshTableLinear
//
class AFX_EXT_CLASS_XMU CMeshTableLinear : public IMeshTable {
public:
	CMeshTableLinear() : IMeshTable() {
	}
	virtual ~CMeshTableLinear() {
	}
	CMeshTableLinear(const CMeshTableLinear& B) = default;

	DECLARE_NEWCLONE(CMeshTableLinear);

	// Operation
public:
	virtual BOOL Trans0(const CPoint3d& v1, cv::Vec6d& v2, BOOL bExtrapolation = TRUE) const;
	virtual BOOL Trans0I(const CPoint3d& v1, cv::Vec6d& v2, BOOL bExtrapolation = TRUE) const;

public:
	static BOOL TransLinear(const cv::Mat& matSource, const cv::Mat& matTarget, const CPoint3d& v1, cv::Vec6d& v2, BOOL bExtrapolation);
protected:
	static BOOL TransLinear1d(const cv::Mat& matSource, const cv::Mat& matTarget, const CPoint3d& v1, cv::Vec6d& v2, BOOL bExtrapolation);
	static BOOL TransLinear2d(const cv::Mat& matSource, const cv::Mat& matTarget, const CPoint3d& v1, cv::Vec6d& v2, BOOL bExtrapolation);
	static BOOL TransLinear3d(const cv::Mat& matSource, const cv::Mat& matTarget, const CPoint3d& v1, cv::Vec6d& v2, BOOL bExtrapolation);

};

//=============================================================================
//class CMeshTablePolynomial
//
class AFX_EXT_CLASS_XMU CMeshTablePolynomial : public IMeshTable {
protected:
	int m_nMaxOrderX, m_nMaxOrderY, m_nMaxOrderZ;
	int m_eMethod;
	BOOL m_bLocalFit;	// Locally Optimize Polynomials
	BOOL m_bSearchEntireRegion;
	class CPolynomial {
	public:
		cv::Rect region;
		int nOrderX;
		int nOrderY;
		std::vector<cv::Mat> matCoeffs;
		CPolynomial() {}
		CPolynomial(const CPolynomial& B) { *this = B; }
		CPolynomial(const cv::Rect& _region, int _nOrderX, int _nOrderY, const std::vector<cv::Mat>& _matCoeffs);
		CPolynomial& operator = (const CPolynomial& B);
	};
	std::vector<CPolynomial> m_table;
	std::vector<CPolynomial> m_tableI;
public:
	CMeshTablePolynomial(BOOL bLocalFit = TRUE, int nMaxOrderX = 4, int nMaxOrderY = -1, int nMaxOrderZ = -1, BOOL bSearchEntireRegion = TRUE) : IMeshTable() {
		m_bLocalFit = bLocalFit;
		m_bSearchEntireRegion = bSearchEntireRegion;
		SetMaxOrder(nMaxOrderX, nMaxOrderY, nMaxOrderZ);
		m_eMethod = cv::DECOMP_LU|cv::DECOMP_NORMAL;
	}
	virtual ~CMeshTablePolynomial() {
	}
	CMeshTablePolynomial(const CMeshTablePolynomial& B) = default;

	DECLARE_NEWCLONE(CMeshTablePolynomial);

	// Initialize
public:
	virtual BOOL Clear();
	virtual BOOL IsEmpty() const { return m_table.empty(); }
	virtual BOOL MakeTable(const cv::Mat& matSource, const cv::Mat& matTarget);
protected:
	BOOL MakeTable(const cv::Mat& matSource, const cv::Mat& matTarget, std::vector<CPolynomial>& table);

	// Operation
public:
	BOOL SetLocalFit(BOOL bLocalFit = TRUE);
	BOOL IsLocalFit(BOOL bLocalFit = TRUE) const { return m_bLocalFit; }
	BOOL SetMaxOrder(int nMaxOrderX = 4, int nMaxOrderY = -1, int nMaxOrderZ = -1);
	int GetMaxOrder(int* pnMaxOrderX = NULL, int* pnMaxOrderY = NULL, int* pnMaxOrderZ = NULL) const;	// returns nMaxOrderX
	BOOL SetSolvingMethod(int eMethod = cv::DECOMP_LU|cv::DECOMP_NORMAL);
	int GetSolvingMethod() const { return m_eMethod; }
	void SetPositionSearchMethod(BOOL bSearchEntireRegion = TRUE) { m_bSearchEntireRegion = bSearchEntireRegion; }
	virtual BOOL Trans0(const CPoint3d& v1, cv::Vec6d& v2, BOOL bExtrapolation = TRUE) const;
	virtual BOOL Trans0I(const CPoint3d& v1, cv::Vec6d& v2, BOOL bExtrapolation = TRUE) const;

public:
	static BOOL TransPolynomial(const cv::Mat& matSource, const cv::Mat& matTarget, const CPoint3d& v1, cv::Vec6d& v2, const std::vector<CPolynomial>& table, BOOL bExtrapolation, BOOL bSearchEntireRegion);
protected:
	static BOOL TransPolynomial1d2d(const cv::Mat& matSource, const cv::Mat& matTarget, const CPoint3d& v1, cv::Vec6d& v2, const std::vector<CPolynomial>& table, BOOL bExtrapolation, BOOL bSearchEntireRegion);
	static BOOL TransPolynomial3d(const cv::Mat& matSource, const cv::Mat& matTarget, const CPoint3d& v1, cv::Vec6d& v2, const std::vector<CPolynomial>& table, BOOL bExtrapolation, BOOL bSearchEntireRegion);

};

#pragma pack(pop)
