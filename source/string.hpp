#pragma once
#include "type.hpp"
#include "tool.hpp"
#include "object.hpp"

namespace dewox::inline string
{
    struct String final
    {
        /// [invariant] first <= last;

        char const* first;
        char const* last;

        static constexpr auto into(String* result, char const* first, char const* last) -> void;
        static constexpr auto wild_into(String* result, char const* first, char const* last) -> void;
        static constexpr auto size_into(String* result, char const* first, Size size) -> void;
        static constexpr auto until_into(String* result, char const* maybe_string) -> void;
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
        wild_into(result, first, max(first, last));
    }

    inline constexpr auto String::wild_into(String* result, char const* first, char const* last) -> void
    {
        result->first = first;
        result->last = last;
    }

    inline constexpr auto String::size_into(String* result, char const* first, Size size) -> void
    {
        wild_into(result, first, first + size);
    }

    inline constexpr auto String::until_into(String* result, char const* maybe_string) -> void
    {
        if (auto string = maybe_string) {
            while (*string) string++;
            wild_into(result, maybe_string, string);
        } else {
            wild_into(result, nullptr, nullptr);
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

    // NOTE: Pay attention to possible overflows!
    inline constexpr auto String::prefix(Size byte_count_after_first_to_keep) -> String { return create(wild_into, first, first + min(byte_count_after_first_to_keep, byte_count())); }
    inline constexpr auto String::suffix(Size byte_count_before_last_to_keep) -> String { return create(wild_into, last - min(byte_count_before_last_to_keep, byte_count()), last); }
    inline constexpr auto String::skip(Size byte_count_after_first_to_skip) -> String { return create(wild_into, first + min(byte_count_after_first_to_skip, byte_count()), last); }
    inline constexpr auto String::pop(Size byte_count_before_last_to_pop) -> String { return create(wild_into, first, last - min(byte_count_before_last_to_pop, byte_count())); }

    inline constexpr auto String::starts_with(String maybe_prefix) -> bool { return (prefix(maybe_prefix.byte_count()) == maybe_prefix); }
    inline constexpr auto String::ends_with(String maybe_suffix) -> bool { return (suffix(maybe_suffix.byte_count()) == maybe_suffix); }

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

