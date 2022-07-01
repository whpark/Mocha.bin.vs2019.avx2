#pragma once

#include "AFX_EXT_SHAPE.h"
#include "ShapeLine.h"
#include "ShapeArc.h"

#pragma pack(push, 8)


template<typename T>
inline T RoundEx(T src, int e=4) {
	src = src*pow((T)10, e-1);
	src = (src>0.)?floor((T)(src+.5)):ceil((T)(src-.5));
	src = src*pow((T)10, -(e-1));
	return src;
}

class AFX_EXT_CLASS_SHAPE CByArc {
public:
	TList<CShapeObject>	m_objects;
	TRefPointer<CShapeObject> m_pObject;	// PWH.
	CByArc(CShapeObject* pObject, const cv::Point2d& ptStart, const double& tanStart, const cv::Point2d& ptEnd, const double& tanEnd, double epsilon);
	virtual ~CByArc();
	double GetByArcFittingError(const cv::Point2d& pt);
};



class AFX_EXT_CLASS_SHAPE CBSpline {
public:
	int							m_iDegree;
	std::vector<double>			m_Knots;
	std::vector<cv::Point3d>	m_ControlPoints;
	int							m_iKnots, 
								m_iControlPoints;
	int							m_iAxis;				// 사용되는 축 수
	std::vector<double>			m_Weights;
	int							m_iWeights;
	std::vector<cv::Point3d>	m_HCPoints;			// 균일 포인트 vector

	TList<CPoint2d>		m_BSplinePoints;
	TList<double>		m_BSplineTangents;
	TList<CPoint2d>		m_Points;	
	CBSpline(int degree, const std::vector<double>& knots, const std::vector<cv::Point3d>& pts, const std::vector<double>& weights);
	virtual ~CBSpline();
	// N번째 유도체에 균일하게 배포 지점의 수를 계산
	// 단일 vector에서 NURBS control point와 가중치 변환
	void				PointsToHPts();
	cv::Point2d			HPtsToPoints(std::vector<double>& hpts);
	void				CalcCurve(int n=0, int nCPtsNum=20);
	// "THE NURBS BOOK"pg.93에서 Algorithm A3.2의 수정 버전
	// N번째 파생에 균일 포인트를 계산
	void				BSpline_DersEvaluate(const int& n, const double& u, std::vector<std::vector<double>>& CK);
	// Algorithm A2.1 from "THE NURBS BOOK" pg.68
	// Knot vector의 위치 계산
	int					FindSpan(double u);
	// Algorithm A2.3 from "THE NURBS BOOK" pg.72
	// U point에서 N번째를 유도함.
	void				BSpline_DersBasis(const int& ispan,  const double& u, int n, std::vector<std::vector<double>>& ders);
};




class AFX_EXT_CLASS_SHAPE CNURBS : public CBSpline {
public:		
	double								epsilon,
										epsilon_high;
	double								segments;
	std::vector<double>					m_MChanges;
	std::vector<std::vector<double>>	m_Ignores;
	CShapeObject*						m_pObject;	
	TList<CShapeObject>					m_ByArcCurves;
	std::vector<cv::Point3d>			m_ptByArcCurves;
	CNURBS(CShapeObject* pObject, const int& degree, const std::vector<double>& knots, const std::vector<cv::Point3d>& pts, const std::vector<double>& weights, double tol, int check);
	virtual ~CNURBS();
	// Knots vector 확인
	// Node로 검색(degree+1 >= 오류)
	BOOL								CheckParameter(double tol=1e-6, int check=1);
	void								NURBS_Evaluate(const int& n, const double& u, cv::Point2d& pt, double& dTan);
	void								CalcHighAccuracyByArcCurve();
	void								CalcUSection(std::vector<std::vector<double>>& uSections);
	void								CalcByArcSection(std::vector<double>& uSections, double dMaxStep);
	void								AnalysisAndCompress();
	void								CompressByArcs();
	double								CheckDiffToPts(std::vector<cv::Point2d>& pts, const CShapeArc& arc0, const CShapeArc& arc1);
	void								CompressLines();
	double								DistanceToPoint(const cv::Point2d& ptStart, const cv::Point2d& ptEnd, const cv::Point2d& ptTarget);
};



class AFX_EXT_CLASS_SHAPE CSplineToArc : public CNURBS {
public:	
	CSplineToArc(CShapeObject* pObject, const int& degree, const std::vector<double>& knots,  const std::vector<cv::Point3d>& pts, const std::vector<double>& weights, double tol=0.01, int check=1);
	//{
		//CheckParameter();
		//CalcHighAccuracyByArcCurve();
		//AnalysisAndCompress();
	//}
	virtual ~CSplineToArc();
};
