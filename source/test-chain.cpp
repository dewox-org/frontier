#include "test.hpp"
#include "chain.hpp"
#include "object.hpp"
#include "native.hpp"

namespace dewox::inline test
{
    inline namespace
    {
        auto count_buffers(Chain* chain) -> Size
        {
            auto buffer_count = Size{};
            chain->search([] (String buffer, void* data) -> bool {
                auto count = (Size*) data;
                (*count)++;
                return false;
            }, &buffer_count);
            return buffer_count;
        }

        auto is_aligned_to(char* pointer, Size alignment) -> bool
        {
            return ((Size(pointer) & (alignment - 1u)) == 0u);
        }
    }

    extern "C" auto dewox_site_test_chain(Test* test) -> void
    {
        auto memory_operation_balance = Size{};
        auto memory_operation_balance_data = (void*) &memory_operation_balance;
        auto report_grow_memory = +[] (Size byte_count, Size alignment, char* memory, void* data) -> void { * (Size*) data += 1u; };
        auto report_drop_memory = +[] (void* maybe_memory, void* data) -> void { * (Size*) data -= (maybe_memory ? 1u : 0u); };
        native::probe_memory(&report_grow_memory, &report_drop_memory, &memory_operation_balance_data);

        if (test->start(test, "chain/empty/drop")) {
            auto chain = Chain{};
            chain.drop();
            test->finish(test, true);
        }

        if (test->start(test, "chain/empty/count")) {
            auto chain = Chain{};
            test->finish(test, (chain.byte_count() == 0u));
        }

        if (test->start(test, "chain/empty/bool")) {
            auto chain = Chain{};
            test->finish(test, ((bool) chain == false));
        }

        if (test->start(test, "chain/empty/pop")) {
            auto chain = Chain{};
            chain.pop(1024u);
            test->finish(test, (chain.byte_count() == 0u));
        }

        if (test->start(test, "chain/empty/merge/count")) {
            auto chain = Chain{};
            test->finish(test, (chain.merge(1024u).byte_count() == 0u));
        }

        if (test->start(test, "chain/empty/merge/null")) {
            auto chain = Chain{};
            test->finish(test, (chain.merge(1024u).begin() == nullptr));
        }

        if (test->start(test, "chain/empty/search")) {
            auto chain = Chain{};
            test->finish(test, (count_buffers(&chain) == 0u));
        }

        if (test->start(test, "chain/grow/count")) {
            auto chain = Sink<Chain>{};
            test->finish(test, (chain->grow(1024u).byte_count() == 1024u));
        }

        if (test->start(test, "chain/grow/alignment/1")) {
            auto chain = Sink<Chain>{};
            auto p1 = chain->grow(2u, 2u).begin();
            auto p2 = chain->grow(1u, 1u).begin();
            test->finish(test, (p2 - p1 == 2u));
        }

        if (test->start(test, "chain/grow/alignment/4")) {
            auto chain = Sink<Chain>{};
            auto p1 = chain->grow(2u, 2u).begin();
            auto p2 = chain->grow(1u, 4u).begin();
            test->finish(test, (p2 - p1 == 4u));
        }

        if (test->start(test, "chain/grow/alignment/under/align")) {
            auto chain = Sink<Chain>{};
            chain->grow(2u, 2u);
            auto p = chain->grow(1u, 64u).begin();
            test->finish(test, is_aligned_to(p, 64u));
        }

        if (test->start(test, "chain/grow/alignment/under/search")) {
            auto chain = Sink<Chain>{};
            chain->grow(2u, 2u);
            chain->grow(1u, 64u);
            test->finish(test, (count_buffers(*chain) == 1u));
        }

        if (test->start(test, "chain/grow/alignment/over/align")) {
            auto chain = Sink<Chain>{};
            chain->grow(2u, 2u);
            auto p = chain->grow(1u, 1024u).begin();
            test->finish(test, is_aligned_to(p, 1024u));
        }

        if (test->start(test, "chain/grow/alignment/over/search")) {
            auto chain = Sink<Chain>{};
            chain->grow(2u, 2u);
            chain->grow(1u, 1024u);
            test->finish(test, (count_buffers(*chain) == 2u));
        }

        if (test->start(test, "chain/grow/alignment/over/count")) {
            auto chain = Sink<Chain>{};
            chain->grow(2u, 2u);
            chain->grow(1u, 1024u);
            test->finish(test, (chain->byte_count() == 3u));
        }

        if (test->start(test, "chain/grow/alignment/over/merge/count")) {
            auto chain = Sink<Chain>{};
            chain->grow(2u, 2u);
            chain->grow(1u, 1024u);
            test->finish(test, (chain->merge(3u).byte_count() == 3u));
        }

        if (test->start(test, "chain/grow/alignment/over/merge/search")) {
            auto chain = Sink<Chain>{};
            chain->grow(2u, 2u);
            chain->grow(1u, 1024u);
            chain->merge(3u);
            test->finish(test, (count_buffers(*chain) == 1u));
        }

        if (test->start(test, "chain/grow/two-step/align")) {
            auto chain = Sink<Chain>{};
            chain->grow(2u, 2u);
            auto p = chain->grow(0u, 1024u).begin();
            test->finish(test, (p == nullptr));
        }

        if (test->start(test, "chain/grow/two-step/grow/align")) {
            auto chain = Sink<Chain>{};
            chain->grow(2u, 2u);
            chain->grow(0u, 1024u);
            auto p = chain->grow(1u, 1u).begin();
            test->finish(test, is_aligned_to(p, 1024u));
        }

        if (test->start(test, "chain/merge/1/grow/count")) {
            auto chain = Sink<Chain>{};
            constexpr auto mebibyte = Size(1u << 20u);
            constexpr auto item_size = Size(32u);
            constexpr auto item_count = Size(mebibyte / item_size);
            for (auto i = Size{}; i < item_count; i++) chain->grow(item_size, item_size);
            test->finish(test, (chain->byte_count() == mebibyte));
        }

        if (test->start(test, "chain/merge/1/grow/search")) {
            auto chain = Sink<Chain>{};
            constexpr auto mebibyte = Size(1u << 20u);
            constexpr auto item_size = Size(32u);
            constexpr auto item_count = Size(mebibyte / item_size);
            for (auto i = Size{}; i < item_count; i++) chain->grow(item_size, item_size);
            test->finish(test, (count_buffers(*chain) == 13u));
        }

        if (test->start(test, "chain/merge/1/count")) {
            auto chain = Sink<Chain>{};
            constexpr auto mebibyte = Size(1u << 20u);
            constexpr auto item_size = Size(32u);
            constexpr auto item_count = Size(mebibyte / item_size);
            for (auto i = Size{}; i < item_count; i++) chain->grow(item_size, item_size);
            chain->merge(chain->byte_count());
            test->finish(test, (chain->byte_count() == mebibyte));
        }

        if (test->start(test, "chain/merge/1/search")) {
            auto chain = Sink<Chain>{};
            constexpr auto mebibyte = Size(1u << 20u);
            constexpr auto item_size = Size(32u);
            constexpr auto item_count = Size(mebibyte / item_size);
            for (auto i = Size{}; i < item_count; i++) chain->grow(item_size, item_size);
            chain->merge(chain->byte_count());
            test->finish(test, (count_buffers(*chain) == 1u));
        }

        if (test->start(test, "chain/merge/2/grow/count")) {
            auto chain = Sink<Chain>{};
            constexpr auto mebibyte = Size(1u << 20u);
            constexpr auto item_size = Size(32u);
            constexpr auto item_count = Size(mebibyte / item_size * 2u);
            for (auto i = Size{}; i < item_count; i++) chain->grow(item_size, item_size);
            test->finish(test, (chain->byte_count() == mebibyte * 2u));
        }

        if (test->start(test, "chain/merge/2/grow/search")) {
            auto chain = Sink<Chain>{};
            constexpr auto mebibyte = Size(1u << 20u);
            constexpr auto item_size = Size(32u);
            constexpr auto item_count = Size(mebibyte / item_size * 2u);
            for (auto i = Size{}; i < item_count; i++) chain->grow(item_size, item_size);
            test->finish(test, (count_buffers(*chain) == 14u));
        }

        if (test->start(test, "chain/merge/2/count")) {
            auto chain = Sink<Chain>{};
            constexpr auto mebibyte = Size(1u << 20u);
            constexpr auto item_size = Size(32u);
            constexpr auto item_count = Size(mebibyte / item_size * 2u);
            for (auto i = Size{}; i < item_count; i++) chain->grow(item_size, item_size);
            chain->merge(chain->byte_count());
            test->finish(test, (chain->byte_count() == mebibyte * 2u));
        }

        if (test->start(test, "chain/merge/2/search")) {
            auto chain = Sink<Chain>{};
            constexpr auto mebibyte = Size(1u << 20u);
            constexpr auto item_size = Size(32u);
            constexpr auto item_count = Size(mebibyte / item_size * 2u);
            for (auto i = Size{}; i < item_count; i++) chain->grow(item_size, item_size);
            chain->merge(chain->byte_count());
            test->finish(test, (count_buffers(*chain) == 1u));
        }

        if (test->start(test, "chain/memory/dirty")) {
            auto chain = Sink<Chain>{};
            chain->grow(2u, 2u);
            chain->grow(1u, 1024u);
            test->finish(test, (memory_operation_balance == 3u));
        }

        if (test->start(test, "chain/memory/clean")) {
            test->finish(test, (memory_operation_balance == 0u));
        }

        native::probe_memory(&report_grow_memory, &report_drop_memory, &memory_operation_balance_data);
    }
}

