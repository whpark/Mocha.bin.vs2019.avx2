#pragma once

#include "AFX_EXT_IMAGE_PROCESSOR.h"
#include "Coordinate.hpp"

namespace imgproc {

//*****************************************************************************
// FIND CORNER : 2014.09.19
//               by 장성규
//
// src			: 입력 영상 Matrix
// maxLines		: 최대 검출 Line 수
// validAngle	: 유효 각 (예) 3.0 이면 수직 수평 3도이내의 각은 컷으로 판별 안함
// 
// 리턴값		: 0 - 코너컷 없음
//				: 1 - 코너컷 존재
//				: 2 - 에러 (라인없음)
//*****************************************************************************

AFX_EXT_API_IMAGE_PROCESSOR int FindCornerCut(const cv::Mat& src, const int& maxLines = 3, const double& validAngle = 5.0);

}; //namespace imgproc
