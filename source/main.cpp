#include "string.hpp"

namespace dewox::inline main
{
    auto run() -> int
    {
        auto a = "HElloWORLd"_s.match_prefix("~AZ@@"_s) - 2u;
        auto b = int("HElloWORLd"_s.match_substring("~AZ@@"_s) == "HE"_s) - 1;
        auto c = int("lloWORLd"_s.match_substring("~AZ@@"_s) == "WORL"_s) - 1;
        return a + b + c;
    }
}

auto main() -> int
{
    return ::dewox::main::run();
}

