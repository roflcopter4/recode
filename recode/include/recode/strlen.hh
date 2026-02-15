#pragma once
#ifndef RECODE_HEADER_EewPw0jGPwrcbAYx2G4g8xbOVuyDhLJBMdNc3J9PgFTaf6pG
#define RECODE_HEADER_EewPw0jGPwrcbAYx2G4g8xbOVuyDhLJBMdNc3J9PgFTaf6pG //NOLINT
/****************************************************************************************/

#include <recode/recode_private.hh>

namespace recode {
/****************************************************************************************/


namespace impl {

#define _UNISTR_STRLEN_C(TYPE) \
  [[nodiscard]] constexpr size_t strlen(TYPE const *const str) noexcept
#define _UNISTR_STRLEN_I(TYPE) \
  [[nodiscard]] inline size_t strlen(TYPE const *const str) noexcept


#ifdef RECODE_UNISTRING_NO_STRLEN
_UNISTR_STRLEN_C (char)    { return std::char_traits<char>::length(str); }
_UNISTR_STRLEN_C (char8_t) { return std::char_traits<char8_t>::length(str); }
_UNISTR_STRLEN_I (uint8_t) { return std::char_traits<char8_t>::length(reinterpret_cast<char8_t const *>(str)); }
#else
_UNISTR_STRLEN_C (char)    { return _STRLEN(str); }
_UNISTR_STRLEN_I (char8_t) { return _STRLEN(reinterpret_cast<char const *>(str)); }
_UNISTR_STRLEN_I (uint8_t) { return _STRLEN(reinterpret_cast<char const *>(str)); }
#endif


#ifdef RECODE_UNISTRING_NO_WCSLEN
# if defined WCHAR_IS_U16
_UNISTR_STRLEN_C (wchar_t) { return std::char_traits<wchar_t>::length(str); }
# elif defined WCHAR_IS_U32
_UNISTR_STRLEN_C (wchar_t) { return std::char_traits<wchar_t>::length(str); }
# endif
#else
_UNISTR_STRLEN_C (wchar_t) { return _WCSLEN(str); }
#endif


#if defined WCHAR_IS_U16 && !defined RECODE_UNISTRING_NO_WCSLEN
_UNISTR_STRLEN_I (uint16_t) { return _WCSLEN(reinterpret_cast<wchar_t const *>(str)); }
_UNISTR_STRLEN_I (char16_t) { return _WCSLEN(reinterpret_cast<wchar_t const *>(str)); }
_UNISTR_STRLEN_I (uint32_t) { return std::char_traits<char32_t>::length(reinterpret_cast<char32_t const *>(str)); }
_UNISTR_STRLEN_C (char32_t) { return std::char_traits<char32_t>::length(str); }
#elif defined WCHAR_IS_U32 && !defined RECODE_UNISTRING_NO_WCSLEN
_UNISTR_STRLEN_I (uint16_t) { return std::char_traits<char16_t>::length(reinterpret_cast<char16_t const *>(str)); }
_UNISTR_STRLEN_C (char16_t) { return std::char_traits<char16_t>::length(str); }
_UNISTR_STRLEN_I (uint32_t) { return _WCSLEN(reinterpret_cast<wchar_t const *>(str)); }
_UNISTR_STRLEN_I (char32_t) { return _WCSLEN(reinterpret_cast<wchar_t const *>(str)); }
#else
_UNISTR_STRLEN_I (uint16_t) { return std::char_traits<char16_t>::length(reinterpret_cast<char16_t const *>(str)); }
_UNISTR_STRLEN_C (char16_t) { return std::char_traits<char16_t>::length(str); }
_UNISTR_STRLEN_I (uint32_t) { return std::char_traits<char32_t>::length(reinterpret_cast<char32_t const *>(str)); }
_UNISTR_STRLEN_C (char32_t) { return std::char_traits<char32_t>::length(str); }
#endif

#undef _UNISTR_STRLEN_C
#undef _UNISTR_STRLEN_I
#undef _WCSLEN
#undef _STRLEN

} // namespace impl

/*--------------------------------------------------------------------------------------*/

template <typename T, size_t N>
[[nodiscard]] consteval size_t strlen(T const (&str)[N]) noexcept
{
    assert(std::char_traits<T>::length(str) == N - 1);
    return N - 1;
}

template <typename T>
    requires (
        concepts::Pointer<T> &&
        !concepts::Reference<T> &&
        !concepts::Array<T> &&
        requires (T x) { {*x} -> concepts::Integral; }
    )
[[nodiscard]] constexpr size_t strlen(T const str) noexcept
{
    return impl::strlen(str);
}


/****************************************************************************************/
} // namespace recode
#endif
