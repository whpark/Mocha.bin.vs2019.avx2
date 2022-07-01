#pragma once

#include "AFX_EXT_XMU.h"

#include "Coordinate.hpp"
#include "TList.hpp"

#pragma pack(push, 8)

//=============================================================================
//class CMeshTransform
//
class AFX_EXT_CLASS_XMU CMeshCompensate {
public:
	enum eCOMPENSATE_METHOD { CM_INVALID = 0, CM_LINEAR, CM_POLYNOMIAL, CM_SCANNER, CM_COS, CM_SQUARE };
protected:
	eCOMPENSATE_METHOD m_eCompensateMethod;
	cv::Mat m_matSource;
	cv::Mat m_matTarget;
	cv::Mat m_matCoefficient;
	cv::Mat m_matCoefficientI;
	double m_dScaleX;
	double m_dScaleY;
	double m_dScaleXI;
	double m_dScaleYI;
public:
	CMeshCompensate() {
		m_eCompensateMethod = CM_INVALID;
		m_dScaleX = 1.0;
		m_dScaleY = 1.0;
		m_dScaleXI = 1.0;
		m_dScaleYI = 1.0;
	}
	~CMeshCompensate() {
	}

public:
	BOOL Load(LPCTSTR pszFileName);
	BOOL Save(LPCTSTR pszFileName) const;
	BOOL MakeTable(const cv::Mat& matSource, const cv::Mat& matTarget, eCOMPENSATE_METHOD eCompensateMethod = CM_LINEAR, int nMaxDegreePolynomial = 4, int eSolutionMethod = 0/*cV::DECOMP_LU*/);
	BOOL PtInRect(const TPointD& pt) const;

public:
	void GetMatrix(cv::Mat* pMatSource = NULL, cv::Mat* pMatTarget = NULL, cv::Mat* pMatCoefficient = NULL, cv::Mat* pMatCoefficientI = NULL) const;
	void GetConstants(double* pdScaleX = NULL, double* pdScaleY = NULL, double* pdScaleXI = NULL, double* pdScaleYI = NULL) const;
	eCOMPENSATE_METHOD GetMethod() const { return m_eCompensateMethod; }

public:
	double Compensate(double dValue, BOOL bExtrapolation = FALSE, BOOL* pbResult = NULL) const;
	TPointD Compensate(const TPointD& pt, BOOL bExtrapolation = FALSE, BOOL* pbResult = NULL) const;
	BOOL Compensate(const TList<TPointD>& ptsSource, TList<TPointD>& ptsTarget, BOOL bExtrapolation = FALSE) const;

	double CompensateI(double dValue, BOOL bExtrapolation = FALSE, BOOL* pbResult = NULL) const;
	TPointD CompensateI(const TPointD& pt, BOOL bExtrapolation = FALSE, BOOL* pbResult = NULL) const;
	BOOL CompensateI(const TList<TPointD>& ptsSource, TList<TPointD>& ptsTarget, BOOL bExtrapolation = FALSE) const;

protected:
	static BOOL MakePolynomialCompensateTable(const cv::Mat& matSource, const cv::Mat& matTarget, int nMaxDegreePolynomial, cv::Mat& matCoefficient, int eSolutionMethod);
	static BOOL MakeScannerCompensateTable(const cv::Mat& matSource, const cv::Mat& matTarget, int nMaxDegreePolynomial, cv::Mat& matCoefficient);
	static BOOL MakeCosCompensateTable(const cv::Mat& matSource, const cv::Mat& matTarget, int nMaxDegreePolynomial, cv::Mat& matCoefficient, double& dScaleX, double& dScaleY);
	static TPointD Compensate(const TPointD& pt, const cv::Mat& matSource, const cv::Mat& matTarget, const cv::Mat& matCoefficient, double dScaleX, double dScaleY, eCOMPENSATE_METHOD eCompensateMethod, BOOL bExtrapolation, BOOL& bResult);
	static BOOL GetRelativePosition(const TPointD ptBounding[4]/*CCW, left top must be the first point*/, const TPointD& pt, double& dRateX, double& dRateY);
};

#pragma pack(pop)
