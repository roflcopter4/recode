#include "internal.hh"

namespace recode::impl {
/****************************************************************************************/

static char const *my_strerror(errno_t errval, char *buf, size_t buflen)
{
      char const *estr;
#if defined _WIN32
      (void)strerror_s(buf, buflen, errval);
      estr = buf;
#elif defined defined __GLIBC__ && defined _GNU_SOURCE && defined __USE_GNU
      estr = strerror_r(errval, buf, buflen);
#else
      (void)strerror_r(errval, buf, buflen);
      estr = buf;
#endif
      return estr;
}

[[noreturn]] void (throw_conversion_error)(int e, char const *from_name, int from_cp, char const *to_name, int to_cp, char const *impl_name)
{
    char errbuf[128];
    char const *eptr = my_strerror(e, errbuf, sizeof errbuf);
    auto msg = std::format(
        "Failed to convert {}{} string to {}-{}.\n0x{:08X}: \"{}\"\nConversion attempted via {} implementation.",
        from_name, from_cp, to_name, to_cp, e, eptr, impl_name);
    std::cerr << msg << '\n';
    std::cerr.flush();
    throw conversion_error(msg);
}

/****************************************************************************************/
} // namespace recode