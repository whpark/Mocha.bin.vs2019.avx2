#pragma once

#include "misc/AFX_EXT_MISC.h"

namespace mocha_features {

typedef enum {
	KF_NONE						= 0,

	KF_MISC						= BIT( 0),
	KF_MIP						= BIT( 1),
	KF_XMATH_UTIL				= BIT( 2),
	KF_FRAME_GRABBER			= BIT( 3),
	KF_IMAGE_PROCESSOR			= BIT( 4),
	KF_DRAWING					= BIT( 5),
	KF_HSMS						= BIT( 6),

	KF_MOCHA					= BIT(16),
	KF_LIGHT_CONTROLER			= BIT(17),
	KF_LASER_CONTROLER			= BIT(18),
	KF_VISION					= BIT(19),
	KF_SCANNER_CALIBRATION		= BIT(20),

	KF_ALL						= (DWORD)-1,

} eKEY_FEATURE;

AFX_EXT_API_MISC BOOL IsFeatureEnabled(DWORD dwFeature /*mocha_feature::eKEY_FEATURE*/, BOOL bShowUI = TRUE);
AFX_EXT_API_MISC BOOL IsFeatureEnabled(LPCTSTR pszFeature);

};	// namespace mocha_features
