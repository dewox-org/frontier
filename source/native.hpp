#pragma once
#include "type.hpp"

namespace dewox::native
{
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
}

