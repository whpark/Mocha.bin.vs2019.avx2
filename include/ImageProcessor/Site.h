#pragma once

#include "AFX_EXT_IMAGE_PROCESSOR.h"

namespace imgproc {

//<< ±Í¶Ñ¶ó¹Ì. Àå¼º±Ô, 2015.08.
AFX_EXT_API_IMAGE_PROCESSOR BOOL KRB(const cv::Mat& imgOrg, cv::Point2d& ptResultCenter, const int& iThreshold = 30);
AFX_EXT_API_IMAGE_PROCESSOR BOOL KRB(const cv::Mat& imgOrg, const cv::Rect& rectRoiTop, const cv::Rect& rectRoiLeft, const cv::Rect& rectRoiBottom, const cv::Rect& rectRoiRight, cv::Point2d& ptResultCenter, double& dResultRadius, double& dScore, const int& iThreshold = 127);
//>>


// old...
AFX_EXT_API_IMAGE_PROCESSOR BOOL SearchControlLine(const cv::Mat& img, CPoint2d& ptImage, uintXX_t eSide, uintXX_t nLineSize, uintXX_t nLineThickness, BOOL bBlackBkgnd, double dRate);
AFX_EXT_API_IMAGE_PROCESSOR BOOL FindEdge(const cv::Mat& img, CPoint2d& ptEdge, int& eDirection, double dThreshold = 20.0, int nOpenOperationSize = 0, int eLineFittingMethod = cv::DIST_HUBER);	// eDirection 0 : ->, 1 : <-, 2 : downward, 3 : upward
AFX_EXT_API_IMAGE_PROCESSOR BOOL FindGrind(const cv::Mat& img, cv::Mat& imgResult, CRect2d& rectPattern, const int iMinPatternLength, const int iMaxPatternLength);
AFX_EXT_API_IMAGE_PROCESSOR BOOL FindPollEdge(const cv::Mat& img, cv::Mat& imgResult, CPoint2d& ptPoll, CPoint2d& ptEdge);

};	// namespace imgproc;
