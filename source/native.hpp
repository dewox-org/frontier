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
    auto format_integer_32bit(char buffer[11], int signed a) -> char*;  // -> last
    auto format_integer_64bit(char buffer[20], int long long signed a) -> char*;  // -> last
    auto format_unteger_32bit(char buffer[10], int unsigned a) -> char*;  // -> last
    auto format_unteger_64bit(char buffer[20], int long long unsigned a) -> char*;  // -> last
    auto format_float_32bit(char buffer[50], float a) -> char*;  // -> last
    auto format_float_64bit(char buffer[328], double a) -> char*;  // -> last
    auto format_hexadecimal_unteger_8bit(char result[2], char unsigned a) -> void;
    auto format_hexadecimal_unteger_64bit(char result[16], int long long unsigned a) -> void;

    auto parse_integer_32bit(int signed* result, char const* first, char const* last) -> Size;  // -> parsed_byte_count
    auto parse_integer_64bit(int long long signed* result, char const* first, char const* last) -> Size;  // -> parsed_byte_count
    auto parse_unteger_32bit(int unsigned* result, char const* first, char const* last) -> Size;  // -> parsed_byte_count
    auto parse_unteger_64bit(int long long unsigned* result, char const* first, char const* last) -> Size;  // -> parsed_byte_count
    auto parse_float_32bit(float* result, char const* first, char const* last) -> Size;  // -> parsed_byte_count
    auto parse_float_64bit(double* result, char const* first, char const* last) -> Size;  // -> parsed_byte_count
    auto parse_hexadecimal_unteger_8bit(char unsigned* result, char const text[2]) -> bool;  // -> ok
    auto parse_hexadecimal_unteger_64bit(int long long unsigned* result, char const text[16]) -> bool;  // -> ok
}

