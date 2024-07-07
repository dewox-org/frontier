#include "map.hpp"
#include "hash.hpp"
#include "list.hpp"
#include "slice.hpp"
#include "object.hpp"

namespace dewox::inline map
{
    inline namespace
    {
        constexpr auto bit_count_per_hash = sizeof(Hash) * 8u;
        constexpr auto entry_hash_bit_count_per_block = 7u;
        constexpr auto subtree_hash_bit_count_per_block = 2u;
        constexpr auto effective_hash_bit_count_per_block = max(entry_hash_bit_count_per_block, subtree_hash_bit_count_per_block);
        constexpr auto layer_count_per_hash = 7u;
        constexpr auto hash_bit_count_per_layer = effective_hash_bit_count_per_block * layer_count_per_hash;
        static_assert(hash_bit_count_per_layer + effective_hash_bit_count_per_block * 2u < bit_count_per_hash, "Not enough entropy.");

        constexpr auto entry_count_per_block = Size(Size(1u) << entry_hash_bit_count_per_block);
        constexpr auto subtree_count_per_block = Size(Size(1u) << subtree_hash_bit_count_per_block);

        constexpr auto subtree_index_mask = Identity((Identity(1u) << subtree_hash_bit_count_per_block) - 1u);
        constexpr auto hash_index_mask = Identity((Identity(1u) << entry_hash_bit_count_per_block) - 1u);
        constexpr auto hash_exhaustion_mask = Identity(-(Identity(1u) << (bit_count_per_hash - hash_bit_count_per_layer)));

        union Entry
        {
            struct Tree
            {
                Entry* maybe_subtrees[subtree_count_per_block];

                auto free(Pool* pool) -> void;
            };

            struct Data
            {
                Identity key;
                Identity value;
                DEWOX_LIST(Entry);
            };

            Tree tree;
            Data data;

            constexpr auto link_list_node() -> Data* { return &data; }
        };
        static_assert(sizeof(Entry) == sizeof(Entry::Data));

        constexpr auto byte_count_per_block = sizeof(Entry) * entry_count_per_block;

        auto Entry::Tree::free(Pool* pool) -> void
        {
            for (auto maybe_subtree: maybe_subtrees) {
                if (auto subtree = maybe_subtree) {
                    subtree->tree.free(pool);
                }
            }
            pool->free(this);
        }

        struct Control
        {
            Pool* control_pool;
            Pool* block_pool;
            Entry* maybe_root;
            Entry list;

            auto free() -> void
            {
                if (auto root = maybe_root) root->tree.free(block_pool);
                control_pool->free(this);
            }

            auto try_lookup(Identity key, bool inserts, bool removes) -> Entry*
            {
                auto do_maybe_tree = &maybe_root;
                auto layer = Identity{};
                auto hash = hash_identity(key, hash_identity(layer++, 0u, 1u), 0u);

                while (true) {
                    auto tree = *do_maybe_tree;
                    if (!tree) {
                        if (inserts) {
                            tree = (Entry*) block_pool->grow().fill().begin();
                            *do_maybe_tree = tree;
                        } else {
                            return {};
                        }
                    }

                    if (auto index = (hash & hash_index_mask)) {
                        auto entry = &tree[index];
                        if (entry->data.next) {
                            if (entry->data.key == key) {
                                if (removes) {
                                    detach_from_list(entry);
                                    if (inserts) {
                                        insert_before_list(&list, entry);
                                        return entry;
                                    } else {
                                        entry->data.next = {};
                                        return {};
                                    }
                                } else {
                                    return entry;
                                }
                            } else {
                                // Fallthrough to continue looking.
                            }
                        } else {
                            if (inserts) {
                                insert_before_list(&list, entry);
                                return entry;
                            } else {
                                // Fallthrough to continue looking.
                            }
                        }
                    } else {
                        // Fallthrough to continue looking.
                    }

                    hash >>= effective_hash_bit_count_per_block;
                    if ((hash & hash_exhaustion_mask) == 0u) {
                        auto subtree_index = (exchange(&hash, hash_identity(key, hash_identity(layer++, 0u, 1u), 0u)) & subtree_index_mask);
                        do_maybe_tree = &tree->tree.maybe_subtrees[subtree_index];
                    }
                }
            }
        };
    }

    auto Map::into_control_pool(Pool* result, Chain* chain) -> void
    {
        Pool::into(result, chain, sizeof(Control), alignof(Control));
    }

    auto Map::into_block_pool(Pool* result, Chain* chain) -> void
    {
        Pool::into(result, chain, byte_count_per_block, alignof(Entry));
    }

    auto Map::into(Map* result, Pool* control_pool, Pool* block_pool) -> void
    {
        auto control = (Control*) control_pool->grow().begin();
        result->control = (void*) control;

        control->control_pool = control_pool;
        control->block_pool = block_pool;
        control->maybe_root = {};
        link_list(&control->list, &control->list);
    }

    auto Map::drop() -> void
    {
        auto control = (Control*) this->control;
        control->free();
    }

    auto Map::contains(Identity key) -> bool
    {
        auto control = (Control*) this->control;
        return (bool) control->try_lookup(key, false, false);
    }

    auto Map::lookup(Identity key) -> Identity
    {
        auto control = (Control*) this->control;
        if (auto entry = control->try_lookup(key, false, false)) {
            return entry->data.value;
        } else {
            return {};
        }
    }

    auto Map::update(Identity key, Identity value) -> void
    {
        auto control = (Control*) this->control;
        auto entry = control->try_lookup(key, true, false);
        entry->data.key = key;
        entry->data.value = value;
    }

    auto Map::remove(Identity key) -> void
    {
        auto control = (Control*) this->control;
        control->try_lookup(key, false, true);
    }

    auto Map::search(Find_Map_Key_Value* find, void* maybe_data) -> bool
    {
        auto control = (Control*) this->control;
        return search_list(control->list.data.next, &control->list, &Entry::Data::next, [&] (Entry* entry) -> bool {
            return find(entry->data.key, entry->data.value, maybe_data);
        });
    }
}

