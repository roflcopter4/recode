#include "internal.hh"

#ifndef RECODE_WIN32_ONLY

#include <unistr.h>
/* "unistr.h" defines these extremely irritating macros. */
#undef true
#undef false
#undef uint8_t
#undef uint16_t
#undef uint32_t
#undef int8_t
#undef int16_t
#undef int32_t
#undef int64_t

#define throw_conversion_error(e, a, b) (::recode::impl::throw_conversion_error)((e), "UTF", (a), "UTF", (b), "libunistring")

namespace recode::impl::unistring {
/****************************************************************************************/

std::u16string char8_to_char16(char8_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    std::u16string str;
    size_t resultlen = length + SIZE_C(1);
    str.reserve(resultlen);

    auto *result = ::u8_to_u16(reinterpret_cast<uint8_t const *>(orig), length,
                               reinterpret_cast<uint16_t *>(str.data()), &resultlen);
    if (!result)
        throw_conversion_error(errno, 8, 16);

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}

std::u32string char8_to_char32(char8_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    std::u32string str;
    size_t resultlen = length + SIZE_C(1);
    str.reserve(resultlen);

    auto *result = ::u8_to_u32(reinterpret_cast<uint8_t const *>(orig), length,
                               reinterpret_cast<uint32_t *>(str.data()), &resultlen);
    if (!result)
        throw_conversion_error(errno, 8, 32);

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}

std::u8string char16_to_char8(char16_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    std::u8string str;
    size_t        resultlen = (length + SIZE_C(1)) * SIZE_C(2);

    for (bool again = true; again;) {
        str.reserve(resultlen + SIZE_C(1));
        size_t true_resultlen = resultlen;
        errno = 0;
        auto *result = ::u16_to_u8(reinterpret_cast<uint16_t const *>(orig), length,
                                   reinterpret_cast<uint8_t *>(str.data()), &true_resultlen);
        if (!result || errno)
            throw_conversion_error(errno, 16, 8);
        if (resultlen >= true_resultlen)
            again = false;
        resultlen = true_resultlen;
    }

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}

std::u32string char16_to_char32(char16_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    std::u32string str;
    size_t resultlen = (length + SIZE_C(1));
    str.reserve(resultlen);

    auto *result = ::u16_to_u32(reinterpret_cast<uint16_t const *>(orig), length,
                                reinterpret_cast<uint32_t *>(str.data()), &resultlen);
    if (!result)
        throw_conversion_error(errno, 16, 32);

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}

std::u8string char32_to_char8(char32_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    std::u8string str;
    size_t        resultlen = (length + SIZE_C(1)) * SIZE_C(4);

    for (bool again = true; again;) {
        str.reserve(resultlen + SIZE_C(1));
        size_t true_resultlen = resultlen;
        errno = 0;
        auto *result = ::u32_to_u8(reinterpret_cast<uint32_t const *>(orig), length,
                                   reinterpret_cast<uint8_t *>(str.data()), &true_resultlen);
        if (!result || errno)
            throw_conversion_error(errno, 32, 8);
        if (resultlen >= true_resultlen)
            again = false;
        resultlen = true_resultlen;
    }

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;

}

std::u16string char32_to_char16(char32_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    std::u16string str;
    size_t resultlen = (length + SIZE_C(1)) * SIZE_C(2);
    str.reserve(resultlen);

    auto *result = ::u32_to_u16(reinterpret_cast<uint32_t const *>(orig), length,
                                reinterpret_cast<uint16_t *>(str.data()), &resultlen);
    if (!result)
        throw_conversion_error(errno, 32, 16);

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}


/*--------------------------------------------------------------------------------------*/

std::string char16_to_char(char16_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    std::string str;
    size_t resultlen = (length + SIZE_C(1)) * SIZE_C(2);

    for (bool again = true; again;) {
        str.reserve(resultlen + SIZE_C(1));
        size_t true_resultlen = resultlen;
        errno = 0;
        auto *result = ::u16_to_u8(reinterpret_cast<uint16_t const *>(orig), length,
                                   reinterpret_cast<uint8_t *>(str.data()),
                                   &true_resultlen);
        if (!result || errno)
            throw_conversion_error(errno, 16, 8);
        if (resultlen >= true_resultlen)
            again = false;
        resultlen = true_resultlen;
    }

    str.data()[resultlen] = '\0';
    resize_string_hack(str, resultlen);
    return str;
}

#ifndef RECODE_WIN32_ONLY
std::string char32_to_char(char32_t const *orig, size_t length)
{
    if (length == 0)
        return {};

    std::string str;
    size_t      resultlen = (length + SIZE_C(1)) * SIZE_C(4);

    for (bool again = true; again;) {
        str.reserve(resultlen + SIZE_C(1));
        size_t true_resultlen = resultlen;
        errno = 0;
        auto *result = ::u32_to_u8(reinterpret_cast<uint32_t const *>(orig), length,
                                   reinterpret_cast<uint8_t *>(str.data()), &true_resultlen);
        if (!result || errno)
            throw_conversion_error(errno, 32, 8);
        if (resultlen >= true_resultlen)
            again = false;
        resultlen = true_resultlen;
    }

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}
#endif

/*--------------------------------------------------------------------------------------*/

std::wstring char8_to_wide(char8_t const *orig, size_t length)
{
    if (length == 0)
        return {};
#if defined WCHAR_IS_U16
    using uint_type = uint16_t;
    constexpr auto func = &::u8_to_u16;
    static constexpr int numbits = 16;
#elif defined WCHAR_IS_U32
    using uint_type = uint32_t;
    static constexpr int numbits = 32;
    constexpr auto func = &::u8_to_u32;
#endif
    std::wstring str;
    size_t resultlen = length + SIZE_C(1);
    str.reserve(resultlen);

    auto *result = func(reinterpret_cast<uint8_t const *>(orig), length,
                        reinterpret_cast<uint_type *>(str.data()), &resultlen);
    if (!result)
          throw_conversion_error(errno, 8, numbits);

    str.data()[resultlen] = '\0';
    resize_string_hack(str, resultlen);
    return str;
}

#ifndef WCHAR_IS_U16
std::wstring char16_to_wide(char16_t const *orig, size_t length)
{
    if (length == 0)
        return {};

    std::wstring str;
    size_t resultlen = length + SIZE_C(1);
    str.reserve(resultlen);

    auto *result = ::u16_to_u32(reinterpret_cast<uint16_t const *>(orig), length,
                                reinterpret_cast<uint32_t *>(str.data()), &resultlen);
    if (!result)
          throw_conversion_error(errno, 16, 32);

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}
#endif

#ifndef WCHAR_IS_U32
std::wstring char32_to_wide(char32_t const *orig, size_t length)
{
    if (length == 0)
        return {};

    std::wstring str;
    size_t resultlen = (length + SIZE_C(1)) * SIZE_C(2);
    str.reserve(resultlen);

    auto *result = ::u32_to_u16(reinterpret_cast<uint32_t const *>(orig), length,
                                reinterpret_cast<uint16_t *>(str.data()), &resultlen);
    if (!result)
        throw_conversion_error(errno, 32, 16);

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}
#endif

/****************************************************************************************/
} // namespace recode::impl::unistring
#endif