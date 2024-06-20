#pragma once
#include "native.hpp"
#include "object.hpp"
#include "tool.hpp"
#include "type.hpp"

namespace dewox::inline string
{
    struct String final
    {
        /// You must ensure first <= last on your own. It is undefined behavior otherwise.

        char const* first;
        char const* last;

        static constexpr auto into(String* result, char const* first, char const* last) -> void;
        static constexpr auto size_into(String* result, char const* first, Size size) -> void;
        static constexpr auto until_into(String* result, char const* maybe_native_string) -> void;
        static auto item_into(String* result, auto const* item) -> void;

        constexpr auto begin() -> char const*;
        constexpr auto end() -> char const*;

        constexpr auto byte_count() -> Size;
        constexpr explicit operator bool ();

        constexpr auto prefix(Size byte_count_after_first_to_keep = {}) -> String;
        constexpr auto suffix(Size byte_count_before_last_to_keep = {}) -> String;
        constexpr auto skip(Size byte_count_after_first_to_skip = {}) -> String;
        constexpr auto pop(Size byte_count_before_last_to_pop = {}) -> String;

        constexpr auto copy(String source) -> String;   // -> written_string
        constexpr auto fill(int a) -> String;           // -> filled_string

        constexpr auto starts_with(String maybe_prefix) -> bool;
        constexpr auto ends_with(String maybe_suffix) -> bool;

        // pattern = body? definition*;
        // definition = ":" <name:byte> body?;
        // body = sequence? branch*;
        // branch = "|" sequence;
        // sequence = command+;
        // command = "~" <from:byte> <to:byte> | "@" <name:byte> | [^~@:|]
        auto match(String pattern) -> Size;     // Returns the max byte count of the string prefix that matches the pattern.
        auto search(String pattern) -> String;  // Returns the first longest substring that matches the pattern.

        auto check_bounds() -> void;
    };

    constexpr auto operator == (String maybe_a, String maybe_b) -> bool;
    constexpr auto operator != (String maybe_a, String maybe_b) -> bool;
}

namespace dewox::inline string::literal
{
    constexpr auto operator ""_s (char const* first, Size byte_count) -> String;
}

namespace dewox
{
    using namespace ::dewox::string::literal;
}

namespace dewox::inline string
{
    inline constexpr auto String::into(String* result, char const* first, char const* last) -> void
    {
        result->first = first;
        result->last = last;
    }

    inline constexpr auto String::size_into(String* result, char const* first, Size size) -> void
    {
        into(result, first, first + size);
    }

    inline constexpr auto String::until_into(String* result, char const* maybe_native_string) -> void
    {
        if (auto native_string = maybe_native_string) {
            while (*native_string) native_string++;
            into(result, maybe_native_string, native_string);
        } else {
            into(result, nullptr, nullptr);
        }
    }

    inline auto String::item_into(String* result, auto const* item) -> void
    {
        size_into(result, (char const*) item, sizeof(*item));
    }

    inline constexpr auto String::begin() -> char const* { return first; }
    inline constexpr auto String::end() -> char const* { return last; }
    inline constexpr auto String::byte_count() -> Size { return Size(last - first); }
    inline constexpr String::operator bool () { return (last > first); }

    inline constexpr auto String::prefix(Size byte_count_after_first_to_keep) -> String { return create(into, first, first + byte_count_after_first_to_keep); }
    inline constexpr auto String::suffix(Size byte_count_before_last_to_keep) -> String { return create(into, last - byte_count_before_last_to_keep, last); }
    inline constexpr auto String::skip(Size byte_count_after_first_to_skip) -> String { return create(into, first + byte_count_after_first_to_skip, last); }
    inline constexpr auto String::pop(Size byte_count_before_last_to_pop) -> String { return create(into, first, last - byte_count_before_last_to_pop); }

    inline constexpr auto String::starts_with(String maybe_prefix) -> bool { return (prefix(min(byte_count(), maybe_prefix.byte_count())) == maybe_prefix); }
    inline constexpr auto String::ends_with(String maybe_suffix) -> bool { return (suffix(min(byte_count(), maybe_suffix.byte_count())) == maybe_suffix); }

    inline auto String::check_bounds() -> void
    {
        if (first > last) native::fatal();
    }

    inline constexpr auto operator == (String maybe_a, String maybe_b) -> bool
    {
        if (maybe_a.byte_count() != maybe_b.byte_count()) return false;
        while (maybe_a.first < maybe_a.last) {
            if (*maybe_a.first++ != *maybe_b.first++) {
                return false;
            }
        }
        return true;
    }

    inline constexpr auto operator != (String maybe_a, String maybe_b) -> bool
    {
        return !(maybe_a == maybe_b);
    }
}

namespace dewox::inline string::literal
{
    inline constexpr auto operator ""_s (char const* first, Size byte_count) -> String
    {
        return create(String::size_into, first, byte_count);
    }
}

