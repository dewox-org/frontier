#pragma once
#include "type.hpp"
#include "object.hpp"

namespace dewox::inline string
{
    struct String final
    {
        char const* first;
        char const* last;

        static constexpr auto into(String* result, char const* first, char const* last) -> void;
        static constexpr auto size_into(String* result, char const* first, Size size) -> void;
        static auto item_into(String* result, auto* item) -> void;

        constexpr auto begin() -> char const*;
        constexpr auto end() -> char const*;

        constexpr auto byte_count() -> Size;
        constexpr explicit operator bool ();

        // pattern = body? definition*;
        // definition = ":" <name:byte> body?;
        // body = sequence? branch*;
        // branch = "/" sequence;
        // sequence = command+;
        // command = "." | "~" <from:byte> <to:byte> | "@" <name:byte> | [^.~@:/]
        auto match_prefix(String pattern) -> Size;
        auto match_substring(String pattern) -> String;
    };

    constexpr auto operator == (String opt_a, String opt_b) -> bool;
    constexpr auto operator != (String opt_a, String opt_b) -> bool;
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

    inline auto String::item_into(String* result, auto* item) -> void
    {
        size_into(result, (char const*) item, sizeof(*item));
    }

    inline constexpr auto String::begin() -> char const* { return first; }
    inline constexpr auto String::end() -> char const* { return last; }
    inline constexpr auto String::byte_count() -> Size { return (last > first ? last - first : 0u); }
    inline constexpr String::operator bool () { return (last > first); }

    inline constexpr auto operator == (String opt_a, String opt_b) -> bool
    {
        if (opt_a.byte_count() != opt_b.byte_count()) return false;
        while (opt_a.first < opt_a.last) {
            if (*opt_a.first++ != *opt_b.first++) {
                return false;
            }
        }
        return true;
    }

    inline constexpr auto operator != (String opt_a, String opt_b) -> bool
    {
        return !(opt_a == opt_b);
    }
}

namespace dewox::inline string::literal
{
    inline constexpr auto operator ""_s (char const* first, Size byte_count) -> String
    {
        return create(String::size_into, first, byte_count);
    }
}

