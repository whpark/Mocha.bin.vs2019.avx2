#pragma once

#pragma pack(push, 8)

#ifdef _IP_DATA_INTERNAL_
typedef int                 long;
typedef int                 INT;
typedef unsigned int        DWORD;
typedef unsigned int        UINT;
typedef int                 BOOL;
typedef unsigned char		bool;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef double              DOUBLE;
typedef DWORD				COLORREF;
#define TRUE                1
#define FALSE               0
#define true                1
#define false               0
#define MAX_PATH			260

typedef char						int8_t;
typedef short						int16_t;
typedef unsigned char				uint8_t;
typedef unsigned short				uint16_t;
typedef unsigned __int32			uint32_t;
typedef unsigned __int64			uint64_t;
//typedef __int32						int32_t;
//typedef __int64						int64_t;

// POINT, SIZE, RECT -- default.defines.h
typedef union { double  val[2]; struct { double x, y; }; }																		POINT2D;
typedef union { double  val[3]; struct { double x, y, z; }; }																	POINT3D;
typedef union { double  val[2]; struct { double cx, cy; }; }																	SIZE2D;
typedef union { double  val[3]; struct { double cx, cy, cz; }; }																SIZE3D;
typedef union { int32_t val[2]; struct { int32_t x, y; }; }																		POINT2I;
typedef union { int32_t val[3]; struct { int32_t x, y, z; }; }																	POINT3I;
typedef union { int32_t val[2]; struct { int32_t cx, cy; }; }																	SIZE2I;
typedef union { int32_t val[3]; struct { int32_t cx, cy, cz; }; }																SIZE3I;
typedef union { double  val[4]; struct { POINT2D pt0, pt1; }; struct { double left, top, right, bottom; }; }					RECT2D;
typedef union { double  val[6]; struct { POINT3D pt0, pt1; }; struct { double left, top, front, right, bottom, back; }; }		RECT3D;
typedef union { int32_t val[4]; struct { POINT2I pt0, pt1; }; struct { int32_t left, top, right, bottom; }; }					RECT2I;
typedef union { int32_t val[6]; struct { POINT3I pt0, pt1; }; struct { int32_t left, top, front, right, bottom, back; }; }		RECT3I;
#endif

//<< By LBH
// 2017.07.03 Modify KMS
typedef struct T_P1 {
#ifndef _IP_DATA_INTERNAL_
	T_P1() = default;
	T_P1(const T_P1&) = default;
	//T_P1(T_P1&&) = default;

	bool operator != (const T_P1& B) const { return !(*this == B); }
	bool operator == (const T_P1& B) const { return dPos == B.dPos && dSpeed == B.dSpeed; }
#endif

	double dPos = 0; double dSpeed = 0;
} T_P1;

typedef struct T_P2 {
#ifndef _IP_DATA_INTERNAL_
	T_P2() = default;
	T_P2(const T_P2&) = default;
	//T_P2(T_P2&&) = default;

	bool operator != (const T_P2& B) const { return !(*this == B); }
	bool operator == (const T_P2& B) const { return dPosX == B.dPosX && dPosY == B.dPosY && dSpeedX == B.dSpeedX && dSpeedY == B.dSpeedY; }
#endif

	double dPosX = 0; double dSpeedX = 0;
	double dPosY = 0; double dSpeedY = 0;
} T_P2;

typedef struct T_P3 {
#ifndef _IP_DATA_INTERNAL_
	T_P3() = default;
	T_P3(const T_P3&) = default;
	//T_P3(T_P3&&) = default;

	bool operator != (const T_P3& B) const { return !(*this == B); }
	bool operator == (const T_P3& B) const { return dPosX == B.dPosX && dPosY == B.dPosY && dPosZ == B.dPosZ && dSpeedX == B.dSpeedX && dSpeedY == B.dSpeedY && dSpeedZ == B.dSpeedZ; }
#endif

	double dPosX = 0; double dSpeedX = 0;
	double dPosY = 0; double dSpeedY = 0;
	double dPosZ = 0; double dSpeedZ = 0;
} T_P3;

typedef struct T_P4 {
#ifndef _IP_DATA_INTERNAL_
	T_P4() = default;
	T_P4(const T_P4&) = default;
	//T_P4(T_P4&&) = default;
	bool operator != (const T_P4& B) const { return !(*this == B); }
	bool operator == (const T_P4& B) const {
		return dPosX == B.dPosX
			&& dPosY == B.dPosY
			&& dPosZ == B.dPosZ
			&& dPosT == B.dPosT
			&& dSpeedX == B.dSpeedX
			&& dSpeedY == B.dSpeedY
			&& dSpeedZ == B.dSpeedZ
			&& dSpeedT == B.dSpeedT;
	}
#endif

	double dPosX = 0; double dSpeedX = 0;
	double dPosY = 0; double dSpeedY = 0;
	double dPosZ = 0; double dSpeedZ = 0;
	double dPosT = 0; double dSpeedT = 0;
} T_P4;
//>>

#pragma pack(pop)
