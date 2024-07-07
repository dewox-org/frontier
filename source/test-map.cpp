#include "test.hpp"
#include "map.hpp"
#include "object.hpp"
#include "native.hpp"

namespace dewox::inline test
{
    extern "C" auto dewox_site_test_map(Test* test) -> void
    {
        auto chain = Sink<Chain>{};
        auto control_pool = create(&Map::into_control_pool, *chain);
        auto block_pool = create(&Map::into_block_pool, *chain);
        auto byte_count_without_maps = chain->byte_count();

        if (test->start(test, "map")) {
            test->finish(test, byte_count_without_maps > 0u);
        }

        if (test->start(test, "map/create")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            test->finish(test, chain->byte_count() > byte_count_without_maps);
        }

        if (test->start(test, "map/create/after-drop")) {
            test->finish(test, chain->byte_count() > byte_count_without_maps);
        }

        if (test->start(test, "map/contains/empty")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            test->finish(test, !map->contains(1u));
        }

        if (test->start(test, "map/contains/single/yes")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            map->update(1u, 10u);
            test->finish(test, map->contains(1u));
        }

        if (test->start(test, "map/contains/single/no")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            map->update(2u, 20u);
            test->finish(test, !map->contains(1u));
        }

        if (test->start(test, "map/contains/many/yes")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            test->finish(test, map->contains(42u));
        }

        auto byte_count_after_first_many = chain->byte_count();

        if (test->start(test, "map/contains/many/no")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            test->finish(test, !map->contains(4200u));
        }

        if (test->start(test, "map/lookup/empty")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            test->finish(test, map->lookup(1u) == 0u);
        }

        if (test->start(test, "map/lookup/many/yes")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            test->finish(test, map->lookup(42u) == 420u);
        }

        if (test->start(test, "map/lookup/many/no")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            test->finish(test, map->lookup(4200u) == 0u);
        }

        if (test->start(test, "map/remove/empty")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            map->remove(1u);
            test->finish(test, !map->contains(1u));
        }

        if (test->start(test, "map/remove/many/yes")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            map->remove(42u);
            test->finish(test, !map->contains(42u));
        }

        if (test->start(test, "map/remove/many/no")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            map->remove(420u);
            test->finish(test, map->contains(42u));
        }

        if (test->start(test, "map/remove/many/reinsert")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            map->remove(42u);
            map->update(42u, 1234u);
            test->finish(test, map->lookup(42u) == 1234u);
        }

        if (test->start(test, "map/search/empty")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            auto n = Size{};
            auto found = map->search([] (Identity key, Identity value, void* data) -> bool {
                auto n = (Size*) data;
                n[0]++;
                return false;
            }, &n);
            test->finish(test, n == 0u && !found);
        }

        if (test->start(test, "map/search/empty/found")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            auto n = Size{};
            auto found = map->search([] (Identity key, Identity value, void* data) -> bool {
                auto n = (Size*) data;
                n[0]++;
                return true;
            }, &n);
            test->finish(test, n == 0u && !found);
        }

        if (test->start(test, "map/search/many")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            struct Data
            {
                Identity result;
                Size count;
            };
            auto data = Data{};
            auto found = map->search([] (Identity key, Identity value, void* unknown_data) -> bool {
                auto data = (Data*) unknown_data;
                data->result += ++data->count * (key * 0x10000u + value);
                return false;
            }, &data);
            test->finish(test, data.result == 23459804825600u && !found);
        }

        if (test->start(test, "map/search/many/found")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            struct Data
            {
                Identity result;
                Size count;
            };
            auto data = Data{};
            auto found = map->search([] (Identity key, Identity value, void* unknown_data) -> bool {
                auto data = (Data*) unknown_data;
                data->result += ++data->count * (key * 0x10000u + value);
                return (key == 42u);
            }, &data);
            test->finish(test, data.result == 1736182448u && found);
        }

        if (test->start(test, "map/search/many/reorder")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            map->remove(32u);
            map->remove(42u);
            map->remove(100u);
            map->update(42u, 420u);
            map->update(100u, 1000u);
            map->update(32u, 320u);
            struct Data
            {
                Identity result;
                Size count;
            };
            auto data = Data{};
            auto found = map->search([] (Identity key, Identity value, void* unknown_data) -> bool {
                auto data = (Data*) unknown_data;
                data->result += ++data->count * (key * 0x10000u + value);
                return false;
            }, &data);
            test->finish(test, data.result == 23368068282650u && !found);
        }

        if (test->start(test, "map/search/many/reorder/found")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            map->remove(32u);
            map->remove(42u);
            map->remove(100u);
            map->update(42u, 420u);
            map->update(100u, 1000u);
            map->update(32u, 320u);
            struct Data
            {
                Identity result;
                Size count;
            };
            auto data = Data{};
            auto found = map->search([] (Identity key, Identity value, void* unknown_data) -> bool {
                auto data = (Data*) unknown_data;
                data->result += ++data->count * (key * 0x10000u + value);
                return (key == 42u);
            }, &data);
            test->finish(test, data.result == 23359215115522u && found);
        }

        if (test->start(test, "map/memory/many")) {
            test->finish(test, chain->byte_count() == byte_count_after_first_many);
        }

        if (test->start(test, "map/memory/share")) {
            auto map1 = Sink<Map>{};
            auto map2 = Sink<Map>{};
            Map::into(*map1, &control_pool, &block_pool);
            Map::into(*map2, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map1->update(i, i * 10u);
            for (auto i = 0u; i < 1024u; i++) map2->update(i, i * 10u);
            test->finish(test, chain->byte_count() > byte_count_after_first_many);
        }

        auto byte_count_after_sharing = chain->byte_count();

        if (test->start(test, "map/memory/share/1")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u; i++) map->update(i, i * 10u);
            test->finish(test, chain->byte_count() == byte_count_after_sharing);
        }

        if (test->start(test, "map/memory/share/0")) {
            test->finish(test, chain->byte_count() == byte_count_after_sharing);
        }

        if (test->start(test, "map/density/huge")) {
            auto map = Sink<Map>{};
            Map::into(*map, &control_pool, &block_pool);
            for (auto i = 0u; i < 1024u * 1024u; i++) map->update(i, i * 10u);
            test->finish(test, true);
        }
    }
}

