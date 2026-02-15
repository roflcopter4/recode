#include "internal.hh"
#include <iconv.h>

#ifndef RECODE_WIN32_ONLY

#define throw_conversion_error(e, a, b) (::recode::impl::throw_conversion_error)((e), "UTF", (a), "UTF", (b), "iconv")

namespace recode::impl::libiconv {
/****************************************************************************************/

class iconv_wrapper
{
  public:
    iconv_wrapper(const char *tocode, const char *fromcode)
        : cd_(::iconv_open(tocode, fromcode))
    {
        if (cd_ == reinterpret_cast<::iconv_t>(-1)) [[unlikely]]
            throw std::runtime_error("Failed to create iconv conversion descriptor");
    }

    ~iconv_wrapper()
    {
        if (cd_ != reinterpret_cast<::iconv_t>(-1))
            ::iconv_close(cd_);
    }

    iconv_wrapper(iconv_wrapper const &)                = delete;
    iconv_wrapper(iconv_wrapper &&) noexcept            = delete;
    iconv_wrapper &operator=(iconv_wrapper const &)     = delete;
    iconv_wrapper &operator=(iconv_wrapper &&) noexcept = delete;

    [[nodiscard]] ::iconv_t get() const { return cd_; }

  private:
    ::iconv_t cd_;
};

std::u16string char8_to_char16(char8_t const *orig, size_t length)
{
    std::u16string str;
    size_t destlen = length + SIZE_C(1);
    str.reserve(destlen);

    size_t origlen_ptr = length  * sizeof(char8_t);
    size_t destlen_ptr = destlen * sizeof(wchar_t);

    auto *dest = str.data();
    auto cd = iconv_wrapper("UTF-16LE", "UTF-8");
    size_t result = ::iconv(cd.get(), reinterpret_cast<char **>(const_cast<char8_t **>(&orig)), &origlen_ptr, reinterpret_cast<char **>(&dest), &destlen_ptr);
    if (result == static_cast<size_t>(-1))
        throw_conversion_error(errno, 8, 16);
    destlen = destlen - destlen_ptr / sizeof(wchar_t);

    str.data()[destlen] = '\0';
    resize_string_hack(str, destlen);
    return str;
}

std::u32string char8_to_char32(char8_t const *orig, size_t length)
{
    std::u32string str;
    size_t destlen = length + SIZE_C(1);
    str.reserve(destlen);

    size_t origlen_ptr = length  * sizeof(char8_t);
    size_t destlen_ptr = destlen * sizeof(char32_t);

    auto *dest = str.data();
    auto cd = iconv_wrapper("UTF-32LE", "UTF-8");
    size_t result = ::iconv(cd.get(), reinterpret_cast<char **>(const_cast<char8_t **>(&orig)), &origlen_ptr, reinterpret_cast<char **>(&dest), &destlen_ptr);
    if (result == static_cast<size_t>(-1))
        throw_conversion_error(errno, 8, 32);
    destlen = destlen - destlen_ptr / sizeof(char32_t);

    str.data()[destlen] = '\0';
    resize_string_hack(str, destlen);
    return str;
}

std::u8string char16_to_char8(char16_t const *orig, size_t length)
{
    std::u8string str;
    size_t destlen = (length * SIZE_C(2)) + SIZE_C(1);
    str.reserve(destlen);

    size_t origlen_ptr = length  * sizeof(char16_t);
    size_t destlen_ptr = destlen * sizeof(char8_t);

    auto *dest = str.data();
    auto cd = iconv_wrapper("UTF-8", "UTF-16LE");
    size_t result = ::iconv(cd.get(), reinterpret_cast<char **>(const_cast<char16_t **>(&orig)), &origlen_ptr, reinterpret_cast<char **>(&dest), &destlen_ptr);
    if (result == static_cast<size_t>(-1))
        throw_conversion_error(errno, 16, 8);
    destlen = destlen - destlen_ptr / sizeof(char8_t);

    str.data()[destlen] = '\0';
    resize_string_hack(str, destlen);
    return str;
}

std::u32string char16_to_char32(char16_t const *orig, size_t length)
{
    std::u32string str;
    size_t destlen = length + SIZE_C(1);
    str.reserve(destlen);

    size_t origlen_ptr = length  * sizeof(char16_t);
    size_t destlen_ptr = destlen * sizeof(char32_t);

    auto *dest = str.data();
    auto cd = iconv_wrapper("UTF-32LE", "UTF-16LE");
    size_t result = ::iconv(cd.get(), reinterpret_cast<char **>(const_cast<char16_t **>(&orig)), &origlen_ptr, reinterpret_cast<char **>(&dest), &destlen_ptr);
    if (result == static_cast<size_t>(-1))
        throw_conversion_error(errno, 16, 32);
    destlen = destlen - destlen_ptr / sizeof(char32_t);

    str.data()[destlen] = '\0';
    resize_string_hack(str, destlen);
    return str;
}

std::u8string char32_to_char8(char32_t const *orig, size_t length)
{
    std::u8string str;
    size_t destlen = (length * 4) + 1;
    str.reserve(destlen);

    size_t origlen_ptr = length  * sizeof(char32_t);
    size_t destlen_ptr = destlen * sizeof(char8_t);

    auto *dest = str.data();
    auto cd = iconv_wrapper("UTF-8", "UTF-32LE");
    size_t result = ::iconv(cd.get(), reinterpret_cast<char **>(const_cast<char32_t **>(&orig)), &origlen_ptr, reinterpret_cast<char **>(&dest), &destlen_ptr);
    if (result == static_cast<size_t>(-1))
        throw_conversion_error(errno, 32, 8);
    destlen = destlen - destlen_ptr / sizeof(char8_t);

    str.data()[destlen] = '\0';
    resize_string_hack(str, destlen);
    return str;
}

std::u16string char32_to_char16(char32_t const *orig, size_t length)
{
    std::u16string str;
    size_t destlen = (length * SIZE_C(2)) + SIZE_C(1);
    str.reserve(destlen);

    size_t origlen_ptr = length  * sizeof(char32_t);
    size_t destlen_ptr = destlen * sizeof(char16_t);

    auto *dest = str.data();
    auto cd = iconv_wrapper("UTF-16LE", "UTF-32LE");
    size_t result = ::iconv(cd.get(), reinterpret_cast<char **>(const_cast<char32_t **>(&orig)), &origlen_ptr, reinterpret_cast<char **>(&dest), &destlen_ptr);
    if (result == static_cast<size_t>(-1))
        throw_conversion_error(errno, 32, 16);
    destlen = destlen - destlen_ptr / sizeof(char16_t);

    str.data()[destlen] = '\0';
    resize_string_hack(str, destlen);
    return str;
}

/*--------------------------------------------------------------------------------------*/

std::string char16_to_char(char16_t const *orig, size_t length)
{
    std::string str;
    size_t destlen = (length * SIZE_C(2)) + SIZE_C(1);
    str.reserve(destlen);

    size_t origlen_ptr = length  * sizeof(char16_t);
    size_t destlen_ptr = destlen * sizeof(char);

    auto *dest = str.data();
    auto cd = iconv_wrapper("UTF-8", "UTF-16LE");
    size_t result = ::iconv(cd.get(), reinterpret_cast<char **>(const_cast<char16_t **>(&orig)), &origlen_ptr, &dest, &destlen_ptr);
    if (result == static_cast<size_t>(-1))
        throw_conversion_error(errno, 16, 8);
    destlen = destlen - destlen_ptr / sizeof(char);

    str.data()[destlen] = '\0';
    resize_string_hack(str, destlen);
    return str;
}

std::string char32_to_char(char32_t const *orig, size_t length)
{
    std::string str;
    size_t destlen = (length * SIZE_C(4)) + SIZE_C(1);
    str.reserve(destlen);

    size_t origlen_ptr = length  * sizeof(char32_t);
    size_t destlen_ptr = destlen * sizeof(char);

    auto *dest = str.data();
    auto cd = iconv_wrapper("UTF-8", "UTF-32LE");
    size_t result = ::iconv(cd.get(), reinterpret_cast<char **>(const_cast<char32_t **>(&orig)), &origlen_ptr, &dest, &destlen_ptr);
    if (result == static_cast<size_t>(-1))
        throw_conversion_error(errno, 32, 8);
    destlen = destlen - destlen_ptr / sizeof(char);

    str.data()[destlen] = '\0';
    resize_string_hack(str, destlen);
    return str;
}

/*--------------------------------------------------------------------------------------*/


std::wstring char8_to_wide(char8_t const *orig, size_t length)
{
    if (length == 0)
        return {};
#if defined WCHAR_IS_U16
    static constexpr char to_name[] = "UTF-16LE";
    static constexpr int numbits = 16;
#elif defined WCHAR_IS_U32
    static constexpr char to_name[] = "UTF-32LE";
    static constexpr int numbits = 32;
#endif
    std::wstring str;
    size_t destlen = length + SIZE_C(1);
    str.reserve(destlen);

    size_t origlen_ptr = length * sizeof(char8_t);
    size_t destlen_ptr = destlen * sizeof(wchar_t);

    auto *dest = str.data();
    auto cd = iconv_wrapper(to_name, "UTF-8");
    size_t result = ::iconv(cd.get(), reinterpret_cast<char **>(const_cast<char8_t **>(&orig)), &origlen_ptr, reinterpret_cast<char **>(&dest), &destlen_ptr);
    if (result == static_cast<size_t>(-1))
        throw_conversion_error(errno, 8, numbits);
    destlen = destlen - destlen_ptr / sizeof(wchar_t);

    str.data()[destlen] = '\0';
    resize_string_hack(str, destlen);
    return str;
}

#ifndef WCHAR_IS_U16
std::wstring char16_to_wide(char16_t const *orig, size_t length)
{
    std::wstring str;
    size_t destlen = length + SIZE_C(1);
    str.reserve(destlen);

    size_t origlen_ptr = length  * sizeof(char16_t);
    size_t destlen_ptr = destlen * sizeof(wchar_t);

    auto *dest = str.data();
    auto cd = iconv_wrapper("UTF-32LE", "UTF-16LE");
    size_t result = ::iconv(cd.get(), reinterpret_cast<char **>(const_cast<char16_t **>(&orig)), &origlen_ptr, reinterpret_cast<char **>(&dest), &destlen_ptr);
    if (result == static_cast<size_t>(-1))
        throw_conversion_error(errno, 16, 32);
    destlen = destlen - destlen_ptr / sizeof(wchar_t);

    str.data()[destlen] = '\0';
    resize_string_hack(str, destlen);
    return str;
}
#endif

#ifndef WCHAR_IS_U32
std::wstring char32_to_wide(char32_t const *orig, size_t length)
{
    std::wstring str;
    size_t destlen = (length * SIZE_C(2)) + SIZE_C(1);
    str.reserve(destlen);

    size_t origlen_ptr = length  * sizeof(char32_t);
    size_t destlen_ptr = destlen * sizeof(wchar_t);

    auto *dest = str.data();
    auto cd = iconv_wrapper("UTF-16LE", "UTF-32LE");
    size_t result = ::iconv(cd.get(), reinterpret_cast<char **>(const_cast<char32_t **>(&orig)), &origlen_ptr, reinterpret_cast<char **>(&dest), &destlen_ptr);
    if (result == static_cast<size_t>(-1))
        throw_conversion_error(errno, 32, 16);
    destlen = destlen - destlen_ptr / sizeof(wchar_t);

    str.data()[destlen] = '\0';
    resize_string_hack(str, destlen);
    return str;
}
#endif

/****************************************************************************************/
} // namespace recode::impl::libiconv
#endif