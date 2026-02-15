#pragma once
#ifndef RECODE_HEADER_VmAKSV8jiyVq2DWcGNuOwHiIbWkdV829Go2zRYF8U93UfbU
#define RECODE_HEADER_VmAKSV8jiyVq2DWcGNuOwHiIbWkdV829Go2zRYF8U93UfbU //NOLINT
/****************************************************************************************/

#include <recode/recode.hh>
#include <recode/strlen.hh>

#if WCHAR_MAX == INT32_MAX || WCHAR_MAX == UINT32_MAX
#  define WCHAR_IS_U32
#elif WCHAR_MAX == INT16_MAX || WCHAR_MAX == UINT16_MAX
#  define WCHAR_IS_U16
#else
# error "Impossible?!"
#endif

#if SIZE_MAX == UINT64_MAX
# define SIZE_C(n) UINT64_C(n)
#elif SIZE_MAX == UINT32_MAX
# define SIZE_C(n) UINT32_C(n)
#elif SIZE_MAX == UINT16_MAX
# define SIZE_C(n) UINT16_C(n)
#else
# error "I have no useful warning message to give here. You know what you did."
#endif

#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# ifndef NOMINMAX
#  define NOMINAMX
# endif
# include <Windows.h>
# include <strsafe.h>
#endif
#ifndef WC_ERR_INVALID_CHARS
# define WC_ERR_INVALID_CHARS 0x00000080
#endif

#ifdef _WIN32
# ifdef _DLL
#  define EXPORT __declspec(dllexport)
# else
#  define EXPORT __declspec(dllimport)
# endif
#else
# define R_EXPORT
#endif

#include <climits>
#include <format>
#include <format>
#include <iostream>
#include <iostream>
#include <memory>

namespace recode::impl {
/****************************************************************************************/


template <typename Elem>
class my_char_traits : public std::char_traits<Elem>
{
  public:
    using std::char_traits<Elem>::assign;

    /* This function is supposed to set `count` elements from `first` to the value
     * `ch`. Typically, `ch` is 0. In other words, this function is meant to zero the
     * memory. Instead, this function does nothing. Much better. */
    static constexpr Elem *assign(Elem *first, size_t, Elem) noexcept
    {
        /* Do nothing. */
        return first;
    }
};

template <typename Ty>
    requires std::derived_from<Ty, std::basic_string<typename Ty::value_type>>
static constexpr void resize_string_hack(Ty &str, size_t new_size)
{
    using value_type  = Ty::value_type;
    using allocator   = std::allocator_traits<typename Ty::allocator_type>::template rebind_alloc<value_type>;
    using string_type = std::basic_string<value_type, my_char_traits<value_type>, allocator>;

    reinterpret_cast<string_type &>(str).resize(new_size);
}

[[noreturn]] void throw_conversion_error(int e, char const *from_name, int from_cp, char const *to_name, int to_cp, char const *impl_name);

/*--------------------------------------------------------------------------------------*/

#ifdef _WIN32
namespace win32 {
std::string    char16_to_char (char16_t const *orig, size_t length);
std::u8string  char16_to_char8(char16_t const *orig, size_t length);
std::wstring   char8_to_wide  (char8_t  const *orig, size_t length);
std::u16string char8_to_char16(char8_t  const *orig, size_t length);
} // namespace win32
#endif


namespace unistring {
std::wstring   char8_to_wide   (char8_t  const *orig, size_t length);
std::u16string char8_to_char16 (char8_t  const *orig, size_t length);
std::u32string char8_to_char32 (char8_t  const *orig, size_t length);
std::string    char16_to_char  (char16_t const *orig, size_t length);
std::wstring   char16_to_wide  (char16_t const *orig, size_t length);
std::u8string  char16_to_char8 (char16_t const *orig, size_t length);
std::u32string char16_to_char32(char16_t const *orig, size_t length);
std::string    char32_to_char  (char32_t const *orig, size_t length);
std::wstring   char32_to_wide  (char32_t const *orig, size_t length);
std::u8string  char32_to_char8 (char32_t const *orig, size_t length);
std::u16string char32_to_char16(char32_t const *orig, size_t length);
} // namespace unistring

namespace libiconv {
std::wstring   char8_to_wide   (char8_t  const *orig, size_t length);
std::u16string char8_to_char16 (char8_t  const *orig, size_t length);
std::u32string char8_to_char32 (char8_t  const *orig, size_t length);
std::string    char16_to_char  (char16_t const *orig, size_t length);
std::wstring   char16_to_wide  (char16_t const *orig, size_t length);
std::u8string  char16_to_char8 (char16_t const *orig, size_t length);
std::u32string char16_to_char32(char16_t const *orig, size_t length);
std::string    char32_to_char  (char32_t const *orig, size_t length);
std::wstring   char32_to_wide  (char32_t const *orig, size_t length);
std::u8string  char32_to_char8 (char32_t const *orig, size_t length);
std::u16string char32_to_char16(char32_t const *orig, size_t length);
} // namespace libiconv

/****************************************************************************************/
} // namespace recode::impl
#endif
