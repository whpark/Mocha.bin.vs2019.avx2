#pragma once

#include "AFX_EXT_IMAGE_PROCESSOR.h"
#include "Coordinate.hpp"

namespace imgproc {

// 2014.06.02 장성규 skjang jsk
AFX_EXT_API_IMAGE_PROCESSOR BOOL SearchCircle(const cv::Mat& img, CPoint2d& ptScreen, double& score, const double& dRadius = 195.0, const int& iRepeat = 10);
AFX_EXT_API_IMAGE_PROCESSOR BOOL SearchCircle_intaglio(const cv::Mat& img, CPoint2d& ptScreen, double& score, const double& dRadius = 195.0, const int& inColor = 120, const int& outColor = 180, const int& iRepeat = 10);


};//	namespace imgproc
