#pragma once
// A pool manages a list of items of the same size and alignment.
// It never returns memory to the chain.
#include "string.hpp"
#include "chain.hpp"
#include "type.hpp"
#include "tool.hpp"

namespace dewox::inline pool
{
    struct Pool
    {
        void* control;

        static auto into(Pool* result, Chain* chain, Size byte_count_per_item, Size alignment_per_item) -> void;

        template <mutable_type Item>
        static auto item_into(Pool* result, Chain* chain) -> void;

        auto chain() -> Chain*;

        auto grow() -> String;
        auto free(void* maybe_memory) -> void;
    };
}

namespace dewox::inline pool
{
    template <mutable_type Item>
    inline auto Pool::item_into(Pool* result, Chain* chain) -> void
    {
        into(result, chain, sizeof(Item), alignof(Item));
    }
}

