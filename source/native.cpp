#include "native.hpp"
#include "tool.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

namespace dewox::native
{
    auto load() -> void
    {
        // Do nothing intentionally.
    }

    auto unload() -> void
    {
        // Do nothing intentionally.
    }

    auto exit() -> void
    {
        ::_Exit(EXIT_SUCCESS);
    }

    auto fatal() -> void
    {
        ::_Exit(EXIT_FAILURE);
    }

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

    auto grow_memory(Size byte_count, Size alignment) -> char*
    {
        if (byte_count == 0u) {
            fatal();
        }

        if ((alignment & (alignment - 1u)) != 0u) {
            fatal();
        }

        if (auto memory = ::aligned_alloc(alignment, byte_count)) {
            return (char*) memory;
        } else {
            fatal();
        }
    }

    auto drop_memory(void* maybe_memory) -> void
    {
        ::free(maybe_memory);
    }
}

