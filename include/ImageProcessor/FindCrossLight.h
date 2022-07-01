#pragma once

#include "AFX_EXT_IMAGE_PROCESSOR.h"

#include "Coordinate.hpp"

namespace imgproc {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2014.03.25 sk-jang
// LGD Curved Glass 중심점 찾기
// 
// 파라미터 - src						: Mat 형식의 color 영상
//			- resultPoint				: 결과 좌표 (Point)
//
// maxRefPoint : 설정된 참조 포인트 개수 이상이 되면 에러
// minRefPoint : 설정된 참조 포인트 개수 이하가 되면 에러
// maxThreshold : 영상의 최대 밝기 - 밝으면 200정도, 어두우면 50정도
// minThreshold : 영상의 최소 밝기 - 10으로 두면 됨
// accuracy :정밀도 - 숫자가 작을수록 정밀도가 높음 (1 - 10)
// return 0:정상, 1:입력영상이 칼라영상이 아님, 2:참조 포인트 개수 에러, 3:인식실패, 4:인식률 저조
//
// 4번의 경우는 라이트가 
//							   * *         |            *
//						   * * * * * *     |      *  *  *  *  * 
//							   * *		   |	        *
//							   * *		   |		    *
//
// 왼쪽의 경우처럼 세로 두줄이 생겼을 경우에는 인식률이 저조하여 Error를 리턴할 수 있음 (테스트시 흰색과 파란색이 유사한 경우를 보임)
// 오른쪽의 경우가 정상인 경우임

// 2014.04.18. PWH. pcFindAlignMarkLGDCurved -> pcFindCrossLight
// return 0:정상, 1:입력영상이 칼라영상이 아님, 2:참조 포인트 갯수 부족, 3:인식실패, 4:인식률 저조
AFX_EXT_API_IMAGE_PROCESSOR int pcFindCrossLight(
						cv::Mat& src,
						CPoint2d& resultPoint,
						double& distPixelX,
						double& distPixelY,
						const int maxThreshold = 50,
						const int minThreshold = 10,
						const int accuracy = 1,
						const int minRefPoint = 8,
						const int maxRefPoint = 25
						);


};	//namespace imgproc
