#pragma once
// A chain is a back-optimized cache-friendly byte container.
// It is a replacement to std::vector<>, with a different design.
// It does not manage object lifetimes, so another typical use is to act as an allocator.
// "Back-optimized" means that efficient operations are designed to work at the "back" of the container.
#include "string.hpp"
#include "slice.hpp"
#include "type.hpp"
#include "object.hpp"

namespace dewox::inline chain
{
    using Find_Buffer = auto (String buffer, void* maybe_data) -> bool;

    // A Chain object manages a list of Block objects that have geometrically-increasing capacity.
    struct Chain
    {
        void* maybe_control;

        auto drop() -> void;

        // When byte_count == 0u, grow() will setup alignment for the next grow().
        auto grow(Size byte_count, Size alignment = {}) -> String;
        auto pop(Size byte_count) -> void;
        auto merge(Size byte_count) -> String;
        auto clear() -> void;

        auto front() -> char*;
        auto back(Size byte_count = 1u) -> char*;

        auto byte_count() -> Size;
        explicit operator bool ();

        auto search(Find_Buffer* find, void* maybe_data) -> bool;

        template <mutable_type Item> auto grow_items(Size item_count = 1u) -> Slice<Item>;
        template <mutable_type Item> auto pop_items(Size item_count) -> void;
        template <mutable_type Item> auto merge_items(Size item_count) -> Slice<Item>;

        template <mutable_type Item> auto front_item() -> Item*;
        template <mutable_type Item> auto back_item() -> Item*;

        template <mutable_type Item> auto item_count() -> Size;
    };
}

namespace dewox::inline chain
{
    inline auto Chain::clear() -> void { pop(byte_count()); }
    inline auto Chain::back(Size byte_count) -> char* { return merge(byte_count).begin(); }
    inline Chain::operator bool () { return (maybe_control ? (byte_count() > 0u) : false); }

    template <mutable_type Item> inline auto Chain::grow_items(Size item_count) -> Slice<Item> { return create(&Slice<Item>::base_into, grow(item_count * sizeof(Item), alignof(Item))); }
    template <mutable_type Item> inline auto Chain::pop_items(Size item_count) -> void { pop(item_count * sizeof(Item)); }
    template <mutable_type Item> inline auto Chain::merge_items(Size item_count) -> Slice<Item> { return create(&Slice<Item>::base_into, merge(item_count * sizeof(Item))); }

    template <mutable_type Item> inline auto Chain::front_item() -> Item* { return (Item*) front(); }
    template <mutable_type Item> inline auto Chain::back_item() -> Item* { return (Item*) back(sizeof(Item)); }

    template <mutable_type Item> inline auto Chain::item_count() -> Size { return byte_count() / sizeof(Item); }
}

