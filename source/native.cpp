#include "native.hpp"
#include "tool.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "requires little endian.");

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
            auto dewox_external_itoa_format_integer_32bit(char buffer[11], int signed a) -> char*;  // -> last
            auto dewox_external_itoa_format_integer_64bit(char buffer[20], int long long signed a) -> char*;  // -> last
            auto dewox_external_itoa_format_unteger_32bit(char buffer[10], int unsigned a) -> char*;  // -> last
            auto dewox_external_itoa_format_unteger_64bit(char buffer[20], int long long unsigned a) -> char*;  // -> last

            auto dewox_external_dragonbox_format_float_32bit(char buffer[50], float a) -> char*;  // -> last
            auto dewox_external_dragonbox_format_float_64bit(char buffer[328], double a) -> char*;  // -> last

            auto dewox_external_fast_float_parse_float_32bit(float* result, char const* first, char const* last) -> char const*;  // -> first
            auto dewox_external_fast_float_parse_float_64bit(double* result, char const* first, char const* last) -> char const*;  // -> first
            auto dewox_external_fast_float_parse_unteger_32bit(int unsigned* result, char const* first, char const* last) -> char const*;  // -> first
            auto dewox_external_fast_float_parse_unteger_64bit(int long long unsigned* result, char const* first, char const* last) -> char const*;  // -> first
            auto dewox_external_fast_float_parse_integer_32bit(int signed* result, char const* first, char const* last) -> char const*;  // -> first
            auto dewox_external_fast_float_parse_integer_64bit(int long long signed* result, char const* first, char const* last) -> char const*;  // -> first
            auto dewox_external_fast_float_parse_hexadecimal_unteger_8bit(char unsigned* result, char const text[2]) -> bool;  // -> ok
            auto dewox_external_fast_float_parse_hexadecimal_unteger_64bit(int long long unsigned* result, char const text[16]) -> bool;  // -> ok
        }

        auto format_hexadecimal_digit(char** result, char unsigned a) -> void
        {
            result[0]++[0] = "0123456789abcdef"[a];
        }

        auto do_format_hexadecimal_unteger_8bit(char** result, char unsigned a) -> void
        {
            format_hexadecimal_digit(result, a >> 4u);
            format_hexadecimal_digit(result, a & 0x0fu);
        }

        auto do_format_hexadecimal_unteger_32bit(char** result, int unsigned a) -> void
        {
            auto p = (char unsigned*) &a;
            do_format_hexadecimal_unteger_8bit(result, p[3]);
            do_format_hexadecimal_unteger_8bit(result, p[2]);
            do_format_hexadecimal_unteger_8bit(result, p[1]);
            do_format_hexadecimal_unteger_8bit(result, p[0]);
        }

        auto do_format_hexadecimal_unteger_64bit(char** result, int long long unsigned a) -> void
        {
            auto p = (int unsigned*) &a;
            do_format_hexadecimal_unteger_32bit(result, p[1]);
            do_format_hexadecimal_unteger_32bit(result, p[0]);
        }
    }

    auto format_integer_32bit(char buffer[11], int signed a) -> char* { return dewox_external_itoa_format_integer_32bit(buffer, a); }
    auto format_integer_64bit(char buffer[20], int long long signed a) -> char* { return dewox_external_itoa_format_integer_64bit(buffer, a); }
    auto format_unteger_32bit(char buffer[10], int unsigned a) -> char* { return dewox_external_itoa_format_unteger_32bit(buffer, a); }
    auto format_unteger_64bit(char buffer[20], int long long unsigned a) -> char* { return dewox_external_itoa_format_unteger_64bit(buffer, a); }
    auto format_float_32bit(char buffer[50], float a) -> char* { return dewox_external_dragonbox_format_float_32bit(buffer, a); }
    auto format_float_64bit(char buffer[328], double a) -> char* { return dewox_external_dragonbox_format_float_64bit(buffer, a); }
    auto format_hexadecimal_unteger_8bit(char result[2], char unsigned a) -> void { do_format_hexadecimal_unteger_8bit(&result, a); }
    auto format_hexadecimal_unteger_64bit(char result[16], int long long unsigned a) -> void { do_format_hexadecimal_unteger_64bit(&result, a); }

    auto parse_integer_32bit(int signed* result, char const* first, char const* last) -> Size { return Size(dewox_external_fast_float_parse_integer_32bit(result, first, last) - first); }
    auto parse_integer_64bit(int long long signed* result, char const* first, char const* last) -> Size { return Size(dewox_external_fast_float_parse_integer_64bit(result, first, last) - first); }
    auto parse_unteger_32bit(int unsigned* result, char const* first, char const* last) -> Size { return Size(dewox_external_fast_float_parse_unteger_32bit(result, first, last) - first); }
    auto parse_unteger_64bit(int long long unsigned* result, char const* first, char const* last) -> Size { return Size(dewox_external_fast_float_parse_unteger_64bit(result, first, last) - first); }
    auto parse_float_32bit(float* result, char const* first, char const* last) -> Size { return Size(dewox_external_fast_float_parse_float_32bit(result, first, last) - first); }
    auto parse_float_64bit(double* result, char const* first, char const* last) -> Size { return Size(dewox_external_fast_float_parse_float_64bit(result, first, last) - first); }
    auto parse_hexadecimal_unteger_8bit(char unsigned* result, char const text[2]) -> bool { return dewox_external_fast_float_parse_hexadecimal_unteger_8bit(result, text); }
    auto parse_hexadecimal_unteger_64bit(int long long unsigned* result, char const text[16]) -> bool { return dewox_external_fast_float_parse_hexadecimal_unteger_64bit(result, text); }
}

