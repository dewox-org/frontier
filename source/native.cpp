#include "native.hpp"
#include <stdio.h>
#include <stdarg.h>

namespace dewox::native
{
    auto printf(char const* format, ...) -> void
    {
        ::va_list ap;
        ::va_start(ap, format);
        ::vfprintf(stderr, format, ap);
        ::va_end(ap);
    }

    auto flush() -> void
    {
        ::fflush(stderr);
    }
}

