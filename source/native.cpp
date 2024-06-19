#include "native.hpp"
#include "tool.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

namespace dewox::native
{
    auto load() -> void
    {
        // Debuggers have higher priority to capture SIGTRAP signal.
        // When something goes wrong, we will raise SIGTRAP.
        // If a debugger is present, it captures the signal and will break the program right on spot.
        // Otherwise, we simply ignore it.
        ::signal(SIGTRAP, [] (int) { ::_Exit(EXIT_FAILURE); });

        // Just don't.
        ::signal(SIGPIPE, SIG_IGN);
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
        // Something goes wrong, we raise SIGTRAP to ask a debugger (if there is one attached) to break the program here.
        ::raise(SIGTRAP);
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
            return nullptr;
        }

        if ((alignment & (alignment - 1u)) != 0u) {
            fatal();
            return nullptr;
        }

        if (auto memory = ::aligned_alloc(alignment, byte_count)) {
            return (char*) memory;
        } else {
            fatal();
            return nullptr;
        }
    }

    auto drop_memory(void* maybe_memory) -> void
    {
        ::free(maybe_memory);
    }
}

