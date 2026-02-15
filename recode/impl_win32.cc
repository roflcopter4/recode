#include "internal.hh"

namespace recode::impl::win32 {
/****************************************************************************************/

static int64_t UTF16_to_UTF8_size(wchar_t const *unicode_string, size_t length, int cp = CP_UTF8)
{
    int size = ::WideCharToMultiByte(
        cp, 0,
        unicode_string, static_cast<int>(length + 1),
        nullptr, 0, nullptr, nullptr
    );
    return size > 0 ? size : -static_cast<int64_t>(GetLastError());
}

static int64_t UTF16_to_UTF8_size(char16_t const *unicode_string, size_t length, int cp = CP_UTF8)
{
    return UTF16_to_UTF8_size(reinterpret_cast<wchar_t const *>(unicode_string), length, cp);
}

[[noreturn]] static void
throw_conversion_error_win32(char const *from_name, int from_cp, char const *to_name, int to_cp, char const *impl_name = "Win32")
{
    DWORD e    = ::GetLastError();
    auto ecode = std::error_code{static_cast<int>(e), std::system_category()};
    auto msg = std::format(
        "Failed to convert {}{} string to {}-{}.\n0x{:08X}: \"{}\"\nConversion attempted via {} implementation.",
        from_name, from_cp, to_name, to_cp, e, ecode.message(), impl_name);
    ::MessageBoxA(nullptr, msg.c_str(), "Error", MB_OK | MB_ICONHAND);
    throw conversion_error(msg);
}

#define throw_unicode_conversion_error_win32(a, b) \
    (throw_conversion_error_win32)("UTF", (a), "UTF", (b))

/*--------------------------------------------------------------------------------------*/

std::wstring char8_to_wide(char8_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    if (length > INT_MAX) [[unlikely]]
        throw std::runtime_error("Cannot convert strings larger than 2147483647");

    std::wstring str;
    size_t resultlen = length;
    str.reserve(resultlen + SIZE_C(1));

    resultlen = ::MultiByteToWideChar(
        CP_UTF8, MB_ERR_INVALID_CHARS,
        reinterpret_cast<LPCSTR>(orig), static_cast<int>(length),
        str.data(), static_cast<int>(resultlen)
    );
    if (resultlen == 0)
        throw_unicode_conversion_error_win32(8, 16);

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}

std::u16string char8_to_char16(char8_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    if (length > INT_MAX) [[unlikely]]
        throw std::runtime_error("Cannot convert strings larger than 2147483647");

    std::u16string str;
    size_t resultlen = length;
    str.reserve(resultlen + SIZE_C(1));

    resultlen = ::MultiByteToWideChar(
        CP_UTF8, MB_ERR_INVALID_CHARS,
        reinterpret_cast<LPCSTR>(orig), static_cast<int>(length),
        reinterpret_cast<LPWSTR>(str.data()), static_cast<int>(resultlen)
    );
    if (resultlen == 0)
        throw_unicode_conversion_error_win32(8, 16);

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}

/*--------------------------------------------------------------------------------------*/

std::string char16_to_char(char16_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    if (length > INT_MAX) [[unlikely]]
        throw std::runtime_error("Cannot convert strings larger than 2147483647");

    size_t resultlen = UTF16_to_UTF8_size(orig, length) - SIZE_C(1);
    if (int64_t(resultlen) < 0)
        throw_unicode_conversion_error_win32(8, 16);
    std::string str;
    str.reserve(resultlen + SIZE_C(1));

    int result = ::WideCharToMultiByte(
        CP_UTF8, WC_ERR_INVALID_CHARS,
        reinterpret_cast<LPCWSTR>(orig), static_cast<int>(length),
        str.data(), static_cast<int>(resultlen),
        nullptr, nullptr
    );
    if (result == 0)
        throw_unicode_conversion_error_win32(8, 16);

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}

std::u8string char16_to_char8(char16_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    if (length > INT_MAX) [[unlikely]]
        throw std::runtime_error("Cannot convert strings larger than 2147483647");

    size_t resultlen = UTF16_to_UTF8_size(orig, length) - SIZE_C(1);
    if (int64_t(resultlen) < 0)
        throw_unicode_conversion_error_win32(8, 16);
    std::u8string str;
    str.reserve(resultlen + SIZE_C(1));

    int result = ::WideCharToMultiByte(
        CP_UTF8, WC_ERR_INVALID_CHARS,
        reinterpret_cast<LPCWSTR>(orig), static_cast<int>(length),
        reinterpret_cast<LPSTR>(str.data()), static_cast<int>(resultlen),
        nullptr, nullptr
    );
    if (result == 0)
        throw_unicode_conversion_error_win32(8, 16);

    str.data()[resultlen] = 0;
    resize_string_hack(str, resultlen);
    return str;
}

/****************************************************************************************/
} // namespace recode::impl::win32

namespace recode {
/****************************************************************************************/

using impl::win32::throw_conversion_error_win32;

EXPORT std::wstring codepage_to_wide(int codepage, uint8_t const *orig, size_t length)
{
    return codepage_to_wide(codepage, reinterpret_cast<char const *>(orig), length);
}

EXPORT std::wstring codepage_to_wide(int codepage, char const *orig, size_t length)
{
    if (length == 0)
        return {};
    if (length > INT_MAX) [[unlikely]]
        throw std::runtime_error("Cannot convert strings larger than 2147483647");

    std::wstring str;
    size_t resultlen = length;
    str.reserve(resultlen + SIZE_C(1));

    resultlen = ::MultiByteToWideChar(
        codepage, 0,
        orig, static_cast<int>(length),
        str.data(), static_cast<int>(resultlen)
    );
    if (resultlen == 0)
        throw_conversion_error_win32("CP", codepage, "UTF", 16);

    str.data()[resultlen] = 0;
    impl::resize_string_hack(str, resultlen);
    return str;
}

EXPORT std::string wide_to_codepage(int codepage, wchar_t const *orig, size_t length)
{
    if (length == 0)
        return {};
    if (length > INT_MAX) [[unlikely]]
        throw std::runtime_error("Cannot convert strings larger than 2147483647");

    std::string str;
    size_t resultlen = impl::win32::UTF16_to_UTF8_size(orig, length, codepage) - SIZE_C(1);
    if (static_cast<int64_t>(resultlen) < 0)
        throw_conversion_error_win32("UTF", 16, "CP", codepage);
    str.reserve(resultlen + SIZE_C(1));

    int result = ::WideCharToMultiByte(
        codepage, 0,
        orig, static_cast<int>(length),
        str.data(), static_cast<int>(resultlen),
        "?", nullptr
    );
    if (result == 0)
        throw_conversion_error_win32("UTF", 16, "CP", codepage);

    str.data()[resultlen] = 0;
    impl::resize_string_hack(str, resultlen);
    return str;
}

/****************************************************************************************/
} // namespace recode