#pragma once

#include "AFX_EXT_IMAGE_PROCESSOR.h"
#include "Coordinate.hpp"

namespace imgproc {

//*****************************************************************************
// FIND CORNER : 2014.01.24
//               by 장성규
//
// * in_marOrg : 입력 이미지는 BGR의 컬러영상
// * out_ptCenter : Edge의 중심점
// * out_rectRatio : 검출된 영역의 사각대비 비율
// * dAreaMin : 검출 최소 영역
// * 에러 리턴 - 0:정상, 1:교점 없음, 2:검출된 영역없음, 3:검출된 직선없음
//*****************************************************************************

AFX_EXT_API_IMAGE_PROCESSOR int pcFindCorner(const cv::Mat& in_matOrg, cv::Point& out_ptCenter, double& out_rectRatio, int iMinThreshold = 60, int iMaxThreshold = 120, int iThresholdStep = 10, double dAreaMin = 10000); // color Image

};	//namespace imgproc
