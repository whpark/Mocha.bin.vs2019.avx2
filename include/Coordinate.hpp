//=============================================================================
// Coordinate System with N-Dimensional (for 2D, 3D) - Uses openCV.
// 2013.03.03. PWH.
// 

#pragma once

#include "default.defines.h"

WARNING_PUSH_AND_DISABLE(4819 4996)
#include "opencv2/opencv.hpp"
WARNING_POP()

#pragma pack(push, 8)

WARNING_PUSH_AND_DISABLE(4244)

//=============================================================================

template < typename T > class TSize2;
template < typename T > class TSize3;
template < typename T > class TPoint2;
template < typename T > class TPoint3;
template < typename T > class TRect2;
template < typename T > class TRect3;

/////////////////////////////////////////////////////////////////////////////
//
//

template < typename T >
class __declspec(novtable) TSize3 {
public:
	typedef cv::Vec<T, 3> type_vec;
public:
	union {
		struct {
			T cx, cy, cz;
		};
		T val[3];
	};
	using coord_t = typename decltype(val);
public:

// Constructors
	TSize3() : cx(0), cy(0), cz(0) {}
	TSize3(const TSize3& B)	{ *this = B; }
	explicit TSize3(const TSize2<T>& B) : cz(0) { *this = B; }
	explicit TSize3(const TPoint2<T>& B) : cz(0) { *this = B; }
	explicit TSize3(const TPoint3<T>& B) { *this = B; }
	TSize3(T _cx, T _cy, T _cz) : cx(_cx), cy(_cy), cz(_cz) {}
	TSize3(const T_SIZE3<T>& B) : T_SIZE3(B) {}

	explicit TSize3(const SIZE& size)							: cz(0)	{ *this = size; }
	explicit TSize3(const POINT& pt)							: cz(0)	{ *this = pt; }
	template < typename T2 > TSize3(const cv::Size_<T2>& size)	: cz(0) { *this = size; }
	template < typename T2 > TSize3(const cv::Point_<T2>& pt)	: cz(0) { *this = pt; }
	template < typename T2 > TSize3(const cv::Point3_<T2>& pt)	: cz(0) { *this = pt; }
	template < typename T2 > TSize3(const cv::Rect_<T2>& rect)	: cz(0) { *this = rect; }
	template < typename T2 > TSize3(T2 _cx, T2 _cy, T2 _cz) : cx(_cx), cy(_cy), cz(_cz) {}

// Assign Operator
	// Size
	TSize3& operator = (const TSize3& B)			{ if (this != &B) memcpy(val, B.val, sizeof(val)); return *this; }
	TSize3& operator = (const TSize2<T>& B)			{ memcpy(val, B.val, sizeof(B.val)); /*cz = 0;*/ return *this; }
	template < typename T2 >
	TSize3& operator = (const TSize2<T2>& B)		{ for (int i = 0; i < countof(B.val); i++) val[i] = B.val[i]; /*cz = 0;*/ return *this; }
	template < typename T2 >
	TSize3& operator = (const TSize3<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	TSize3& operator = (const SIZE& size)			{ cx = size.cx; cy = size.cy; /*cz = 0;*/ return *this; }
	template < typename T2 >
	TSize3& operator = (const cv::Size_<T2>& B)		{ cx = B.width; cy = B.height; /*cz = 0;*/ return *this; }

	TSize3& operator = (const T_SIZE3<T>& B)		{ memcpy(val, &B.val, sizeof(B.val)); return *this; }
	template < typename T2 >
	TSize3& operator = (const T_SIZE3<T2>& B)		{ cx = B.cx; cy = B.cy; cz = B.cz; return *this; }

	// Point
	TSize3& operator = (const TPoint3<T>& B)		{ memcpy(val, B.val, sizeof(val)); return *this; }
	TSize3& operator = (const TPoint2<T>& B)		{ memcpy(val, B.val, sizeof(B.val)); /*cz = 0;*/ return *this; }
	template < typename T2 >
	TSize3& operator = (const TPoint2<T2>& B)		{ for (int i = 0; i < countof(B.val); i++) val[i] = B.val[i]; /*cz = 0;*/ return *this; }
	template < typename T2 >
	TSize3& operator = (const TPoint3<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	TSize3& operator = (const POINT& B)				{ cx = B.x; cy = B.y; /*cz = 0;*/ return *this; }
	template < typename T2 >
	TSize3& operator = (const cv::Point_<T2>& B)	{ cx = B.x; cy = B.y; /*cz = 0;*/ return *this; }
	template < typename T2 >
	TSize3& operator = (const cv::Point3_<T2>& B)	{ cx = B.x; cy = B.y; cz = B.z; return *this; }

	//// Rect
	//template < typename T2 >
	//TSize3& operator = (const cv::Rect_<T2>& B)		{ cx = B.width; cy = B.height; cz = 0; return *this; }

// Compare
	bool operator == (const TSize3& B) const { return memcmp(val, B.val, sizeof(val)) == 0; }
	bool operator != (const TSize3& B) const { return memcmp(val, B.val, sizeof(val)) != 0; }
	int CountNonZero() const { int n = 0; for (int i = 0; i < countof(val); i++) if (val[i] != 0) n++; return n; }

// Member Access
	const T& operator [](int i) const	{ ASSERT(i < countof(val)); return val[i]; }
    T& operator [](int i)				{ ASSERT(i < countof(val)); return val[i]; }
    const T& operator ()(int i) const	{ ASSERT(i < countof(val)); return val[i]; }
    T& operator ()(int i)				{ ASSERT(i < countof(val)); return val[i]; }

// Type Casting
	//operator SIZE () const				{ SIZE size = { (long)val[0], (long)val[1] }; return size; }
	operator CSize () const				{ return CSize((long)val[0], (long)val[1]); }
	operator T_SIZE3<T> () const		{ return *(T_SIZE3<T>*)this; }
	template < typename T2 >
	operator cv::Size_<T2> () const		{ return cv::Size_<T2>((T2)val[0], (T2)val[1]); }
	operator type_vec& ()				{ return *(type_vec*)this; }
	operator const type_vec& () const	{ return *(type_vec*)this; }
	operator T_SIZE3<T>& ()				{ return *(T_SIZE3<T>*)this; }
	operator const T_SIZE3<T>& () const	{ return *(T_SIZE3<T>*)this; }

// Numerical Operators
	TSize3& operator +=  (const TSize3& B)					{ for (int i = 0; i < countof(val); i++) val[i] += B.val[i]; return *this; }
	TSize3& operator -=  (const TSize3& B)					{ for (int i = 0; i < countof(val); i++) val[i] -= B.val[i]; return *this; }
	TSize3& operator +=  (const TPoint3<T>& B)				{ for (int i = 0; i < countof(val); i++) val[i] += B.val[i]; return *this; }
	TSize3& operator -=  (const TPoint3<T>& B)				{ for (int i = 0; i < countof(val); i++) val[i] -= B.val[i]; return *this; }

	TSize3 operator + (const TSize3& B) const				{ return TSize3(*this) += B; }
	TSize3 operator - (const TSize3& B) const				{ return TSize3(*this) -= B; }
	TSize3 operator + (const TPoint3<T>& B) const			{ return TSize3(*this) += B; }
	TSize3 operator - (const TPoint3<T>& B) const			{ return TSize3(*this) -= B; }

	TSize3 operator - () const								{ return TSize3(-cx, -cy, -cz); }

// Operators returning TRect values
	TSize3& operator *= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] *= d; return *this; }
	TSize3& operator /= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] /= d; return *this; }
	TSize3 operator * (T d) const							{ return TSize3(*this) *= d; }
	TSize3 operator / (T d) const							{ return TSize3(*this) /= d; }
	friend TSize3 operator * (T B, const TSize3& A)			{ TSize3 C(B, B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] *= A.val[i]; return C; }
	friend TSize3 operator / (T B, const TSize3& A)			{ TSize3 C(B, B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] /= A.val[i]; return C; }

	TSize3& operator += (T d)								{ for (int i = 0; i < countof(val); i++) val[i] += d; return *this; }
	TSize3& operator -= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] -= d; return *this; }
	TSize3 operator + (T d) const							{ return TSize3(*this) += d; }
	TSize3 operator - (T d) const							{ return TSize3(*this) -= d; }
	friend TSize3 operator + (T B, const TSize3& A)			{ TSize3 C(B, B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] += A.val[i]; return C; }
	friend TSize3 operator - (T B, const TSize3& A)			{ TSize3 C(B, B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] -= A.val[i]; return C; }

// Functions
	//void SetSize(T cx, T cy) { val[0] = cx; val[1] = cy;}
	void SetSize(T cx = 0, T cy = 0, T cz = 0)	{ this->cx = cx; this->cy = cy; this->cz = cz; }
	void SetSizeAll(T v)						{ for (int i = 0; i <  countof(val); i++) val[i] = v; }
	void Set(const T* values, int nCount = 3) {
		//if (nCount < countof(val))
		//	memset(val, 0, sizeof(val));
		memcpy(val, values, _min((signed)countof(val), nCount)*sizeof(val[0]));
	}
	void SetZero()								{ memset(val, 0, sizeof(val)); }
	void Offset(T x = 0, T y = 0, T z = 0)		{ cx += x; cy += y; cz += z; }

	bool IsAllValid() const {
		for (int i = 0; i < countof(val); i++)
			if (_isnan(val[i]))
				return false;
		return true;
	}

	template < class ARCHIVE > friend ARCHIVE& operator << (ARCHIVE& ar, const TSize3& B) {
		for (int i = 0; i < countof(val); i++)
			ar << B.val[i];
		return ar;
	}
	template < class ARCHIVE > friend ARCHIVE& operator >> (ARCHIVE& ar, TSize3& B) {
		for (int i = 0; i < countof(val); i++)
			ar >> B.val[i];
		return ar;
	}
	template < class ARCHIVE > friend ARCHIVE& operator || (ARCHIVE& ar, const TSize3& B) {
		for (int i = 0; i < countof(val); i++)
			ar || B.val[i];
		return ar;
	}
};

//-----------------------------------------------------------------------------

template < typename T >
class __declspec(novtable) TSize2 {
public:
	typedef cv::Vec<T, 2> type_vec;
public:
	union {
		struct {
			T cx, cy;
		};
		T val[2];
	};
	using coord_t = typename decltype(val);
public:

// Constructors
	TSize2() : cx(0), cy(0) {}
	TSize2(const TSize2& B) { *this = B; }
	TSize2(const TSize3<T>& B)	{ *this = B; }
	TSize2(const TPoint2<T>& B) { *this = B; }
	TSize2(const TPoint3<T>& B) { *this = B; }
	TSize2(T _cx, T _cy) : cx(_cx), cy(_cy) {}
	TSize2(const T_SIZE2<T>& B) { *this = B; }

	explicit TSize2(const SIZE& size)	{ *this = size; }
	explicit TSize2(const POINT& pt)	{ *this = pt; }
	template < typename T2 > TSize2(const cv::Size_<T2>& B)		{ *this = B; }
	template < typename T2 > TSize2(const cv::Point_<T2>& B)	{ *this = B; }
	template < typename T2 > TSize2(const cv::Point3_<T2>& B)	{ *this = B; }
	template < typename T2 > TSize2(const cv::Rect_<T2>& B)		{ *this = B; }
	template < typename T2 > TSize2(T2 _cx, T2 _cy) : cx(_cx), cy(_cy) {}

// Assign Operator
	// Size
	TSize2& operator = (const TSize2& B)			{ if (this != &B) memcpy(val, B.val, sizeof(val)); return *this; }
	TSize2& operator = (const TSize3<T>& B)			{ memcpy(val, B.val, sizeof(val)); return *this; }
	template < typename T2 >
	TSize2& operator = (const TSize2<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	template < typename T2 >
	TSize2& operator = (const TSize3<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	TSize2& operator = (const SIZE& size)			{ cx = size.cx; cy = size.cy; return *this; }
	template < typename T2 >
	TSize2& operator = (const cv::Size_<T2>& B)		{ cx = B.width; cy = B.height; return *this; }

	TSize2& operator = (const T_SIZE2<T>& B)		{ memcpy(val, B.val, sizeof(val)); return *this; }
	template < typename T2 >
	TSize2& operator = (const T_SIZE2<T2>& B)		{ cx = B.cx; cy = B.cy; return *this; }

	// Point
	TSize2& operator = (const TPoint2<T>& B)		{ memcpy(val, B.val, sizeof(val)); return *this; }
	TSize2& operator = (const TPoint3<T>& B)		{ memcpy(val, B.val, sizeof(val)); return *this; }
	template < typename T2 >
	TSize2& operator = (const TPoint2<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	template < typename T2 >
	TSize2& operator = (const TPoint3<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	TSize2& operator = (const POINT& B)				{ cx = B.x; cy = B.y; return *this; }
	template < typename T2 >
	TSize2& operator = (const cv::Point_<T2>& B)	{ cx = B.x; cy = B.y; return *this; }
	template < typename T2 >
	TSize2& operator = (const cv::Point3_<T2>& B)	{ cx = B.x; cy = B.y; return *this; }

	//// Rect
	//template < typename T2 >
	//TSize2& operator = (const cv::Rect_<T2>& B)		{ cx = B.width; cy = B.height; return *this; }

// Compare
	bool operator == (const TSize2& B) const { return memcmp(val, B.val, sizeof(val)) == 0; }
	bool operator != (const TSize2& B) const { return memcmp(val, B.val, sizeof(val)) != 0; }
	int CountNonZero() const { int n = 0; for (int i = 0; i < countof(val); i++) if (val[i] != 0) n++; return n; }

// Member Access
	const T& operator [](int i) const	{ ASSERT(i < countof(val)); return val[i]; }
    T& operator [](int i)				{ ASSERT(i < countof(val)); return val[i]; }
    const T& operator ()(int i) const	{ ASSERT(i < countof(val)); return val[i]; }
    T& operator ()(int i)				{ ASSERT(i < countof(val)); return val[i]; }

// Type Casting
	//operator SIZE () const				{ SIZE size = { (long)val[0], (long)val[1] }; return size; }
	operator CSize () const				{ return CSize((long)val[0], (long)val[1]); }
	template < typename T2 >
	operator cv::Size_<T2> () const		{ return cv::Size_<T2>((T2)val[0], (T2)val[1]); }
	operator type_vec& ()				{ return *(type_vec*)this; }
	operator const type_vec& () const	{ return *(type_vec*)this; }
	operator T_SIZE2<T>& ()				{ return *(T_SIZE2<T>*)this; }
	operator const T_SIZE2<T>& () const	{ return *(T_SIZE2<T>*)this; }

// Numerical Operators
	TSize2& operator +=  (const TSize2& B)					{ for (int i = 0; i < countof(val); i++) val[i] += B.val[i]; return *this; }
	TSize2& operator -=  (const TSize2& B)					{ for (int i = 0; i < countof(val); i++) val[i] -= B.val[i]; return *this; }
	TSize2& operator +=  (const TPoint2<T>& B)				{ for (int i = 0; i < countof(val); i++) val[i] += B.val[i]; return *this; }
	TSize2& operator -=  (const TPoint2<T>& B)				{ for (int i = 0; i < countof(val); i++) val[i] -= B.val[i]; return *this; }

	TSize2 operator + (const TSize2& B) const				{ return TSize2(*this) += B; }
	TSize2 operator - (const TSize2& B) const				{ return TSize2(*this) -= B; }
	TSize2 operator + (const TPoint2<T>& B) const			{ return TSize2(*this) += B; }
	TSize2 operator - (const TPoint2<T>& B) const			{ return TSize2(*this) -= B; }

	TSize2 operator - () const									{ return TSize2(-cx, -cy); }

// Operators returning TRect values
	TSize2& operator *= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] *= d; return *this; }
	TSize2& operator /= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] /= d; return *this; }
	TSize2 operator * (T d) const							{ return TSize2(*this) *= d; }
	TSize2 operator / (T d) const							{ return TSize2(*this) /= d; }
	friend TSize2 operator * (T B, const TSize2& A)			{ TSize2 C(B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] *= A.val[i]; return C; }
	friend TSize2 operator / (T B, const TSize2& A)			{ TSize2 C(B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] /= A.val[i]; return C; }

	TSize2& operator += (T d)								{ for (int i = 0; i < countof(val); i++) val[i] += d; return *this; }
	TSize2& operator -= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] -= d; return *this; }
	TSize2 operator + (T d) const							{ return TSize2(*this) += d; }
	TSize2 operator - (T d) const							{ return TSize2(*this) -= d; }
	friend TSize2 operator + (T B, const TSize2& A)			{ TSize2 C(B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] += A.val[i]; return C; }
	friend TSize2 operator - (T B, const TSize2& A)			{ TSize2 C(B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] -= A.val[i]; return C; }

// Functions
	//void SetSize(T cx, T cy) { val[0] = cx; val[1] = cy;}
	void SetSize(T cx = 0, T cy = 0)			{ this->cx = cx; this->cy = cy;}
	void SetSizeAll(T v)						{ for (int i = 0; i < countof(val); i++) val[i] = v; }
	void Set(const T* values, int nCount = 2) {
		if (nCount < countof(val))
			memset(val, 0, sizeof(val));
		memcpy(val, values, _min((signed)countof(val), nCount)*sizeof(val[0]));
	}
	void SetZero()								{ memset(val, 0, sizeof(val)); }
	void Offset(T x = 0, T y = 0)				{ cx += x; cy += y; }

	bool IsAllValid() const {
		for (int i = 0; i < countof(val); i++)
			if (_isnan(val[i]))
				return false;
		return true;
	}

	template < class ARCHIVE > friend ARCHIVE& operator << (ARCHIVE& ar, const TSize2& B) {
		for (int i = 0; i < countof(val); i++)
			ar << B.val[i];
		return ar;
	}
	template < class ARCHIVE > friend ARCHIVE& operator >> (ARCHIVE& ar, TSize2& B) {
		for (int i = 0; i < countof(val); i++)
			ar >> B.val[i];
		return ar;
	}
	template < class ARCHIVE > friend ARCHIVE& operator || (ARCHIVE& ar, const TSize2& B) {
		for (int i = 0; i < countof(val); i++)
			ar || B.val[i];
		return ar;
	}
};


/////////////////////////////////////////////////////////////////////////////
// TPoint3
//

template < typename T >
class __declspec(novtable) TPoint3 {
public:
	typedef cv::Vec<T, 3> type_vec;
public:
	union {
		struct {
			T x, y, z;
		};
		T val[3];
	};
	using coord_t = typename decltype(val);
public:

// Constructors
	TPoint3() : x(0), y(0), z(0) {}
	TPoint3(const TPoint3& B) { *this = B; }
	TPoint3(const TPoint2<T>& B) : z(0) { *this = B; }
	TPoint3(const TSize2<T>& B) : z(0) { *this = B; }
	TPoint3(const TSize3<T>& B) { *this = B; }
	TPoint3(T _x, T _y, T _z = 0) : x(_x), y(_y), z(_z) {}
	TPoint3(const T_POINT3<T>& B) { *this = B; }

	explicit TPoint3(const SIZE& B)								: z(0) { *this = B; }
	explicit TPoint3(const POINT& B)							: z(0) { *this = B; }
	template < typename T2 > TPoint3(const cv::Size_<T2>& B)	: z(0) { *this = B; }
	template < typename T2 > TPoint3(const cv::Point_<T2>& B)	: z(0) { *this = B; }
	template < typename T2 > TPoint3(const cv::Point3_<T2>& B)	: z(0) { *this = B; }
	template < typename T2 > TPoint3(const cv::Rect_<T2>& B)	: z(0) { *this = B; }
	template < typename T2 > TPoint3(T2 _x, T2 _y, T2 _z) : x(_x), y(_y), z(_z) {}

// Assign Operator
	// Size
	TPoint3& operator = (const TSize2<T>& B)		{ memcpy(val, B.val, sizeof(B.val)); /*z = 0;*/ return *this; }
	TPoint3& operator = (const TSize3<T>& B)		{ memcpy(val, B.val, sizeof(val)); return *this; }
	template < typename T2 >
	TPoint3& operator = (const TSize2<T2>& B)		{ for (int i = 0; i < countof(B.val); i++) val[i] = B.val[i]; /*z = 0;*/ return *this; }
	template < typename T2 >
	TPoint3& operator = (const TSize3<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	TPoint3& operator = (const SIZE& size)			{ x = size.cx; y = size.cy; /*z = 0;*/ return *this; }
	template < typename T2 >
	TPoint3& operator = (const cv::Size_<T2>& B)	{ x = B.width; y = B.height; /*z = 0;*/ return *this; }

	// Point
	TPoint3& operator = (const TPoint2<T>& B)		{ memcpy(val, B.val, sizeof(B.val)); /*z = 0;*/ return *this; }
	TPoint3& operator = (const TPoint3& B)			{ if (this != &B) memcpy(val, B.val, sizeof(val)); return *this; }
	template < typename T2 >
	TPoint3& operator = (const TPoint2<T2>& B)		{ for (int i = 0; i < countof(B.val); i++) val[i] = B.val[i]; /*z = 0;*/ return *this; }
	template < typename T2 >
	TPoint3& operator = (const TPoint3<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	TPoint3& operator = (const POINT& B)			{ x = B.x; y = B.y; /*z = 0;*/ return *this; }
	template < typename T2 >
	TPoint3& operator = (const cv::Point_<T2>& B)	{ x = B.x; y = B.y; /*z = 0;*/ return *this; }
	template < typename T2 >
	TPoint3& operator = (const cv::Point3_<T2>& B)	{ x = B.x; y = B.y; z = B.z; return *this; }

	TPoint3& operator = (const T_POINT3<T>& B)		{ memcpy(val, B.val, sizeof(val)); return *this; }
	template < typename T2 >
	TPoint3& operator = (const T_POINT3<T2>& B)		{ x = B.x; y = B.y; z = B.z; return *this; }

	//// Rect
	//template < typename T2 >
	//TPoint3& operator = (const cv::Rect_<T2>& B)	{ x = B.width; y = B.height; /*z = 0;*/ return *this; }

// Compare
	bool operator == (const TPoint3& B) const		{ return memcmp(val, B.val, sizeof(val)) == 0; }
	bool operator != (const TPoint3& B) const		{ return memcmp(val, B.val, sizeof(val)) != 0; }
	int CountNonZero() const { int n = 0; for (int i = 0; i < countof(val); i++) if (val[i] != 0) n++; return n; }

// Member Access
	const T& operator [](int i) const				{ ASSERT(i < countof(val)); return val[i]; }
    T& operator [](int i)							{ ASSERT(i < countof(val)); return val[i]; }
    const T& operator ()(int i) const				{ ASSERT(i < countof(val)); return val[i]; }
    T& operator ()(int i)							{ ASSERT(i < countof(val)); return val[i]; }

// Type Casting
	//operator POINT () const							{ POINT pt = { (long)val[0], (long)val[1] }; return pt; }
	operator CPoint () const						{ return CPoint((long)val[0], (long)val[1]); }
	template < typename T2 >
	operator cv::Point_<T2> () const				{ return cv::Point_<T2>((T2)val[0], (T2)val[1]); }
	template < typename T2 >
	operator cv::Point3_<T2> () const				{ return cv::Point3_<T2>((T2)val[0], (T2)val[1]); }
	operator type_vec& ()							{ return *(type_vec*)this; }
	operator const type_vec& () const				{ return *(type_vec*)this; }
	operator TPoint2<T> ()							{ return *(TPoint2<T>*)this; }
	operator const TPoint2<T> () const				{ return *(TPoint2<T>*)this; }
	operator T_POINT3<T>& ()						{ return *(T_POINT3<T>*)this; }
	operator const T_POINT3<T>& () const			{ return *(T_POINT3<T>*)this; }

// Numerical Operators
	TPoint3& operator +=  (const TSize3<T>& B)				{ for (int i = 0; i < countof(val); i++) val[i] += B.val[i]; return *this; }
	TPoint3& operator -=  (const TSize3<T>& B)				{ for (int i = 0; i < countof(val); i++) val[i] -= B.val[i]; return *this; }
	TPoint3& operator +=  (const TPoint3& B)				{ for (int i = 0; i < countof(val); i++) val[i] += B.val[i]; return *this; }
	TPoint3& operator -=  (const TPoint3& B)				{ for (int i = 0; i < countof(val); i++) val[i] -= B.val[i]; return *this; }

	TPoint3 operator + (const TSize3<T>& B) const			{ return TPoint3(*this) += B; }
	TPoint3 operator - (const TSize3<T>& B) const			{ return TPoint3(*this) -= B; }
	TPoint3 operator + (const TPoint3& B) const				{ return TPoint3(*this) += B; }
	TPoint3 operator - (const TPoint3& B) const				{ return TPoint3(*this) -= B; }

	TPoint3 operator - () const								{ return TPoint3(-x, -y, -z); }

// Operators returning TRect values
	TPoint3& operator *= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] *= d; return *this; }
	TPoint3& operator /= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] /= d; return *this; }
	TPoint3 operator * (T d) const							{ return TPoint3(*this) *= d; }
	TPoint3 operator / (T d) const							{ return TPoint3(*this) /= d; }
	friend TPoint3 operator * (T B, const TPoint3& A)		{ TPoint3 C(B, B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] *= A.val[i]; return C; }
	friend TPoint3 operator / (T B, const TPoint3& A)		{ TPoint3 C(B, B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] /= A.val[i]; return C; }
	TPoint3 operator * (const TPoint3& B) const				{ return TPoint3(*this).Cross(B); }

	TPoint3& operator += (T d)								{ for (int i = 0; i < countof(val); i++) val[i] += d; return *this; }
	TPoint3& operator -= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] -= d; return *this; }
	TPoint3 operator + (T d) const							{ return TPoint3(*this) += d; }
	TPoint3 operator - (T d) const							{ return TPoint3(*this) -= d; }
	friend TPoint3 operator + (T B, const TPoint3& A)		{ TPoint3 C(B, B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] += A.val[i]; return C; }
	friend TPoint3 operator - (T B, const TPoint3& A)		{ TPoint3 C(B, B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] -= A.val[i]; return C; }

// Mathmatical Operator
	template < typename T2, int m, int n >
	friend TPoint3 operator * (const cv::Matx<T2, m, n>& A, const TPoint3& B) {
		TPoint3 C;
		if ( (m >= countof(val)) && (n >= countof(val)) ) {
			int irc = 0;
			//for (int i = 0; i < countof(val); i++) {
			//	for (int j = 0; j < countof(val); j++) {
			//		C.val[i] = A.val[i*n + j] * B.val[j];
			//	}
			//}
			C.x = A.val[irc + 0] * B.x + A.val[irc + 1] * B.y + A.val[irc + 2] * B.z;	irc += n;
			C.y = A.val[irc + 0] * B.x + A.val[irc + 1] * B.y + A.val[irc + 2] * B.z;	irc += n;
			C.z = A.val[irc + 0] * B.x + A.val[irc + 1] * B.y + A.val[irc + 2] * B.z;	irc += n;
		}
		if (n >= 4) {	// alpha channel
			C.x += A.val[0*n + 3];
			C.y += A.val[1*n + 3];
			C.z += A.val[2*n + 3];
		}
		if (m >= 4) {
			T alpha = 0;
			int irc = 3*n;
			alpha = A.val[irc + 0] * B.x + A.val[irc + 1] * B.y + A.val[irc+2] * B.z;
			if (n >= 4)
				alpha += A.val[irc +  3];
			C *= alpha;
		}

		return C;
	}

	__if_exists(cv::Mat) {
		friend TPoint3 operator * (const cv::Mat& A, const TPoint3& B) {
			ASSERT(A.channels() == 1);
			if (A.channels() != 1)
				return B;
			ASSERT((A.rows >= countof(B.val)) && (A.cols >= countof(B.val)));
			if ( (A.rows < countof(B.val)) || (A.cols < countof(B.val)) )
				return B;

#define MAT_MUL_PT(mat, pt, type, row) (mat.at<type>(row, 0) * pt.x + mat.at<type>(row, 1) * pt.y + mat.at<type>(row, 2) * pt.z)

			TPoint3 C;
			switch (A.depth()) {
			case CV_8U :	C.x = MAT_MUL_PT(A, B, BYTE,   0); C.y = MAT_MUL_PT(A, B, BYTE,   1); C.z = MAT_MUL_PT(A, B, BYTE,   2); break;
			case CV_8S :	C.x = MAT_MUL_PT(A, B, char,   0); C.y = MAT_MUL_PT(A, B, char,   1); C.z = MAT_MUL_PT(A, B, char,   2); break;
			case CV_16U :	C.x = MAT_MUL_PT(A, B, WORD,   0); C.y = MAT_MUL_PT(A, B, WORD,   1); C.z = MAT_MUL_PT(A, B, WORD,   2); break;
			case CV_16S :	C.x = MAT_MUL_PT(A, B, SHORT,  0); C.y = MAT_MUL_PT(A, B, SHORT,  1); C.z = MAT_MUL_PT(A, B, SHORT,  2); break;
			case CV_32S :	C.x = MAT_MUL_PT(A, B, int,    0); C.y = MAT_MUL_PT(A, B, int,    1); C.z = MAT_MUL_PT(A, B, int,    2); break;
			case CV_32F :	C.x = MAT_MUL_PT(A, B, float,  0); C.y = MAT_MUL_PT(A, B, float,  1); C.z = MAT_MUL_PT(A, B, float,  2); break;
			case CV_64F :	C.x = MAT_MUL_PT(A, B, double, 0); C.y = MAT_MUL_PT(A, B, double, 1); C.z = MAT_MUL_PT(A, B, double, 2); break;
			default:
				C = B;
				break;
			}

			if (A.cols >= 4) {	// alpha channel
				switch (A.depth()) {
#define MAT_MUL_SHIFT(mat, type, row) mat.at<type>(row, 3)
				case CV_8U :	C.x += MAT_MUL_SHIFT(A, BYTE,   0); C.y += MAT_MUL_SHIFT(A, BYTE,   1); C.z += MAT_MUL_SHIFT(A, BYTE,   2); break;
				case CV_8S :	C.x += MAT_MUL_SHIFT(A, char,   0); C.y += MAT_MUL_SHIFT(A, char,   1); C.z += MAT_MUL_SHIFT(A, char,   2); break;
				case CV_16U :	C.x += MAT_MUL_SHIFT(A, WORD,   0); C.y += MAT_MUL_SHIFT(A, WORD,   1); C.z += MAT_MUL_SHIFT(A, WORD,   2); break;
				case CV_16S :	C.x += MAT_MUL_SHIFT(A, SHORT,  0); C.y += MAT_MUL_SHIFT(A, SHORT,  1); C.z += MAT_MUL_SHIFT(A, SHORT,  2); break;
				case CV_32S :	C.x += MAT_MUL_SHIFT(A, int,    0); C.y += MAT_MUL_SHIFT(A, int,    1); C.z += MAT_MUL_SHIFT(A, int,    2); break;
				case CV_32F :	C.x += MAT_MUL_SHIFT(A, float,  0); C.y += MAT_MUL_SHIFT(A, float,  1); C.z += MAT_MUL_SHIFT(A, float,  2); break;
				case CV_64F :	C.x += MAT_MUL_SHIFT(A, double, 0); C.y += MAT_MUL_SHIFT(A, double, 1); C.z += MAT_MUL_SHIFT(A, double, 2); break;
#undef MAT_MUL_SHIFT
				}
			}
			if (A.rows >= 4) {
				T alpha = 0;
				switch (A.depth()) {
				case CV_8U :	alpha = MAT_MUL_PT(A, B, BYTE,   3); break;
				case CV_8S :	alpha = MAT_MUL_PT(A, B, char,   3); break;
				case CV_16U :	alpha = MAT_MUL_PT(A, B, WORD,   3); break;
				case CV_16S :	alpha = MAT_MUL_PT(A, B, SHORT,  3); break;
				case CV_32S :	alpha = MAT_MUL_PT(A, B, int,    3); break;
				case CV_32F :	alpha = MAT_MUL_PT(A, B, float,  3); break;
				case CV_64F :	alpha = MAT_MUL_PT(A, B, double, 3); break;
				}
				if (A.cols >= 4) {
					switch (A.depth()) {
					case CV_8U :	alpha += A.at<BYTE>  (3, 3); break;
					case CV_8S :	alpha += A.at<char>  (3, 3); break;
					case CV_16U :	alpha += A.at<WORD>  (3, 3); break;
					case CV_16S :	alpha += A.at<SHORT> (3, 3); break;
					case CV_32S :	alpha += A.at<int>   (3, 3); break;
					case CV_32F :	alpha += A.at<float> (3, 3); break;
					case CV_64F :	alpha += A.at<double>(3, 3); break;
					}
				}
				C *= alpha;
			}

#undef MAT_MUL_PT

			return C;
		}
	}

// Functions
	void SetPoint(T x = 0, T y = 0, T z = 0)	{ this->x = x; this->y = y; this->z = z; }
	void SetPointAll(T v)						{ for (int i = 0; i <  countof(val); i++) val[i] = v; }
	void Set(const T* values, int nCount = 3) {
		if (nCount < countof(val))
			memset(val, 0, sizeof(val));
		memcpy(val, values, _min((signed)countof(val), nCount)*sizeof(val[0]));
	}
	void SetZero()								{ memset(val, 0, sizeof(val)); }
	void Offset(T x = 0, T y = 0, T z = 0)		{ this->x += x; this->y += y; this->z += z; }

	bool IsAllValid() const {
		for (int i = 0; i < countof(val); i++)
			if (_isnan(val[i]))
				return false;
		return true;
	}

	template < class ARCHIVE > friend ARCHIVE& operator << (ARCHIVE& ar, const TPoint3& B) {
		for (int i = 0; i < countof(val); i++)
			ar << B.val[i];
		return ar;
	}
	template < class ARCHIVE > friend ARCHIVE& operator >> (ARCHIVE& ar, TPoint3& B) {
		for (int i = 0; i < countof(val); i++)
			ar >> B.val[i];
		return ar;
	}
	template < class ARCHIVE > friend ARCHIVE& operator || (ARCHIVE& ar, const TPoint3& B) {
		for (int i = 0; i < countof(val); i++)
			ar || B.val[i];
		return ar;
	}

public:
	// Math

	double Distance(const TPoint3& pt2) const {
		double dSum = 0;
		for (int i = 0; i < countof(val); i++)
			dSum += _sqr(val[i]-pt2.val[i]);
		return sqrt(dSum);
	}
	double GetLength() const {
		double dLength = 0;
		for (int i = 0; i < countof(val); i++)
			dLength += _sqr(val[i]);
		return sqrt(dLength);
	}
	double GetAngleXY() const { return atan2(y, x); }
	double GetAngleYZ() const { return atan2(z, y); }
	double GetAngleZX() const { return atan2(x, z); }
	template <typename T2> double GetAngleXY(const T2& B) const { return (TPoint2(B) - *this).GetAngleXY(); }
	template <typename T2> double GetAngleYZ(const T2& B) const { return (TPoint2(B) - *this).GetAngleYZ(); }
	template <typename T2> double GetAngleZX(const T2& B) const { return (TPoint2(B) - *this).GetAngleZX(); }

	TPoint3 GetNormalizedVector() const		{ return *this / GetLength(); }	// Length == 1.0
	bool Normalize()						{ *this /= GetLength(); return IsAllValid(); }

	TPoint3 GetNormalVectorXY() const { return TPoint3(*this) * TPoint3(0, 0, 1); }		// Perpendicular(Normal) Vector (XY-Plan)
	TPoint3 GetNormalVectorYZ() const { return TPoint3(*this) * TPoint3(1, 0, 0); }		// Perpendicular(Normal) Vector (YZ-Plan)
	TPoint3 GetNormalVectorZX() const { return TPoint3(*this) * TPoint3(0, 1, 0); }		// Perpendicular(Normal) Vector (ZX-Plan)

	TPoint3 Cross(const TPoint3& B) const {
		return TPoint3(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x);
	}
	T CrossZ(const TPoint3& B) const {
		return x*B.y - y*B.x;
	}
	T Dot(const TPoint3& B) const {
		T dSum = 0;
		for (int i = 0; i < countof(val); i++) dSum += val[i] * B.val[i];
		return dSum;
	}
	TPoint3 Mul(const TPoint3& B) const {
		TPoint3 C;
		for (int i = 0; i < countof(val); i++)
			C.val[i] = val[i] * B.val[i];
	}
	bool CheckMinMax(TPoint3& ptMin, TPoint3& ptMax) const {
		bool bModified = false;
		for (int i = 0; i < countof(val); i++) {
			if (ptMin.val[i] > val[i]) { ptMin.val[i] = val[i]; bModified = true; }
			if (ptMax.val[i] < val[i]) { ptMax.val[i] = val[i]; bModified = true; }
		}
		return bModified;
	}
};

//-----------------------------------------------------------------------------

template < typename T >
class __declspec(novtable) TPoint2 {
public:
	typedef cv::Vec<T, 2> type_vec;
public:
	union {
		struct {
			T x, y;
		};
		T val[2];
	};
	using coord_t = typename decltype(val);
public:

// Constructors
	TPoint2() : x(0), y(0) {}
	TPoint2(const TPoint2& B)	{ *this = B; }
	TPoint2(const TPoint3<T>& B) { *this = B; }
	TPoint2(const TSize2<T>& B) { *this = B; }
	TPoint2(const TSize3<T>& B) { *this = B; }
	TPoint2(T _x, T _y) : x(_x), y(_y) {}
	TPoint2(const T_POINT2<T>& B) { *this = B; }

	TPoint2(const SIZE& B)	{ *this = B; }
	TPoint2(const POINT& B)	{ *this = B; }
	template < typename T2 > TPoint2(const cv::Size_<T2>& B)	{ *this = B; }
	template < typename T2 > TPoint2(const cv::Point_<T2>& B)	{ *this = B; }
	template < typename T2 > TPoint2(const cv::Point3_<T2>& B)	{ *this = B; }
	template < typename T2 > TPoint2(const cv::Rect_<T2>& B)	{ *this = B; }
	template < typename T2 > TPoint2(T2 _x, T2 _y) : x(_x), y(_y) {}

// Assign Operator
	// Size
	TPoint2& operator = (const TSize2<T>& B)		{ memcpy(val, B.val, sizeof(B.val)); return *this; }
	TPoint2& operator = (const TSize3<T>& B)		{ if (this != &B) memcpy(val, B.val, sizeof(val)); return *this; }
	template < typename T2 >
	TPoint2& operator = (const TSize2<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	template < typename T2 >
	TPoint2& operator = (const TSize3<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	TPoint2& operator = (const SIZE& size)			{ x = size.cx; y = size.cy; return *this; }
	template < typename T2 >
	TPoint2& operator = (const cv::Size_<T2>& B)	{ x = B.width; y = B.height; return *this; }

	// Point
	TPoint2& operator = (const TPoint2& B)			{ if (this != &B) memcpy(val, B.val, sizeof(B.val)); return *this; }
	TPoint2& operator = (const TPoint3<T>& B)		{ memcpy(val, B.val, sizeof(val)); return *this; }
	template < typename T2 >
	TPoint2& operator = (const TPoint2<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	template < typename T2 >
	TPoint2& operator = (const TPoint3<T2>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = B.val[i]; return *this; }
	TPoint2& operator = (const POINT& B)			{ x = B.x; y = B.y; return *this; }
	template < typename T2 >
	TPoint2& operator = (const cv::Point_<T2>& B)	{ x = B.x; y = B.y; return *this; }
	template < typename T2 >
	TPoint2& operator = (const cv::Point3_<T2>& B)	{ x = B.x; y = B.y; return *this; }

	TPoint2& operator = (const T_POINT2<T>& B)		{ memcpy(val, B.val, sizeof(val)); return *this; }
	template < typename T2 >
	TPoint2& operator = (const T_POINT2<T2>& B)		{ x = B.x; y = B.y; return *this; }

	//// Rect
	//template < typename T2 >
	//TPoint2& operator = (const cv::Rect_<T2>& B)	{ x = B.x; y = B.y; return *this; }

// Compare
	bool operator == (const TPoint2& B) const		{ return memcmp(val, B.val, sizeof(val)) == 0; }
	bool operator != (const TPoint2& B) const		{ return memcmp(val, B.val, sizeof(val)) != 0; }
	int CountNonZero() const { int n = 0; for (int i = 0; i < countof(val); i++) if (val[i] != 0) n++; return n; }

// Member Access
	const T& operator [](int i) const				{ ASSERT(i < countof(val)); return val[i]; }
    T& operator [](int i)							{ ASSERT(i < countof(val)); return val[i]; }
    const T& operator ()(int i) const				{ ASSERT(i < countof(val)); return val[i]; }
    T& operator ()(int i)							{ ASSERT(i < countof(val)); return val[i]; }

// Type Casting
	//operator POINT () const							{ POINT pt = { (long)val[0], (long)val[1] }; return pt; }
	operator CPoint () const						{ return CPoint((long)val[0], (long)val[1]); }
	template < typename T2 >
	operator cv::Point_<T2> () const				{ return cv::Point_<T2>((T2)val[0], (T2)val[1]); }
	operator type_vec& ()							{ return *(type_vec*)this; }
	operator const type_vec& () const				{ return *(type_vec*)this; }
	operator T_POINT2<T>& () 						{ return *(T_POINT2<T>*)this; }
	operator const T_POINT2<T>& () const			{ return *(T_POINT2<T>*)this; }

// Numerical Operators
	TPoint2& operator +=  (const TSize2<T>& B)				{ for (int i = 0; i < countof(val); i++) val[i] += B.val[i]; return *this; }
	TPoint2& operator -=  (const TSize2<T>& B)				{ for (int i = 0; i < countof(val); i++) val[i] -= B.val[i]; return *this; }
	TPoint2& operator +=  (const TPoint2& B)				{ for (int i = 0; i < countof(val); i++) val[i] += B.val[i]; return *this; }
	TPoint2& operator -=  (const TPoint2& B)					{ for (int i = 0; i < countof(val); i++) val[i] -= B.val[i]; return *this; }

	TPoint2 operator + (const TSize2<T>& B) const			{ return TPoint2(*this) += B; }
	TPoint2 operator - (const TSize2<T>& B) const			{ return TPoint2(*this) -= B; }
	TPoint2 operator + (const TPoint2& B) const				{ return TPoint2(*this) += B; }
	TPoint2 operator - (const TPoint2& B) const				{ return TPoint2(*this) -= B; }

	TPoint2 operator - () const								{ return TPoint2(-x, -y); }

// Operators returning TRect values
	TPoint2& operator *= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] *= d; return *this; }
	TPoint2& operator /= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] /= d; return *this; }
	TPoint2 operator * (T d) const							{ return TPoint2(*this) *= d; }
	TPoint2 operator / (T d) const							{ return TPoint2(*this) /= d; }
	friend TPoint2 operator * (T B, const TPoint2& A)		{ TPoint2 C(B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] *= A.val[i]; return C; }
	friend TPoint2 operator / (T B, const TPoint2& A)		{ TPoint2 C(B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] /= A.val[i]; return C; }

	TPoint2& operator += (T d)								{ for (int i = 0; i < countof(val); i++) val[i] += d; return *this; }
	TPoint2& operator -= (T d)								{ for (int i = 0; i < countof(val); i++) val[i] -= d; return *this; }
	TPoint2 operator + (T d) const							{ return TPoint2(*this) += d; }
	TPoint2 operator - (T d) const							{ return TPoint2(*this) -= d; }
	friend TPoint2 operator + (T B, const TPoint2& A)		{ TPoint2 C(B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] *= A.val[i]; return C; }
	friend TPoint2 operator - (T B, const TPoint2& A)		{ TPoint2 C(B, B); for (int i = 0; i < countof(A.val); i++) C.val[i] /= A.val[i]; return C; }

// Mathmatical Operator
	template < typename T2, int m, int n >
	friend TPoint2 operator * (const cv::Matx<T2, m, n>& A, const TPoint2& B) {
		TPoint2 C;
		if ( (m >= countof(val)) && (n >= countof(val)) ) {
			int irc = 0;
			C.x = A.val[irc + 0] * B.x + A.val[irc + 1] * B.y;	irc += n;
			C.y = A.val[irc + 0] * B.x + A.val[irc + 1] * B.y;	irc += n;
		} else {
			throw std::exception(std::invalid_argument(__FMSG("mat * pt FAILED")));
		};
		return C;
	}
	__if_exists(cv::Mat) {
		friend TPoint2 operator * (const cv::Mat& A, const TPoint2& B) {
			ASSERT(A.channels() == 1);
			if (A.channels() != 1)
				return B;
			ASSERT((A.rows >= countof(val)) && (A.cols >= countof(val)));
			if ( (A.rows < countof(val)) || (A.cols < countof(val)) )
				return B;

			switch (A.depth()) {
#define MAT_MUL_PT(mat, pt, type, row) (mat.at<type>(row, 0) * pt.x + mat.at<type>(row, 1) * pt.y)
			case CV_8U :	{ return TPoint2(MAT_MUL_PT(A, B, BYTE,   0), MAT_MUL_PT(A, B, BYTE,   1)); } break;
			case CV_8S :	{ return TPoint2(MAT_MUL_PT(A, B, char,   0), MAT_MUL_PT(A, B, char,   1)); } break;
			case CV_16U :	{ return TPoint2(MAT_MUL_PT(A, B, WORD,   0), MAT_MUL_PT(A, B, WORD,   1)); } break;
			case CV_16S :	{ return TPoint2(MAT_MUL_PT(A, B, SHORT,  0), MAT_MUL_PT(A, B, SHORT,  1)); } break;
			case CV_32S :	{ return TPoint2(MAT_MUL_PT(A, B, int,    0), MAT_MUL_PT(A, B, int,    1)); } break;
			case CV_32F :	{ return TPoint2(MAT_MUL_PT(A, B, float,  0), MAT_MUL_PT(A, B, float,  1)); } break;
			case CV_64F :	{ return TPoint2(MAT_MUL_PT(A, B, double, 0), MAT_MUL_PT(A, B, double, 1)); } break;
#undef MAT_MUL_PT
			default:
				break;
			}
			return B;
		}
	}

// Functions
	void SetPoint(T x = 0, T y = 0)				{ this->x = x; this->y = y; }
	void SetPointAll(T v)						{ for (int i = 0; i <  countof(val); i++) val[i] = v; }
	void Set(const T* values, int nCount = 2) {
		if (nCount < countof(val))
			memset(val, 0, sizeof(val));
		memcpy(val, values, _min((signed)countof(val), nCount)*sizeof(val[0]));
	}
	void SetZero()								{ memset(val, 0, sizeof(val)); }
	void Offset(T x = 0, T y = 0)				{ this->x += x; this->y += y; }

	bool IsAllValid() const {
		for (int i = 0; i < countof(val); i++)
			if (_isnan(val[i]))
				return false;
		return true;
	}

	template < class ARCHIVE > friend ARCHIVE& operator << (ARCHIVE& ar, const TPoint2& B) {
		for (int i = 0; i < countof(B.val); i++)
			ar << B.val[i];
		return ar;
	}
	template < class ARCHIVE > friend ARCHIVE& operator >> (ARCHIVE& ar, TPoint2& B) {
		for (int i = 0; i < countof(B.val); i++)
			ar >> B.val[i];
		return ar;
	}
	template < class ARCHIVE > friend ARCHIVE& operator || (ARCHIVE& ar, const TPoint2& B) {
		for (int i = 0; i < countof(val); i++)
			ar || B.val[i];
		return ar;
	}

public:
	// Math

	double Distance(const TPoint2& pt2) const {
		double dSum = 0;
		for (int i = 0; i < countof(val); i++)
			dSum += _sqr(val[i]-pt2.val[i]);
		return sqrt(dSum);
	}
	double GetLength() const {
		double dLength = 0;
		for (int i = 0; i < countof(val); i++)
			dLength += _sqr(val[i]);
		return sqrt(dLength);
	}
	double GetAngleXY() const { return atan2(y, x); }
	//double GetAngleYZ() const { return atan2(z, y); }
	//double GetAngleZX() const { return atan2(x, z); }
	template <typename T2> double GetAngleXY(const T2& B) const { return (TPoint2(B)-*this).GetAngleXY(); }
	//template <typename T2> double GetAngleYZ(const T2& B) const { return (TPoint2(B) - *this).GetAngleYZ(); }
	//template <typename T2> double GetAngleZX(const T2& B) const { return (TPoint2(B) - *this).GetAngleZX(); }

	TPoint2 GetNormalizedVector() const		{ return *this / GetLength(); }	// Length == 1.0
	bool Normalize()						{ *this /= GetLength(); return IsAllValid(); }

	TPoint3<T> Cross(const TPoint2& B) const {
		const TPoint2& A(*this);
		return TPoint3(0, 0, A.x*B.y - A.y*B.x);
	}
	T CrossZ(const TPoint2& B) const {
		return x*B.y - y*B.x;
	}
	T Dot(const TPoint2& B) const {
		T dSum = 0;
		for (int i = 0; i < countof(val); i++) dSum += val[i] * B.val[i];
		return dSum;
	}
	TPoint2 Mul(const TPoint2& B) const {
		TPoint2 C;
		for (int i = 0; i < countof(val); i++)
			C.val[i] = val[i] * B.val[i];
	}
	bool CheckMinMax(TPoint2& ptMin, TPoint2& ptMax) const {
		bool bModified = false;
		for (int i = 0; i < countof(val); i++) {
			if (ptMin.val[i] > val[i]) { ptMin.val[i] = val[i]; bModified = true; }
			if (ptMax.val[i] < val[i]) { ptMax.val[i] = val[i]; bModified = true; }
		}
		return bModified;
	}
};


/////////////////////////////////////////////////////////////////////////////
// TRect3 - A 3-D rectangle

template < typename T >
class TRect3 {
public:
	union {
		struct {
			union {
				struct { TPoint3<T> pt0; };
				struct { T left, top, front; };
			};
			union {
				struct { TPoint3<T> pt1; };
				struct { T right, bottom, back; };
			};
		};
		T val[6];
	};
	using coord_t = typename decltype(val);
	// Constructors
public:
	constexpr T default_front() const { return -1; }
	constexpr T default_back() const { return 1; }

	TRect3() : left(0), top(0), front(default_front()), right(0), bottom(0), back(default_back()) {}
	TRect3(const TRect3& B) { *this = B; }
	explicit TRect3(const TRect2<T>& B) : front(default_front()), back(default_back()) { *this = B; }
	TRect3(T l, T t, T f, T r, T b, T bk) : left(l), top(t), front(f), right(r), bottom(b), back(bk) {}
	TRect3(const TPoint3<T>& pt, const TSize3<T>& size) : pt0(pt), pt1(pt0+size) {}
	TRect3(const TPoint3<T>& _pt0, const TPoint3<T>& _pt1) : pt0(_pt0), pt1(_pt1) {}
	explicit TRect3(const RECT& B) { *this = B; }
	TRect3(POINT point, SIZE size) : pt0(point), pt1(point) { pt1 += size; }
	TRect3(POINT topLeft, POINT bottomRight) : pt0(topLeft), pt1(bottomRight) {}
	TRect3(const T_RECT3<T>& B) { B = *this; }
	
	template < typename T2 > explicit TRect3(const TRect2<T2>& B) : front(default_front()), back(default_back()) { *this = B; }
	template < typename T2 > TRect3(T2 l, T2 t, T2 f, T2 r, T2 b, T2 bk) : left(l), top(t), front(f), right(r), bottom(b), back(bk) {}
	template < typename T2 > TRect3(const TPoint3<T2>& pt, const TSize3<T2>& size) : pt0(pt), pt1(pt0+size) {}
	template < typename T2 > TRect3(const TPoint3<T2>& _pt0, const TPoint3<T2>& _pt1) : pt0(_pt0), pt1(_pt1) {}
	template < typename T2 > TRect3(const TRect3<T2>& B) { *this = B; }
	template < typename T2 > TRect3(const T_RECT3<T2>& B) { *this = B; }


	TRect3& operator = (const TRect3& B) { if (this == &B) return *this; pt0 = B.pt0; pt1 = B.pt1; return *this; }
	TRect3& operator = (const TRect2<T>& B) { pt0 = B.pt0; pt1 = B.pt1; return *this; }
	TRect3& operator = (const RECT& srcRect) { left = srcRect.left; top = srcRect.top; /*front = 0;*/ right = srcRect.right; bottom = srcRect.bottom; /*back = 0;*/ return *this; }

	TRect3& operator = (const T_RECT3<T>& B) { pt0 = B.pt0; pt1 = B.pt1; return *this; }

	template < typename T2 > TRect3& operator = (const TRect3<T2>& B) { if (this == &B) return *this; pt0 = B.pt0; pt1 = B.pt1; return *this; }
	template < typename T2 > TRect3& operator = (const TRect2<T2>& B) { pt0 = B.pt0; pt1 = B.pt1; return *this; }
	template < typename T2 > TRect3& operator = (const T_RECT3<T2>& B) { pt0 = B.pt0; pt1 = B.pt1; return *this; }

// Attributes (in addition to RECT members)

	T Width() const { return right - left; }
	T Height() const { return bottom - top; }
	T Depth() const { return back - front; }

	operator TRect2<T> () const					{ return TRect2((TPoint2<T>)pt0, (TPoint2<T>)pt1); }
	operator TSize3<T> () const					{ return pt1 - pt0; }
	//operator SIZE () const						{ return (SIZE)(TSize3<T>)this; }
	operator CSize () const						{ return (CSize)(TSize3<T>)this; }
	operator CRect () const						{ return CRect((CPoint)pt0, (CPoint)pt1); }
	operator T_RECT3<T>& () 					{ return *(T_RECT3<T>*)this; }
	operator const T_RECT3<T>& () const			{ return *(T_RECT3<T>*)this; }
	template < typename T2 >
	operator cv::Rect_<T2>& () const			{ return cv::Rect_<T2>(pt0.x, pt0.y, pt1.x-pt0.x, pt1.y-pt0.y); }

	TPoint2<T>& TopLeft() { return pt0; }
	const TPoint2<T>& TopLeft() const { return pt0; }
	TPoint3<T>& TopLeftFront() { return pt0; }
	const TPoint3<T>& TopLeftFront() const { return pt0; }

	TPoint2<T>& BottomRight() { return pt1; }
	const TPoint2<T>& BottomRight() const { return pt1; }
	TPoint3<T>& BottomRightEnd() { return pt1; }
	const TPoint3<T>& BottomRightEnd() const { return pt1; }

	TPoint3<T> CenterPoint() const { return (pt0+pt1)/(T)2; }

	TSize3<T> GetSize() const { return (pt1-pt0); }

	//const operator CRect () const { return CRect(left, top, right, bottom); }

	// returns true if rectangle has no area
	bool IsRectEmpty() const {
		for (int i = 0; i < countof(pt0.val); i++) {
			if (pt0[i] >= pt1[i])
				return true;
		}
		return false;
	}
	// returns true if rectangle is at (0,0,0) and has no area
	bool IsRectNull() const {
		for (int i = 0; i < countof(pt0.val); i++) {
			if (pt0[i] != 0.0)
				return false;
			if (pt0[i] != pt1[i])
				return false;
		}
		return true;
	}
	// returns true if point is within rectangle
	bool PtInRect(const TPoint3<T>& pt) const {
		for (int i = 0; i < _min(countof(pt.val), countof(pt0.val)); i++) {
			if ( (pt[i] < pt0[i]) || (pt[i] >= pt1[i]) )
				return false;
		}
		return true;
	}
	// returns true if point is within rectangle
	bool PtInRect(const TPoint2<T>& pt) const {
		for (int i = 0; i < _min(countof(pt.val), countof(pt0.val)); i++) {
			if ( (pt[i] < pt0[i]) || (pt[i] >= pt1[i]) )
				return false;
		}
		return true;
	}
	// returns true if rect is within rectangle
	bool RectInRect(const TRect3<T>& rect) const {
		for (int i = 0; i < countof(pt0.val); i++) {
			if ( (rect.pt0.val[i] < pt0.val[i]) || (rect.pt0.val[i] > pt1.val[i]) )
				return false;
			if ( (rect.pt1.val[i] < pt0.val[i]) || (rect.pt1.val[i] > pt1.val[i]) )
				return false;
		}
		return true;
	}

// Operations

	// set rectangle from left, top, right, and bottom
	void SetRect(T x1 = 0, T y1 = 0, T z1 = -1, T x2 = 0, T y2 = 0, T z2 = 1) {
		left = x1; top = y1; front = z1; right = x2; bottom = y2; back = z2;
	}

	void SetRect(const TPoint3<T>& pt, const TSize3<T>& size)		{ pt0 = pt; pt1 = pt+size; }
	void SetRect(const TPoint3<T>& _pt0, const TPoint3<T>& _pt1)	{ pt0 = _pt0; pt1 = _pt1; }
	void SetRectEmpty() { pt0.SetZero(); pt1.SetZero(); }
	// copy from another rectangle
	void CopyRect(const TRect3& rectSource) {
		pt0 = rectSource.pt0; pt1 = rectSource.pt1;
	}
	// true if exactly the same as another rectangle
	bool EqualRect(const TRect3& rect) const {
		return (pt0 == rect.pt0) && (pt1 == rect.pt1);
	}

	// infflate the rectangles's width, height and depth
	TRect3& InflateRect(T x, T y, T z) { left -= x; top -= y; front -= z; right += x; bottom += y; back += z; return *this; }
	TRect3& InflateRect(const TSize3<T>& size) { pt0 -= size; pt1 += size; return *this; }
	TRect3& InflateRect(const TRect3<T>& rect) { pt0 -= rect.pt0; pt1 += rect.pt1; return *this; }
	TRect3& InflateRect(T l, T t, T r, T b) { left -= l; top -= t; right += r; bottom += b; return *this; }
	TRect3& InflateRect(T l, T t, T f, T r, T b, T bk) { left -= l; top -= t; front -= f; right += r; bottom += b; back += bk; return *this; }

	// deflate the rectangle's width, height and depth
	TRect3& DeflateRect(T x, T y, T z) { left += x; top += y; front += z; right -= x; bottom -= y; back -= z; return *this; }
	TRect3& DeflateRect(const TSize3<T>& size) { pt0 += size; pt1 -= size; return *this; }
	TRect3& DeflateRect(const TRect3<T>& rect) { pt0 += rect.pt0; pt1 -= rect.pt1; return *this; }
	TRect3& DeflateRect(T l, T t, T r, T b)  { left += l; top += t; right -= r; bottom -= b; return *this; }
	TRect3& DeflateRect(T l, T t, T f, T r, T b, T bk) { left += l; top += t; front += f; right -= r; bottom -= b; back -= bk; return *this; }

	// translate the rectangle by moving its top and left
	void OffsetRect(T x, T y, T z = 0) { left += x; top += y; front += z; right += x; bottom += y; back += z; }
	void OffsetRect(const TSize3<T>& size) { pt0 += size; pt1 += size; }
	void OffsetRect(const TPoint3<T>& pt) { pt0 += pt; pt1 += pt; }

private:
	template < class T > void swap(T& x, T& y) { T c = x; x = y; y = c; }
public:
	void NormalizeRect() {
		for (int i = 0; i < countof(pt0.val); i++) {
			if (pt0.val[i] > pt1.val[i])
				swap(pt0.val[i], pt1.val[i]);
		}
	}

	// absolute position of rectangle
	void MoveToX(T x) { right += (x-left); left = x; }
	void MoveToY(T y) { bottom += (y-top); top = y; }
	void MoveToZ(T z) { back += (z-front); front = z; }
	void MoveToN(int i, T v) { ASSERT(countof(pt0.val) > i); pt1[i] += (v-pt0[i]); pt0[i] = v; }

	void MoveToXY(T x, T y) { MoveToX(x); MoveToY(y); }
	void MoveToYZ(T y, T z) { MoveToY(y); MoveToZ(z); }
	void MoveToZX(T z, T x) { MoveToZ(z); MoveToX(x); }
	void MoveToXYZ(T x, T y, T z) { MoveToX(x); MoveToY(y); MoveToZ(z); }

	void MoveTo(const TPoint3<T>& pt) {
		for (int i = 0; i < countof(pt0.val);  i++)
			MoveToN(i, pt[i]);
	}

	// set this rectangle to intersection of two others
	bool IntersectRect(const TRect3<T>& _rect1, const TRect3<T>& _rect2) { 
		TRect3<T> rect, rect1(_rect1), rect2(_rect2);
		rect1.NormalizeRect();
		rect2.NormalizeRect();

		for (int i = 0; i < countof(pt0.val); i++) {
			rect.pt0[i] = _max(rect1.pt0[i], rect2.pt0[i]);
			rect.pt1[i] = _min(rect1.pt1[i], rect2.pt1[i]);
		}
		*this = rect;
		return !rect.IsRectEmpty();
		//if (rect.IsRectEmpty())
		//	return false;
		//*this = rect;
		//return true;
	}

	// set this rectangle to bounding union of two others
	bool UnionRect(const TRect3<T>& _rect1, const TRect3<T>& _rect2) { 
		TRect3<T> rect, rect1(_rect1), rect2(_rect2);
		rect1.NormalizeRect();
		rect2.NormalizeRect();

		for (int i = 0; i < countof(pt0.val); i++) {
			rect.pt0[i] = _min(rect1.pt0[i], rect2.pt0[i]);
			rect.pt1[i] = _max(rect1.pt1[i], rect2.pt1[i]);
		}
		*this = rect;
		return !rect.IsRectEmpty();
		//if (rect.IsRectEmpty())
		//	return false;
		//*this = rect;
		//return true;
	}

// Additional Operations
	bool operator == (const TRect3<T>& B) const		{ return EqualRect(B); }
	bool operator != (const TRect3<T>& B) const		{ return !EqualRect(B); }
	TRect3& operator += (const TPoint3<T>& pt)		{ OffsetRect(pt); return *this; }
	TRect3& operator -= (const TPoint3<T>& pt)		{ OffsetRect(-pt); return *this; }
	TRect3& operator += (const TSize3<T>& size)		{ OffsetRect(size); return *this;}
	TRect3& operator -= (const TSize3<T>& size)		{ OffsetRect(-size); return *this; }
	TRect3& operator += (const TRect3& rect)		{ return InflateRect(rect); }
	TRect3& operator -= (const TRect3& rect)			{ return DeflateRect(rect); }
	TRect3& operator &= (const TRect3& rect)		{ TRect3<T> rect1(*this); IntersectRect(rect1, rect); return *this; }
	TRect3& operator |= (const TRect3& rect)		{ TRect3<T> rect1(*this); UnionRect(rect1, rect); return *this; }

// Operators returning TRect3 values
	TRect3 operator + (const TPoint3<T>& pt) const	{ TRect3 C(*this); C += pt; return C; }
	TRect3 operator - (const TPoint3<T>& pt) const	{ TRect3 C(*this); C -= pt; return C; }
	TRect3 operator + (const TSize3<T>& size) const	{ TRect3 C(*this); C += size; return C; }
	TRect3 operator - (const TSize3<T>& size) const	{ TRect3 C(*this); C -= size; return C; }
	TRect3 operator + (const TRect3& rect) const	{ TRect3 C(*this); C += rect; return C; }
	TRect3 operator - (const TRect3& rect) const		{ TRect3 C(*this); C -= rect; return C; }
	TRect3 operator & (const TRect3& rect) const	{ TRect3 C; C.IntersectRect(*this, rect); return C; }
	TRect3 operator | (const TRect3& rect) const	{ TRect3 C; C.UnionRect(*this, rect); return C; }

	template < class ARCHIVE > friend ARCHIVE& operator << (ARCHIVE& ar, const TRect3& B) {
		return ar << B.pt0 << B.pt1;
	}
	template < class ARCHIVE > friend ARCHIVE& operator >> (ARCHIVE& ar, TRect3& B) {
		return ar >> B.pt0 >> B.pt1;
	}
	template < class ARCHIVE > friend ARCHIVE& operator || (ARCHIVE& ar, const TRect3& B) {
		return ar || B.pt0 || B.pt1;
	}
};

//-----------------------------------------------------------------------------

template < typename T >
class TRect2 {
public:
	union {
		struct {
			union {
				struct { TPoint2<T> pt0; };
				struct { T left, top; };
			};
			union {
				struct { TPoint2<T> pt1; };
				struct { T right, bottom; };
			};
		};
		T val[4];
	};
	using coord_t = typename decltype(val);

// Constructors
public:
	TRect2() : left(0), top(0), right(0), bottom(0) {}
	TRect2(const TRect2& B) { *this = B; }
	TRect2(const TRect3<T>& B) { *this = B; }
	TRect2(T l, T t, T r, T b) : left(l), top(t), right(r), bottom(b) {}
	TRect2(const TPoint2<T>& pt, const TSize2<T>& size) : pt0(pt), pt1(pt0+size) {}
	TRect2(const TPoint2<T>& _pt0, const TPoint2<T>& _pt1) : pt0(_pt0), pt1(_pt1) {}
	TRect2(const RECT& B) { *this = B; }
	TRect2(POINT point, SIZE size) : pt0(point), pt1(point) { pt1 += size; }
	TRect2(POINT topLeft, POINT bottomRight) : pt0(topLeft), pt1(bottomRight) {}
	TRect2(const T_RECT2<T>& B) { *this = B; }

	template < typename T2 > TRect2(const TRect2<T2>& B) { *this = B; }
	template < typename T2 > TRect2(const TRect3<T2>& B) { *this = B; }
	template < typename T2 > TRect2(T2 l, T2 t, T2 r, T2 b) : left(l), top(t), right(r), bottom(b) {}
	template < typename T2 > TRect2(const TPoint2<T2>& pt, const TSize2<T2>& size) : pt0(pt), pt1(pt0+size) {}
	template < typename T2 > TRect2(const TPoint2<T2>& _pt0, const TPoint2<T2>& _pt1) : pt0(_pt0), pt1(_pt1) {}
	template < typename T2 > TRect2(const cv::Rect_<T2>& B) { *this = B; }

	TRect2& operator = (const TRect2& B) { if (this == &B) return *this; pt0 = B.pt0; pt1 = B.pt1; return *this; }
	TRect2& operator = (const TRect3<T>& B) { pt0 = B.pt0; pt1 = B.pt1; return *this; }
	TRect2& operator = (const RECT& srcRect) { left = srcRect.left; top = srcRect.top; right = srcRect.right; bottom = srcRect.bottom; return *this; }
	TRect2& operator = (const T_RECT2<T>& B) { pt0 = B.pt0; pt1 = B.pt1; return *this; }

	template < typename T2 > TRect2& operator = (const TRect2<T2>& B) { pt0 = B.pt0; pt1 = B.pt1; return *this; }
	template < typename T2 > TRect2& operator = (const TRect3<T2>& B) { pt0 = B.pt0; pt1 = B.pt1; return *this; }
	template < typename T2 > TRect2& operator = (const T_RECT2<T2>& B) { pt0 = B.pt0; pt1 = B.pt1; return *this; }
	template < typename T2 > TRect2& operator = (const cv::Rect_<T2>& B) { pt0 = B.tl(); pt1 = B.br(); return *this; }

// Attributes (in addition to RECT members)

	T Width() const { return right - left; }
	T Height() const { return bottom - top; }

	operator TSize2<T> () const					{ return pt1 - pt0; }
	//operator SIZE () const						{ return (TSize2<int>)*this; }
	operator CSize () const						{ return (TSize2<int>)*this; }
	operator CRect () const						{ return CRect((long)left, (long)top, (long)right, (long)bottom); }
	operator T_RECT2<T>& () 					{ return *(T_RECT2<T>*)this; }
	operator const T_RECT2<T>& () const			{ return *(T_RECT2<T>*)this; }
	template < typename T2 >
	operator cv::Rect_<T2> () const				{ return cv::Rect_<T2>(pt0.x, pt0.y, pt1.x-pt0.x, pt1.y-pt0.y); }

	TPoint2<T>& TopLeft() { return pt0; }
	const TPoint2<T>& TopLeft() const { return pt0; }

	TPoint2<T>& BottomRight() { return pt1; }
	const TPoint2<T>& BottomRight() const { return pt1; }

	TPoint2<T> CenterPoint() const { return (pt0+pt1)/(T)2; }

	TSize2<T> GetSize() const { return (pt1-pt0); }

	//const operator CRect () const { return CRect(left, top, right, bottom); }

	// returns true if rectangle has no area
	bool IsRectEmpty() const {
		for (int i = 0; i < countof(pt0.val); i++) {
			if (pt0[i] >= pt1[i])
				return true;
		}
		return false;
	}
	// returns true if rectangle is at (0,0,0) and has no area
	bool IsRectNull() const {
		for (int i = 0; i < countof(pt0.val); i++) {
			if (pt0[i] != 0.0)
				return false;
			if (pt0[i] != pt1[i])
				return false;
		}
		return true;
	}
	// returns true if point is within rectangle
	bool PtInRect(const TPoint2<T>& pt) const {
		for (int i = 0; i < _min(countof(pt.val), countof(pt0.val)); i++) {
			if ( (pt[i] < pt0[i]) || (pt[i] >= pt1[i]) )
				return false;
		}
		return true;
	}
	// returns true if rect is within rectangle
	bool RectInRect(const TRect2<T>& rect) const {
		for (int i = 0; i < countof(pt0.val); i++) {
			if ( (rect.pt0.val[i] < pt0.val[i]) || (rect.pt0.val[i] > pt1.val[i]) )
				return false;
			if ( (rect.pt1.val[i] < pt0.val[i]) || (rect.pt1.val[i] > pt1.val[i]) )
				return false;
		}
		return true;
	}

// Operations

	// set rectangle from left, top, right, and bottom
	void SetRect(T x1 = 0, T y1 = 0, T x2 = 0, T y2 = 0) { left = x1; top = y1; right = x2; bottom = y2; }
	void SetRect(const TPoint2<T>& pt, const TSize2<T>& size)		{ pt0 = pt; pt1 = pt+size; }
	void SetRect(const TPoint2<T>& _pt0, const TPoint2<T>& _pt1)	{ pt0 = _pt0; pt1 = _pt1; }
	void SetRectEmpty() { pt0.SetZero(); pt1.SetZero(); }
	// copy from another rectangle
	void CopyRect(const TRect2& rectSource) {
		pt0 = rectSource.pt0; pt1 = rectSource.pt1;
	}
	// true if exactly the same as another rectangle
	bool EqualRect(const TRect2& rect) const {
		return (pt0 == rect.pt0) && (pt1 == rect.pt1);
	}

	// infflate the rectangles's width, height and depth
	TRect2& InflateRect(T x, T y) { left -= x; top -= y; right += x; bottom += y; return *this; }
	TRect2& InflateRect(const TSize2<T>& size)  { pt0 -= size; pt1 += size; return *this; }
	TRect2& InflateRect(const TRect2<T>& rect) { pt0 -= rect.pt0; pt1 += rect.pt1; return *this; }
	TRect2& InflateRect(T l, T t, T r, T b) { left -= l; top -= t; right += r; bottom += b; return *this; }

	// deflate the rectangle's width, height and depth
	TRect2& DeflateRect(T x, T y) { left += x; top += y; right -= x; bottom -= y; return *this; }
	TRect2& DeflateRect(const TSize2<T>& size) { pt0 += size; pt1 -= size; return *this; }
	TRect2& DeflateRect(const TRect2<T>& rect) { pt0 += rect.pt0; pt1 -= rect.pt1; return *this; }
	TRect2& DeflateRect(T l, T t, T r, T b)  { left += l; top += t; right -= r; bottom -= b; return *this; }

	// translate the rectangle by moving its top and left
	void OffsetRect(T x, T y) { left += x; top += y; right += x; bottom += y; }
	void OffsetRect(const TSize2<T>& size) { pt0 += size; pt1 += size; }
	void OffsetRect(const TPoint2<T>& pt) { pt0 += pt; pt1 += pt; }

private:
	template < class T > void swap(T& x, T& y) { T c = x; x = y; y = c; }
public:
	void NormalizeRect() {
		for (int i = 0; i < countof(pt0.val); i++) {
			if (pt0.val[i] > pt1.val[i])
				swap(pt0.val[i], pt1.val[i]);
		}
	}

	// absolute position of rectangle
	void MoveToX(T x) { right += (x-left); left = x; }
	void MoveToY(T y) { bottom += (y-top); top = y; }
	void MoveToN(int i, T v) { ASSERT(countof(pt0.val) > i); pt1[i] += (v-pt0[i]); pt0[i] = v; }

	void MoveToXY(T x, T y) { MoveToX(x); MoveToY(y); }

	void MoveTo(const TPoint2<T>& pt) {
		for (int i = 0; i < countof(pt0.val);  i++)
			MoveToN(i, pt[i]);
	}

	// set this rectangle to intersection of two others
	bool IntersectRect(const TRect2<T>& _rect1, const TRect2<T>& _rect2) { 
		TRect2<T> rect, rect1(_rect1), rect2(_rect2);
		rect1.NormalizeRect();
		rect2.NormalizeRect();

		for (int i = 0; i < countof(pt0.val); i++) {
			rect.pt0[i] = _max(rect1.pt0[i], rect2.pt0[i]);
			rect.pt1[i] = _min(rect1.pt1[i], rect2.pt1[i]);
		}
		*this = rect;
		return !rect.IsRectEmpty();
		//if (rect.IsRectEmpty())
		//	return false;
		//*this = rect;
		//return true;
	}

	// set this rectangle to bounding union of two others
	bool UnionRect(const TRect2<T>& _rect1, const TRect2<T>& _rect2) { 
		TRect2<T> rect, rect1(_rect1), rect2(_rect2);
		rect1.NormalizeRect();
		rect2.NormalizeRect();

		for (int i = 0; i < countof(pt0.val); i++) {
			rect.pt0[i] = _min(rect1.pt0[i], rect2.pt0[i]);
			rect.pt1[i] = _max(rect1.pt1[i], rect2.pt1[i]);
		}
		*this = rect;
		return !rect.IsRectEmpty();
		//if (rect.IsRectEmpty())
		//	return false;
		//*this = rect;
		//return true;
	}

// Additional Operations
	bool operator==(const TRect2<T>& B) const		{ return EqualRect(B); }
	bool operator!=(const TRect2<T>& B) const		{ return !EqualRect(B); }
	TRect2& operator += (const TPoint2<T>& pt)		{ OffsetRect(pt); return *this; }
	TRect2& operator -= (const TPoint2<T>& pt)		{ OffsetRect(-pt); return *this; }
	TRect2& operator += (const TSize2<T>& size)		{ OffsetRect(size); return *this;}
	TRect2& operator -= (const TSize2<T>& size)		{ OffsetRect(-size); return *this; }
	TRect2& operator += (const TRect2& rect)		{ return InflateRect(rect); }
	TRect2& operator -= (const TRect2& rect)			{ return DeflateRect(rect); }
	TRect2& operator &= (const TRect2& rect)		{ TRect2 rect1(*this); IntersectRect(rect1, rect); return *this; }
	TRect2& operator |= (const TRect2& rect)		{ TRect2 rect1(*this); UnionRect(rect1, rect); return *this; }

// Operators returning TRect2 values
	TRect2 operator + (const TPoint2<T>& pt) const	{ TRect2 C(*this); C += pt; return C; }
	TRect2 operator - (const TPoint2<T>& pt) const	{ TRect2 C(*this); C -= pt; return C; }
	TRect2 operator + (const TSize2<T>& size) const	{ TRect2 C(*this); C += size; return C; }
	TRect2 operator - (const TSize2<T>& size) const	{ TRect2 C(*this); C -= size; return C; }
	TRect2 operator + (const TRect2& rect) const	{ TRect2 C(*this); C += rect; return C; }
	TRect2 operator - (const TRect2& rect) const	{ TRect2 C(*this); C -= rect; return C; }
	TRect2 operator & (const TRect2& rect) const	{ TRect2 C; C.IntersectRect(*this, rect); return C; }
	TRect2 operator | (const TRect2& rect) const	{ TRect2 C; C.UnionRect(*this, rect); return C; }

	template < class ARCHIVE > friend ARCHIVE& operator << (ARCHIVE& ar, const TRect2& B) {
		return ar << B.pt0 << B.pt1;
	}
	template < class ARCHIVE > friend ARCHIVE& operator >> (ARCHIVE& ar, TRect2& B) {
		return ar >> B.pt0 >> B.pt1;
	}
	template < class ARCHIVE > friend ARCHIVE& operator || (ARCHIVE& ar, const TRect2& B) {
		return ar || B.pt0 || B.pt1;
	}
};

//=============================================================================

//-----------------------------------------------------------------------------
// TSize3
template <> template <>
inline TSize3<int>::TSize3(double _cx, double _cy, double _cz) : cx(_round_int(_cx)), cy(_round_int(_cy)), cz(_round_int(_cz)) {}
// Assign Operator (double -> int)
// Size
template <> template <>
inline TSize3<int>& TSize3<int>::operator = (const TSize2<double>& B)			{ for (int i = 0; i < countof(B.val); i++) val[i] = _round_int(B.val[i]); /*cz = 0;*/ return *this; }
template <> template <>
inline TSize3<int>& TSize3<int>::operator = (const TSize3<double>& B)			{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TSize3<int>& TSize3<int>::operator = (const cv::Size_<double>& B)		{ cx = _round_int(B.width); cy = _round_int(B.height); /*cz = 0;*/ return *this; }
template <> template <>
inline TSize3<int>& TSize3<int>::operator = (const T_SIZE3<double>& B)			{ cx = _round_int(B.cx); cy = _round_int(B.cy); cz = _round_int(B.cz); return *this; }

// Point
template <> template <>
inline TSize3<int>& TSize3<int>::operator = (const TPoint2<double>& B)			{ for (int i = 0; i < countof(B.val); i++) val[i] = _round_int(B.val[i]); /*cz = 0;*/ return *this; }
template <> template <>
inline TSize3<int>& TSize3<int>::operator = (const TPoint3<double>& B)			{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TSize3<int>& TSize3<int>::operator = (const cv::Point_<double>& B)		{ cx = _round_int(B.x); cy = _round_int(B.y); /*cz = 0;*/ return *this; }
template <> template <>
inline TSize3<int>& TSize3<int>::operator = (const cv::Point3_<double>& B)		{ cx = _round_int(B.x); cy = _round_int(B.y); cz = _round_int(B.z); return *this; }

// Type Casting (double -> int)
//template <>
//inline TSize3<double>::operator SIZE () const									{ SIZE size = { (LONG)_round_int(val[0]), (LONG)_round_int(val[1]) }; return size; }
template <>
inline TSize3<double>::operator CSize () const									{ return CSize(_round_int(val[0]), _round_int(val[1])); }
template<> template <>
inline TSize3<double>::operator cv::Size_<int> () const						{ return cv::Size_<int>(_round_int(val[0]), _round_int(val[1])); }


//-----------------------------------------------------------------------------
// TSize2
template <> template <>
inline TSize2<int>::TSize2(double _cx, double _cy) : cx(_round_int(_cx)), cy(_round_int(_cy)) {}
// Assign Operator (double -> int)
// Size
template <> template <>
inline TSize2<int>& TSize2<int>::operator = (const TSize2<double>& B)			{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TSize2<int>& TSize2<int>::operator = (const TSize3<double>& B)			{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TSize2<int>& TSize2<int>::operator = (const cv::Size_<double>& B)		{ cx = _round_int(B.width); cy = _round_int(B.height); return *this; }
template <> template <>
inline TSize2<int>& TSize2<int>::operator = (const T_SIZE2<double>& B)			{ cx = _round_int(B.cx); cy = _round_int(B.cy); return *this; }

// Point
template <> template <>
inline TSize2<int>& TSize2<int>::operator = (const TPoint2<double>& B)			{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TSize2<int>& TSize2<int>::operator = (const TPoint3<double>& B)			{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TSize2<int>& TSize2<int>::operator = (const cv::Point_<double>& B)		{ cx = _round_int(B.x); cy = _round_int(B.y); return *this; }
template <> template <>
inline TSize2<int>& TSize2<int>::operator = (const cv::Point3_<double>& B)		{ cx = _round_int(B.x); cy = _round_int(B.y); return *this; }

// Type Casting (double -> int)
//template <>
//inline TSize2<double>::operator SIZE () const									{ SIZE size = { _round_int(val[0]), _round_int(val[1]) }; return size; }
template <>
inline TSize2<double>::operator CSize () const									{ return CSize(_round_int(val[0]), _round_int(val[1])); }
template <> template <>
inline TSize2<double>::operator cv::Size_<int> () const						{ return cv::Size_<int>(_round_int(val[0]), _round_int(val[1])); }


//-----------------------------------------------------------------------------
// TPoint3
template <> template <>
inline TPoint3<int>::TPoint3(double _x, double _y, double _z) : x(_round_int(_x)), y(_round_int(_y)), z(_round_int(_z)) {}
// Assign Operator (double -> int)
// Size
template <> template <>
inline TPoint3<int>& TPoint3<int>::operator = (const TSize2<double>& B)		{ for (int i = 0; i < countof(B.val); i++) val[i] = _round_int(B.val[i]); /*z = 0;*/ return *this; }
template <> template <>
inline TPoint3<int>& TPoint3<int>::operator = (const TSize3<double>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TPoint3<int>& TPoint3<int>::operator = (const cv::Size_<double>& B)		{ x = _round_int(B.width); y = _round_int(B.height); /*z = 0;*/ return *this; }

// Point
template <> template <>
inline TPoint3<int>& TPoint3<int>::operator = (const TPoint2<double>& B)		{ for (int i = 0; i < countof(B.val); i++) val[i] = _round_int(B.val[i]); /*z = 0;*/ return *this; }
template <> template <>
inline TPoint3<int>& TPoint3<int>::operator = (const TPoint3<double>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TPoint3<int>& TPoint3<int>::operator = (const cv::Point_<double>& B)	{ x = _round_int(B.x); y = _round_int(B.y); /*z = 0;*/ return *this; }
template <> template <>
inline TPoint3<int>& TPoint3<int>::operator = (const cv::Point3_<double>& B)	{ x = _round_int(B.x); y = _round_int(B.y); z = _round_int(B.z); return *this; }
template <> template <>
inline TPoint3<int>& TPoint3<int>::operator = (const T_POINT3<double>& B)		{ x = _round_int(B.x); y = _round_int(B.y); z = _round_int(B.z); return *this; }

// Type Casting
//template <>
//inline TPoint3<double>::operator POINT () const								{ POINT pt = { _round_int(val[0]), _round_int(val[1]) }; return pt; }
template <>
inline TPoint3<double>::operator CPoint () const								{ return CPoint(_round_int(val[0]), _round_int(val[1])); }
template <> template <>
inline TPoint3<double>::operator cv::Point_<int> () const						{ return cv::Point_<int>(_round_int(val[0]), _round_int(val[1])); }
template <> template <>
inline TPoint3<double>::operator cv::Point3_<int> () const						{ return cv::Point3_<int>(_round_int(val[0]), _round_int(val[1]), _round_int(val[2])); }


//-----------------------------------------------------------------------------
// TPoint2
template <> template <>
inline TPoint2<int>::TPoint2(double _x, double _y) : x(_round_int(_x)), y(_round_int(_y)) {}
// Assign Operator
// Size
template <> template <>
inline TPoint2<int>& TPoint2<int>::operator = (const TSize2<double>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TPoint2<int>& TPoint2<int>::operator = (const TSize3<double>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TPoint2<int>& TPoint2<int>::operator = (const cv::Size_<double>& B)	{ x = B.width; y = B.height; return *this; }

// Point
template <> template <>
inline TPoint2<int>& TPoint2<int>::operator = (const TPoint2<double>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TPoint2<int>& TPoint2<int>::operator = (const TPoint3<double>& B)		{ for (int i = 0; i < countof(val); i++) val[i] = _round_int(B.val[i]); return *this; }
template <> template <>
inline TPoint2<int>& TPoint2<int>::operator = (const cv::Point_<double>& B)	{ x = _round_int(B.x); y = _round_int(B.y); return *this; }
template <> template <>
inline TPoint2<int>& TPoint2<int>::operator = (const cv::Point3_<double>& B)	{ x = _round_int(B.x); y = _round_int(B.y); return *this; }
template <> template <>
inline TPoint2<int>& TPoint2<int>::operator = (const T_POINT2<double>& B)		{ x = _round_int(B.x); y = _round_int(B.y); return *this; }

// Type Casting
//template <>
//inline TPoint2<double>::operator POINT () const								{ POINT pt = { _round_int(val[0]), _round_int(val[1]) }; return pt; }
template <>
inline TPoint2<double>::operator CPoint () const								{ return CPoint(_round_int(val[0]), _round_int(val[1])); }
template <> template <>
inline TPoint2<double>::operator cv::Point_<int> () const						{ return cv::Point_<int>(_round_int(val[0]), _round_int(val[1])); }


//-----------------------------------------------------------------------------
// TRect3
template <> template <>
inline TRect3<int>::TRect3(double l, double t, double f, double r, double b, double bk)
	: left(_round_int(l)), top(_round_int(t)), front(_round_int(f)), right(_round_int(r)), bottom(_round_int(b)), back(_round_int(bk)) {}

template <> template <>
inline TRect2<int>::TRect2(double l, double t, double r, double b)
	: left(_round_int(l)), top(_round_int(t)), right(_round_int(r)), bottom(_round_int(b)) {}

//=============================================================================

//#if defined(_OPENCV_VER) //&& (_OPENCV_VER < 0x245)
namespace cv {
	template<> class DataDepth<long> { public: enum { value = CV_32S, fmt=(int)'i' }; };
};
//#endif

//#if defined(_OPENCV_VER) && (_OPENCV_VER >= 0x245)
namespace cv {
	template<typename _Tp> class DataType<TPoint2<_Tp> >
	{
	public:
		typedef TPoint2<_Tp> value_type;
		typedef TPoint2<typename DataType<_Tp>::work_type> work_type;
		typedef _Tp channel_type;
		enum { generic_type = 0, depth = DataDepth<channel_type>::value, channels = 2,
			   fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
			   type = CV_MAKETYPE(depth, channels) };
		typedef Vec<channel_type, channels> vec_type;
	};
	template<typename _Tp> class DataType<TPoint3<_Tp> >
	{
	public:
		typedef TPoint3<_Tp> value_type;
		typedef TPoint3<typename DataType<_Tp>::work_type> work_type;
		typedef _Tp channel_type;
		enum { generic_type = 0, depth = DataDepth<channel_type>::value, channels = 3,
			   fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
			   type = CV_MAKETYPE(depth, channels) };
		typedef Vec<channel_type, channels> vec_type;
	};
	template<typename _Tp> class DataType<TSize2<_Tp> >
	{
	public:
		typedef TSize2<_Tp> value_type;
		typedef TSize2<typename DataType<_Tp>::work_type> work_type;
		typedef _Tp channel_type;
		enum { generic_type = 0, depth = DataDepth<channel_type>::value, channels = 2,
			   fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
			   type = CV_MAKETYPE(depth, channels) };
		typedef Vec<channel_type, channels> vec_type;
	};
	template<typename _Tp> class DataType<TSize3<_Tp> >
	{
	public:
		typedef TSize3<_Tp> value_type;
		typedef TSize3<typename DataType<_Tp>::work_type> work_type;
		typedef _Tp channel_type;
		enum { generic_type = 0, depth = DataDepth<channel_type>::value, channels = 3,
			   fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
			   type = CV_MAKETYPE(depth, channels) };
		typedef Vec<channel_type, channels> vec_type;
	};
	template<typename _Tp> class DataType<TRect2<_Tp> >
	{
	public:
		typedef TRect2<_Tp> value_type;
		typedef TRect2<typename DataType<_Tp>::work_type> work_type;
		typedef _Tp channel_type;
		enum { generic_type = 0, depth = DataDepth<channel_type>::value, channels = 4,
			   fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
			   type = CV_MAKETYPE(depth, channels) };
		typedef Vec<channel_type, channels> vec_type;
	};
	template<typename _Tp> class DataType<TRect3<_Tp> >
	{
	public:
		typedef TRect3<_Tp> value_type;
		typedef TRect3<typename DataType<_Tp>::work_type> work_type;
		typedef _Tp channel_type;
		enum { generic_type = 0, depth = DataDepth<channel_type>::value, channels = 6,
			   fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
			   type = CV_MAKETYPE(depth, channels) };
		typedef Vec<channel_type, channels> vec_type;
	};
};
//#endif

//typedef TSize2<int64_t>		CSize2l, TSizeL;
//typedef TSize3<int64_t>		CSize3l;
typedef TSize2<int32_t>		CSize2i, TSizeI;
typedef TSize3<int32_t>		CSize3i;
typedef TSize2<double>		CSize2d, TSizeD;
typedef TSize3<double>		CSize3d;

//typedef TPoint2<int64_t>	CPoint2l, TPointL;
//typedef TPoint3<int64_t>	CPoint3l;
typedef TPoint2<int32_t>	CPoint2i, TPointI;
typedef TPoint3<int32_t>	CPoint3i;
typedef TPoint2<double>		CPoint2d, TPointD;
typedef TPoint3<double>		CPoint3d;

//typedef TRect2<int64_t>		CRect2l, TRectL;
//typedef TRect3<int64_t>		CRect3l;
typedef TRect2<int32_t>		CRect2i, TRectI;
typedef TRect3<int32_t>		CRect3i;
typedef TRect2<double>		CRect2d, TRectD;
typedef TRect3<double>		CRect3d;

//=============================================================================
// cv::Mat(rectROI)
template <typename T, typename T2>
bool CheckROI(const T_RECT2<T>& r, const T_SIZE2<T2>& size = T_SIZE2<double>(-1, -1)) {
	return 1
		&& (r.left >= 0)
		&& (r.top >= 0)
		&& ( (size.cx < 0) || ( (r.left < size.cx) && (r.right < size.cx) ) )
		&& ( (size.cy < 0) || ( (r.top < size.cy) && (r.bottom < size.cy) ) )
		;
}
inline bool CheckROI(const T_RECT2<double>& r, const T_SIZE2<double>& size = T_SIZE2<double>(-1, -1)) {
	return CheckROI<double, double>(r, size);
}
inline bool CheckROI(const cv::Rect& rc, const cv::Size& size = cv::Size(0, 0)) {
	return 1
		&& ( (rc.x >= 0) && (rc.y >= 0) )
		&& ( (rc.width > 0) && (rc.height > 0) )
		&& ( (size.width < 0) || (rc.x < size.width) && (rc.x+rc.width <= size.width) )
		&& ( (size.height < 0) || (rc.y < size.height) && (rc.y+rc.height <= size.height) )
		;
}

template <typename T, typename T2>
bool AdjustROI(const T_RECT2<T>& rect, T_RECT2<T>& rectAdjusted, const T_SIZE2<T2>& size = T_SIZE2<double>(0, 0)) {
	TRect2<T> r(rect);

	r.NormalizeRect();
	if (r.left < 0)
		r.left = 0;
	if (r.top < 0)
		r.top = 0;
	if ( (size.cx > 0) && (r.right > size.cx) )
		r.right = size.cx;
	if ( (size.cy > 0) && (r.bottom > size.cy) )
		r.bottom = size.cy;

	if (r.IsRectEmpty())
		return false;

	rectAdjusted = r;
	return true;
}
inline bool AdjustROI(const T_RECT2<int>& rect, T_RECT2<int>& rectAdjusted, const T_SIZE2<int>& size = T_SIZE2<int>(0, 0)) {
	return AdjustROI<int, int>(rect, rectAdjusted, size);
}
inline bool AdjustROI(const T_RECT2<double>& rect, T_RECT2<double>& rectAdjusted, const T_SIZE2<double>& size = T_SIZE2<double>(0, 0)) {
	return AdjustROI<double, double>(rect, rectAdjusted, size);
}
inline bool AdjustROI(const cv::Rect& rc, cv::Rect& rcAdjusted, const cv::Size& size = cv::Size(0, 0)) {
	cv::Rect r(rc);

	if (r.width < 0) {
		r.x += r.width;
		r.width = -r.width;
	}
	if (r.height < 0) {
		r.y += r.height;
		r.height = -r.height;
	}

	if (r.x < 0)
		r.x = 0;
	if (r.y < 0)
		r.y = 0;
	if ( (r.x >= size.width) || (r.y >= size.height) )
		return false;
	if ( (size.width > 0) && (r.x + r.width > size.width) )
		r.width = size.width - r.x;
	if ( (size.height > 0) && (r.y + r.height > size.height) )
		r.height = size.height - r.y;
	if ( (size.width <= 0) || (size.height <= 0) )
		return false;

	rcAdjusted = r;
	return true;
}


//=============================================================================
// axis
namespace axis {
	enum eDIRECTION {
		//D_NW = -4, D_NZ, D_NY, D_NX, D_NONE = 0, D_X, D_Y, D_Z, D_W,

		D_NONE = 0,
		D_X =     1, D_NX = -D_X,
		D_Y = D_X+1, D_NY = -D_Y,
		D_Z = D_Y+1, D_NZ = -D_Z,
		D_W = D_Z+1, D_NW = -D_W,

		D_PX = D_X,
		D_PY = D_Y,
		D_PZ = D_Z,
		D_PW = D_W,
	};

	inline bool IsValidCoordSystem(eDIRECTION eDX, eDIRECTION eDY) {
		int iDX = abs(eDX), iDY = abs(eDY);
		return eDX && eDY && (iDX != iDY);
	}
	inline bool IsValidCoordSystem(eDIRECTION eDX, eDIRECTION eDY, eDIRECTION eDZ) {
		int iDX = abs(eDX), iDY = abs(eDY), iDZ = abs(eDZ);
		return eDX && eDY && eDZ && (iDX != iDY) && (iDY != iDZ) && (iDZ != iDX);
	}

	inline eDIRECTION GetOppositeDirection(eDIRECTION eDirection) { return (eDIRECTION)(-eDirection); }

	inline eDIRECTION GetDirection(const char* pszName) {
		signed char eNegative = strchr(pszName, '-') ? -1 : 1;
		if (strchr(pszName, 'x') || strchr(pszName, 'X')) return (eDIRECTION)(D_X*eNegative);
		if (strchr(pszName, 'y') || strchr(pszName, 'Y')) return (eDIRECTION)(D_Y*eNegative);
		if (strchr(pszName, 'z') || strchr(pszName, 'Z')) return (eDIRECTION)(D_Z*eNegative);
		return D_NONE;
	}
	inline eDIRECTION GetDirection(const wchar_t* pszName) {
		signed char eNegative = wcschr(pszName, '-') ? -1 : 1;
		if (wcschr(pszName, 'x') || wcschr(pszName, 'X')) return (eDIRECTION)(D_X*eNegative);
		if (wcschr(pszName, 'y') || wcschr(pszName, 'Y')) return (eDIRECTION)(D_Y*eNegative);
		if (wcschr(pszName, 'z') || wcschr(pszName, 'Z')) return (eDIRECTION)(D_Z*eNegative);
		return D_NONE;
	}
	inline const char* GetDirectionName(eDIRECTION eDirection) {
		switch (eDirection) {
		case D_NONE : return "";
		case D_X  : return  "X"; case  D_Y : return  "Y"; case D_Z  : return  "Z";
		case D_NX : return "-X"; case D_NY : return "-Y"; case D_NZ : return "-Z";
		}
		return "";
	}
};

WARNING_POP()

#pragma pack(pop)
