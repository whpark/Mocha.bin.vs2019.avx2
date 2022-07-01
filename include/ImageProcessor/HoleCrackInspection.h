#pragma once
#include "AFX_EXT_IMAGE_PROCESSOR.h"
#include "ImageProcessing.h"
#include "Coordinate.hpp"

#include <vector>

namespace imgproc {


#define RESULT_GOOD			0
#define	RESULT_NOTCUTTED	1
#define RESULT_CRACK		2
#define RESULT_ERROR		3

// 타입1 : 서클
// 타입2 : L 서클 -> T 라인 -> R 서클 -> B 라인
// 타입3 : LT 서클 -> T 라인 -> RT 서클 -> R 라인 -> RB 서클 -> B 라인 -> LB 서클 -> L 라인

class AFX_EXT_CLASS_IMAGE_PROCESSOR CInspectionTarget
{
public:
	CInspectionTarget(const CString& modelName)
		: m_strModelName(modelName), m_Image(cv::Size(1024,768), CV_8UC1, cv::Scalar(0)), m_FilledImage(cv::Size(1024,768), CV_8UC1, cv::Scalar(0)), 
		m_CrackInspectionImage(cv::Size(1024,768), CV_8UC1, cv::Scalar(0)), m_infoCrackInspectionSize(0), m_iThreshold(0), m_bCUDA(TRUE), m_bCrackInspection(0), m_iAlignMethod(0) {};
	BOOL Making(const CString& strTargetName);
	cv::Mat GetImage(){return m_Image;}
	cv::Mat GetFilledImage(){return m_FilledImage;}
	cv::Mat GetCrackInspectionImage(){return m_CrackInspectionImage;}
	CString GetModeName(){return m_strModelName;} 
	CString GetTargetName(){return m_strTargetName;} 
	int GetCrackInspectionSize(){return m_infoCrackInspectionSize;}
	int GetThreshold(){return m_iThreshold;}
	BOOL GetCUDA(){return m_bCUDA;}
	int GetCrackInspection(){return m_bCrackInspection;}
	int GetAlignMethod(){return m_iAlignMethod;}

private:
	CString m_strModelName;
	CString m_strTargetName;
	int m_infoCrackInspectionSize;
	int m_iThreshold;
	int m_bCrackInspection;
	int m_iAlignMethod;
	
	BOOL m_bCUDA;
	cv::Mat	m_Image;
	cv::Mat m_FilledImage;
	cv::Mat m_CrackInspectionImage;
};

class AFX_EXT_CLASS_IMAGE_PROCESSOR CHoleCrackInspector : public CImageProcessing
{
public:
	CHoleCrackInspector(const cv::Mat& image, const CString& modelName):m_originalImage(image.clone()), m_processingImage(image.size(), CV_8UC1, cv::Scalar(0)), m_target(modelName) {}
	BOOL ImageProcessing();
	BOOL AlignmarkFindingUseTemplate(const CString& strTargetName, CPoint2d& resultPoint, double& score);
	BOOL AlignmarkFinding(const CString& strTargetName, CPoint2d& resultPoint, double& score);
	BOOL TargetFinding(const CString& strTargetName, const CPoint2d& centerPoint, int& result);
	BOOL FindCenterOfCross(const cv::Mat& in_matOrg, CPoint2d& out_ptCenter, double dThreshold = 90, double dAreaMin = 10000, float fRatio = 0.7f, int iLineCount = 200, BOOL bBlackBkgnd = FALSE);
	cv::Mat GetOriginalImage(){return m_originalImage;}
	cv::Mat GetProcessingImage(){return m_processingImage;}

private:
	cv::Mat m_binaryImage;
	cv::Mat m_originalImage;
	cv::Mat m_processingImage;
	std::vector<std::vector<cv::Point>> m_contours;
	CInspectionTarget	m_target;
};


};	// namespace imgproc
