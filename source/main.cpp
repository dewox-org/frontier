#include "test.hpp"
#include "tool.hpp"

namespace dewox::inline main
{
    auto run() -> int
    {
        filter_trace();
        if (!run_tests()) return 1;
        return 0;
    }
}

auto main() -> int
{
    return ::dewox::main::run();
}

