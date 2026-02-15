#pragma once
#ifndef RECODE_HEADER_S2hWb63DtLnc8M9YlQO1Wderz0yw3uLW3cJnTuSWRDCLd9JR
#define RECODE_HEADER_S2hWb63DtLnc8M9YlQO1Wderz0yw3uLW3cJnTuSWRDCLd9JR //NOLINT
/****************************************************************************************/


//#define RECODE_WIN32_ONLY
#define RECODE_USE_WIN32_STR_CONVERSION_FUNCS 1
//#define RECODE_UNISTRING_NO_WCSLEN 1
//#define RECODE_UNISTRING_NO_STRLEN 1

#if __has_include("cuchar")
# include <cuchar>
#else
# include <uchar.h>
#endif
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cwchar>

#include <concepts>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#if defined RECODE_WIN32_ONLY && !defined _WIN32
# undef RECODE_WIN32_ONLY
#endif
#ifdef RECODE_WIN32_ONLY
# ifndef RECODE_USE_WIN32_STR_CONVERSION_FUNCS 
#  define RECODE_USE_WIN32_STR_CONVERSION_FUNCS 1
# endif
# ifdef RECODE_UNISTRING_NO_STRLEN
#  undef RECODE_UNISTRING_NO_STRLEN
# endif
# ifdef RECODE_UNISTRING_NO_WCSLEN
#  undef RECODE_UNISTRING_NO_WCSLEN
# endif
#endif

#if defined _MSC_VER || defined __GNUC__ || defined __clang__
# define _STRLEN(str) __builtin_strlen(str)
#else
# define _STRLEN(str) std::char_traits<char>::length(str)
#endif
#if defined _MSC_VER || defined __clang__
# define _WCSLEN(str) __builtin_wcslen(str)
#else
# define _WCSLEN(str) std::char_traits<wchar_t>::length(str)
#endif

#if WCHAR_MAX == INT32_MAX || WCHAR_MAX == UINT32_MAX
# define WCHAR_IS_U32
#elif WCHAR_MAX == INT16_MAX || WCHAR_MAX == UINT16_MAX
# define WCHAR_IS_U16
#else
# error "Impossible?!"
#endif

#ifdef _WIN32
# ifdef _DLL
#  define RECODE_EXPORT __declspec(dllexport)
# else
#  define RECODE_EXPORT __declspec(dllimport)
# endif
#else
# define RECODE_EXPORT
#endif

namespace recode::concepts {

template <typename T> concept Array     = std::is_array_v<std::remove_reference_t<T>>;
template <typename T> concept Constant  = std::is_const_v<std::remove_reference_t<T>>;
template <typename T> concept Integral  = std::is_integral_v<std::remove_reference_t<T>>;
template <typename T> concept Pointer   = std::is_pointer_v<std::remove_reference_t<T>>;
template <typename T> concept Reference = std::is_reference_v<T>;

} // namespace recode::concepts


/****************************************************************************************/
#endif
