//////////////////////////////////////////////////////////////////////
//
// json_proxy.h: json adapter for (boost::json, nlohmann::json)
//
// PWH
//    2021.01.11~16.
// key : string / u8string.
// !!! importance !!! std::string contains ansi(MBCS) string (codepage). std::u8string will contain utf-8 string.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "gtl/_config.h"
#include "gtl/concepts.h"

#if (GTL__BOOST_JSON__AS_STANDALONE)
#	define BOOST_JSON_STANDALONE
//#	define BOOST_NO_EXCEPTIONS
#elif (GTL__BOOST_JSON__AS_NESTED_LIB)
#	define BOOST_JSON_STANDALONE
//#	define BOOST_NO_EXCEPTIONS
#	ifdef GTL__EXPORTS
#		define BOOST_JSON_DECL       __declspec(dllexport)
#		define BOOST_JSON_CLASS_DECL __declspec(dllexport)
#	else
#		define BOOST_JSON_DECL       __declspec(dllimport)
#		define BOOST_JSON_CLASS_DECL __declspec(dllimport)
#	endif
#elif (GTL__BOOST_JSON__AS_SHARED_LIB)
#	define BOOST_JSON_DECL       __declspec(dllimport)
#	define BOOST_JSON_CLASS_DECL __declspec(dllimport)
//#	pragma comment(lib, "boost.json.lib")
#endif


// https://github.com/nlohmann/json
#include "nlohmann/json.hpp"


//#include "gtl/string/convert_codepage.h"


// from/to json
//template < typename tjson, typename T >
//void from_json(tjson const& j, T& object);
//template < typename tjson, typename T >
//void to_json(tjson&& j, T const& object);

template < typename tjson, gtlc::arithmetic T >
void from_json(tjson const& j, T& value) {
	value = j;
}
template < typename tjson, gtlc::arithmetic T >
void to_json(tjson&& j, T const& value) {
	j = value;
}
template < typename tjson, gtlc::string_elem tchar >
void from_json(tjson const& j, std::basic_string<tchar>& str) {
	str = j;
}
template < typename tjson, gtlc::string_elem tchar >
void to_json(tjson&& j, std::basic_string<tchar> const& str) {
	j = str;
}
template < typename tjson, gtlc::string_elem tchar >
void to_json(tjson&& j, std::basic_string_view<tchar> const& sv) {
	j = sv;
}

namespace gtl {
#pragma pack(push, 8)

    /// @brief json proxy for nlohmann:json
	template < typename json_t = nlohmann::json >
	class njson {
		std::optional<json_t> j__;
		json_t& j_;
	public:
		njson() : j__(json_t{}), j_(j__.value()) {};
		njson(njson const&) = default;
		njson(njson &&) = default;
		njson& operator = (njson const&) = default;
		njson& operator = (njson &&) = default;

		njson(json_t& b) : j_(b) {}

		json_t& json() {
			return j_;
		}
		json_t const& json() const {
			return j_;
		}

		//template < typename T > requires (requires (json_t j, T v) { j = v; })
		//njson& operator = (T const& b) {
		//	j_ = b;
		//	return *this;
		//}
		template < typename T >//requires (!requires (json_t j, T v) { j = v; })
		njson& operator = (T const& b) {
			using value_type = std::remove_cvref_t<T>;
			if constexpr (std::is_integral_v<value_type> or std::is_floating_point_v<value_type>) {
				j_ = b;
			} else {
				to_json(*this, b);
			}
			return *this;
		}
		njson& operator = (std::string const& str) { j_ = str; return *this; }
		//njson& operator = (std::string const& str) { j_ = reinterpret_cast<std::string&&>(gtl::ToStringU8(str)); return *this; }
		njson& operator = (std::wstring const& str) {
			return *this = std::wstring_view(str);
		}
		//njson& operator = (std::u8string const& str) { j_ = reinterpret_cast<std::string const&>(str); return *this; }
		//njson& operator = (std::u16string const& str) { j_ = reinterpret_cast<std::string&&>(gtl::ToStringU8(str)); return *this; }
		//njson& operator = (std::u32string const& str) { j_ = reinterpret_cast<std::string&&>(gtl::ToStringU8(str)); return *this; }

		njson& operator = (std::string_view sv) { j_ = std::string(sv); return *this; }
		//njson& operator = (std::string_view sv) { j_ = reinterpret_cast<std::string&&>(gtl::ToStringU8(sv)); return *this; }
		njson& operator = (std::wstring_view sv) {
			int nLen = WideCharToMultiByte(CP_UTF8, 0, sv.data(), sv.size(), nullptr, 0, nullptr, nullptr);
			if (nLen > 0) {
				CStringA strU8;
				WideCharToMultiByte(CP_UTF8, 0, sv.data(), sv.size(), strU8.GetBufferSetLength(nLen), nLen, nullptr, nullptr);
				strU8.ReleaseBuffer();
				j_ = (LPCSTR)strU8;
			} else {
				j_ = "";
			}
			return *this;
		}
		//njson& operator = (std::u8string_view sv) { j_ = reinterpret_cast<std::string_view&>(sv); return *this; }
		//njson& operator = (std::u16string_view sv) { j_ = reinterpret_cast<std::string&&>(gtl::ToStringU8(sv)); return *this; }
		//njson& operator = (std::u32string_view sv) { j_ = reinterpret_cast<std::string&&>(gtl::ToStringU8(sv)); return *this; }


		template < size_t n >
		njson operator [] (char const (&sz)[n]) { return operator [](std::string_view{sz}); }
		template < size_t n >
		njson operator [] (char8_t const (&sz)[n]) { return operator [](std::u8string_view{sz}); }
		njson operator [] (std::string_view svKey) {
			return j_[std::string(svKey)];
		}
		njson operator [] (std::u8string_view svKey) {
			// todo : how to use string_view???
			return j_[std::string((char const*)svKey.data(), svKey.size())];
		}
		njson operator [] (std::size_t index) {
			return j_[index];
		}
		template < size_t n >
		njson operator [] (char const (&sz)[n]) const { return operator [](std::string_view{sz}); }
		template < size_t n >
		njson operator [] (char8_t const (&sz)[n]) const { return operator [](std::u8string_view{sz}); }
		njson const operator [] (std::string_view svKey) const {
			return j_[std::string(svKey)];
		}
		njson const operator [] (std::u8string_view svKey) const {
			return j_[std::string((char const*)svKey.data(), svKey.size())];
		}
		njson const operator [] (std::size_t index) const {
			return j_[index];
		}

		operator bool() const { return j_.is_boolean() ? (bool)j_ : false; }
		operator int() const { return j_.is_number_integer() ? (int)j_ : 0; }
		operator int64_t() const { return j_.is_number_integer() ? (int64_t)j_ : 0ll; }
		operator double() const { return j_.is_number_float() ? (double)j_ : 0.0; }
		operator float() const { return j_.is_number_float() ? (float)(double)j_ : 0.0f; }
		operator std::string () const {
			return j_.is_string() ? (std::string)j_ : std::string{};
			//auto jstrU8 = (std::string)j_;
			//if constexpr (std::is_same_v<tchar_t, char8_t>) {
			//	return std::u8string((char8_t const*)jstrU8.data(), jstrU8.size());
			//} else {
			//	std::u8string_view svU8{(char8_t const*)jstrU8.data(), jstrU8.size()};
			//	return gtl::ToString<tchar_t, char8_t, false>(svU8);
			//}
		}
		operator std::wstring () const {
			std::wstring str;
			if (!j_.is_string())
				return str;
			auto strU8 = (std::string)j_;
			if (int nLen = MultiByteToWideChar(CP_UTF8, 0, strU8.c_str(), (int)strU8.length(), nullptr, 0); nLen > 0) {
				str.resize((size_t)nLen);
				MultiByteToWideChar(CP_UTF8, 0, strU8.c_str(), (int)strU8.length(), str.data(), nLen);
			}
			return str;
		}
		template < typename T >
			requires (
				(std::is_class_v<T> || std::is_enum_v<T> )
				&& !std::is_convertible_v<T, std::string> && !std::is_convertible_v<T, std::wstring>
				&& !std::is_convertible_v<T, std::u8string> && !std::is_convertible_v<T, std::u16string> && !std::is_convertible_v<T, std::u32string>
				&& !std::is_convertible_v<T, std::string_view> && !std::is_convertible_v<T, std::wstring_view>
				&& !std::is_convertible_v<T, std::u8string_view> && !std::is_convertible_v<T, std::u16string_view> && !std::is_convertible_v<T, std::u32string_view>
				)
		operator T() const {
			T a{};
			if (j_.is_object())
				from_json(*this, a);
			return a;
		}

	};

#pragma pack(pop)
}
