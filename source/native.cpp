#include "native.hpp"
#include <cstdio>
#include <cstdarg>

namespace dewox::native
{
    auto printf(char const* format, ...) -> void
    {
        va_list ap;
        va_start(ap, format);
        std::vfprintf(stderr, format, ap);
        va_end(ap);
    }

    auto flush() -> void
    {
        std::fflush(stderr);
    }
}

