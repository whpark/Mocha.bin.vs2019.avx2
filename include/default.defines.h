// default.h:
// PWH
//////////////////////////////////////////////////////////////////////

#pragma once


//=================================================================================================================================
// include files. 아래 파일을 stdafx.h 로 복사하면 컴파일 시간을 단축시킬 수 있음.
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <clocale>
#include <cstring>
#include <cwchar>
#include <tchar.h>
//#include <cstdbool>

#include <initializer_list>
#include <exception>
#include <typeinfo>
#include <type_traits>
#include <algorithm>
#include <compare>
#include <tuple>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <string_view>
#include <iterator>
#include <bitset>
#include <map>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <queue>
#include <memory>
#include <atomic>
#include <chrono>
#include <functional>
#include <system_error>
#include <thread>
#include <utility>
#include <mutex>
#include <shared_mutex>
#include <utility>
#include <future>
#include <optional>
#include <filesystem>
#include <random>
#include <numbers>
//#include <tchar.h>

#define _QRS_MOCHA_

//////////////////////////////////////////////////////////////////////


#if defined(_M_X64) || defined(_M_IA64) || defined(_M_AMD64) || defined(_WIN64)
#	define _M_64_ 1
#else
#	undef _M_64_	// NOT _M_64_ 1
#endif

#if _M_64_
#	define _X86_COMPATIBLE_MODE_FOR_IO_			1
#else
#	undef _X86_COMPATIBLE_MODE_FOR_IO_
#endif

#define _X64_FORCE_INT64_NATIVE_				0		// 0 : sizeof(intXX_t, uintXX_t, sizeXX_t) becomes 4 bytes. on Win64.
														// 1 : sizeof(intXX_t, uintXX_t, sizeXX_t) becomes 8 bytes. on Win64.


//#if defined(_M_64_) && defined(_WIN32)
//#	error Delete [WIN32] from Project Settings
//#endif

//====================================================================
// _MSC_VER
// Visual C++ 4.x (4.0)          1000
// Visual C++ 5    (5.0)         1100
// Visual C++ 6    (6.0)         1200
// Visual C++ .NET (7.0)         1300
// Visual C++ .NET 2003 (7.1)    1310
// Visual C++ .NET 2005 (8.0)    1400
// Visual C++ .NET 2008 (9.0)    1500
// Visual C++ 2010 (10.0)        1600
// Visual C++ 2012 (11.0)        1700
// Visual C++ 2013 (12.0)        1800
// Visual C++ 2015 (14.0)        1900
// Visual C++ 2017 (14.1)        1910
#define _MSC_VER_VS4		1000
#define _MSC_VER_VS5		1100
#define _MSC_VER_VS6		1200
#define _MSC_VER_VS2005		1400
#define _MSC_VER_VS2010		1600
#define _MSC_VER_VS2013		1800
#define _MSC_VER_VS2015		1900
#define _MSC_VER_VS2017		1910
#define _MSC_VER_VS2019		1920
#define _MSC_VER_VS2019_v16_3	1923
#define _MSC_VER_VS2022		1930

#if (_MSC_VER < _MSC_VER_VS2019)
#	error ONLY Supports - Visual Studio v2019 -
#endif

//====================================================================
// INT/UINT
#if defined(_M_64_)
#	if (_X64_FORCE_INT64_NATIVE_)
		typedef int64_t				intXX_t;
		typedef uint64_t			uintXX_t;
#	else
		typedef int32_t				intXX_t;
		typedef uint32_t			uintXX_t;
#	endif
#else
	typedef int32_t					intXX_t;
	typedef uint32_t				uintXX_t;
#endif

// size_t
#if defined(_M_64_)
	typedef uint32_t				size32_t;
	typedef size_t					size64_t;
#	if (_X64_FORCE_INT64_NATIVE_)
		typedef size_t				sizeXX_t;
#	else
		typedef uint32_t			sizeXX_t;
#	endif
#else
	typedef size_t					size32_t;
	typedef uint64_t				size64_t;
	typedef size_t					sizeXX_t;
#endif

#ifndef STRUCT_ALIGN
#	define STRUCT_ALIGN(x) __declspec(align(x))
#	define STRUCT_ALIGN_1 STRUCT_ALIGN(1)
#	define STRUCT_ALIGN_8 STRUCT_ALIGN(8)
#endif
#ifndef STRUCT_1
#	define STRUCT_1 __declspec(align(1)) struct
#endif
#ifndef STRUCT_4
#	define STRUCT_4 __declspec(align(4)) struct
#endif
#ifndef STRUCT_8
#	define STRUCT_8 __declspec(align(8)) struct
#endif
#ifndef STRUCT_16
#	define STRUCT_16 __declspec(align(16)) struct
#endif
#ifndef STRUCT_32
#	define STRUCT_32 __declspec(align(32)) struct
#endif

#if (1)
#	define WARNING_DISABLE(id)				__pragma(warning( disable : id ))
#	define WARNING_PUSH()					__pragma(warning( push ))
#	define WARNING_PUSH_AND_DISABLE(id)		__pragma(warning( push )) __pragma(warning( disable : id ))
#	define WARNING_POP()					__pragma(warning(pop))
#else
#	define WARNING_DISABLE(id)
#	define WARNING_PUSH()
#	define WARNING_PUSH_AND_DISABLE(id)
#	define WARNING_POP()
#endif

#ifndef MOCHA_DEPRECATED
#	ifdef _DEBUG
#		define MOCHA_DEPRECATED __declspec(deprecated)
#	else
#		define MOCHA_DEPRECATED
#	endif
#endif

WARNING_DISABLE(4819)

#if _M_64_
#	define ASSERT_M64(x) ASSERT(x)
#else
#	define ASSERT_M64(x)
#endif

#ifndef countof
#	define countof(arr) ( sizeof(arr) / sizeof( (arr)[0] ) )
#endif
#ifndef sizeof_member
#	define sizeof_member(_struct, _member) sizeof( ((_struct*)0)->_member )
#endif
typedef size_t count_t;

//#define FOR_C(iter, var)					for (int iter = 0; iter < countof(var); iter++)
//#define FOR_B(iter, var, var_item)			FOR_C(iter, var) { const auto& var_item = var[iter];
//#define FOR_E()								}


#ifndef ZeroVar
	#define ZeroVar(var) memset(&var, 0, sizeof(var))
#endif
#ifndef ZeroArr
#	define ZeroArr(arr) memset(arr, 0, sizeof(arr))
#endif
#ifndef ZeroAddress
#	define ZeroAddress(arr, size) memset(arr, 0, size)
#endif

#ifndef BOOL
	typedef int BOOL;
#endif

#ifndef FALSE
#	define FALSE 0
#endif

#ifndef TRUE
#	define TRUE 1
#endif

#ifndef _B
#	define _B(x) (false != (x))
#endif


#ifndef NULL
#	define NULL 0
#endif
#if !defined(nullptr) && (_MSC_VER < _MSC_VER_VS1300)	// or 아님.
#	define nullptr NULL
#endif

#ifndef _min
#	define _min(a, b) ( ( (a) < (b) ) ? (a) : (b) )
#endif
#ifndef _max
#	define _max(a, b) ( ( (a) > (b) ) ? (a) : (b) )
#endif
#ifndef min2
#	define min2(a, b) ( ( (a) < (b) ) ? (a) : (b) )
#endif
#ifndef max2
#	define max2(a, b) ( ( (a) > (b) ) ? (a) : (b) )
#endif

#ifndef square
#	define square(x) ((x)*(x))
#endif
#ifndef _sqr
#	define _sqr(x) ((x)*(x))
#endif

#ifndef _round_int
#	define _round_int(v) ((int32_t)((v) + 0.5))
#endif
#ifndef _round_uint
#	define _round_uint(v) ((uint32_t)((v) + 0.5))
#endif
#ifndef _round_int64
#	define _round_int64(v) ((int64_t)((v) + 0.5))
#endif
#ifndef _round_uint64
#	define _round_uint64(v) ((uint64_t)((v) + 0.5))
#endif
#ifndef _round
#	define _round _round_int
#endif
#ifndef __round
#	define __round(v, e) (int32_t)(((v) / (double)(e) + 0.5) * (e))
#endif

#ifndef _M_PI
#	define _M_PI 3.1415926535897932384626433832795028
#endif
#ifndef rad2deg	// radian to degree
#	define rad2deg(x) ((x) * 180. / _M_PI )
#endif
#ifndef deg2rad	// degree to radian
#	define deg2rad(x) ((x) * _M_PI / 180. )
#endif

#ifndef NOT
#	define NOT !
#endif

//-----------------------------------------------------------------------------
#ifndef SAFE_DELETE
#	define SAFE_DELETE(p) { if (p) delete p; p = NULL; }
#endif
#ifndef SAFE_DELETE_ARRAY
#	define SAFE_DELETE_ARRAY(p) { if (p) delete [] p; p = NULL; }
#endif

#ifndef WAIT_FOR_THREAD_AND_CLOSE
#	define WAIT_FOR_THREAD_AND_CLOSE(handle, timeout)\
		if (handle) {\
			WaitForSingleObject(handle, timeout);\
			CloseHandle(handle);\
			handle = NULL;\
		}
#endif

//-----------------------------------------------------------------------------

#ifndef EOL
	#ifdef _UNICODE
		#define EOL L"\r\n"
	#else
		#define EOL "\r\n"
	#endif
#endif

#ifndef EOLW
	#define EOLW L"\r\n"
#endif

#ifndef EOLA
	#define EOLA "\r\n"
#endif

#ifndef BIT
	#define BIT(x) (0x01 << (x))
#endif
#ifndef BIT64
	#define BIT64(x) (0x01ull << (x))
#endif

#define ADJUST_QWORD_ALIGN(w) (((w)+7)/8*8)
#define ADJUST_DWORD_ALIGN(w) (((w)+3)/4*4)
#define ADJUST_WORD_ALIGN(w) (((w)+1)/2*2)

#pragma warning(error: 4715)

//-----------------------------------------------------------------------------

inline void swapo(void* _var, int nSize) {
	uint8_t* var = (uint8_t*)_var;
	for (int i = 0, j = nSize-1; i < j; i++, j--) {
		BYTE c;
		c = var[i];
		var[i] = var[j];
		var[j] = c;
	}
}
template <typename T> void SwapByteOrder(T& v) {
	int nSize = sizeof(v);
	switch (nSize) {
	case sizeof(WORD) :		v = (T)_byteswap_ushort((WORD&)v); break;
	case sizeof(DWORD) :	v = (T)_byteswap_ulong((DWORD&)v); break;
	case sizeof(uint64_t) :	v = (T)_byteswap_uint64((uint64_t&)v); break;
	default :				swapo(&v, nSize); break;
	}
}
#define swapos _byteswap_ushort
#define swapol _byteswap_ulong
#define swapoll _byteswap_uint64

inline void SwapByte(int16_t& value)  { value = _byteswap_ushort(value); }
inline void SwapByte(int32_t& value)  { value = _byteswap_ulong(value); }
inline void SwapByte(int64_t& value)  { value = _byteswap_uint64(value); }
inline void SwapByte(uint16_t& value) { value = _byteswap_ushort(value); }
inline void SwapByte(uint32_t& value) { value = _byteswap_ulong(value); }
inline void SwapByte(uint64_t& value) { value = _byteswap_uint64(value); }

inline int16_t  GetByteSwap(int16_t value)  { return _byteswap_ushort(value); }
inline int32_t  GetByteSwap(int32_t value)  { return _byteswap_ulong(value); }
inline int64_t  GetByteSwap(int64_t value)  { return _byteswap_uint64(value); }
inline uint16_t GetByteSwap(uint16_t value) { return _byteswap_ushort(value); }
inline uint32_t GetByteSwap(uint32_t value) { return _byteswap_ulong(value); }
inline uint64_t GetByteSwap(uint64_t value) { return _byteswap_uint64(value); }


//-----------------------------------------------------------------------------
// IP / MAC
//
#pragma pack(push, 1)
//WARNING_PUSH_AND_DISABLE(4359)
typedef struct T_IP_ADDRESS {
	union {
		struct { uint8_t b0, b1, b2, b3; };	// 만들고 보니... Big Endian 으로 만들어버림.
		uint8_t addr[4];
		uint32_t ip;
	};
	T_IP_ADDRESS(uint8_t _b0 = 0, uint8_t _b1 = 0, uint8_t _b2 = 0, uint8_t _b3 = 0) : b0(_b0), b1(_b1), b2(_b2), b3(_b3) { }
	template < typename TCHAR >
	BOOL SetFromString(const TCHAR* pszIP) {
		for (int i = 0; i < countof(addr); i++) {
			TCHAR* pszEnd = NULL;
			if (sizeof(TCHAR) == sizeof(char))
				addr[i] = (BYTE)strtoul((const char*)pszIP, (char**)&pszEnd, 10);
			else
				addr[i] = (BYTE)wcstoul((const wchar_t*)pszIP, (wchar_t**)&pszEnd, 10);
			if ( (i < countof(addr)-1) && (*pszEnd != '.') )
				return FALSE;
			if (!*pszEnd)
				break;
			pszIP = pszEnd+1;
		}
		return TRUE;
	}
#ifdef _MFC_VER
	CStringA ToStringA() const { CStringA str; str.Format(   "%d.%d.%d.%d",  b0, b1, b2, b3); return str; }
	CStringW ToStringW() const { CStringW str; str.Format(  L"%d.%d.%d.%d",  b0, b1, b2, b3); return str; }
	CString ToString()	 const { CString  str; str.Format(_T("%d.%d.%d.%d"), b0, b1, b2, b3); return str; }
	BOOL SetFromString(const CStringA& str) { return SetFromString((LPCSTR)str); }
	BOOL SetFromString(const CStringW& str) { return SetFromString((LPCWSTR)str); }
#endif
	BOOL operator == (const T_IP_ADDRESS& B) const { return ip == B.ip; }
	BOOL operator != (const T_IP_ADDRESS& B) const { return ip != B.ip; }
} T_IP_ADDRESS;
//WARNING_POP()

typedef struct T_MAC_ADDRESS {
	union {
		struct { uint8_t b0, b1, b2, b3, b4, b5; };
		uint8_t addr[6];
	};
	T_MAC_ADDRESS(uint8_t _b0 = 0, uint8_t _b1 = 0, uint8_t _b2 = 0, uint8_t _b3 = 0, uint8_t _b4 = 0, uint8_t _b5 = 0) : b0(_b0), b1(_b1), b2(_b2), b3(_b3), b4(_b4), b5(_b5) { }
	template < typename TCHAR >
	BOOL SetFromString(const TCHAR* pszMAC) {
		for (int i = 0; i < countof(addr); i++) {
			TCHAR* pszEnd = NULL;
			addr[i] = (BYTE)_tcstoul(pszMAC, &pszEnd, 16);
			if ( (i < countof(addr)-1) && (*pszEnd != ':') )
				return FALSE;
			if (!*pszEnd)
				break;
			pszMAC = pszEnd+1;
		}
		return TRUE;
	}
#ifdef _MFC_VER
	CStringA ToStringA() const { CStringA str; str.Format(   "%02X:%02X:%02X:%02X:%02X:%02X",  b0, b1, b2, b3, b4, b5); return str; }
	CStringW ToStringW() const { CStringW str; str.Format(  L"%02X:%02X:%02X:%02X:%02X:%02X",  b0, b1, b2, b3, b4, b5); return str; }
	CString  ToString()	 const { CString  str; str.Format(_T("%02X:%02X:%02X:%02X:%02X:%02X"), b0, b1, b2, b3, b4, b5); return str; }
#endif
	BOOL operator == (const T_MAC_ADDRESS& B) const { return memcmp(addr, B.addr, sizeof(addr)) == 0; }
	BOOL operator != (const T_MAC_ADDRESS& B) const { return memcmp(addr, B.addr, sizeof(addr)) != 0; }
} T_MAC_ADDRESS;

typedef struct T_IP_MAC_ADDRESS {
	T_IP_ADDRESS ip;
	T_MAC_ADDRESS mac;

	T_IP_MAC_ADDRESS(const T_IP_ADDRESS& _ip = T_IP_ADDRESS(), const T_MAC_ADDRESS& _mac = T_MAC_ADDRESS()) : ip(_ip), mac(_mac) {  }
} T_IP_MAC_ADDRESS;
#pragma pack(pop)

//-----------------------------------------------------------------------------
// POINT, SIZE, RECT
#pragma pack(push, 8)

template < typename T > class __declspec(novtable) T_POINT2 {
public:
	union { struct { T x, y; }; T val[2]; };
	T_POINT2(T _x = 0, T _y = 0) : x(_x), y(_y) {}
	using coord_t = typename decltype(val);

	bool operator == (const T_POINT2& B) const { return memcmp(val, B.val, sizeof(val)) == 0; }
	bool operator != (const T_POINT2& B) const { return !(operator == (B)); }
};
template < typename T > class __declspec(novtable) T_POINT3 {
public:
	union { struct { T x, y, z; }; T val[3]; };
	using coord_t = typename decltype(val);

	T_POINT3(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) {}

	bool operator == (const T_POINT3& B) const { return memcmp(val, B.val, sizeof(val)) == 0; }
	bool operator != (const T_POINT3& B) const { return !(operator == (B)); }
};
template < typename T > class __declspec(novtable) T_SIZE2 {
public:
	union { struct { T cx, cy; }; T val[2]; };
	using coord_t = typename decltype(val);

	T_SIZE2(T _cx = 0, T _cy = 0) : cx(_cx), cy(_cy) {}

	bool operator == (const T_SIZE2& B) const { return memcmp(val, B.val, sizeof(val)) == 0; }
	bool operator != (const T_SIZE2& B) const { return !(operator == (B)); }
};
template < typename T > class __declspec(novtable) T_SIZE3 {
public:
	union { struct { T cx, cy, cz; }; T val[3]; };
	using coord_t = typename decltype(val);

	T_SIZE3(T _cx = 0, T _cy = 0, T _cz = 0) : cx(_cx), cy(_cy), cz(_cz) {}

	bool operator == (const T_SIZE3& B) const { return memcmp(val, B.val, sizeof(val)) == 0; }
	bool operator != (const T_SIZE3& B) const { return !(operator == (B)); }
};
template < typename T >
class __declspec(novtable) T_RECT2 {
public:
	union {
		struct {
			union {
				struct { T_POINT2<T> pt0; };
				struct { T left, top; };
			};
			union {
				struct { T_POINT2<T> pt1; };
				struct { T right, bottom; };
			};
		};
		T val[4];
	};
	using coord_t = typename decltype(val);

	T_RECT2(T l = 0, T t = 0, T r = 0, T b = 0) : left(l), top(t), right(r), bottom(b) {}
	T_RECT2(const T_POINT2<T>& pt, const T_SIZE2<T>& size) : pt0(pt), pt1(pt0+size) {}
	T_RECT2(const T_POINT2<T>& _pt0, const T_POINT2<T>& _pt1) : pt0(_pt0), pt1(_pt1) {}

	bool operator == (const T_RECT2& B) const { return memcmp(val, B.val, sizeof(val)) == 0; }
	bool operator != (const T_RECT2& B) const { return !(operator == (B)); }
};
template < typename T >
class __declspec(novtable) T_RECT3 {
public:
	union {
		struct {
			union {
				struct { T_POINT3<T> pt0; };
				struct { T left, top, front; };
			};
			union {
				struct { T_POINT3<T> pt1; };
				struct { T right, bottom, back; };
			};
		};
		T val[6];
	};
	using coord_t = typename decltype(val);

	T_RECT3(T l = 0, T t = 0, T f = 0, T r = 0, T b = 0, T bk = 0) : left(l), top(t), front(f), right(r), bottom(b), back(bk) {}
	T_RECT3(const T_POINT3<T>& pt, const T_SIZE3<T>& size) : pt0(pt), pt1(pt0+size) {}
	T_RECT3(const T_POINT3<T>& _pt0, const T_POINT3<T>& _pt1) : pt0(_pt0), pt1(_pt1) {}

	bool operator == (const T_RECT3& B) const { return memcmp(val, B.val, sizeof(val)) == 0; }
	bool operator != (const T_RECT3& B) const { return !(operator == (B)); }
};

typedef T_SIZE2<int32_t>  SIZE2I;
typedef T_SIZE3<int32_t>  SIZE3I;
typedef T_SIZE2<double>   SIZE2D;
typedef T_SIZE3<double>   SIZE3D;
typedef T_POINT2<int32_t> POINT2I;
typedef T_POINT3<int32_t> POINT3I;
typedef T_POINT2<double>  POINT2D;
typedef T_POINT3<double>  POINT3D;
typedef T_RECT2<int32_t>  RECT2I;
typedef T_RECT3<int32_t>  RECT3I;
typedef T_RECT2<double>   RECT2D;
typedef T_RECT3<double>   RECT3D;


//-----------------------------------------------------------------------------
// Archive <---------> TPoint2/3, TSize2/3, TRect2/3, T_POINT2/3, T_SIZE2/3, T_RECT2/3
//
template < typename Archive, class T_COORD, class = T_COORD::coord_t >
Archive& Coord2Archive(const T_COORD& coord, Archive& ar) {
	for (int i = 0; i < countof(coord.val); i++)
		ar << coord.val[i];

	return ar;
}

template < typename Archive, class T_COORD, class = T_COORD::coord_t >
Archive& Archive2Coord(Archive& ar, T_COORD& coord) {
	for (int i = 0; i < countof(coord.val); i++)
		ar >> coord.val[i];

	return ar;
}

template < typename Archive, class T_COORD, class = T_COORD::coord_t >
Archive& SerializeCoord(Archive& ar, T_COORD& coord) {
	return ar.IsStoring() ? Coord2Archive(coord, ar) : Archive2Coord(ar, coord);
}

#pragma pack(pop)

//-----------------------------------------------------------------------------


#ifndef QUOTE
#define _QUOTE(x) x
#define QUOTE(x) _QUOTE(x)
#endif
#ifndef LQUOTE
#define _LQUOTE(x) L ## x
#define LQUOTE(x) _LQUOTE(x)
#endif

#define CONV_A(x)		((LPCSTR)(CStringA(x)))
#define CONV_W(x)		((LPCWSTR)(CStringW(x)))
#define CONV_T(x)		((LPCTSTR)(CString(x)))

#ifndef RGBA
#	define RGBA(r,g,b,a)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))| (((((DWORD)(BYTE)(b)) << 16)|(((DWORD)(BYTE)(a)) << 24)))))
#endif
typedef union _rgba {
	struct { BYTE r, g, b, a; };
	COLORREF cr;

	_rgba(BYTE _r, BYTE _g, BYTE _b, BYTE _a) : r(_r), g(_g), b(_b), a(_a) {}
} T_RGBA;

inline COLORREF GetRGBA(BYTE r, BYTE g, BYTE b, BYTE a) {
	return T_RGBA(r, g, b, a).cr;
}

#if (_MSC_VER <= _MSC_VER_VS6)
	#define __FILE__LINE__ _T(__FILE__) _T(":(") _T(QUOTE(__LINE__)) _T(")")
	#define __FUNCTION__ __FILE__LINE__
	#define __TFUNCTION__ __FUNCTION__
#else
	#define __FILE__LINE__ _T(__FILE__) _T(":(") _T(QUOTE(__LINE__)) _T(")")
	#define __TFUNCTION__ _T(__FUNCTION__)
#endif

#define __TFMSG(msg) __TFUNCTION__ _T(" - ") _T(msg)
#define __WFMSG(msg) LQUOTE(__FUNCTION__) L" - " L ## msg
#define __FMSG(msg) __FUNCTION__ " - " msg

#ifndef UNICODE_PAGE1200_MARK
#	define UNICODE_PAGE1200_MARK (0xFEFF)		// Unicode Little Endian
#endif

#ifndef UNICODE_PAGE1201_MARK
#	define UNICODE_PAGE1201_MARK (0xFFFE)		// Unicode BIG Endian
#endif

#ifndef UTF8_MARK
#	define UTF8_MARK "\xEF\xBB\xBF"
#endif
#ifndef UTF8_BOM
#	define UTF8_BOM "\xEF\xBB\xBF"
#endif

#ifndef INT32_MAX
#	define INT32_MAX 2147483647i32
#endif

#define EXT_DEFAULT_IMAGE_FILE	_T(".png")
#define FILTER_IMAGE_FILES		_T("Image Files(*.bmp;*.jpg;*.tiff;*.png)|*.bmp;*.jpg;*.tiff;*.png|Bitmap(*.bmp)|*.bmp|PNG File(*.png)|*.png|JPEG File(*.jpg)|*.jpg|All Files(*.*)|*.*||")
#define IMAGE_FILE_EXTS			_T("*.bmp;*.jpg;*.jpeg;*.tiff;*.png;*.gif")

#define INVALID_CHAR_FOR_FILE_NAME _T("\\/:*?<>|")


//-----------------------------------------------------------------------------
// Exception

#ifndef __trace
#	ifdef _MFC_VER
#		define __trace TRACE
#	else
#		define __trace(...)
#	endif
#endif


#ifdef _MFC_VER
#pragma pack(push, 8)
class __ExceptionBase__ {
public:
	CString m_strFunctionName;
	CString m_strReason;
public:
	__ExceptionBase__(const char* pszPosition, const char* pszMessage) : m_strFunctionName(pszPosition), m_strReason(pszMessage) {
		__trace("//==============================================================\n"
				"//  EXCEPTION\n"
				"//\n"
				"//    position : %s\n"
				"//    reasion  : %s\n"
				"//\n"
				"//\n",
				pszPosition, pszMessage);
	}
	__ExceptionBase__(const wchar_t* pszPosition, const wchar_t* pszMessage) : m_strFunctionName(pszPosition), m_strReason(pszMessage) {
		__trace(L"//==============================================================\n"
				L"//  EXCEPTION\n"
				L"//\n"
				L"//    position : %s\n"
				L"//    reasion  : %s\n"
				L"//\n"
				L"//\n",
				pszPosition, pszMessage);
	}
};
#pragma pack(pop)
#define __throw_exception__(msg) throw __ExceptionBase__(__FUNCTION__, msg)
#endif


//-----------------------------------------------------------------------------
// Copy Clone
//
// DECLARE_NEWCLONE_BASE			: Declare Baseclass for virtual NewClone
// DECLARE_NEWCLONE_BASE_INTERFACE	: Declare Baseclass for virtual NewClone ( = NULL)
// DECLARE_NEWCLONE					: Declare virtual NewClone
//
#ifndef DECLARE_NEWCLONE_BASE
#	define DECLARE_NEWCLONE_BASE(className) \
	typedef className* TP_BASE_CLASS;\
	virtual TP_BASE_CLASS NewClone() const { return new className(*this); }
#endif
#ifndef DECLARE_NEWCLONE_BASE_INTERFACE
#	define DECLARE_NEWCLONE_BASE_INTERFACE(className) \
	typedef className* TP_BASE_CLASS;\
	virtual TP_BASE_CLASS NewClone() const = NULL;
#endif
#ifndef DECLARE_NEWCLONE
#	define DECLARE_NEWCLONE(className) virtual TP_BASE_CLASS NewClone() const { return new className(*this); }
#endif


//-----------------------------------------------------------------------------
// Dynamic Create. (Creating with IDENTIFIER)
//
// DECLARE_DYNAMIC_CREATE_BASE
// DECLARE_DYNAMIC_CREATE_BASE_INTERFACE
// IMPLEMENT_DYNAMIC_CREATE_BASE
//
// DECLARE_DYNAMIC_CREATE
// IMPLEMENT_DYNAMIC_CREATE
//

// 2013.12.05. s_DYNAMIC_CREATE 변수의 위치를 CDYNAMIC_CREATE의 멤버변수로 두었다가 BaseClass 의 멤버로 옮김. DLL 로 만들 때, CLASS 내부의 static 변수는 Export 되지 않음.
#ifndef _DECLARE_DYNAMIC_CREATE_BASE
#	define _DECLARE_DYNAMIC_CREATE_BASE(className, TYPE_IDENTIFIER, TYPE_IDENTIFIER_SERIALIZE_AS)\
		typedef TYPE_IDENTIFIER T_TYPE_IDENTIFIER;\
		typedef TYPE_IDENTIFIER_SERIALIZE_AS T_TYPE_IDENTIFIER_SERIALIZE_AS;\
		typedef className T_BASE_CLASS;\
		class CDYNAMIC_CREATE {\
		public:\
			T_TYPE_IDENTIFIER identifier;\
			CDYNAMIC_CREATE(T_TYPE_IDENTIFIER _identifier = (T_TYPE_IDENTIFIER)0) : identifier(_identifier) { T_BASE_CLASS::s_DYNAMIC_CREATE.reserve(100); }\
			virtual T_BASE_CLASS* NewObject() const = NULL;\
		};\
		static std::vector<CDYNAMIC_CREATE*> s_DYNAMIC_CREATE;\
		static T_BASE_CLASS* DYNAMIC_CREATE_NewObject(T_TYPE_IDENTIFIER identifier) {\
			auto rbegin = s_DYNAMIC_CREATE.rbegin(), rend = s_DYNAMIC_CREATE.rend();\
			for (auto iter = rbegin; iter != rend; iter++) {\
				if (T_TYPE_IDENTIFIER_SERIALIZE_AS((*iter)->identifier) == identifier) {\
					T_BASE_CLASS* pNewObject = (*iter)->NewObject();\
					return pNewObject;\
				}\
			}\
			return NULL;\
		}\
		static void DYNAMIC_CREATE_AddClass(CDYNAMIC_CREATE* pObjectBase) {\
			s_DYNAMIC_CREATE.push_back(pObjectBase);\
		}
#endif

#ifndef DECLARE_DYNAMIC_CREATE_BASE
#	define DECLARE_DYNAMIC_CREATE_BASE(className, TYPE_IDENTIFIER, TYPE_IDENTIFIER_SERIALIZE_AS, identifier)\
		_DECLARE_DYNAMIC_CREATE_BASE(className, TYPE_IDENTIFIER, TYPE_IDENTIFIER_SERIALIZE_AS)\
		DECLARE_DYNAMIC_CREATE(className, identifier)
#endif


#ifndef DECLARE_DYNAMIC_CREATE_BASE_INTERFACE
#	define DECLARE_DYNAMIC_CREATE_BASE_INTERFACE(className, TYPE_IDENTIFIER, TYPE_IDENTIFIER_SERIALIZE_AS)\
		_DECLARE_DYNAMIC_CREATE_BASE(className, TYPE_IDENTIFIER, TYPE_IDENTIFIER_SERIALIZE_AS)\
		DECLARE_DYNAMIC_CREATE_INTERFACE(className)
#endif

#ifndef IMPLEMENT_DYNAMIC_CREATE_BASE
#	define IMPLEMENT_DYNAMIC_CREATE_BASE(className)\
		std::vector<className::CDYNAMIC_CREATE*> className::s_DYNAMIC_CREATE;
#endif

#ifndef DECLARE_DYNAMIC_CREATE
#	define DECLARE_DYNAMIC_CREATE(className, identifier)\
		class CDYNAMIC_CREATE_CHILD : public T_BASE_CLASS::CDYNAMIC_CREATE {\
		public:\
			CDYNAMIC_CREATE_CHILD() : T_BASE_CLASS::CDYNAMIC_CREATE(identifier) {\
				TRACE("DYNAMIC_CREATE : " #className "\r\n");\
				T_BASE_CLASS::s_DYNAMIC_CREATE.push_back(this);\
			}\
			virtual T_BASE_CLASS* NewObject() const { return new className; }\
		};\
		static CDYNAMIC_CREATE_CHILD s_DYNAMIC_CREATE_CHILD;\
		virtual T_TYPE_IDENTIFIER DYNAMIC_CREATE_GetIdentifer() const { return identifier; }
#endif

#ifndef DECLARE_DYNAMIC_CREATE_INTERFACE
#	define DECLARE_DYNAMIC_CREATE_INTERFACE(className)\
		class CDYNAMIC_CREATE_CHILD : public T_BASE_CLASS::CDYNAMIC_CREATE {\
		public:\
			CDYNAMIC_CREATE_CHILD () : T_BASE_CLASS::CDYNAMIC_CREATE(identifier) {\
				T_BASE_CLASS::s_DYNAMIC_CREATE.push_back(this);\
			}\
			virtual T_BASE_CLASS* NewObject() const = NULL;\
		};\
		virtual T_TYPE_IDENTIFIER DYNAMIC_CREATE_GetIdentifer() const = NULL;
#endif

#ifndef IMPLEMENT_DYNAMIC_CREATE
#	define IMPLEMENT_DYNAMIC_CREATE(className)\
		className::CDYNAMIC_CREATE_CHILD className::s_DYNAMIC_CREATE_CHILD;
#endif

//-----------------------------------------------------------------------------
// Dynamic Serializing. (Load/Store with IDENTIFIER)
//		(DYNAMIC_CREATE 가 먼저 선언 되어 있어야 함.
// DECLARE_DYNAMIC_SERIALIZE_BASE
// IMPLEMENT_DYNAMIC_SERIALIZE_BASE
//
// DECLARE_DYNAMIC_SERIALIZE
// IMPLEMENT_DYNAMIC_SERIALIZE
//
#ifndef DECLARE_DYNAMIC_SERIALIZE_BASE
#	define DECLARE_DYNAMIC_SERIALIZE_BASE(className)\
		template < class ARCHIVE >\
		static className* SerializeDynamic(ARCHIVE& ar, className* pObject) {\
			if (ar.IsStoring()) {\
				if (!pObject)\
					__throw_exception__("Cannot store NULL object");\
				ar << T_TYPE_IDENTIFIER_SERIALIZE_AS(pObject->DYNAMIC_CREATE_GetIdentifer());\
				pObject->Serialize(ar);\
				return NULL;\
			} else {\
				T_TYPE_IDENTIFIER_SERIALIZE_AS var;\
				ar >> var;\
				pObject = DYNAMIC_CREATE_NewObject((T_TYPE_IDENTIFIER)var);\
				if (!pObject)\
					__throw_exception__("Unknown Object");\
				pObject->Serialize(ar);\
				return pObject;\
			}\
			return NULL;\
		}
#endif

#ifndef IMPLEMENT_DYNAMIC_SERIALIZE_BASE
#	define IMPLEMENT_DYNAMIC_SERIALIZE_BASE(className)
#endif

#ifndef DECLARE_DYNAMIC_SERIALIZE
#	define DECLARE_DYNAMIC_SERIALIZE(className)\
		template < class ARCHIVE >\
		static className* SerializeDynamic(ARCHIVE& ar, className* pObject) {\
			if (ar.IsStoring()) {\
				if (!pObject)\
					__throw_exception__("Cannot store NULL object");\
				ar << T_TYPE_IDENTIFIER_SERIALIZE_AS(pObject->DYNAMIC_CREATE_GetIdentifer());\
				pObject->Serialize(ar);\
				return NULL;\
			} else {\
				T_TYPE_IDENTIFIER_SERIALIZE_AS var;\
				ar >> var;\
				pObject = (className*)DYNAMIC_CREATE_NewObject((T_TYPE_IDENTIFIER)var);\
				if (!pObject)\
					__throw_exception__("Unknown Object");\
				pObject->Serialize(ar);\
				return pObject;\
			}\
			return NULL;\
		}
#endif

#ifndef IMPLEMENT_DYNAMIC_SERIALIZE
#	define IMPLEMENT_DYNAMIC_SERIALIZE(className)
#endif


//-----------------------------------------------------------------------------
// enum helper
template < typename T, typename eT >	T& enum_as(eT& e)	{ ASSERT(sizeof(eT) == sizeof(T)); return (T&)e; }
template < typename eT >				int inc_enum(eT& e)	{ ASSERT(sizeof(eT) == sizeof(int)); return ((int&)e)++; }
template < typename eT >				int dec_enum(eT& e)	{ ASSERT(sizeof(eT) == sizeof(int)); return ((int&)e)--; }


//-----------------------------------------------------------------------------
// unit

// length - mm
#if (_MSC_VER >= _MSC_VER_VS2015)

class length_mm_t {	// represents length in mm
public:
	double dValue = 0.0;

	length_mm_t(double v) : dValue(v) { }

	operator double& () { return dValue; };
	operator double () const { return dValue; };

	auto operator <=> (const length_mm_t& ) const = default;
	auto operator <=> (double b) const { return dValue <=> b; }
};
inline length_mm_t operator "" _nm (long double v)					{ return length_mm_t(v * 1.e-6); }
inline length_mm_t operator "" _um (long double v)					{ return length_mm_t(v * 1.e-3); }
inline length_mm_t operator "" _mm (long double v)					{ return length_mm_t(v); }
inline length_mm_t operator "" _cm (long double v)					{ return length_mm_t(v * 1.e1); }
inline length_mm_t operator ""  _m (long double v)					{ return length_mm_t(v * 1.e3); }
inline length_mm_t operator "" _km (long double v)					{ return length_mm_t(v * 1.e6); }
inline length_mm_t operator "" _nm (unsigned long long v)			{ return length_mm_t(v * 1.e-6); }
inline length_mm_t operator "" _um (unsigned long long v)			{ return length_mm_t(v * 1.e-3); }
inline length_mm_t operator "" _mm (unsigned long long v)			{ return length_mm_t((double)v); }
inline length_mm_t operator "" _cm (unsigned long long v)			{ return length_mm_t(v * 1.e1); }
inline length_mm_t operator ""  _m (unsigned long long v)			{ return length_mm_t(v * 1.e3); }
inline length_mm_t operator "" _km (unsigned long long v)			{ return length_mm_t(v * 1.e6); }

inline unsigned long long operator "" _KB (unsigned long long v)	{ return v * 1024; }
inline unsigned long long operator "" _MB (unsigned long long v)	{ return v * 1024_KB; }
inline unsigned long long operator "" _GB (unsigned long long v)	{ return v * 1024_MB; }

using mm_t = length_mm_t;

// Percent - %
inline double operator "" _percent (long double v)					{ return double(v * 1.e-2); }
inline double operator "" _percent (unsigned long long v)			{ return double(v * 1.e-2); }

// angle - rad, deg
class angle_deg_t;
class angle_rad_t;
class __declspec(novtable) angle_rad_t {	// represents angle in rad
public:
	double dValue = 0.0;

	angle_rad_t() = default;
	angle_rad_t(const angle_rad_t&) = default;
	angle_rad_t(angle_rad_t&&) = default;
	angle_rad_t(const angle_deg_t& b);
	angle_rad_t(double v) : dValue(v) { }

	operator double& () { return dValue; };
	operator double () const { return dValue; };
	//operator angle_deg_t() const;
	angle_rad_t& operator = (const angle_rad_t&) = default;
	angle_rad_t& operator = (const double& b) { dValue = b; return *this; }
	angle_rad_t& operator = (const angle_deg_t& b);
	angle_rad_t operator - () const { return angle_rad_t(-dValue); }
	auto operator <=> (const angle_rad_t&) const = default;
	auto operator <=> (const angle_deg_t& deg) const { return *this <=> (angle_rad_t)deg; }
	auto operator <=> (double rad) const { return dValue <=> rad; }

	angle_rad_t operator + (const angle_rad_t& b) const { return angle_rad_t(dValue + b.dValue); }
	angle_rad_t operator - (const angle_rad_t& b) const { return angle_rad_t(dValue - b.dValue); }
	angle_rad_t operator * (const angle_rad_t& b) const { return angle_rad_t(dValue * b.dValue); }
	angle_rad_t operator / (const angle_rad_t& b) const { return angle_rad_t(dValue / b.dValue); }
	angle_rad_t& operator += (const angle_rad_t& b) { dValue += b.dValue; return *this; }
	angle_rad_t& operator -= (const angle_rad_t& b) { dValue -= b.dValue; return *this; }
	angle_rad_t& operator *= (const angle_rad_t& b) { dValue *= b.dValue; return *this; }
	angle_rad_t& operator /= (const angle_rad_t& b) { dValue /= b.dValue; return *this; }

	angle_rad_t operator + (const angle_deg_t& b) const;
	angle_rad_t operator - (const angle_deg_t& b) const;
	angle_rad_t operator * (const angle_deg_t& b) const;
	angle_rad_t operator / (const angle_deg_t& b) const;
	angle_rad_t& operator += (const angle_deg_t& b);
	angle_rad_t& operator -= (const angle_deg_t& b);
	angle_rad_t& operator *= (const angle_deg_t& b);
	angle_rad_t& operator /= (const angle_deg_t& b);

	angle_rad_t operator + (const double b) const;
	angle_rad_t operator - (const double b) const;
	angle_rad_t operator * (const double b) const;
	angle_rad_t operator / (const double b) const;
	angle_rad_t& operator += (const double b);
	angle_rad_t& operator -= (const double b);
	angle_rad_t& operator *= (const double b);
	angle_rad_t& operator /= (const double b);

	static angle_rad_t atan(double v)				{ return angle_rad_t(::atan(v)); }
	static angle_rad_t atan2(double y, double x)	{ return angle_rad_t(::atan2(y, x)); }
	static angle_rad_t asin(double v)				{ return angle_rad_t(::asin(v)); }
	static angle_rad_t acos(double v)				{ return angle_rad_t(::acos(v)); }
};
class __declspec(novtable) angle_deg_t {	// represents angle in deg
public:
	double dValue = 0.0;

	angle_deg_t() = default;
	angle_deg_t(const angle_deg_t&) = default;
	angle_deg_t(angle_deg_t&&) = default;
	angle_deg_t(const angle_rad_t& b);
	angle_deg_t(double v) : dValue(v) { }

	operator double& () { return dValue; };
	operator double () const { return dValue; };
	//operator angle_rad_t() const;
	angle_deg_t& operator = (const angle_deg_t&) = default;
	angle_deg_t& operator = (const double& b) { dValue = b; return *this; }
	angle_deg_t& operator = (const angle_rad_t& b);
	angle_deg_t operator - () const { return angle_deg_t(-dValue); }
	auto operator <=> (const angle_deg_t&) const = default;
	auto operator <=> (const angle_rad_t& deg) const { return *this <=> (angle_deg_t)deg; }
	auto operator <=> (double deg) const { return dValue <=> deg; }
	angle_deg_t operator + (const angle_deg_t& b) const { return angle_deg_t(dValue + b.dValue); }
	angle_deg_t operator - (const angle_deg_t& b) const { return angle_deg_t(dValue - b.dValue); }
	angle_deg_t operator * (const angle_deg_t& b) const { return angle_deg_t(dValue * b.dValue); }
	angle_deg_t operator / (const angle_deg_t& b) const { return angle_deg_t(dValue / b.dValue); }
	angle_deg_t& operator += (const angle_deg_t& b) { dValue += b.dValue; return *this; }
	angle_deg_t& operator -= (const angle_deg_t& b) { dValue -= b.dValue; return *this; }
	angle_deg_t& operator *= (const angle_deg_t& b) { dValue *= b.dValue; return *this; }
	angle_deg_t& operator /= (const angle_deg_t& b) { dValue /= b.dValue; return *this; }

	angle_deg_t operator + (const angle_rad_t& b) const;
	angle_deg_t operator - (const angle_rad_t& b) const;
	angle_deg_t operator * (const angle_rad_t& b) const;
	angle_deg_t operator / (const angle_rad_t& b) const;
	angle_deg_t& operator += (const angle_rad_t& b);
	angle_deg_t& operator -= (const angle_rad_t& b);
	angle_deg_t& operator *= (const angle_rad_t& b);
	angle_deg_t& operator /= (const angle_rad_t& b);

	angle_deg_t operator + (const double b) const;
	angle_deg_t operator - (const double b) const;
	angle_deg_t operator * (const double b) const;
	angle_deg_t operator / (const double b) const;
	angle_deg_t& operator += (const double b);
	angle_deg_t& operator -= (const double b);
	angle_deg_t& operator *= (const double b);
	angle_deg_t& operator /= (const double b);

	static angle_deg_t atan(double v)				{ return angle_rad_t::atan(v); }
	static angle_deg_t atan2(double y, double x)	{ return angle_rad_t::atan2(y, x); }
	static angle_deg_t asin(double v)				{ return angle_rad_t::asin(v); }
	static angle_deg_t acos(double v)				{ return angle_rad_t::acos(v); }
};
inline angle_rad_t::angle_rad_t(const angle_deg_t& b) : dValue(deg2rad(b.dValue)) {}
inline angle_deg_t::angle_deg_t(const angle_rad_t& b) : dValue(rad2deg(b.dValue)) {}
inline angle_rad_t& angle_rad_t::operator = (const angle_deg_t& b) { dValue = deg2rad(b.dValue); return *this; };
inline angle_deg_t& angle_deg_t::operator = (const angle_rad_t& b) { dValue = rad2deg(b.dValue); return *this; };
//inline angle_rad_t::operator angle_deg_t() const { return angle_deg_t(rad2deg(dValue)); }
//inline angle_deg_t::operator angle_rad_t() const { return angle_rad_t(deg2rad(dValue)); }

inline angle_rad_t operator "" _rad (long double v)					{ return angle_rad_t(v); }
inline angle_rad_t operator "" _rad (unsigned long long v)			{ return angle_rad_t((double) v); }
inline angle_deg_t operator "" _deg (long double v)					{ return angle_deg_t(v); }
inline angle_deg_t operator "" _deg (unsigned long long v)			{ return angle_deg_t((double) v); }

inline angle_rad_t angle_rad_t::operator + (const angle_deg_t& b) const { return angle_rad_t(dValue + (angle_rad_t)b); }
inline angle_rad_t angle_rad_t::operator - (const angle_deg_t& b) const { return angle_rad_t(dValue - (angle_rad_t)b); }
inline angle_rad_t angle_rad_t::operator * (const angle_deg_t& b) const { return angle_rad_t(dValue * (angle_rad_t)b); }
inline angle_rad_t angle_rad_t::operator / (const angle_deg_t& b) const { return angle_rad_t(dValue / (angle_rad_t)b); }
inline angle_rad_t& angle_rad_t::operator += (const angle_deg_t& b) { dValue += (angle_rad_t)b; return *this; }
inline angle_rad_t& angle_rad_t::operator -= (const angle_deg_t& b) { dValue -= (angle_rad_t)b; return *this; }
inline angle_rad_t& angle_rad_t::operator *= (const angle_deg_t& b) { dValue *= (angle_rad_t)b; return *this; }
inline angle_rad_t& angle_rad_t::operator /= (const angle_deg_t& b) { dValue /= (angle_rad_t)b; return *this; }

inline angle_rad_t angle_rad_t::operator + (const double b) const { return angle_rad_t(dValue + b); }
inline angle_rad_t angle_rad_t::operator - (const double b) const { return angle_rad_t(dValue - b); }
inline angle_rad_t angle_rad_t::operator * (const double b) const { return angle_rad_t(dValue * b); }
inline angle_rad_t angle_rad_t::operator / (const double b) const { return angle_rad_t(dValue / b); }
inline angle_rad_t& angle_rad_t::operator += (const double b) { dValue += b; return *this; }
inline angle_rad_t& angle_rad_t::operator -= (const double b) { dValue -= b; return *this; }
inline angle_rad_t& angle_rad_t::operator *= (const double b) { dValue *= b; return *this; }
inline angle_rad_t& angle_rad_t::operator /= (const double b) { dValue /= b; return *this; }

inline angle_deg_t angle_deg_t::operator + (const angle_rad_t& b) const { return angle_deg_t(dValue + (angle_deg_t)b); }
inline angle_deg_t angle_deg_t::operator - (const angle_rad_t& b) const { return angle_deg_t(dValue - (angle_deg_t)b); }
inline angle_deg_t angle_deg_t::operator * (const angle_rad_t& b) const { return angle_deg_t(dValue * (angle_deg_t)b); }
inline angle_deg_t angle_deg_t::operator / (const angle_rad_t& b) const { return angle_deg_t(dValue / (angle_deg_t)b); }
inline angle_deg_t& angle_deg_t::operator += (const angle_rad_t& b) { dValue += (angle_deg_t)b; return *this; }
inline angle_deg_t& angle_deg_t::operator -= (const angle_rad_t& b) { dValue -= (angle_deg_t)b; return *this; }
inline angle_deg_t& angle_deg_t::operator *= (const angle_rad_t& b) { dValue *= (angle_deg_t)b; return *this; }
inline angle_deg_t& angle_deg_t::operator /= (const angle_rad_t& b) { dValue /= (angle_deg_t)b; return *this; }

inline angle_deg_t angle_deg_t::operator + (const double b) const { return angle_deg_t(dValue + b); }
inline angle_deg_t angle_deg_t::operator - (const double b) const { return angle_deg_t(dValue - b); }
inline angle_deg_t angle_deg_t::operator * (const double b) const { return angle_deg_t(dValue * b); }
inline angle_deg_t angle_deg_t::operator / (const double b) const { return angle_deg_t(dValue / b); }
inline angle_deg_t& angle_deg_t::operator += (const double b) { dValue += b; return *this; }
inline angle_deg_t& angle_deg_t::operator -= (const double b) { dValue -= b; return *this; }
inline angle_deg_t& angle_deg_t::operator *= (const double b) { dValue *= b; return *this; }
inline angle_deg_t& angle_deg_t::operator /= (const double b) { dValue /= b; return *this; }

//typedef angle_rad_t rad_t;
//typedef angle_deg_t deg_t;

using rad_t = angle_rad_t;
using deg_t = angle_deg_t;

inline double cos(angle_rad_t t)				{ return ::cos((double)t); }
inline double sin(angle_rad_t t)				{ return ::sin((double)t); }
inline double tan(angle_rad_t t)				{ return ::tan((double)t); }
inline double cos(angle_deg_t t)				{ return ::cos((double)(rad_t)t); }
inline double sin(angle_deg_t t)				{ return ::sin((double)(rad_t)t); }
inline double tan(angle_deg_t t)				{ return ::tan((double)(rad_t)t); }

#endif	// (_MSC_VER >= _MSC_VER_VS2015)


//-----------------------------------------------------------------------------
// defaulted Copy Constructor/Assignment 에서 제외할 멤버 변수용
template < typename T >
class TCopyTransparent : public T {
public:
	using base_t = T;

	using T::T;
	//TCopyTransparent() : T() {}
	TCopyTransparent(const TCopyTransparent& B) { }
	TCopyTransparent(TCopyTransparent&& B) = default;
	TCopyTransparent& operator = (const TCopyTransparent& B) { return *this; }
	TCopyTransparent& operator = (TCopyTransparent&& B) = default;
};


//-----------------------------------------------------------------------------
// Helper Class..
// Auto Flag Switcher
//
template < typename T >
class TFlagSwitch {
public:
	T& flag;
	T vEnd;
public:
	TFlagSwitch(T& _flag, T _vEnd)				: flag(_flag), vEnd(_vEnd)	{ }
	TFlagSwitch(T& _flag, T _vStart, T _vEnd)	: flag(_flag), vEnd(_vEnd)	{ flag = _vStart; }
	~TFlagSwitch()															{ flag = vEnd; }

	// ex 
	//
	//		{
	//			bool bStarted;
	//			....
	//			{
	//				TFlagSwitch<bool> switch_flag(bStarted, true, false);
	//				...
	//				,,,
	//			} 블럭 진입할 때, bStarted == true, 블럭 나가면서 bStarted == false.
	//		}
	//
};

//-----------------------------------------------------------------------------
// TFlagSetAndRestore
//
template < typename T >
class TFlagSetAndRestore {
private:
	T& flag;
	T vOrg;
public:
	TFlagSetAndRestore(T& _flag, const T& vNew) : flag(_flag) {
		vOrg = flag;
		flag = vNew;
	}
	~TFlagSetAndRestore() { flag = vOrg; }

public:
	void Restore() { flag = vOrg; }
	const T& GetOriginal() const { return vOrg; }
};


//-----------------------------------------------------------------------------
// Clean Up
//
class CCleanUp {
public:
	typedef std::function<void(void)> Cleaner;
	Cleaner m_cleaner;
public:
	CCleanUp(Cleaner cleaner) : m_cleaner(cleaner) { }
	~CCleanUp() { if (m_cleaner) m_cleaner(); }

public:
	void CleanUp() { if (m_cleaner) m_cleaner(); m_cleaner = nullptr; }

	// ex 
	//
	//		int* pValue = new int(3);
	//		{
	//			CCleanUp cleanup([&]() { delete pValue; pValue = nullptr; });
	//			...
	//		}// 블럭 나가면서 pValue 삭제.
	//
};


//-----------------------------------------------------------------------------
#ifndef _MFC_VER
#	define AFX_EXPORT __declspec(dllexport)
#	define AFX_IMPORT __declspec(dllimport)

#	define AFX_CLASS_EXPORT		AFX_EXPORT
#	define AFX_API_EXPORT		AFX_EXPORT
#	define AFX_DATA_EXPORT		AFX_EXPORT

#	define AFX_CLASS_IMPORT		AFX_IMPORT
#	define AFX_API_IMPORT		AFX_IMPORT
#	define AFX_DATA_IMPORT		AFX_IMPORT
#endif	// _MFC_VER


//-----------------------------------------------------------------------------
#if (_MSC_VER >= _MSC_VER_VS2022)
#	define __PLATFORM_TOOLSET_VER__ "v143"
#elif (_MSC_VER >= _MSC_VER_VS2019)
#	define __PLATFORM_TOOLSET_VER__ "v142"
#elif (_MSC_VER >= _MSC_VER_VS2017)
#	define __PLATFORM_TOOLSET_VER__ "v141"
#elif (_MSC_VER >= _MSC_VER_VS2015)
#	define __PLATFORM_TOOLSET_VER__ "v140"
#elif (_MSC_VER >= _MSC_VER_VS2013)
#	define __PLATFORM_TOOLSET_VER__ "v120"
#elif (_MSC_VER >= _MSC_VER_VS2010)
#	define __PLATFORM_TOOLSET_VER__ "v100"
#else
#	define __PLATFORM_TOOLSET_VER__
#endif

#if _M_64_
#	define __PLATFORM_MACHINE__ "64"
#else
#	define __PLATFORM_MACHINE__ "32"
#endif
#ifdef _DEBUG
#	define __PLATFORM_CONFIGURE__ "D"
#else
#	define __PLATFORM_CONFIGURE__ "R"
#endif
#ifdef _UNICODE
#	define __PLATFORM_CHARSET__ ""
#else
#	define __PLATFORM_CHARSET__ "A"
#endif

#define __IMPORT_MOCHA_LIB__(name) \
	__pragma(comment(lib, #name "." __PLATFORM_MACHINE__ __PLATFORM_CHARSET__ __PLATFORM_CONFIGURE__ ".lib" ))
#define __IMPORT_MOCHA_LIB_TOOLSET__(name) \
	__pragma(comment(lib, #name "." __PLATFORM_TOOLSET_VER__ "." __PLATFORM_MACHINE__  __PLATFORM_CHARSET__ __PLATFORM_CONFIGURE__ ".lib" ))

//-----------------------------------------------------------------------------

