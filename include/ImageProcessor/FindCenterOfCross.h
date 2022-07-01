#pragma once

#include "AFX_EXT_IMAGE_PROCESSOR.h"

#include "Coordinate.hpp"

namespace imgproc {

///////////////////////////////////////////////////////////////////////////////
//
// 2014.01.16. by 장성규.
// 2014.06.24. by 장성규. 여러개의 크로스라인이 생성될 경우 중심에서 가까운 결과를 리턴
//

AFX_EXT_API_IMAGE_PROCESSOR BOOL pcFindCenterOfCross(
						const cv::Mat&		in_matOrg,			// 입력영상
						CPoint2d&			out_ptCenter,				// 결과값에서 가장 가까운 포인트 출력
						const double&		dThreshold = 90,			// 영상 이진화시 Threshold값 (0 - 255)
						const int&			iLineCount = 300,			// 라인 피팅시 참조가 되는 점의 최소 갯수
						const BOOL&			bBlackBkgnd = FALSE,		// 배경색(배경이 흰색이고 라인이 검은색이면 TRUE)
						const int&			iResizeCount = 0			// 리사이즈 횟수 - 디버그 모드에서 속도증가 (해상도 2000 이상시 2, 1000 이상시 1 권장)
						);

// Result (Error)
// 0:no error, 1:x rating, 2:x data_no, 3:y rating, 4:y data_no
// dAreaMin : 이미지 레이블링시 최소의 면적 (계산용)
// fRatio : x, y축의 이미지 평균값 분리 후 라인 피팅시 사용되는 점의 비율
// iLineCount : x, y축의 이미지 평균값 분리 개수 (라인 피팅시 사용
AFX_EXT_API_IMAGE_PROCESSOR int pcFindCenterOfCross(
						const cv::Mat& in_matOrg,
						CPoint2d& out_ptCenter,
						double dThreshold = 90,
						double dAreaMin = 10000,
						float fRatio = 0.7f,
						int iLineCount = 200,
						BOOL bBlackBkgnd = FALSE
						);

// FindCenterOfCross
// imgOrg : 입력이미지
// ptCenter : 결과포인트
// dThreshold : 이진화 값.
// dAreaMin : 필요한 최소 영역 - 최소영역보다 작은 영역은 날림..
// iLineCount : 라인피팅 참조 길이
// iOutlier : 라인피팅 참조 거리 - 선택 영역 내의 라인만 결과값에 영향을 줌 (튀는 값 제거)
// iLineFittingCount : 라인피팅 갯수 (만약 5이면 가로 5개, 세로 5개 총 25개의 포인트 검출 후 중심 점에서 가장 가까운 포인트 반환)
// bDetail : 정밀검출
// iDetailLevel : 정밀검출시 확대 비율.
// bBlackBkgnd : 만약 True 이면 라인은 흰색 배경은 검은색.
AFX_EXT_API_IMAGE_PROCESSOR BOOL pcFindCenterOfCross201702(
		const cv::Mat& imgOrg,
		cv::Point2d& ptCenter,
		double dThreshold = 90,		// 이진화.
		double dAreaMin = 10000,	// 최소 면적..
		int iLineCount = 200,		// 라인피팅 참조 길이 200 pixel
		int iOutlier = 1,			// 라인피팅 참조 거리 1 pixel
		int iLineFittingCount = 5,	// 라인피팅 갯수..
		BOOL bDetail = TRUE,		// 정밀 검출
		int iDetailLevel = 20,		// 20배 확대.
		BOOL bBlackBkgnd = FALSE	// 배경이 검은색.
		);

//UTI용 - 복잡한 이미지 내에서 가장 조건에 맞는 중심점
AFX_EXT_API_IMAGE_PROCESSOR BOOL FindCenterOfCrossForUTI(
						const cv::Mat&		in_matOrg,					// 입력영상
						CPoint2d&			out_ptCenter,				// 결과값에서 가장 가까운 포인트 출력
						const int&			iLineFindingCount = 10,		// 이미지에서 찾아내는 라인 수 (자세한건 기술 문서확인)
						const int&			iLineCount = 1200,			// 가공된 선의 길이
						const BOOL&			bBlackBkgnd = FALSE			// 배경색(배경이 흰색이고 라인이 검은색이면 FALSE)
						);


}	// namespace imgproc
