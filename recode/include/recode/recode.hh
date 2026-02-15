#pragma once
#ifndef RECODE_HEADER_MaXl9MDeka6unIFWbbBydHdRXCHhCOHbyqQRjHiofiVppcO
#define RECODE_HEADER_MaXl9MDeka6unIFWbbBydHdRXCHhCOHbyqQRjHiofiVppcO //NOLINT
/****************************************************************************************/

#include <recode/recode_private.hh>
#include <recode/strlen.hh>
#pragma push_macro("ND")
#define ND [[nodiscard]]

namespace recode {
/****************************************************************************************/

template <typename To, typename From>
ND std::basic_string<To>
convert(From const *orig, size_t length);

template <typename To, typename From, size_t N>
    requires (concepts::Constant<From>)
ND std::basic_string<To>
convert(From (&orig)[N])
{
    assert(std::char_traits<From>::length(orig) == N - 1);
    return convert<To, std::remove_cvref_t<From>>(orig, N - 1);
}

template <typename To, class Container>
    requires (concepts::Integral<typename Container::value_type> &&
              requires (Container c) { c.data(); c.size(); })
ND std::basic_string<To>
convert(Container const &orig)
{
    return convert<To, typename Container::value_type>(orig.data(), orig.size());
}

template <typename To, typename From>
    requires (concepts::Pointer<From> && !concepts::Reference<From>)
ND std::basic_string<To>
convert(From orig)
{
    return convert<To>(orig, strlen<From>(orig));
}

#ifdef _WIN32
// I'm not writing template overloads for these. If you're still using code pages, you
// deserve to be punished a little.
ND RECODE_EXPORT std::wstring codepage_to_wide(int codepage, char    const *orig, size_t length);
ND RECODE_EXPORT std::wstring codepage_to_wide(int codepage, uint8_t const *orig, size_t length);
ND RECODE_EXPORT std::string  wide_to_codepage(int codepage, wchar_t const *orig, size_t length);
#endif

/*--------------------------------------------------------------------------------------*/
/* Instantiation prototypes. */

#define CONVERT(TO, FROM)       \
    template<> ND RECODE_EXPORT \
    std::basic_string<TO> convert<TO, FROM>(FROM const *orig, size_t length)
#define DECLARE_CONVERT(TYPE)                       \
    CONVERT(TYPE, char);    CONVERT(TYPE, wchar_t); \
    CONVERT(TYPE, char8_t); CONVERT(TYPE, char16_t); CONVERT(TYPE, char32_t)

DECLARE_CONVERT(char);
DECLARE_CONVERT(wchar_t);
DECLARE_CONVERT(char8_t);
DECLARE_CONVERT(char16_t);
DECLARE_CONVERT(char32_t);

#undef CONVERT
#undef DECLARE_CONVERT

/*--------------------------------------------------------------------------------------*/

#if 0 && !defined RECODE_WIN32_ONLY
namespace impl {

ND RECODE_EXPORT size_t mbsnlen (char     const *str, size_t size) noexcept;
#if defined WCHAR_IS_U16
ND RECODE_EXPORT size_t mbsnlen (wchar_t  const *str, size_t size) noexcept;
#elif defined WCHAR_IS_U32
ND RECODE_EXPORT size_t mbsnlen (wchar_t  const *str, size_t size) noexcept;
#endif

ND RECODE_EXPORT size_t mbsnlen (uint8_t  const *str, size_t size) noexcept;
ND RECODE_EXPORT size_t mbsnlen (uint16_t const *str, size_t size) noexcept;
ND RECODE_EXPORT size_t mbsnlen (uint32_t const *str, size_t size) noexcept;
ND RECODE_EXPORT size_t mbsnlen (char8_t  const *str, size_t size) noexcept;
ND RECODE_EXPORT size_t mbsnlen (char16_t const *str, size_t size) noexcept;
ND RECODE_EXPORT size_t mbsnlen (char32_t const *str, size_t size) noexcept;

} // namespace impl


template <typename Container>
    requires (concepts::Integral<typename Container::value_type>)
ND size_t
mbsnlen(Container const &orig) noexcept
{
    return impl::mbsnlen(orig.data(), orig.size());
}

template <typename Ty, size_t Len>
    requires (!concepts::Pointer<Ty> && concepts::Integral<Ty>)
ND size_t mbsnlen(Ty const (&str)[Len]) noexcept
{
    return impl::mbsnlen(str, Len - 1);
}

template <typename Ty>
    requires (!concepts::Pointer<Ty> && concepts::Integral<Ty>)
ND size_t mbsnlen(Ty const *str, size_t size) noexcept
{
    return impl::mbsnlen(str, size);
}

template <typename Ty>
    requires (concepts::Pointer<Ty> &&
              requires (Ty x) {{*x} -> concepts::Integral;})
ND size_t mbsnlen(std::remove_pointer_t<std::remove_cv_t<Ty>> const *str) noexcept
{
    return impl::mbsnlen(str, strlen(str));
}

#endif

/*--------------------------------------------------------------------------------------*/

class conversion_error : public std::runtime_error
{
    using base = std::runtime_error;
  public:
    explicit conversion_error(std::string const &msg) : base(msg.c_str()) {}
    explicit conversion_error(char const *msg) : base(msg) {}
};

/*--------------------------------------------------------------------------------------*/
} // namespace recode

// Let's not pollute the global namespace more than we have to.
#undef _STRLEN
#undef _WCSLEN
#undef RECODE_EXPORT
#pragma pop_macro("ND")
#if defined WCHAR_IS_U16
# undef WCHAR_IS_U16
#elif defined WCHAR_IS_U32
# undef WCHAR_IS_U32
#endif


/****************************************************************************************/
#endif
