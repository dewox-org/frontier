#include "test.hpp"
#include "tool.hpp"
#include "native.hpp"

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
    ::dewox::native::load();
    auto result = ::dewox::main::run();
    ::dewox::native::unload();
    return result;
}

