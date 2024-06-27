#pragma once
#include "type.hpp"

namespace dewox::native
{
    using Report_Grow_Memory = auto (Size byte_count, Size alignment, char* memory, void* maybe_data) -> void;
    using Report_Drop_Memory = auto (void* maybe_memory, void* maybe_data) -> void;

    auto load() -> void;
    auto unload() -> void;

    struct Fatal { template <typename Type> operator Type () { return Type{}; } };
    [[nodiscard]] auto fatal() -> Fatal;  // fatal() never cause the program to exit. Always return 0.
    [[noreturn]] auto exit() -> void;

    __attribute__((__format__(printf, 1, 2))) auto printf(char const* format, ...) -> void;
    auto print_string(char const* maybe_string, Size byte_count) -> void;
    auto flush() -> void;

    // Growing and dropping memory will either success or crash. They never fail.
    // Aligning to non-power-of-two is impossible and will result in a crash.
    [[nodiscard]] auto grow_memory(Size byte_count, Size alignment = 16u) -> char*;
    auto drop_memory(void* maybe_memory) -> void;
    auto probe_memory(Report_Grow_Memory** do_maybe_report_grow_memory, Report_Drop_Memory** do_maybe_report_drop_memory, void** do_maybe_data) -> void;
}

// Convert between a number and a text.
namespace dewox::native
{
    auto format_integer_32bit(char buffer[11], int signed a) -> char*;
    auto format_integer_64bit(char buffer[20], int long long signed a) -> char*;
    auto format_unteger_32bit(char buffer[10], int unsigned a) -> char*;
    auto format_unteger_64bit(char buffer[20], int long long unsigned a) -> char*;
    auto format_float_32bit() -> void;
    auto format_float_64bit() -> void;
    auto format_hexadecimal_unteger_8bit() -> void;
    auto format_hexadecimal_unteger_64bit() -> void;

    auto parse_integer_32bit() -> void;
    auto parse_integer_64bit() -> void;
    auto parse_unteger_32bit() -> void;
    auto parse_unteger_64bit() -> void;
    auto parse_float_32bit() -> void;
    auto parse_float_64bit() -> void;
    auto parse_hexadecimal_unteger_8bit() -> void;
    auto parse_hexadecimal_unteger_64bit() -> void;
}

