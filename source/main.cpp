#include "test.hpp"
#include "tool.hpp"
#include "native.hpp"

namespace dewox
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
    auto result = ::dewox::run();
    ::dewox::native::unload();
    return result;
}

