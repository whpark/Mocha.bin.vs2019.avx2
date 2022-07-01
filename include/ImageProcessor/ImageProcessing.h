#pragma once

#include "AFX_EXT_IMAGE_PROCESSOR.h"
#include "Coordinate.hpp"

#include <vector>

namespace imgproc {


// 장성규
// 이미지 전처리

typedef struct s_SeedPoint
{
	cv::Point seedPoint;
	int color;
}S_SEEDPOINT;

typedef struct GeoMatch
{
	int						noOfCordinates;		//Number of elements in coordinate array
	std::vector<cv::Point>	cordinates;			//Coordinates array to store model points	
	cv::Size				modelSize;			//Template Size
	std::vector<double>		edgeMagnitude;		//gradient magnitude
	std::vector<double>		edgeDerivativeX;	//gradient in X direction
	std::vector<double>		edgeDerivativeY;	//gradient in Y direction	
	cv::Point				centerOfGravity;	//Center of gravity of template 
	bool					modelDefined;
}S_GEOMATCH;

typedef enum {SAUVOLA, NIBLACK, WOLF, NICK} ThresholdMethod;

class CImageProcessing
{
public:
	CImageProcessing(){}
	virtual ~CImageProcessing(){}
	virtual BOOL ColorToGrayConverting(const cv::Mat& src, cv::Mat& dst);
	virtual BOOL Thresholding(const cv::Mat& src, cv::Mat& dst, const int& iThreshold = 128, const BOOL& bInv = FALSE, const BOOL& bOtsu = FALSE);
	virtual BOOL ShadingCorrection(const cv::Mat& src, cv::Mat& dst, const cv::Point& pCenter, const int& iCenterValue = 230, const int& iEdgeValue = 60);
	virtual BOOL Labeling(const cv::Mat& src, cv::Mat& dst, std::vector<std::vector<cv::Point>>& contours, const double& dMinArea = 5000, const double& dMaxArea = 3133440, const BOOL& bFilled = FALSE, const BOOL& bExternal = FALSE);
	virtual BOOL GetMaxObject(const cv::Mat& src, cv::Mat& dst, cv::Rect& rect);
	virtual BOOL TemplateMatching(const cv::Mat& src, const cv::Mat& tmp, CPoint2d& resultPoint, double& matchingRate, const int& resizeCount = 0);
	virtual BOOL TemplateMatching(const cv::Mat& src, const cv::Mat& tmp, const cv::Mat& mask, CPoint2d& resultPoint, double& matchingRate, const int& resizeCount = 0);	// 템플릿 매칭 (마스크 설정 영역은 매칭 제외)
	virtual BOOL EdgeBasedMatching(const cv::Mat& src, const cv::Mat& tmp, CPoint2d& resultPoint, double& matchingRate);
	virtual BOOL RansacLineFitting(const std::vector<cv::Point2d>& pointSet, const double& distance, double& score, cv::Vec4d& model);	//0 : mx, 1 : my, 2 : sx, 3: sy // mx : 기울기, sx : 시작점
	virtual BOOL RansacCircleFitting(const std::vector<cv::Point2d>& pointSet, const double& distance, double& score, cv::Vec3d& model);
	virtual BOOL RegionGrowing(const cv::Mat& src, cv::Mat& dst, const int& iFirstSeedValue = 170, const int& iThresholdLow = 50, const int& iBoundary = 1, const int& iMaxSeedCount = 5000, const BOOL& bInv = FALSE);
	virtual BOOL AdaptiveThresholding(const cv::Mat& src, cv::Mat& dst, const int& iWindowSize = 120, const BOOL& bInv = FALSE, const ThresholdMethod& method = WOLF);
	virtual BOOL Delaunay(cv::Mat& src, const double& dStdRadius = 100);
	virtual std::vector<cv::Point> GetPoint(const cv::Mat& src);
	virtual std::vector<cv::Point2d> GetPoint2d(const cv::Mat& src);
	virtual std::vector<cv::Point2f> GetPoint2f(const cv::Mat& src);
	virtual BOOL ChamferMatching(const cv::Mat& src, const cv::Mat& tmp, cv::Point& resultPoint);

	// with CUDA
	virtual BOOL TemplateMatchingCUDA(const cv::Mat& src, const cv::Mat& tmp, CPoint2d& resultPoint, double& matchingRate);

//// 아래는 추가중..
//	virtual BOOL DistanceTrasform(const cv::Mat& src, cv::Mat& dst, cv::Point& centerPoint, const double& dScore = 0.7){return TRUE;} 
//	virtual BOOL DistanceTrasform(const cv::Mat& src, cv::Mat& dst, const double& dScore = 0.7){return TRUE;} 
//
//	virtual BOOL RansacObjectFitting(){return TRUE;}	// object 특징점을 추출하여 Fitting
//	virtual BOOL Filter(){return TRUE;}
//	virtual BOOL DeNoising(){return TRUE;}
//	virtual BOOL DeBlurring(){return TRUE;}
//
//	virtual BOOL EdgeBasedMatching(){return TRUE;}	// Edge와 그 Direction(크기)를 이용
//	virtual BOOL Kmeans(){return TRUE;}		// 입력된 수 만큼 색상 분류 (k-means)
//	virtual BOOL ImageMerge(const vector<cv::Mat>& src, Mat& dst){return TRUE;}		// 파노라마 이미지 생성
//	virtual BOOL Susan(){return TRUE;}	// susan 특징점 
};

};	// namespace imgproc
