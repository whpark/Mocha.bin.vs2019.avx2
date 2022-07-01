#pragma once

#include "AFX_EXT_IMAGE_PROCESSOR.h"
#include "Coordinate.hpp"

namespace imgproc {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2014.03.12 sk-jang
// 오브젝트 엣지의 중심점 찾기
// 
// 파라미터 - src						: Mat 형식의 color 영상 혹은 gray 영상
//			- resultPoint				: 결과 좌표
//			- resultMatchingRate			: 결과 확률
//			- topBottom					: 오브젝트가 위에 있는 경우 TOP, 아래에 있는경우 BOTTOM
//			- leftCenterRight			: 좌표상의 위치 - LEFT, CENTER, RIGHT / LEFT경우 오브젝트는 오른쪽에 보여지고 RIGHT경우 오브젝트는 왼쪽에 보여짐
//			- iOffset					: 결과 좌표의 Offset 만큼 Pixel이 이동 ( 오브젝트가 TOP 일경우 아래쪽으로 OFFSET 적용, BOTTOM일 경우 위쪽으로 OFFSET 적용)
//			- iThreshold					: Threshold값
//			- dMatchingRate				: 매칭 확룔이 파라미터의 값보다 작게 될 경우 에러 리턴
//
//			BOTTOM,LEFT			BOTTOM,CENTER		BOTTOM,RIGHT
//			TOP(BOTTOM),LEFT	TOP(BOTTOM),CENTER	TOP(BOTTOM),RIGHT
//			TOP,LEFT			TOP,CENTER			TOP,RIGHT
//
//	리턴	- 0 : 정상 종료
//			- 1 : 매칭율 에러
//			- 2 : 서칭 에러
//
//	실행 예제
//
//	Point point;
//	double matchingRate;
//	int res = pcFinderLineCenter(mat, point, matchingRate, BOTTOM, CENTER, 30, 128, 0.1);
// 중심의 아래쪽 오브젝트를 기준으로 서칭	

namespace FindLineCenter {
	typedef enum {TOP, BOTTOM, LEFT, CENTER, RIGHT} pcFinderLineCenterMethod;
};
AFX_EXT_API_IMAGE_PROCESSOR int pcFindLineCenter(
						const cv::Mat& src,
						CPoint2d& resultPoint,
						double& resultMatchingRate,
						const FindLineCenter::pcFinderLineCenterMethod &topBottom,
						const FindLineCenter::pcFinderLineCenterMethod &leftCenterRight,
						const int iOffset,
						const int iThreshold,
						const double dMatchingRate
						);


};	//namespace imgproc
