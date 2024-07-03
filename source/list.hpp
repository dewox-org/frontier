#pragma once
#include "object.hpp"

namespace dewox::inline list
{
    #define DEWOX_LIST(STRUCT)  // Turn a structure into a bidirectional linked-list.

    constexpr auto link_list_node(auto* a) -> auto*;

    constexpr auto link_list(auto* a, auto* b) -> void;
    constexpr auto weld_list(auto* a, auto* b, auto* c) -> void;

    constexpr auto detach_from_list(auto* a) -> decltype(a);

    constexpr auto insert_before_list(auto* list, auto* a) -> void;
    constexpr auto insert_after_list(auto* list, auto* a) -> void;

    constexpr auto drop_list(auto* list) -> void;
    constexpr auto search_list(auto* first, auto* last, auto next, auto find_item) -> bool;  // -> found
}

namespace dewox::inline list
{
    #undef DEWOX_LIST
    #define DEWOX_LIST(STRUCT)  STRUCT* back; STRUCT* next

    inline constexpr auto link_list_node(auto* a) -> auto*
    {
        if constexpr (requires (decltype(a) b) { b->link_list_node(); }) {
            return a->link_list_node();
        } else {
            return a;
        }
    }

    inline constexpr auto link_list(auto* a, auto* b) -> void
    {
        link_list_node(a)->next = b;
        link_list_node(b)->back = a;
    }

    inline constexpr auto weld_list(auto* a, auto* b, auto* c) -> void
    {
        link_list(a, b);
        link_list(b, c);
    }

    inline constexpr auto detach_from_list(auto* a) -> decltype(a)
    {
        link_list(link_list_node(a)->back, link_list_node(a)->next);
        return a;
    }

    inline constexpr auto insert_before_list(auto* list, auto* a) -> void
    {
        weld_list(link_list_node(list)->back, a, list);
    }

    inline constexpr auto insert_after_list(auto* list, auto* a) -> void
    {
        weld_list(list, a, link_list_node(list)->next);
    }

    inline constexpr auto drop_list(auto* list) -> void
    {
        while (link_list_node(list)->back != list) {
            drop(detach_from_list(link_list_node(list)->back));
        }
        drop(list);
    }

    inline constexpr auto search_list(auto* first, auto* last, auto next, auto find_item) -> bool
    {
        for(; first != last; first = link_list_node(first)->*next) {
            if (find_item(first)) {
                return true;
            }
        }
        return false;
    }
}

