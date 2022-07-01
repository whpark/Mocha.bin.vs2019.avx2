#pragma once

#ifndef __HANGUEL_CODE__	// PWH
#define __HANGUEL_CODE__

#include "AFX_EXT_MISC.h"

#pragma pack(push, 8)

#define CHAR_KOR_MASK		0x1F
#define CHAR_KOR_C			0x0100
#define CHAR_KOR_V			0x0200
#define CHAR_KOR_INI		0x0100
#define CHAR_KOR_MID		0x0200
#define CHAR_KOR_FIN		0x0400

// 자음
#define CHAR_KOR_C_G		(  1 | CHAR_KOR_C )	// ㄱ
#define CHAR_KOR_C_GG		(  2 | CHAR_KOR_C )	// ㄲ
#define CHAR_KOR_C_GS		(  3 | CHAR_KOR_C )	// ㄳ
#define CHAR_KOR_C_N		(  4 | CHAR_KOR_C )	// ㄴ
#define CHAR_KOR_C_NJ		(  5 | CHAR_KOR_C )	// ㄵ
#define CHAR_KOR_C_NH		(  6 | CHAR_KOR_C )	// ㄶ
#define CHAR_KOR_C_D		(  7 | CHAR_KOR_C )	// ㄷ
#define CHAR_KOR_C_DD		(  8 | CHAR_KOR_C )	// ㄸ
#define CHAR_KOR_C_L		(  9 | CHAR_KOR_C )	// ㄹ
#define CHAR_KOR_C_LG		( 10 | CHAR_KOR_C )	// ㄺ
#define CHAR_KOR_C_LM		( 11 | CHAR_KOR_C )	// ㄻ
#define CHAR_KOR_C_LB		( 12 | CHAR_KOR_C )	// ㄼ
#define CHAR_KOR_C_LS		( 13 | CHAR_KOR_C )	// ㄽ
#define CHAR_KOR_C_LT		( 14 | CHAR_KOR_C )	// ㄾ
#define CHAR_KOR_C_LP		( 15 | CHAR_KOR_C )	// ㄿ
#define CHAR_KOR_C_LH		( 16 | CHAR_KOR_C )	// ㅀ
#define CHAR_KOR_C_M		( 17 | CHAR_KOR_C )	// ㅁ
#define CHAR_KOR_C_B		( 18 | CHAR_KOR_C )	// ㅂ
#define CHAR_KOR_C_BB		( 19 | CHAR_KOR_C )	// ㅃ
#define CHAR_KOR_C_BS		( 20 | CHAR_KOR_C )	// ㅄ
#define CHAR_KOR_C_S		( 21 | CHAR_KOR_C )	// ㅅ
#define CHAR_KOR_C_SS		( 22 | CHAR_KOR_C )	// ㅆ
#define CHAR_KOR_C_O		( 23 | CHAR_KOR_C )	// ㅇ
#define CHAR_KOR_C_J		( 24 | CHAR_KOR_C )	// ㅈ
#define CHAR_KOR_C_JJ		( 25 | CHAR_KOR_C )	// ㅉ
#define CHAR_KOR_C_CH		( 26 | CHAR_KOR_C )	// ㅊ
#define CHAR_KOR_C_K		( 27 | CHAR_KOR_C )	// ㅋ
#define CHAR_KOR_C_T		( 28 | CHAR_KOR_C )	// ㅌ
#define CHAR_KOR_C_P		( 29 | CHAR_KOR_C )	// ㅍ
#define CHAR_KOR_C_H		( 30 | CHAR_KOR_C )	// ㅎ

// 모음
#define CHAR_KOR_V_R		(  3 | CHAR_KOR_V )	// ㅏ
#define CHAR_KOR_V_RV		(  4 | CHAR_KOR_V )	// ㅐ
#define CHAR_KOR_V_RR		(  5 | CHAR_KOR_V )	// ㅑ
#define CHAR_KOR_V_RRV		(  6 | CHAR_KOR_V )	// ㅒ
#define CHAR_KOR_V_L		(  7 | CHAR_KOR_V )	// ㅓ
#define CHAR_KOR_V_LV		( 10 | CHAR_KOR_V )	// ㅔ
#define CHAR_KOR_V_LL		( 11 | CHAR_KOR_V )	// ㅕ
#define CHAR_KOR_V_LLV		( 12 | CHAR_KOR_V )	// ㅖ
#define CHAR_KOR_V_U		( 13 | CHAR_KOR_V )	// ㅗ
#define CHAR_KOR_V_UR		( 14 | CHAR_KOR_V )	// ㅘ
#define CHAR_KOR_V_URV		( 15 | CHAR_KOR_V )	// ㅙ
#define CHAR_KOR_V_UV		( 18 | CHAR_KOR_V )	// ㅚ
#define CHAR_KOR_V_UU		( 19 | CHAR_KOR_V )	// ㅛ
#define CHAR_KOR_V_D		( 20 | CHAR_KOR_V )	// ㅜ
#define CHAR_KOR_V_DL		( 21 | CHAR_KOR_V )	// ㅝ
#define CHAR_KOR_V_DLV		( 22 | CHAR_KOR_V )	// ㅞ
#define CHAR_KOR_V_DV		( 23 | CHAR_KOR_V )	// ㅟ
#define CHAR_KOR_V_DD		( 26 | CHAR_KOR_V )	// ㅠ
#define CHAR_KOR_V_H		( 27 | CHAR_KOR_V )	// ㅡ
#define CHAR_KOR_V_HV		( 28 | CHAR_KOR_V )	// ㅢ
#define CHAR_KOR_V_V		( 29 | CHAR_KOR_V )	// ㅣ

// 초성
#define CHAR_KOR_INI_FILL	(  1 | CHAR_KOR_INI )
#define CHAR_KOR_INI_G		(  2 | CHAR_KOR_INI )	// ㄱ
#define CHAR_KOR_INI_GG		(  3 | CHAR_KOR_INI )	// ㄲ
#define CHAR_KOR_INI_N		(  4 | CHAR_KOR_INI )	// ㄴ
#define CHAR_KOR_INI_D		(  5 | CHAR_KOR_INI )	// ㄷ
#define CHAR_KOR_INI_DD		(  6 | CHAR_KOR_INI )	// ㄸ
#define CHAR_KOR_INI_L		(  7 | CHAR_KOR_INI )	// ㄹ
#define CHAR_KOR_INI_M		(  8 | CHAR_KOR_INI )	// ㅁ
#define CHAR_KOR_INI_B		(  9 | CHAR_KOR_INI )	// ㅂ
#define CHAR_KOR_INI_BB		( 10 | CHAR_KOR_INI )	// ㅃ
#define CHAR_KOR_INI_S		( 11 | CHAR_KOR_INI )	// ㅅ
#define CHAR_KOR_INI_SS		( 12 | CHAR_KOR_INI )	// ㅆ
#define CHAR_KOR_INI_O		( 13 | CHAR_KOR_INI )	// ㅇ
#define CHAR_KOR_INI_J		( 14 | CHAR_KOR_INI )	// ㅈ
#define CHAR_KOR_INI_JJ		( 15 | CHAR_KOR_INI )	// ㅉ
#define CHAR_KOR_INI_CH		( 16 | CHAR_KOR_INI )	// ㅊ
#define CHAR_KOR_INI_K		( 17 | CHAR_KOR_INI )	// ㅋ
#define CHAR_KOR_INI_T		( 18 | CHAR_KOR_INI )	// ㅌ
#define CHAR_KOR_INI_P		( 19 | CHAR_KOR_INI )	// ㅍ
#define CHAR_KOR_INI_H		( 20 | CHAR_KOR_INI )	// ㅎ

// 중성
#define CHAR_KOR_MID_FILL	(  2 | CHAR_KOR_MID )	// U : Up, D : Down, R : Right, L : Left, H : Horizontal bar, V : Vertical bar
#define CHAR_KOR_MID_R		(  3 | CHAR_KOR_MID )	// ㅏ
#define CHAR_KOR_MID_RV		(  4 | CHAR_KOR_MID )	// ㅐ
#define CHAR_KOR_MID_RR		(  5 | CHAR_KOR_MID )	// ㅑ
#define CHAR_KOR_MID_RRV	(  6 | CHAR_KOR_MID )	// ㅒ
#define CHAR_KOR_MID_L		(  7 | CHAR_KOR_MID )	// ㅓ
#define CHAR_KOR_MID_LV		( 10 | CHAR_KOR_MID )	// ㅔ
#define CHAR_KOR_MID_LL		( 11 | CHAR_KOR_MID )	// ㅕ
#define CHAR_KOR_MID_LLV	( 12 | CHAR_KOR_MID )	// ㅖ
#define CHAR_KOR_MID_U		( 13 | CHAR_KOR_MID )	// ㅗ
#define CHAR_KOR_MID_UR		( 14 | CHAR_KOR_MID )	// ㅘ
#define CHAR_KOR_MID_URV	( 15 | CHAR_KOR_MID )	// ㅙ
#define CHAR_KOR_MID_UV		( 18 | CHAR_KOR_MID )	// ㅚ
#define CHAR_KOR_MID_UU		( 19 | CHAR_KOR_MID )	// ㅛ
#define CHAR_KOR_MID_D		( 20 | CHAR_KOR_MID )	// ㅜ
#define CHAR_KOR_MID_DL		( 21 | CHAR_KOR_MID )	// ㅝ
#define CHAR_KOR_MID_DLV	( 22 | CHAR_KOR_MID )	// ㅞ
#define CHAR_KOR_MID_DV		( 23 | CHAR_KOR_MID )	// ㅟ
#define CHAR_KOR_MID_DD		( 26 | CHAR_KOR_MID )	// ㅠ
#define CHAR_KOR_MID_H		( 27 | CHAR_KOR_MID )	// ㅡ
#define CHAR_KOR_MID_HV		( 28 | CHAR_KOR_MID )	// ㅢ
#define CHAR_KOR_MID_V		( 29 | CHAR_KOR_MID )	// ㅣ

// 종성
#define CHAR_KOR_FIN_FILL	(  1 | CHAR_KOR_FIN )
#define CHAR_KOR_FIN_G		(  2 | CHAR_KOR_FIN )	// ㄱ
#define CHAR_KOR_FIN_GG		(  3 | CHAR_KOR_FIN )	// ㄲ
#define CHAR_KOR_FIN_GS		(  4 | CHAR_KOR_FIN )	// ㄳ
#define CHAR_KOR_FIN_N		(  5 | CHAR_KOR_FIN )	// ㄴ
#define CHAR_KOR_FIN_NJ		(  6 | CHAR_KOR_FIN )	// ㄵ
#define CHAR_KOR_FIN_NH		(  7 | CHAR_KOR_FIN )	// ㄶ
#define CHAR_KOR_FIN_D		(  8 | CHAR_KOR_FIN )	// ㄷ
#define CHAR_KOR_FIN_L		(  9 | CHAR_KOR_FIN )	// ㄹ
#define CHAR_KOR_FIN_LG		( 10 | CHAR_KOR_FIN )	// ㄺ
#define CHAR_KOR_FIN_LM		( 11 | CHAR_KOR_FIN )	// ㄻ
#define CHAR_KOR_FIN_LB		( 12 | CHAR_KOR_FIN )	// ㄼ
#define CHAR_KOR_FIN_LS		( 13 | CHAR_KOR_FIN )	// ㄽ
#define CHAR_KOR_FIN_LT		( 14 | CHAR_KOR_FIN )	// ㄾ
#define CHAR_KOR_FIN_LP		( 15 | CHAR_KOR_FIN )	// ㄿ
#define CHAR_KOR_FIN_LH		( 16 | CHAR_KOR_FIN )	// ㅀ
#define CHAR_KOR_FIN_M		( 17 | CHAR_KOR_FIN )	// ㅁ
#define CHAR_KOR_FIN_B		( 19 | CHAR_KOR_FIN )	// ㅂ
#define CHAR_KOR_FIN_BS		( 20 | CHAR_KOR_FIN )	// ㅄ
#define CHAR_KOR_FIN_S		( 21 | CHAR_KOR_FIN )	// ㅅ
#define CHAR_KOR_FIN_SS		( 22 | CHAR_KOR_FIN )	// ㅆ
#define CHAR_KOR_FIN_O		( 23 | CHAR_KOR_FIN )	// ㅇ
#define CHAR_KOR_FIN_J		( 24 | CHAR_KOR_FIN )	// ㅈ
#define CHAR_KOR_FIN_CH		( 25 | CHAR_KOR_FIN )	// ㅊ
#define CHAR_KOR_FIN_K		( 26 | CHAR_KOR_FIN )	// ㅋ
#define CHAR_KOR_FIN_T		( 27 | CHAR_KOR_FIN )	// ㅌ
#define CHAR_KOR_FIN_P		( 28 | CHAR_KOR_FIN )	// ㅍ
#define CHAR_KOR_FIN_H		( 29 | CHAR_KOR_FIN )	// ㅎ

struct DHAN_CODE_CONV {
	wchar_t cKSC, cUni, cKSSM;
};

AFX_EXT_API_MISC const DHAN_CODE_CONV* GetHanguelTable();
AFX_EXT_API_MISC int GetHanguelTableSize();
AFX_EXT_API_MISC const DHAN_CODE_CONV* SearchHanguelTable(wchar_t cKSC, wchar_t cUni, wchar_t cKSSM);

#pragma pack(pop)

#endif // __HANGUEL_CODE__
