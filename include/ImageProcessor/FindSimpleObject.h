#pragma once

#include "AFX_EXT_IMAGE_PROCESSOR.h"

#include "Coordinate.hpp"
#include "TList.hpp"

namespace imgproc {

AFX_EXT_API_IMAGE_PROCESSOR BOOL FindSimpleObject(const cv::Mat& img, CPoint2d& ptCenter, TList<CPoint2d>& outline, double dThreshold = 120, double dMinArea = 0.0, double dMaxArea = 0.0);

};	// namespace imgproc
