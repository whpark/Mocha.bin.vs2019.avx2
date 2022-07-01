#pragma once

#include "AFX_EXT_IMAGE_PROCESSOR.h"

#include "ImageProcessing.h"
#include "Coordinate.hpp"

namespace imgproc {

// 2014. 06. 20 FOR R2R
AFX_EXT_API_IMAGE_PROCESSOR BOOL FindMaxObject(const cv::Mat& src, cv::Rect& rect, const int& threshold = 150, const BOOL& bResize = FALSE, const BOOL& bInv = FALSE, const BOOL& bMorph = TRUE);

};	//namespace imgproc
