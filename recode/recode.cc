#include "internal.hh"

namespace recode {
/****************************************************************************************/

namespace impl {
/****************************************************************************************/

namespace implementation = unistring;
//namespace implementation = libiconv;

static std::u16string char8_to_char16(char8_t const *orig, size_t length)
{
#if defined _WIN32 && defined RECODE_USE_WIN32_STR_CONVERSION_FUNCS
    return win32::char8_to_char16(orig, length);
#else
    return implementation::char8_to_char16(orig, length);
#endif
}

static std::u8string char16_to_char8(char16_t const *orig, size_t length)
{
#if defined _WIN32 && defined RECODE_USE_WIN32_STR_CONVERSION_FUNCS
    return win32::char16_to_char8(orig, length);
#else
    return implementation::char16_to_char8(orig, length);
#endif
}

#ifndef RECODE_WIN32_ONLY
static std::u32string char8_to_char32(char8_t const *orig, size_t length)
{
    return implementation::char8_to_char32(orig, length);
}

static std::u32string char16_to_char32(char16_t const *orig, size_t length)
{
    return implementation::char16_to_char32(orig, length);
}

static std::u8string char32_to_char8(char32_t const *orig, size_t length)
{
    return implementation::char32_to_char8(orig, length);
}

static std::u16string char32_to_char16(char32_t const *orig, size_t length)
{
    return implementation::char32_to_char16(orig, length);
}
#endif

static std::string char16_to_char(char16_t const *orig, size_t length)
{
#if defined _WIN32 && defined RECODE_USE_WIN32_STR_CONVERSION_FUNCS
    return win32::char16_to_char(orig, length);
#else
    return implementation::char16_to_char(orig, length);
#endif
}

#ifndef RECODE_WIN32_ONLY
static std::string char32_to_char(char32_t const *orig, size_t length)
{
    return implementation::char32_to_char(orig, length);
}
#endif

static std::wstring char8_to_wide(char8_t const *orig, size_t length)
{
#if defined _WIN32 && defined RECODE_USE_WIN32_STR_CONVERSION_FUNCS
    return win32::char8_to_wide(orig, length);
#else
    return implementation::char8_to_wide(orig, length);
#endif
}

#ifndef WCHAR_IS_U16
static std::wstring char16_to_wide(char16_t const *orig, size_t length)
{
    return implementation::char16_to_wide(orig, length);
}
#endif

#ifndef RECODE_WIN32_ONLY
# ifndef WCHAR_IS_U32
static std::wstring char32_to_wide(char32_t const *orig, size_t length)
{
    return implementation::char32_to_wide(orig, length);
}
# endif
#endif

/****************************************************************************************/
} // namespace impl

/*--------------------------------------------------------------------------------------*/
/* The fundamental types: char and wchar_t. */

template <> std::string convert<char, wchar_t>(wchar_t const *orig, size_t length)
{
#ifdef WCHAR_IS_U16
    return impl::char16_to_char(reinterpret_cast<char16_t const *>(orig), length);
#else
    return impl::char32_to_char(reinterpret_cast<char32_t const *>(orig), length);
#endif
}

template <> std::wstring convert<wchar_t, char>(char const *orig, size_t length)
{
    return impl::char8_to_wide(reinterpret_cast<char8_t const *>(orig), length);
}

/*--------------------------------------------------------------------------------------*/
/* Conversions to and from char (excluding wchar_t). */

template <> std::string convert<char, char8_t>(char8_t const *orig, size_t length)
{
    return std::string{reinterpret_cast<char const *>(orig), length};
}

template <> std::string convert<char, char16_t>(char16_t const *orig, size_t length)
{
    return impl::char16_to_char(orig, length);
}

#ifndef RECODE_WIN32_ONLY
template <> std::string convert<char, char32_t>(char32_t const *orig, size_t length)
{
    return impl::char32_to_char(orig, length);
}
#endif

template <> std::u8string convert<char8_t, char>(char const *orig, size_t length)
{
    return std::u8string{reinterpret_cast<char8_t const *>(orig), length};
}

template <> std::u16string convert<char16_t, char>(char const *orig, size_t length)
{
    return impl::char8_to_char16(reinterpret_cast<char8_t const *>(orig), length);
}

#ifndef RECODE_WIN32_ONLY
template <> std::u32string convert<char32_t, char>(char const *orig, size_t length)
{
    return impl::char8_to_char32(reinterpret_cast<char8_t const *>(orig), length);
}
#endif

/*--------------------------------------------------------------------------------------*/
/* Conversions to and from wchar_t (excluding char). */

template <> std::wstring convert<wchar_t, char8_t>(char8_t const *orig, size_t length)
{
    return impl::char8_to_wide(orig, length);
}

template <> std::wstring convert<wchar_t, char16_t>(char16_t const *orig, size_t length)
{
#ifdef WCHAR_IS_U16
    return {reinterpret_cast<wchar_t const *>(orig), length};
#else
    return impl::char16_to_wide(orig, length);
#endif
}

#ifndef RECODE_WIN32_ONLY
template <> std::wstring convert<wchar_t, char32_t>(char32_t const *orig, size_t length)
{
# ifdef WCHAR_IS_U16
    return impl::char32_to_wide(orig, length);
# else
    return {reinterpret_cast<wchar_t const *>(orig), length};
# endif
}
#endif

template <> std::u8string convert<char8_t, wchar_t>(wchar_t const *orig, size_t length)
{
#ifdef WCHAR_IS_U16
    return impl::char16_to_char8(reinterpret_cast<char16_t const *>(orig), length);
#else
    return impl::char32_to_char8(reinterpret_cast<char32_t const *>(orig), length);
#endif
}

template <> std::u16string convert<char16_t, wchar_t>(wchar_t const *orig, size_t length)
{
#ifdef WCHAR_IS_U16
    return std::u16string{reinterpret_cast<char16_t const *>(orig), length};
#else
    return impl::char32_to_char16(reinterpret_cast<char32_t const *>(orig), length);
#endif
}

#ifndef RECODE_WIN32_ONLY
template <> std::u32string convert<char32_t, wchar_t>(wchar_t const *orig, size_t length)
{
# ifdef WCHAR_IS_U16
    return impl::char16_to_char32(reinterpret_cast<char16_t const *>(orig), length);
# else
    return std::u32string{reinterpret_cast<char32_t const *>(orig), length};
# endif
}
#endif

/*--------------------------------------------------------------------------------------*/
/* The rest. */

#ifndef RECODE_WIN32_ONLY
template <> std::u16string convert<char16_t, char8_t>(char8_t const *orig, size_t length)
{
    return impl::char8_to_char16(orig, length);
}

template <> std::u32string convert<char32_t, char8_t>(char8_t const *orig, size_t length)
{
    return impl::char8_to_char32(orig, length);
}

template <> std::u8string convert<char8_t, char16_t>(char16_t const *orig, size_t length)
{
    return impl::char16_to_char8(orig, length);
}

template <> std::u32string convert<char32_t, char16_t>(char16_t const *orig, size_t length)
{
    return impl::char16_to_char32(orig, length);
}

template <> std::u16string convert<char16_t, char32_t>(char32_t const *orig, size_t length)
{
    return impl::char32_to_char16(orig, length);
}

template <> std::u8string convert<char8_t, char32_t>(char32_t const *orig, size_t length)
{
    return impl::char32_to_char8(orig, length);
}
#endif

/*--------------------------------------------------------------------------------------*/
/* What the hell; why not. */

template <> std::string convert<char, char>(char const *orig, size_t length)
{
    return std::string{orig, length};
}

template <> std::wstring convert<wchar_t, wchar_t>(wchar_t const *orig, size_t length)
{
    return std::wstring{orig, length};
}

template <> std::u8string convert<char8_t, char8_t>(char8_t const *orig, size_t length)
{
    return std::u8string{orig, length};
}

template <> std::u16string convert<char16_t, char16_t>(char16_t const *orig, size_t length)
{
    return std::u16string{orig, length};
}

template <> std::u32string convert<char32_t, char32_t>(char32_t const *orig, size_t length)
{
    return std::u32string{orig, length};
}

/****************************************************************************************/
} // namespace recode