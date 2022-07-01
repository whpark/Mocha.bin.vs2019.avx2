#pragma once

#include "AFX_EXT_IMAGE_PROCESSOR.h"
#include "ImageProcessor.h"
#include "Coordinate.hpp"

#include "xMathUtil/ImageListBox.h"

namespace imgproc {

//=================================
// PWH.

	AFX_EXT_API_IMAGE_PROCESSOR BOOL FindPattern(const cv::Mat& img, const cv::Mat& imgPattern, CPoint2d& ptImage, int eMethod = cv::TM_CCOEFF_NORMED, double dRate = -1, double* pdRateFound = NULL, const CPoint2d* pptOffsetImage = NULL);
	AFX_EXT_API_IMAGE_PROCESSOR BOOL FindPatternEx(const cv::Mat& img, const cv::Mat& imgPattern, CPoint2d& ptImage, int eMethod = cv::TM_CCOEFF_NORMED, double dRate = -1, double* pdRateFound = NULL, const CPoint2d* pptOffsetImage = NULL, double dScaleDown = 1.0, double dScaleUp = 1.0, int eScaleUpMethod = cv::INTER_LINEAR);

	struct T_PATTERN_MATCHING_RESULT {
		CPoint2d ptImage;
		double dRate;
		cv::Mat imgFound;
	};
	AFX_EXT_API_IMAGE_PROCESSOR BOOL FindPatternMulti(const cv::Mat& img, const cv::Mat& imgPattern, std::vector<T_PATTERN_MATCHING_RESULT>& results, BOOL bGetImage = FALSE, int eMethod = cv::TM_CCOEFF_NORMED, double dRate = 0.5, const CPoint2d* pptOffsetImage = NULL, double dScaleDown = 1.0, double dScaleUp = 1.0, int eScaleUpMethod = cv::INTER_LINEAR);
	//AFX_EXT_API_IMAGE_PROCESSOR BOOL FindPatternShape(const cv::Mat& img, const cv::Mat& imgPattern, std::vector<T_PATTERN_MATCHING_RESULT>& results, BOOL bGetImage = FALSE, int eMethod = cv::TM_CCOEFF_NORMED, double dRate = 0.5, const CPoint2d* pptOffsetImage = NULL, double dScaleDown = 1.0, double dScaleUp = 1.0, int eScaleUpMethod = cv::INTER_LINEAR);	// NOT YET

	AFX_EXT_API_IMAGE_PROCESSOR bool FindMultiPattern(const cv::Mat& img, const std::vector<CImageListBoxItem>& marks, CPoint2d& ptImage, int eMethod = cv::TM_CCOEFF_NORMED, double dScaleDown = 0.25, double dScaleUp = 4.0, int* pIMark = nullptr, double* pDRate = nullptr, bool bSearchAll = false);

};
