#pragma once
#include "object.hpp"
#include "tool.hpp"
#include "type.hpp"
#include "writer.hpp"

namespace dewox
{
    struct String
    {
        /// You must ensure first <= last on your own. It is undefined behavior otherwise.

        char const* first;
        char const* last;

        static constexpr auto into(String* result, char const* first, char const* last) -> void;
        static constexpr auto count_into(String* result, char const* first, Size byte_count) -> void;
        static constexpr auto until_into(String* result, char const* maybe_native_string) -> void;
        static auto item_into(String* result, auto const* item) -> void;

        constexpr auto begin() -> char*;
        constexpr auto end() -> char*;

        constexpr auto byte_count() -> Size;
        constexpr explicit operator bool ();

        constexpr auto prefix(Size prefix_byte_count) -> String;
        constexpr auto suffix(Size suffix_byte_count) -> String;
        constexpr auto skip(Size prefix_byte_count) -> String;
        constexpr auto pop(Size suffix_byte_count) -> String;

        constexpr auto starts_with(String maybe_prefix) -> bool;
        constexpr auto ends_with(String maybe_suffix) -> bool;

        constexpr auto copy(String maybe_source) -> String;     // -> written_string
        constexpr auto fill(int a = {}) -> String;              // -> filled_string

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

namespace dewox
{
    auto write(Chain* chain, String piece) -> void;

    template <Size byte_count>
    auto write(Chain* chain, char const (&piece)[byte_count]) -> void;
}

namespace dewox::literal::inline string
{
    constexpr auto operator ""_s (char const* first, Size byte_count) -> String;
}

namespace dewox
{
    using namespace ::dewox::literal::string;
}

namespace dewox
{
    inline constexpr auto String::into(String* result, char const* first, char const* last) -> void
    {
        result->first = first;
        result->last = last;
    }

    inline constexpr auto String::count_into(String* result, char const* first, Size byte_count) -> void
    {
        into(result, first, first + byte_count);
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
        count_into(result, (char const*) item, sizeof(*item));
    }

    inline constexpr auto String::begin() -> char* { return (char*) first; }
    inline constexpr auto String::end() -> char* { return (char*) last; }
    inline constexpr auto String::byte_count() -> Size { return Size(last - first); }
    inline constexpr String::operator bool () { return (last > first); }

    inline constexpr auto String::prefix(Size prefix_byte_count) -> String { return create(into, first, first + prefix_byte_count); }
    inline constexpr auto String::suffix(Size suffix_byte_count) -> String { return create(into, last - suffix_byte_count, last); }
    inline constexpr auto String::skip(Size prefix_byte_count) -> String { return create(into, first + prefix_byte_count, last); }
    inline constexpr auto String::pop(Size suffix_byte_count) -> String { return create(into, first, last - suffix_byte_count); }

    inline constexpr auto String::starts_with(String maybe_prefix) -> bool { return (prefix(min(byte_count(), maybe_prefix.byte_count())) == maybe_prefix); }
    inline constexpr auto String::ends_with(String maybe_suffix) -> bool { return (suffix(min(byte_count(), maybe_suffix.byte_count())) == maybe_suffix); }

    inline constexpr auto String::copy(String maybe_source) -> String
    {
        auto target = prefix(min(byte_count(), maybe_source.byte_count()));
        auto p = maybe_source.begin();
        for (auto& byte: target) {
            byte = *p++;
        }
        return target;
    }

    inline constexpr auto String::fill(int a) -> String
    {
        for (auto& byte: *this) {
            byte = (char) a;
        }
        return *this;
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

namespace dewox
{
    template <Size byte_count>
    inline auto write(Chain* chain, char const (&piece)[byte_count]) -> void
    {
        write(chain, create(&String::count_into, piece, byte_count - 1u));  // "- 1u" to remove the suffix '\0'.
    }
}

namespace dewox::literal::inline string
{
    inline constexpr auto operator ""_s (char const* native_string, Size byte_count) -> String
    {
        return create(&String::count_into, native_string, byte_count);
    }
}

