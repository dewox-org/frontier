#pragma once
#include "string.hpp"
#include "chain.hpp"
#include "tool.hpp"

namespace dewox::inline journal
{
    auto format(auto... pieces) -> String;
    auto format_native(auto... pieces) -> char const*;

    auto dump(String maybe_text) -> void;
    auto print(auto... pieces) -> void;
}

namespace dewox::inline journal
{
    auto use_journal(Chain** do_maybe_chain) -> void;
    auto try_journal_scratch() -> Chain*;
    auto do_format(Chain* scratch, auto... pieces) -> String;
}

namespace dewox::inline journal
{
    inline auto format(auto... pieces) -> String
    {
        if (auto scratch = try_journal_scratch()) {
            return do_format(scratch, pieces...);
        } else {
            return {};
        }
    }

    inline auto format_native(auto... pieces) -> char const*
    {
        if (auto scratch = try_journal_scratch()) {
            return do_format(scratch, pieces..., '\0').begin();
        } else {
            return {};
        }
    }

    inline auto print(auto... pieces) -> void
    {
        if (auto scratch = try_journal_scratch()) {
            dump(do_format(scratch, pieces..., '\n'));
        } else {
            // Do nothing if journal scratch not available.
        }
    }
}

namespace dewox::inline journal
{
    extern thread_local Chain* maybe_journal_scratch;

    inline auto use_journal(Chain** do_maybe_chain) -> void
    {
        swap(do_maybe_chain, &maybe_journal_scratch);
    }

    inline auto try_journal_scratch() -> Chain*
    {
        return maybe_journal_scratch;
    }

    inline auto do_format(Chain* chain, auto... pieces) -> String
    {
        auto byte_count_before_writing = chain->byte_count();
        (::dewox::writer::write(chain, pieces), ...);
        auto byte_count_after_writing = chain->byte_count();
        // [invariant] byte_count_after_writing >= byte_count_before_writing;
        auto byte_count_written = byte_count_after_writing - byte_count_before_writing;
        return chain->merge(byte_count_written);
    }
}

