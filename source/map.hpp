#pragma once
// A mapping from Identity to Identity.
#include "chain.hpp"
#include "pool.hpp"
#include "type.hpp"

namespace dewox::inline map
{
    using Find_Map_Key_Value = auto (Identity key, Identity value, void* maybe_data) -> bool;  // -> found

    struct Map
    {
        void* control;

        static auto into_control_pool(Pool* result, Chain* chain) -> void;
        static auto into_block_pool(Pool* result, Chain* chain) -> void;
        static auto into(Map* result, Pool* control_pool, Pool* block_pool) -> void;

        auto drop() -> void;

        auto contains(Identity key) -> bool;
        auto lookup(Identity key) -> Identity;
        auto update(Identity key, Identity value) -> void;
        auto remove(Identity key) -> void;

        auto search(Find_Map_Key_Value* find, void* maybe_data) -> bool;
    };
}

