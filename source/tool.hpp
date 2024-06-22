#pragma once

namespace dewox::inline tool
{
    using Traceable = auto (void* maybe_data, char const* maybe_message, char const* file, int line, char const* function) -> bool;
    auto trace(char const* maybe_message = {}, char const* file = __builtin_FILE(), int line = __builtin_LINE(), char const* function = __builtin_FUNCTION()) -> void;
    auto filter_trace(Traceable* maybe_traceable = {}, void* maybe_data = {}) -> void;  // Default to constant_traceable(maybe_data)
    auto constant_traceable(void* constant, char const* maybe_message, char const* file, int line, char const* function) -> bool;
    auto some_file_traceable(void* filename, char const* maybe_message, char const* file, int line, char const* function) -> bool;

    [[nodiscard]] constexpr auto min(auto a, decltype(a) b) -> decltype(a);
    [[nodiscard]] constexpr auto max(auto a, decltype(a) b) -> decltype(a);

    template <typename Object> struct Do_Self { using Type = Object; };
    template <typename Object> using Self = typename Do_Self<Object>::Type;

    // A non-reference non-const type. Rejects "T&" and "T const".
    template <typename Type> concept mutable_type = requires (Type* a, Type* b) { *a = *b; };

    template <mutable_type Type> constexpr auto exchange(Type* variable, Self<Type> new_value) -> Type;
    template <mutable_type Type> constexpr auto swap(Type* a, Type* b) -> void;
}

namespace dewox::inline tool
{
    inline constexpr auto min(auto a, decltype(a) b) -> decltype(a) { return (a < b ? a : b); }
    inline constexpr auto max(auto a, decltype(a) b) -> decltype(a) { return (a > b ? a : b); }

    template <mutable_type Type> inline constexpr auto exchange(Type* variable, Self<Type> new_value) -> Type { auto old_value = *variable; *variable = new_value; return old_value; }
    template <mutable_type Type> inline constexpr auto swap(Type* a, Type* b) -> void { exchange(b, exchange(a, *b)); }
}

