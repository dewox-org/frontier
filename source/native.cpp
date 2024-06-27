#include "native.hpp"
#include "tool.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

namespace dewox::native
{
    inline namespace
    {
        auto maybe_report_grow_memory = (Report_Grow_Memory*) nullptr;
        auto maybe_report_drop_memory = (Report_Drop_Memory*) nullptr;
        auto maybe_memory_probe_data = (void*) nullptr;
    }

    auto load() -> void
    {
        // Debuggers have higher priority to capture SIGTRAP signal.
        // When something goes wrong, we will raise SIGTRAP.
        // If a debugger is present, it captures the signal and will break the program right on spot.
        // Otherwise, we simply ignore it.
        ::signal(SIGTRAP, [] (int) {
            native::printf("[BAD] Something goes wrong. Attach a debugger for more details.\n");
        });

        // Just don't.
        ::signal(SIGPIPE, SIG_IGN);
    }

    auto unload() -> void
    {
        // Do nothing intentionally.
    }

    auto fatal() -> Fatal
    {
        // Something goes wrong, we raise SIGTRAP to ask a debugger (if there is one attached) to break the program here.
        ::raise(SIGTRAP);
        return {};
    }

    auto exit() -> void
    {
        ::_Exit(EXIT_SUCCESS);
    }

    auto printf(char const* format, ...) -> void
    {
        ::va_list ap;
        ::va_start(ap, format);
        ::vfprintf(stderr, format, ap);
        ::va_end(ap);
    }

    auto print_string(char const* maybe_string, Size byte_count) -> void
    {
        if (auto string = maybe_string) {
            ::fwrite(string, byte_count, 1u, stderr);
        } else {
            if (byte_count > 0u) {
                (void) fatal();
                return;
            }
        }
    }

    auto flush() -> void
    {
        ::fflush(stderr);
    }

    auto grow_memory(Size byte_count, Size alignment) -> char*
    {
        if (byte_count == 0u) {
            return fatal();
        }

        if ((alignment & (alignment - 1u)) != 0u) {
            return fatal();
        }

        if (auto memory = ::aligned_alloc(alignment, byte_count)) {
            if (auto report_grow_memory = maybe_report_grow_memory) {
                report_grow_memory(byte_count, alignment, (char*) memory, maybe_memory_probe_data);
            }
            return (char*) memory;
        } else {
            return fatal();
        }
    }

    auto drop_memory(void* maybe_memory) -> void
    {
        if (auto report_drop_memory = maybe_report_drop_memory) {
            report_drop_memory(maybe_memory, maybe_memory_probe_data);
        }
        ::free(maybe_memory);
    }

    auto probe_memory(Report_Grow_Memory** do_maybe_report_grow_memory, Report_Drop_Memory** do_maybe_report_drop_memory, void** do_maybe_data) -> void
    {
        swap(&maybe_report_grow_memory, do_maybe_report_grow_memory);
        swap(&maybe_report_drop_memory, do_maybe_report_drop_memory);
        swap(&maybe_memory_probe_data, do_maybe_data);
    }
}

namespace dewox::native
{
    inline namespace
    {
        extern "C"
        {
            auto dewox_external_itoa_format_integer_32bit(char buffer[11], int signed a) -> char*;
            auto dewox_external_itoa_format_integer_64bit(char buffer[20], int long long signed a) -> char*;
            auto dewox_external_itoa_format_unteger_32bit(char buffer[10], int unsigned a) -> char*;
            auto dewox_external_itoa_format_unteger_64bit(char buffer[20], int long long unsigned a) -> char*;
        }
    }

    auto format_integer_32bit(char buffer[11], int signed a) -> char* { return dewox_external_itoa_format_integer_32bit(buffer, a); }
    auto format_integer_64bit(char buffer[20], int long long signed a) -> char* { return dewox_external_itoa_format_integer_64bit(buffer, a); }
    auto format_unteger_32bit(char buffer[10], int unsigned a) -> char* { return dewox_external_itoa_format_unteger_32bit(buffer, a); }
    auto format_unteger_64bit(char buffer[20], int long long unsigned a) -> char* { return dewox_external_itoa_format_unteger_64bit(buffer, a); }
}

