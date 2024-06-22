#pragma once
#include "type.hpp"

namespace dewox::native
{
    using Report_Grow_Memory = auto (Size byte_count, Size alignment, char* memory, void* maybe_data) -> void;
    using Report_Drop_Memory = auto (void* maybe_memory, void* maybe_data) -> void;

    auto load() -> void;
    auto unload() -> void;

    [[noreturn]] auto exit() -> void;
    auto fatal() -> void;

    __attribute__((__format__(printf, 1, 2))) auto printf(char const* format, ...) -> void;
    auto flush() -> void;

    // Growing and dropping memory will either success or crash. They never fail.
    // Aligning to non-power-of-two is impossible and will result in a crash.
    [[nodiscard]] auto grow_memory(Size byte_count, Size alignment = 16u) -> char*;
    auto drop_memory(void* maybe_memory) -> void;
    auto probe_memory(Report_Grow_Memory** do_maybe_report_grow_memory, Report_Drop_Memory** do_maybe_report_drop_memory, void** do_maybe_data) -> void;
}

