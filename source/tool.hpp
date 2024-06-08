#pragma once

namespace dewox::inline tool
{
    using Traceable = auto (void* opt_data, char const* opt_message, char const* file, int line, char const* function) -> bool;
    auto trace(char const* opt_message = {}, char const* file = __builtin_FILE(), int line = __builtin_LINE(), char const* function = __builtin_FUNCTION()) -> void;
    auto filter_trace(Traceable* opt_traceable = {}, void* opt_data = {}) -> void;  // Default to constant_traceable(opt_data)
    auto constant_traceable(void* constant, char const* opt_message, char const* file, int line, char const* function) -> bool;
    auto some_file_traceable(void* filename, char const* opt_message, char const* file, int line, char const* function) -> bool;

    [[nodiscard]] constexpr auto min(auto a, decltype(a) b) -> decltype(a);
    [[nodiscard]] constexpr auto max(auto a, decltype(a) b) -> decltype(a);

    template <typename Object> struct Do_Self final { using Type = Object; };
    template <typename Object> using Self = typename Do_Self<Object>::Type;
}

namespace dewox::inline tool
{
    inline constexpr auto min(auto a, decltype(a) b) -> decltype(a) { return (a < b ? a : b); }
    inline constexpr auto max(auto a, decltype(a) b) -> decltype(a) { return (a > b ? a : b); }
}

