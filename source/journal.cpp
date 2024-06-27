#include "journal.hpp"
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
    thread_local Chain* maybe_journal_scratch{};
}

