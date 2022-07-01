// for_each_member.h:
// PWH. 2018.06.02.
//////////////////////////////////////////////////////////////////////


#pragma once


#include "default.defines.h"
#include <afxstr.h>
#include "qtl/qtl.hpp"
#include "misc/TString.h"
#include "nlohmann/json.hpp"
#include "Coordinate.hpp"
//using namespace nlohmann;
using json = nlohmann::json;


//---------------------------------------------------------------------------------------------------------------------------------
// Sync
[[nodiscard]] inline bool StoreJson(std::filesystem::path path, const nlohmann::json& j) try {
	std::ofstream f(path, std::ios_base::binary|std::ios_base::trunc);
	f << std::setw(2) << j;
	return true;
} catch (...) {
	//throw;
	return false;
}
[[nodiscard]] inline bool LoadJson(std::filesystem::path path, nlohmann::json& j) try {
	std::ifstream f(path, std::ios_base::binary);
	f >> j;
	return true;
} catch (...) {
	//throw;
	return false;
}
[[nodiscard]] inline bool SyncJson(bool bStore, std::filesystem::path path, nlohmann::json& j) try {
	return bStore ? StoreJson(path, j) : LoadJson(path, j);
} catch (...) {
	return false;
}
template < typename TDATA >
[[nodiscard]] bool SyncJsonData(bool bStore, std::filesystem::path path, TDATA& data) try {
	nlohmann::json j;
	if (bStore) {
		j = data;
		if (!StoreJson(path, j))
			return false;
	} else {
		if (!LoadJson(path, j))
			return false;
		data = j;
	}
	return true;
} catch (...) {
	//throw;
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------------
//

#define FEM_BEGIN_TBL(className) \
	typedef className this_t;\
	typedef className base_t;\
	typedef std::function<void(json&, const className&)> func_to_json_t;\
	typedef std::function<void(const json&, className&)> func_from_json_t;\
	typedef std::function<bool(const className& a, const className& b)> func_comp_t;\
	typedef std::tuple<func_to_json_t, func_from_json_t, func_comp_t> member_t;\
	typedef std::vector<member_t> members_t;\
	\
	bool operator != (const className& B) const { return !(*this == B); }\
	bool operator == (const className& B) const {\
		const auto& members = __GetMembers();\
		for (auto& member : members) {\
			if (!std::get<func_comp_t>(member)(*this, B))\
				return false;\
		}\
		return true;\
	}\
	friend void to_json(json& j, const className& B) {\
		j.clear();\
		auto& members = __GetMembers();\
		for (auto& member : members) {\
			std::get<func_to_json_t>(member)(j, B);\
		}\
	}\
	friend void from_json(const json& j, className& B) {\
		auto& members = __GetMembers();\
		for (auto& member : members) {\
			try {\
				std::get<func_from_json_t>(member)(j, B);\
			} catch (std::exception&) {\
			}\
		}\
	}\
	className& operator = (const json& j) {\
		auto& members = __GetMembers();\
		for (auto& member : members) {\
			try {\
				std::get<func_from_json_t>(member)(j, *this);\
			} catch (std::exception&) {\
			}\
		}\
		return *this;\
	}\
	static members_t& __GetMembers() { \
		static members_t members = {\


#define FEM_BEGIN_TBL_C(className, parentClassName)\
	typedef className this_t;\
	typedef parentClassName parent_t;\
	typedef std::function<void(json&, const this_t&)> func_to_json_t;\
	typedef std::function<void(const json&, this_t&)> func_from_json_t;\
	typedef std::function<bool(const this_t& a, const this_t& b)> func_comp_t;\
	typedef std::tuple<func_to_json_t, func_from_json_t, func_comp_t> member_t;\
	typedef std::vector<member_t> members_t;\
	\
	bool operator != (const className& B) const { return !(*this == B); }\
	bool operator == (const className& B) const {\
		const auto& members = __GetMembers();\
		if ( ((parent_t&)*this) == ((const parent_t&)B) ) {\
			for (auto& member : members) {\
				if (!std::get<func_comp_t>(member)(*this, B))\
					return false;\
			}\
			return true;\
		}\
		return false;\
	}\
	friend void to_json(json& j, const className& B) {\
		j.clear();\
		to_json(j, (const parent_t&)B);\
		auto& members = __GetMembers();\
		for (auto& member : members) {\
			std::get<func_to_json_t>(member)(j, B);\
		}\
	}\
	friend void from_json(const json& j, className& B) {\
		from_json(j, (parent_t&)B);\
		auto& members = __GetMembers();\
		for (auto& member : members) {\
			try {\
				std::get<func_from_json_t>(member)(j, B);\
			} catch (std::exception&) {\
			}\
		}\
	}\
	static members_t& __GetMembers() { \
		static members_t members = {\


#define FEM_END_TBL()\
		};\
		return members;\
	}\


#define FEM_ADD_MEMBER(varName)\
	{\
		[&] (json& j, const this_t& b) -> void { nlohmann::to_json(j[#varName], b.varName); },\
		[&] (const json& j, this_t& b) -> void { if (!j.at(#varName).is_null() && !j[#varName].is_null()) nlohmann::from_json(j[#varName], b.varName); },\
		[&] (const this_t& a, const this_t& b) -> bool { return (a.varName == b.varName); },\
	},
#define FEM_ADD_MEMBER_SZ(varName)\
	{\
		[&] (json& j, const this_t& b) -> void { to_json_sz(j[#varName], b.varName); },\
		[&] (const json& j, this_t& b) -> void { if (!j.at(#varName).is_null() && !j[#varName].is_null()) from_json_sz(j[#varName], b.varName, countof(b.varName)); },\
		[&] (const this_t& a, const this_t& b) -> bool { return tszcmp(a.varName, b.varName) == 0; },\
	},



#define FEM_VFUNC(className)\
	virtual void FromJson(const json& j) { from_json(j, *this); }\
	virtual void ToJson(json& j) const { to_json(j, *this); }\
	virtual bool Compare(const className& B) const {\
		if (!dynamic_cast<const className*>(&B))\
			return false;\
		return *this == B;\
	}\



#define FEM_VFUNC_C(className)\
	void FromJson(const json& j) override { from_json(j, *this); }\
	void ToJson(json& j) const override { to_json(j, *this); }\
	bool Compare(const base_t& B) const override {\
		if (!dynamic_cast<const className*>(&B))\
			return false;\
		return (*this) == (const className&)B;\
	}\



#define FEM_SERIALIZE(className)\
	template < class ARCHIVE >\
	ARCHIVE& StoreTo(ARCHIVE& ar) const {\
		std::vector<uint8_t> buf;\
		json j;\
		j = *this;\
		json::to_cbor(j, buf);\
		ar << (uint32_t)buf.size();\
		if (buf.size())\
			ar.Write(buf.data(), buf.size());\
		return ar;\
	}\
	template < class ARCHIVE >\
	ARCHIVE& LoadFrom(ARCHIVE& ar) {\
		std::vector<uint8_t> buf;\
		uint32_t n = 0;\
		ar >> n;\
		if (n > 0) {\
			buf.assign(n, 0);\
			ar.Read(buf.data(), buf.size());\
			*this = json::from_cbor(buf);\
		} else {\
			*this = className();\
		}\
		return ar;\
	}\
	template < class ARCHIVE >\
	friend ARCHIVE& operator << (ARCHIVE& ar, const className& B) { return B.StoreTo(ar); }\
	template < class ARCHIVE >\
	friend ARCHIVE& operator >> (ARCHIVE& ar, className& B) { return B.LoadFrom(ar); }\
	template < class ARCHIVE >\
	friend ARCHIVE& operator || (ARCHIVE& ar, className& B) { return B.Serialize(ar); }\
	template < class ARCHIVE >\
	ARCHIVE& Serialize(ARCHIVE& ar) { if (ar.IsStoring()) StoreTo(ar); else  LoadFrom(ar); return ar; }


//---------------------------------------------------------------------------------------------------------------------------------
// array
template < typename T, size_t n >
void to_json(json& j, const std::array<T, n>& arr) {
	j.clear();
	for (size_t i = 0; i < n; i++)
		nlohmann::to_json(j[i], arr[i]);
}
template < typename T, size_t n >
void from_json(const json& j, std::array<T, n>& arr) {
	size_t nItem = _min(n, j.size());
	for (size_t i = 0; i < nItem; i++)
		nlohmann::from_json(j[i], arr[i]);
}


// CStringA
inline void to_json(json& j, const CStringA& str) {
	CStringW strW(str);
	int nLen = WideCharToMultiByte(CP_UTF8, 0, strW, strW.GetLength(), nullptr, 0, nullptr, nullptr);
	if (nLen > 0) {
		CStringA strU8;
		WideCharToMultiByte(CP_UTF8, 0, strW, strW.GetLength(), strU8.GetBufferSetLength(nLen), nLen, nullptr, nullptr);
		strU8.ReleaseBuffer();
		j = (LPCSTR)strU8;
	} else {
		j = u8"";
	}
}
inline void from_json(const json& j, CStringA& str) {
	std::string strU8 = j;
	int nLen = MultiByteToWideChar(CP_UTF8, 0, strU8.c_str(), (int)strU8.length(), nullptr, 0);
	if (nLen > 0) {
		CStringW strW;
		MultiByteToWideChar(CP_UTF8, 0, strU8.c_str(), (int)strU8.length(), strW.GetBuffer(nLen), nLen);
		strW.ReleaseBuffer();
		str = strW;
	} else {
		str.Empty();
	}
}


// CStringW
inline void to_json(json& j, const CStringW& str) {
	int nLen = WideCharToMultiByte(CP_UTF8, 0, str, str.GetLength(), nullptr, 0, nullptr, nullptr);
	if (nLen > 0) {
		CStringA strU8;
		WideCharToMultiByte(CP_UTF8, 0, str, str.GetLength(), strU8.GetBufferSetLength(nLen), nLen, nullptr, nullptr);
		strU8.ReleaseBuffer();
		j = (LPCSTR)strU8;
	} else {
		j = u8"";
	}
}
inline void from_json(const json& j, CStringW& str) {
	std::string strU8 = j;
	int nLen = MultiByteToWideChar(CP_UTF8, 0, strU8.c_str(), (int)strU8.length(), nullptr, 0);
	if (nLen > 0) {
		MultiByteToWideChar(CP_UTF8, 0, strU8.c_str(), (int)strU8.length(), str.GetBuffer(nLen), nLen);
		str.ReleaseBuffer();
	} else {
		str.Empty();
	}
}


namespace std {
	// wstring
	inline void to_json(json& j, wstring const& str) {
		int nLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.size(), nullptr, 0, nullptr, nullptr);
		if (nLen > 0) {
			CStringA strU8;
			WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.size(), strU8.GetBufferSetLength(nLen), nLen, nullptr, nullptr);
			strU8.ReleaseBuffer();
			j = (LPCSTR)strU8;
		} else {
			j = u8"";
		}
	}
	inline void from_json(json const& j, wstring& str) {
		std::string strU8 = j;
		int nLen = MultiByteToWideChar(CP_UTF8, 0, strU8.c_str(), (int)strU8.length(), nullptr, 0);
		if (nLen > 0) {
			str.resize((size_t)nLen);
			MultiByteToWideChar(CP_UTF8, 0, strU8.c_str(), (int)strU8.length(), str.data(), nLen);
		} else {
			str.clear();
		}
	}
}


// sz
inline void to_json_sz(json& j, const char* psz) {
	CStringA strU8;
	strU8 = CONV_U8(psz);
	j = (LPCSTR)strU8;
}
inline void from_json_sz(const json& j, char* sz, int n) {
	if (n <= 0) return;
	if (n == 1) {
		sz[0] = 0;
		return;
	}
	CStringA str = j;
	tszncpy(sz, (LPCSTR)str, n);
}
inline void to_json_sz(json& j, const wchar_t* psz) {
	CStringA strU8;
	strU8 = CONV_U8(psz);
	j = (LPCSTR)strU8;
}
inline void from_json_sz(const json& j, wchar_t* sz, int n) {
	if (n <= 0) return;
	if (n == 1) {
		sz[0] = 0;
		return;
	}
	CStringW str = j;
	tszncpy(sz, (LPCWSTR)str, n);
}



// CPoint2d / CSize2d / CRect2d / CPoint3d / CSize3d / CRect3d ...
template < class T_COORD, class = typename T_COORD::coord_t >
void to_json(json& j, const T_COORD& v) {
	for (const auto& value : v.val)
		j.push_back(value);
}
template < class T_COORD, class = typename T_COORD::coord_t >
void from_json(const json& j, T_COORD& v) {
	size_t n = _min(j.size(), countof(v.val));
	for (size_t i = 0; i < n; i++)
		v.val[i] = j[i];
}


// length_mm
inline void to_json(json& j, const length_mm_t& v) { j = (double)(v); }
inline void from_json(const json& j, length_mm_t& v) { v = j.get<double>(); }


// angle (rad_t, deg_t)
inline void to_json(json& j, const angle_rad_t& v) { j = (double)v; }
inline void from_json(const json& j, angle_rad_t& v) { v = j.get<double>(); }
inline void to_json(json& j, const angle_deg_t& v) { j = (double)v; }
inline void from_json(const json& j, angle_deg_t& v) { v = j.get<double>(); }


// QTL Container
namespace qtl {
	template < class T, class TPContainer >
	void to_json(json& j, const TContainerWorker<T, TPContainer>& lst) {
		j.clear();
		for (int i = 0; i < lst.size(); i++)
			j.push_back(lst[i]);
	}
	template < class T, class TPContainer >
	void from_json(const json& j, TContainerWorker<T, TPContainer>& lst) {
		lst.DeleteAll();
		for (int i = 0; i < j.size(); i++) {
			T* p = new T;
			*p = j[i];
			lst.Push(p);
		}
	}
}


// Mat
namespace cv {
	template<typename _Tp, int m, int n>
	inline void to_json(json& j, const Matx<_Tp, m, n>& mat) {
		j = mat.val;
		//for (int i = 0; i < m*n; i++)
		//	j[i] = mat.val[i];
	}
	template<typename _Tp, int m, int n>
	inline void from_json(const json& j, Matx<_Tp, m, n>& mat) {
		for (int i = 0; i < m*n; i++)
			mat.val[i] = j[i];
	}

	inline void to_json(json& j, const Mat& mat) {
		using namespace std;
		j = json({
			{ "rows", mat.rows},
			{ "cols", mat.cols},
			{ "type", mat.type()},
		});
		std::vector<unsigned char> data;
		size_t nRowSize = mat.cols*mat.elemSize();
		data.assign(mat.rows*nRowSize, 0);
		unsigned char* pos = data.data();
		for (int y = 0; y < mat.rows; y++, pos += nRowSize) {
			memcpy(pos, mat.ptr<unsigned char*>(y), nRowSize);
		}
		j["data"] = data;
	}
	inline void from_json(const json& j, Mat& mat) {
		using namespace std;
		mat = Mat(j["rows"].get<int>(), j["cols"].get<int>(), j["type"].get<int>());
	#if 0		// to speed up
		const auto& data = j["data"].get<const std::vector<unsigned char>>();
	#else
		const auto& jData = j["data"];
		std::vector<unsigned char> data;
		size_t nMatSizeInByte = mat.rows*mat.cols*mat.elemSize();
		nMatSizeInByte = _min(nMatSizeInByte, jData.size());
		data.assign(nMatSizeInByte, 0);
		for (int i = 0; i < nMatSizeInByte; i++)
			data[i] = jData.at(i).get<unsigned char>();
	#endif
		memcpy(mat.ptr(0), data.data(), nMatSizeInByte);
	}
}


