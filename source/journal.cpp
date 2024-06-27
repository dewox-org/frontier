#include "journal.hpp"
#include "tool.hpp"
#include "native.hpp"

namespace dewox::inline journal
{
    auto dump(String maybe_text) -> void
    {
        native::print_string(maybe_text.begin(), maybe_text.byte_count());
        native::flush();
    }
}

namespace dewox::inline journal
{
    inline namespace
    {
        thread_local Chain* maybe_scratch{};
    }

    auto use_journal(Chain** do_maybe_chain) -> void
    {
        swap(do_maybe_chain, &maybe_scratch);
    }

    auto try_journal_scratch() -> Chain*
    {
        return maybe_scratch;
    }
}

