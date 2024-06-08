#include "string.hpp"

namespace dewox::inline main
{
    auto run() -> int
    {
        filter_trace();
        auto pattern = "~AZ@a:a~AZ@@|"_s;
        auto how_pattern1 = "@a|@b:ahello:bworld"_s;
        auto how_pattern2 = "hello|world"_s;
        auto a = "HElloWORLd"_s.match(pattern) - 2u;
        auto b = int("HElloWORLd"_s.search(pattern) == "HE"_s) - 1;
        auto c = int("lloWORLd"_s.search(pattern) == "WORL"_s) - 1;
        auto d1 = int("helworldlo"_s.search(how_pattern1) == "world"_s) - 1;
        auto d2 = int("helworldlo"_s.search(how_pattern2) == "helworld"_s) - 1;
        return a + b + c + d1 + d2;
    }
}

auto main() -> int
{
    return ::dewox::main::run();
}

