#pragma once
#include "string.hpp"
#include "object.hpp"
#include "tool.hpp"

namespace dewox::inline slice
{
    template <mutable_type Item>
    struct Slice
    {
        using Base = String;

        Base base;

        static constexpr auto into(Slice* result, Item const* first, Item const* last) -> void;
        static constexpr auto base_into(Slice* result, Base base) -> void;
        static constexpr auto count_into(Slice* result, Item const* first, Size item_count) -> void;
        static auto item_into(Slice* result, auto const* item) -> void;

        constexpr auto begin() -> Item*;
        constexpr auto end() -> Item*;

        constexpr auto byte_count() -> Size;
        constexpr auto item_count() -> Size;
        constexpr explicit operator bool ();
    };
}

namespace dewox::inline slice
{
    template <mutable_type Item>
    inline constexpr auto Slice<Item>::into(Slice* result, Item const* first, Item const* last) -> void
    {
        base_into(result, create(&Base::into, (char const*) first, (char const*) last));
    }

    template <mutable_type Item>
    inline constexpr auto Slice<Item>::base_into(Slice* result, Base base) -> void
    {
        result->base = base;
    }

    template <mutable_type Item>
    inline constexpr auto Slice<Item>::count_into(Slice* result, Item const* first, Size item_count) -> void
    {
        into(result, first, first + item_count);
    }

    template <mutable_type Item>
    inline auto Slice<Item>::item_into(Slice* result, auto const* item) -> void
    {
        count_into(result, (Item const*) item, sizeof(*item) / sizeof(Item));
    }

    template <mutable_type Item> inline constexpr auto Slice<Item>::begin() -> Item* { return (Item*) base.begin(); }
    template <mutable_type Item> inline constexpr auto Slice<Item>::end() -> Item* { return (Item*) base.end(); }
    template <mutable_type Item> inline constexpr auto Slice<Item>::byte_count() -> Size { return base.byte_count(); }
    template <mutable_type Item> inline constexpr auto Slice<Item>::item_count() -> Size { return base.byte_count() / sizeof(Item); }
    template <mutable_type Item> inline constexpr Slice<Item>::operator bool () { return (bool) base; }
}

