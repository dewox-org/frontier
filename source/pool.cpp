#include "pool.hpp"
#include "tool.hpp"
#include "object.hpp"

namespace dewox::inline pool
{
    inline namespace
    {
        struct Item
        {
            Item* maybe_next;
        };

        struct Control
        {
            Size byte_count_per_item;
            Size alignment_per_item;
            Chain* chain;
            Item* maybe_items;
        };
    }

    auto Pool::into(Pool* result, Chain* chain, Size byte_count_per_item, Size alignment_per_item) -> void
    {
        auto control = chain->grow_items<Control>().begin();
        result->control = (void*) control;

        control->byte_count_per_item = max(byte_count_per_item, sizeof(Item));
        control->alignment_per_item = max(alignment_per_item, alignof(Item));
        control->chain = chain;
        control->maybe_items = {};
    }

    auto Pool::chain() -> Chain*
    {
        auto control = (Control*) this->control;
        return control->chain;
    }

    auto Pool::grow() -> String
    {
        auto control = (Control*) this->control;
        if (auto item = control->maybe_items) {
            auto memory = (char*) exchange(&control->maybe_items, item->maybe_next);
            return create(&String::count_into, memory, control->byte_count_per_item);
        } else {
            return control->chain->grow(control->byte_count_per_item, control->alignment_per_item);
        }
    }

    auto Pool::free(void* maybe_memory) -> void
    {
        if (auto item = (Item*) maybe_memory) {
            auto control = (Control*) this->control;
            item->maybe_next = exchange(&control->maybe_items, item);
        }
    }
}

