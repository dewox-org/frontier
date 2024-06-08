#include "test.hpp"
#include "string.hpp"
#include "object.hpp"

namespace dewox::inline test
{
    extern "C" auto dewox_site_test_string(Test* test) -> void
    {
        auto pattern = "~AZ@a:a~AZ@@|"_s;
        auto how_pattern1 = "@a|@b:ahello:bworld"_s;
        auto how_pattern2 = "hello|world"_s;

        if (test->start(test, "string/into/literal-size")) {
            test->finish(test, ("hello"_s.byte_count() == 5));
        }

        if (test->start(test, "string/into/until-size")) {
            test->finish(test, (create(&String::until_into, "hello").byte_count() == 5));
        }

        if (test->start(test, "string/into/literal-eq-until")) {
            test->finish(test, ("hello"_s == create(&String::until_into, "hello")));
        }

        if (test->start(test, "string/match")) {
            test->finish(test, ("HElloWORLd"_s.match(pattern) == 2));
        }

        if (test->start(test, "string/search/front")) {
            test->finish(test, ("HElloWORLd"_s.search(pattern) == "HE"_s));
        }

        if (test->start(test, "string/search/middle")) {
            test->finish(test, ("lloWORLd"_s.search(pattern) == "WORL"_s));
        }

        if (test->start(test, "string/search/branching/true")) {
            test->finish(test, ("helworldlo"_s.search(how_pattern1) == "world"_s));
        }

        if (test->start(test, "string/search/branching/fake")) {
            test->finish(test, ("helworldlo"_s.search(how_pattern2) == "helworld"_s));
        }
    }
}

