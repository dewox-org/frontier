#pragma once

namespace dewox::native
{
    __attribute__((__format__(printf, 1, 2))) auto printf(char const* format, ...) -> void;
    auto flush() -> void;
}

