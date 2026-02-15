#include <recode/recode.hh>

#include <cinttypes>
#include <concepts>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

using namespace std::literals;

#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# ifndef NOMINMAX
#  define NOMINMAX
# endif
# include <Windows.h>
#endif

/****************************************************************************************/


template <typename T> concept StringViewIsh = std::convertible_to<T, std::basic_string_view<std::iter_value_t<T>>>;

template <StringViewIsh OrigT, StringViewIsh DestT>
static bool check_same(wchar_t const *name, OrigT const &orig, DestT const &dest)
{
    auto conv = recode::convert<std::iter_value_t<OrigT>>(dest);
    bool ret  = orig == conv;

    std::basic_stringstream<std::iter_value_t<OrigT>> out;
    out << std::boolalpha
        << name << L'"' << conv.c_str() << L"\" -> "
        << ret << L'\n';

    auto str = out.str();
#ifdef _WIN32
    DWORD mode;
    if (::GetConsoleMode(::GetStdHandle(STD_OUTPUT_HANDLE), &mode))
        ::WriteConsoleW(::GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), static_cast<DWORD>(str.size()), nullptr, nullptr);
    else
        std::wcout << str;
#else
    std::cout << str;
#endif
    return ret;
}

int main()
{
#ifdef _WIN32
    auto orig = L"😒👍💕😂👌😁😍"s;
#else
    auto orig = "😒👍💕😂👌😁😍"s
#endif

    std::string    str    = recode::convert<char>(orig);
    std::wstring   wstr   = recode::convert<wchar_t>(orig);
    std::u8string  u8str  = recode::convert<char8_t>(orig);
    std::u16string u16str = recode::convert<char16_t>(orig);
    std::u32string u32str = recode::convert<char32_t>(orig);

    bool ret = true;
    ret = check_same(L"char:   ", orig, str) && ret;
    ret = check_same(L"wchar:  ", orig, wstr) && ret;
    ret = check_same(L"char8:  ", orig, u8str) && ret;
    ret = check_same(L"char16: ", orig, u16str) && ret;
    ret = check_same(L"char32: ", orig, u32str) && ret;

    return ret ? 0 : 1;
}
